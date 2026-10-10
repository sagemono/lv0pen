#include <spu_intrinsics.h>
#include "spp.h"
#include "aes.h"
#include "sha1.h"
#include "hmac_sha1.h"
#include "ec.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

static const unsigned char g_metadata_key[32] __attribute__((aligned(16))) = {
    0xD2, 0x96, 0x1B, 0xBF, 0xFE, 0xDA, 0xEE, 0x26, 0x9B, 0x06, 0x14, 0x54, 0xD6, 0xAC, 0xF2, 0x62,
    0xCD, 0x71, 0xBC, 0x6F, 0x53, 0x20, 0x18, 0x0F, 0x0A, 0x04, 0xA0, 0x75, 0x83, 0xCC, 0xF7, 0xC5,
};
static const unsigned char g_metadata_iv[16] __attribute__((aligned(16))) = {
    0x6D, 0x30, 0x8B, 0x59, 0xBC, 0x90, 0x54, 0xB1, 0x2C, 0x83, 0x33, 0x59, 0xD3, 0x1B, 0xAE, 0x77,
};
static const unsigned char g_pub[40] __attribute__((aligned(16))) = {
    0x0C, 0xDD, 0x0D, 0x15, 0x41, 0xA6, 0xB0, 0xDF, 0xD1, 0x2E, 0x95, 0x15, 0x78, 0x53, 0x6C, 0xA6,
    0x0C, 0x93, 0x85, 0x3B, 0x88, 0xFC, 0xE1, 0x75, 0x92, 0xDC, 0x8D, 0x13, 0x4B, 0x08, 0xA9, 0x4A,
    0xF8, 0xBB, 0x08, 0x66, 0x7E, 0x2D, 0x70, 0xA5,
};

unsigned int g_curve = 7;

unsigned char g_metadata[0x300] __attribute__((aligned(16)));
unsigned char *g_metadata_info = g_metadata;
unsigned char *g_metadata_ctr_iv = g_metadata_info + 32;
unsigned int g_section_count;
u64 g_header_len;
dma_reader *g_reader;
dma_writer *g_writer;
ls_span g_in;
ls_span g_out;
quad_uint g_out_end;

#define LS_HEADER       0x3E000
#define LS_READ         0x3E000
#define LS_WRITE        0x3F000
#define TAG_READ        6
#define TAG_WRITE       7

unsigned char *key_slot(metadata *m, unsigned int i)
{
    return (unsigned char *)m + sizeof m->info + sizeof m->header +
           g_section_count * sizeof(metadata_section) + i * 16;
}

int verify_section(metadata *m, u64 ea, unsigned int i)
{
    volatile metadata_section *s = (metadata_section *)((unsigned char *)m + sizeof m->info + sizeof m->header +
                                               i * sizeof(metadata_section));
    unsigned char digest[20] __attribute__((aligned(16)));
    hmac_sha1_ctx ctx;
    unsigned int type = s->type;
    unsigned int encrypted = s->encrypted;
    unsigned char *hash, *key, *iv;
    unsigned int n, size, done;
    unsigned int sha1_index, key_index, iv_index;
    u64 total, offset;

    if (!((type == 1 && encrypted == 1) || (type == 2 && encrypted == 3)))
        return -1;
    sha1_index = s->sha1_index;
    hash = key_slot((metadata *)g_metadata, sha1_index);
    hmac_sha1_init(&ctx, key_slot((metadata *)g_metadata, sha1_index + 2), 64);
    key_index = s->key_index;
    iv_index = s->iv_index;
    key = key_slot((metadata *)g_metadata, key_index);
    iv = key_slot((metadata *)g_metadata, iv_index);
    total = s->size;
    offset = s->offset;
    if (offset < 32)
        return -1;
    ea += offset;
    size = total;
    done = 0;
    g_reader->seek(ea);
    g_reader->set_size(size);
    g_writer->seek(ea);
    g_writer->set_size(size);
    do {
        unsigned char *in, *out;
        unsigned int len;

        g_reader->read(&g_in);
        n = g_in.end - g_in.begin;
        g_writer->get_buffer(&g_out, &g_out_end);
        done += n;
        in = (unsigned char *)g_in.begin;
        len = g_in.end - g_in.begin;
        out = (unsigned char *)g_out.begin;
        if (encrypted == 3) {
            if (aes_ctr((vec_uchar16 *)out, (const vec_uchar16 *)in, len, key, 128, (vec_uchar16 *)iv, 0) < 0)
                return -1;
        } else {
            memcpy(out, in, len);
        }
        if (hmac_sha1_update(&ctx, out, n))
            return -1;
        g_writer->put(&g_out, &g_out_end);
    } while (done < total);
    if (hmac_sha1_final(digest, &ctx))
        return -1;
    if (bytes_differ(digest, hash, 20))
        return -1;
    return 0;
}

