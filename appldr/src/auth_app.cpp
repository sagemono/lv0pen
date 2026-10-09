#include "auth.h"
#include "cipher.h"
#include "verifier.h"
#include "util.h"
#include "chain_cipher.h"

void authenticator::reset()
{
    clear_parts();
    m_tail.reset();
}

void auth_tail::reset()
{
    header_data::clear();
}

struct section_entry {
    u64 offset;
    u64 size;
    u32 compressed;
    u32 pad;
    u64 encrypted;
} __attribute__((aligned(16)));

long authenticator::select_section(unsigned int type, unsigned int index)
{
    if (!m_loaded)
        return -4;
    if (!m_1b9) {
        if (find_section(type, index, &m_section))
            return 19;
    } else {
        section_entry e;
        Elf32_Phdr ph32;
        Elf64_Phdr ph64;
        long r;

        if (is_elf64())
            r = read_program_header(index, &ph64);
        else
            r = read_program_header(index, &ph32);
        if (r)
            return 19;
        if (get_section_info(index, &e))
            return 19;
        m_section.offset = e.offset;
        m_section.size = e.size;
        m_section.type = type;
        m_section.index = index;
        m_section.hashed = 2;
        m_section.digest = 0;
        if (e.encrypted & 2)
            m_section.encrypted = 3;
        else
            m_section.encrypted = 1;
        m_section.key = 0;
        m_section.iv = 0;
        m_section.compressed = e.compressed;
    }
    g_hmac.init(m_keys + (m_section.digest << 4) + 32, 64);
    g_aes128_ctr.set_key(m_keys + (m_section.key << 4), m_keys + (m_section.iv << 4));
    m_tail.clear();
    m_1f0 = 0;
    return 0;
}

long auth_block::get_w(u64 *out)
{
    *out = w;
    return 0;
}

long auth_block::get_digest(unsigned char *out)
{
    memcpy(out, digest, sizeof digest);
    return 0;
}

