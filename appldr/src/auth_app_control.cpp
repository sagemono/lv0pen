#include "auth.h"
#include "util.h"

long authenticator::get_control_w(u64 *out)
{
    if (!m_loaded)
        return -4;
    if (!out)
        return 21;
    return m_block.get_w(out) ? 19 : 0;
}

long authenticator::get_control_values(u64 *out)
{
    if (!m_loaded)
        return -4;
    if (!out)
        return 21;
    return m_block.get(out) ? -1 : 0;
}

long authenticator::get_section_digest(unsigned int type, unsigned int index, unsigned char *out,
                                       unsigned int size)
{
    auth_section s;
    long r;

    if (!m_loaded)
        return -4;
    if (out == 0 || size < 20)
        return 21;
    memset(&s, 0, sizeof s);
    r = find_section(type, index, &s);
    if (r)
        return r;
    memcpy(out, m_keys + (s.digest << 4), 20);
    return 0;
}

typedef sce_header sce_header_q __attribute__((aligned(16)));

long authenticator::build_meta(sce_image *image, auth_meta *meta)
{
    u32 ext, off;
    unsigned int n, i;

    if (image == 0 || meta == 0)
        return 21;

    const sce_header_q *h = image->m_sce;

    meta->signed_len = h->header_len - 48;
    meta->sig_type = 1;
    if (image->get_ext_header(&ext, &off))
        return -1;

    const sce_ext_header *e = (const sce_ext_header *)ext;
    const unsigned char *elf = (const unsigned char *)image->m_sce + e->elf_offset;
    const unsigned char *ph = (const unsigned char *)image->m_sce + e->phdr_offset;

    n = 0;
    if (elf[4] == 2) {
        const Elf64_Phdr *p = (const Elf64_Phdr *)ph;

        for (i = 0; i < ((const Elf64_Ehdr *)elf)->e_phnum; i++)
            if (p[i].p_type == 1 || p[i].p_type == 0x700000A4)
                n++;
    } else if (elf[4] == 1) {
        const Elf32_Phdr *p = (const Elf32_Phdr *)ph;

        for (i = 0; i < ((const Elf32_Ehdr *)elf)->e_phnum; i++)
            if (p[i].p_type == 1 || p[i].p_type == 0x700000A4)
                n++;
    } else
        return ~0UL;
    meta->section_count = n;
    meta->key_count = n * 8;
    meta->opt_header_size = (h->header_len - h->metadata_offset - n * 48 - n * 128 - 176) & ~15;
    meta->reserved = 0;
    return 0;
}
