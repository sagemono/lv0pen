#include <spu_intrinsics.h>
#include "pkg_rvk.h"
#include "aes.h"
#include "sha1.h"
#include "hmac_sha1.h"
#include "ec.h"
#include "util.h"

sce_verifier::sce_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                           const unsigned char *iv, const unsigned char *pub, unsigned int curve)
    : m_reader(reader), m_writer(writer), m_sizes()
{
    m_key = key;
    m_iv = iv;
    m_pub = pub;
    m_curve = curve;
    m_data_len = 0;
}

int sce_verifier::verify_section(const metadata_section *s, const unsigned char *keys,
                                 u64 in_ea, unsigned int in_size, u64 out_ea,
                                 unsigned int out_size, unsigned char *copy)
{
    unsigned char iv[16] __attribute__((aligned(16)));
    unsigned char digest[20] __attribute__((aligned(16)));
    hmac_sha1_ctx ctx;
    const unsigned char *key = 0;
    unsigned char *ivp = 0;
    const unsigned char *hash, *p;
    u64 size, tail;
    int r, i;

    if (in_size == 0 || out_size == 0)
        return VERIFY_EINVAL;
    if (in_ea == 0 || out_ea == 0)
        return VERIFY_EINVAL;
    if (s == 0 || keys == 0)
        return VERIFY_EINVAL;
    if (s->hashed != 2 || !(s->encrypted == 1 || s->encrypted == 3))
        return VERIFY_EBADF;
    hmac_sha1_init(&ctx, keys + (u64)(s->sha1_index + 2) * 16, 64);
    size = in_size;
    tail = size & 15;
    if (s->encrypted != 1) {
        memcpy(iv, keys + (u64)s->iv_index * 16, 16);
        key = keys + (u64)s->key_index * 16;
        ivp = iv;
    }
    if (in_size / 16) {
        r = transfer_blocks(in_ea, in_size - tail, out_ea, out_size - tail, key, ivp, &ctx, copy);
        if (r)
            return r;
    }
    if (tail) {
        r = transfer_tail(in_ea + size - tail, tail, out_ea + (u64)out_size - tail, tail, key, ivp,
                          &ctx, copy ? copy + (in_size - tail) : 0);
        if (r)
            return r;
    }
    if (hmac_sha1_final(digest, &ctx))
        return VERIFY_EBADF;
    hash = keys + (u64)s->sha1_index * 16;
    p = digest;
    for (i = 0; i < 20; i++)
        if (p[i] != hash[i])
            return VERIFY_EBADF;
    return 0;
}

int sce_verifier::read_header(u64 ea, unsigned int size, sce_header *h)
{
    ls_span in, s;
    unsigned int done = 0;

    span_set(&s, (unsigned char *)h, size);
    if (h == 0 || ea == 0)
        return VERIFY_EINVAL;
    if (size != sizeof *h)
        return VERIFY_EINVAL;
    m_reader->seek(ea);
    m_reader->set_size(size);
    do {
        m_reader->read(&in);
        done += in.end - in.begin;
        span_fill(&s, (const unsigned char *)in.begin, (const unsigned char *)in.end);
        s.begin += in.end - in.begin;
    } while (done < sizeof *h);
    m_data_len = h->sizes.data_len;
    m_sizes = h->sizes;
    return 0;
}

int sce_verifier::read_metadata_info(u64 ea, unsigned int size, unsigned char *buf,
                                     unsigned char *info)
{
    ls_span in, s;
    unsigned int done = 0;

    span_set(&s, buf, size);
    if (buf == 0 || ea == 0)
        return VERIFY_EINVAL;
    if (size == 0 || info == 0)
        return VERIFY_EINVAL;
    m_reader->seek(ea);
    m_reader->set_size(size);
    do {
        m_reader->read(&in);
        done += in.end - in.begin;
        span_fill(&s, (const unsigned char *)in.begin, (const unsigned char *)in.end);
        s.begin += in.end - in.begin;
    } while (size > done);
    return aes_cbc_decrypt((vec_uchar16 *)info, (const vec_uchar16 *)buf, 64, m_key, 256,
                           (const vec_uchar16 *)m_iv) ? VERIFY_ECRYPTO : 0;
}

int sce_verifier::verify_signature(const unsigned char *headers)
{
    unsigned char digest[20] __attribute__((aligned(16)));
    struct {
        unsigned char r[21];
        unsigned char s[21];
    } sig;
    const unsigned char *p;
    unsigned int len;

    if (headers == 0)
        return VERIFY_EINVAL;
    len = ((const metadata_header *)(headers + 96))->sig_input_length;
    p = headers + len;
    if (sha1_digest(digest, headers, len))
        return VERIFY_EBADF;
    memcpy(sig.r, p, 21);
    memcpy(sig.s, p + 21, 21);
    if (ecdsa_verify_id(sig.r, digest, m_pub, m_curve))
        return VERIFY_EBADF;
    return 0;
}

int sce_verifier::decrypt_metadata(const unsigned char *in, unsigned int size,
                                   const unsigned char *info, unsigned char *out)
{
    unsigned char iv[16] __attribute__((aligned(16)));

    if (in == 0 || out == 0)
        return VERIFY_EINVAL;
    if (size == 0 || info == 0)
        return VERIFY_EINVAL;
    memcpy(out, in, 32);
    memcpy(out + 32, info, 64);
    memcpy(iv, info + 32, 16);
    if (aes_ctr((vec_uchar16 *)(out + 96), (const vec_uchar16 *)(in + 96), size - 96, info, 128,
                (vec_uchar16 *)iv, 0))
        return VERIFY_ECRYPTO;
    return verify_signature(out);
}