int verify_header(u64 ea, unsigned int size)
{
    dma_channel dma;
    unsigned char iv[16] __attribute__((aligned(16)));
    unsigned char digest[32] __attribute__((aligned(16)));
    struct {
        unsigned char r[21];
        unsigned char s[21];
    } sig;
    sha1_ctx ctx;
    sce_header *h = (sce_header *)LS_HEADER;
    metadata *m = (metadata *)g_metadata;
    unsigned int len, ctr_len;
    u64 sig_offset;
    unsigned char *p;
    int i;

    for (i = 0; i < 32; i++)
        digest[i] = 0;
    if (dma.issue(LS_HEADER, ea, 32, TAG_READ, 0, MFC_GET))
        return -1;
    while (!dma.tag_done(TAG_READ))
        ;
    if (h->magic != SCE_MAGIC)
        return -1;
    if (h->header_type != SCE_TYPE_SPP)
        return -1;
    if (h->version != SCE_VERSION)
        return -1;
    g_header_len = h->header_len;
    if (g_header_len > sizeof g_metadata)
        return -1;
    if (h->key_revision & 0xF)
        return -1;
    if (h->key_revision & 0x8000)
        return -1;
    if (g_header_len > size)
        return -1;
    if (dma.issue(LS_HEADER, ea, g_header_len, TAG_READ, 0, MFC_GET))
        return -1;
    while (!dma.tag_done(TAG_READ))
        ;
    if (aes_cbc_decrypt((vec_uchar16 *)g_metadata_info, (const vec_uchar16 *)(LS_HEADER + 32), 64,
                        g_metadata_key, 256, (const vec_uchar16 *)g_metadata_iv) < 0)
        return -1;
    ctr_len = g_header_len - 96;
    memcpy(iv, g_metadata_ctr_iv, 16);
    if (aes_ctr((vec_uchar16 *)&m->header, (const vec_uchar16 *)(LS_HEADER + 96), ctr_len,
                g_metadata_info, 128, (vec_uchar16 *)iv, 0) < 0)
        return -1;
    g_section_count = m->header.section_count;
    len = 20;
    if (m->header.sig_type != SIG_SHA1) {
        if (m->header.sig_type != SIG_ECDSA)
            return -1;
        len = 42;
    }
    sig_offset = m->header.sig_input_length;
    if (sig_offset + len > g_header_len)
        return -1;
    if (sha1_init(&ctx) < 0)
        return -1;
    if (sha1_update(&ctx, (const unsigned char *)LS_HEADER, 32) < 0)
        return -1;
    if (sha1_update(&ctx, g_metadata, m->header.sig_input_length - 32) < 0)
        return -1;
    if (sha1_final(digest, &ctx) < 0)
        return -1;
    p = g_metadata + sig_offset - 32;
    if (m->header.sig_type == SIG_SHA1) {
        if (bytes_differ(p, digest, 20))
            return -2;
    } else if (m->header.sig_type == SIG_ECDSA) {
        memcpy(sig.r, p, 21);
        memcpy(sig.s, p + 21, 21);
        if (ecdsa_verify_id(sig.r, digest, g_pub, g_curve) < 0)
            return -2;
    }
    return 0;
}

int main(u64 ea, u64 size)
{
    read_cursor rc;
    write_cursor wc;
    dma_reader reader(LS_READ, 0x1000, TAG_READ, &rc);
    dma_writer writer(LS_WRITE, 0x1000, TAG_WRITE, &wc);
    unsigned int i;
    int r;

    g_reader = &reader;
    g_writer = &writer;
    dma_channel dma;
    r = -1;
    if (__builtin_expect(size > 0x17F, 1))
        r = verify_header(ea, size);
    if (r < 0) {
        if (r == -1)
            spu_stop(0x1004);
        else if (r == -2)
            spu_stop(0x1005);
        else if (r != -15)
            spu_stop(0x103);
        else
            spu_stop(0x1015);
        return -1;
    }
    for (i = 0; i < g_section_count; i++) {
        r = verify_section((metadata *)g_metadata, ea, i);
        if (r < 0) {
            if (r == -1)
                spu_stop(0x1006);
            else if (r != -15)
                spu_stop(0x1006);
            else
                spu_stop(0x1015);
            return -1;
        }
    }
    spu_stop(0x100);
    return 0;
}