long authenticator::load_header(unsigned int type, const unsigned char *key,
                                const unsigned char *iv, const unsigned char *pub,
                                const unsigned int *curve, dma_buffer **bufs,
                                unsigned int nbufs, void *args, const unsigned char *args_iv,
                                bool flag)
{
    unsigned char *p;
    unsigned int off, end;
    unsigned int i;
    long rc;

    if (m_queue.open(0, 0, m_offset, 32))
        return 21;
    for (i = 0; i != nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return 21;
    if (read_parts())
        return 21;
    m_queue.close();

    m_sce = m_image.m_sce;
    m_key_revision = m_image.m_sce->key_revision & 0xFFF;
    if (check_sce_header((const vec_uchar16 *)m_image.m_sce, (const void *)flag, 1,
                         m_image.m_size))
        return 19;
    if ((m_image.m_sce->key_revision & 0x8000) && flag)
        m_1b9 = true;

    if (m_queue.open(0, 0, m_offset + 32, m_sce->header_len - 32))
        return 21;
    for (i = 0; i != nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return 21;
    if (read_parts())
        return 21;
    m_queue.close();

    m_info = (unsigned char *)m_image.m_sce + m_sce->metadata_offset + 32;
    m_meta = (const auth_meta *)((const unsigned char *)m_image.m_sce
                                 + m_sce->metadata_offset + 96);
    m_ext = (const sce_ext_header *)((const unsigned char *)m_image.m_sce + 32);
    m_app = (const auth_app_info *)((const unsigned char *)m_image.m_sce + m_ext->app_offset);
    unsigned int app_type = m_app->type;

    if (app_type == 8 && type == 8) {
        g_aes128_cbc_chain.set_key((const unsigned char *)args, args_iv);
        if (g_aes128_cbc_chain.decrypt(m_info, 64, m_info))
            return 21;
    }
    g_aes256_cbc.set_key(key, iv);
    if (g_aes256_cbc.decrypt(m_info, 64, m_info))
        return 21;
    off = m_sce->metadata_offset + 96;
    end = ((const unsigned int *)&m_sce->header_len)[1] - off;
    g_aes128_ctr.set_key(m_info, m_info + 32);
    p = (unsigned char *)m_image.m_sce + off;
    if (g_aes128_ctr.decrypt(p, end, p))
        return 21;

    if (m_1b9)
        if (build_meta(&m_image, (auth_meta *)m_meta))
            return 19;
    if (check_meta_header((const vec_uchar16 *)m_sce, m_meta, 1))
        return 19;
    if (!m_1b9)
        if (check_sections(&m_image))
            return 19;

    ecdsa_verifier v(pub, *curve);

    rc = 14;
    if (v.verify((const unsigned char *)m_image.m_sce, m_meta->signed_len,
                 (const unsigned char *)m_image.m_sce + m_meta->signed_len) == 0 || m_1b9) {
        rc = check_app(&m_image, type);
        if (rc == 0) {
            const unsigned char *base = (const unsigned char *)m_image.m_sce;

            m_section_info = base + m_ext->section_info_offset;
            m_version = base + m_ext->version_offset;
            m_control = (const auth_entry *)(base + m_ext->control_offset);
            if (m_block.load(m_control, m_ext->control_size))
                rc = 19;
            else {
                rc = m_elf.load((const unsigned char *)m_image.m_sce + m_ext->elf_offset);
                if (rc)
                    rc = 19;
                else {
                    m_sections = (const auth_section *)((const unsigned char *)m_image.m_sce
                                                        + m_sce->metadata_offset + 128);
                    m_keys = (const unsigned char *)(m_sections + m_meta->section_count);
                    m_opt = (const auth_entry *)(m_keys + (m_meta->key_count << 4));
                    if (m_1b9 || m_state.load(m_opt, m_meta->opt_header_size) == 0)
                        m_loaded = true;
                    else
                        rc = 19;
                }
            }
        }
    }
    return rc;
}

long header_data::decrypt()
{
    return g_aes128_ctr.decrypt(buf, len, buf) ? -2 : 0;
}

long header_data::hash()
{
    return g_hmac.update(buf, len) ? -2 : 0;
}

long header_data::restore(const dma_buffer *b)
{
    if (len > b->size - b->offset)
        return -1;

    unsigned char *d = (unsigned char *)b->ls + b->offset;
    unsigned char *end = d + len;
    const unsigned char *s = buf;

    while (end > d)
        *d++ = *s++;
    return 0;
}

long header_data::prepend(const dma_buffer *b, unsigned int n)
{
    if (n + len > 32)
        return -1;

    unsigned char *base = buf + n;
    unsigned char *dst = base + len;
    unsigned char *src = buf + len;

    while (dst > base)
        *--dst = *--src;

    const unsigned char *end = (const unsigned char *)b->ls + b->length;
    const unsigned char *p = end - n;
    unsigned char *d = buf;

    while (p < end)
        *d++ = *p++;
    len += n;
    return 0;
}

long header_data::save(const dma_buffer *b)
{
    if (b->length > 32)
        return -1;
    len = b->length;

    const unsigned char *s = (const unsigned char *)b->ls + b->offset;
    const unsigned char *end = s + b->length;
    unsigned char *d = buf;

    while (end > s)
        *d++ = *s++;
    return 0;
}

long authenticator::set_header(unsigned int type, const void *p)
{
    const sce_header *h = (const sce_header *)p;

    m_sce = h;
    if (h->magic != 0x53434500)
        return 19;
    if (h->version != 2)
        return 19;
    if (h->type != type)
        return 19;
    if (h->metadata_offset & 15)
        return 19;
    if (h->header_len & 0x7F)
        return 19;
    if (h->header_len <= 0x7F)
        return 19;
    m_key_revision = h->key_revision & 0xFFF;
    if (h->key_revision & 0x8000)
        m_1b9 = 1;
    return 0;
}