int sce_verifier::read_metadata(u64 ea, const sce_header *h, unsigned char *out)
{
    u64 size = h->sizes.header_len;
    unsigned char info[64] __attribute__((aligned(16)));
    unsigned char buf[size] __attribute__((aligned(16)));
    int i, r;

    if (out == 0 || ea == 0)
        return VERIFY_EINVAL;
    for (i = 0; i < 32; i++)
        buf[i] = ((const unsigned char *)h)[i];
    unsigned int len = size;
    r = read_metadata_info(ea + 32, len - 32, buf + 32, info);
    if (r == 0)
        r = decrypt_metadata(buf, len, info, out);
    return r;
}

int sce_verifier::transfer_blocks(u64 in_ea, unsigned int in_size, u64 out_ea,
                                  unsigned int out_size, const unsigned char *key,
                                  unsigned char *iv, hmac_sha1_ctx *ctx, unsigned char *copy)
{
    ls_span in, out, saved;
    quad_uint end;
    unsigned int done = 0;
    int r = 0;

    span_set(&saved, copy, in_size);
    m_reader->seek(in_ea);
    m_reader->set_size(in_size);
    m_writer->seek(out_ea);
    m_writer->set_size(out_size);
    do {
        m_reader->read(&in);
        done += in.end - in.begin;
        m_writer->get_buffer(&out, &end);
        if (key) {
            r = aes_ctr((vec_uchar16 *)out.begin, (const vec_uchar16 *)in.begin, in.end - in.begin,
                        key, 128, (vec_uchar16 *)iv, 0);
            out.end = out.begin + (in.end - in.begin);
        } else {
            span_fill(&out, (const unsigned char *)in.begin, (const unsigned char *)in.end);
            out.end = out.begin + (in.end - in.begin);
        }
        if (copy) {
            span_fill(&saved, (const unsigned char *)out.begin, (const unsigned char *)out.end);
            saved.begin += out.end - out.begin;
        }
        if (key) {
            if (r)
                return VERIFY_ECRYPTO;
        }
        if (hmac_sha1_update(ctx, (const unsigned char *)out.begin, out.end - out.begin))
            return VERIFY_EBADF;
        m_writer->put(&out, &end);
    } while (in_size > done);
    m_writer->finish(&out);
    return 0;
}

int sce_verifier::transfer_tail(u64 in_ea, unsigned int in_size, u64 out_ea,
                                unsigned int out_size, const unsigned char *key,
                                unsigned char *iv, hmac_sha1_ctx *ctx, unsigned char *copy)
{
    ls_span in, out;
    quad_uint end;
    unsigned char block[16] __attribute__((aligned(16)));
    unsigned char plain[16] __attribute__((aligned(16)));
    unsigned char *p = block;
    const unsigned char *src;
    unsigned int done = 0;
    u64 left, n;

    m_reader->seek(in_ea);
    m_reader->set_size(in_size);
    m_writer->seek(out_ea);
    m_writer->set_size(out_size);
    do {
        m_reader->read(&in);
        done += in.end - in.begin;
        memcpy(p, (const void *)in.begin, in.end - in.begin);
        p += in.end - in.begin;
    } while (in_size > done);
    if (key) {
        if (aes_ctr((vec_uchar16 *)plain, (const vec_uchar16 *)block, in_size, key, 128,
                    (vec_uchar16 *)iv, 0))
            return VERIFY_ECRYPTO;
    }
    if (hmac_sha1_update(ctx, plain, in_size))
        return VERIFY_EBADF;
    if (copy)
        memcpy(copy, plain, in_size);
    src = plain;
    left = out_size;
    do {
        m_writer->get_buffer(&out, &end);
        if (left >= 8)
            n = 8;
        else if (left >= 4)
            n = 4;
        else if (left >= 2)
            n = 2;
        else
            n = 1;
        left -= n;
        memcpy((void *)out.begin, src, n);
        src += n;
        out.end = out.begin + n;
        m_writer->put(&out, &end);
    } while (left != 0);
    m_writer->finish(&out);
    return 0;
}

int sce_verifier::check_header(const sce_header *h, unsigned int version,
                               unsigned int header_type, unsigned int metadata_offset)
{
    if (h == 0)
        return VERIFY_EINVAL;
    if (h->magic != SCE_MAGIC || h->version != version || h->header_type != header_type ||
        h->metadata_offset != metadata_offset)
        return VERIFY_EBADF;
    return 0;
}

int sce_verifier::check_section_count(const metadata_header *m, unsigned int count)
{
    if (m == 0)
        return VERIFY_EINVAL;
    return m->section_count != count ? VERIFY_EBADF : 0;
}

sce_verifier::~sce_verifier()
{
}

int sce_verifier::check_section(const metadata_section *s, unsigned int type, unsigned int index)
{
    if (s == 0)
        return VERIFY_EINVAL;
    return s->type != type || s->index != index ? VERIFY_EBADF : 0;
}

void span_set(ls_span *s, unsigned char *p, unsigned int size)
{
    s->begin = (unsigned int)p;
    s->end = (unsigned int)p + size;
}

const unsigned char *span_fill(ls_span *s, const unsigned char *src, const unsigned char *src_end)
{
    unsigned int start = s->begin;

    while (s->begin < s->end && src < src_end)
        *(unsigned char *)s->begin++ = *src++;
    s->end = s->begin;
    s->begin = start;
    return src;
}
