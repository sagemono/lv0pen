#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

long elf_image::load(const void *image)
{
    const char *p = (const char *)image;

    if (p[0] != 0x7F || p[1] != 'E' || p[2] != 'L' || p[3] != 'F' || p[6] != 1 || p[5] != 2)
        return -1;
    long rc = -1;

    switch ((unsigned int)p[4]) {
    case 2:
        ehdr = (const Elf64_Ehdr *)p;
        phdr = (const Elf64_Phdr *)(((unsigned int)p + sizeof(Elf64_Ehdr) + 15) & ~15);
        phdr_end = (const unsigned char *)(((unsigned int)phdr
                                            + (ehdr->e_phnum * sizeof(Elf64_Phdr) + 15)) & ~15);
        if (ehdr->e_machine == 21 && ehdr->e_version == 1)
            rc = 0;
        break;
    case 1:
        ehdr32 = (const Elf32_Ehdr *)p;
        phdr32 = (const Elf32_Phdr *)(((unsigned int)p + sizeof(Elf32_Ehdr) + 15) & ~15);
        phdr32_end = (const unsigned char *)(ehdr32->e_phnum * sizeof(Elf32_Phdr)
                                             + (unsigned int)phdr32);
        if (ehdr32->e_machine == 23 && ehdr32->e_version == 1)
            rc = 0;
        break;
    }
    return rc;
}

authenticator::authenticator()
    : m_sce(0), m_ext(0), m_info(0), m_app(0), m_section_info(0), m_version(0),
      m_control(0), m_block(), m_meta(0), m_sections(0), m_keys(0), m_opt(0),
      m_state(), m_offset(0), m_loaded(false), m_1b9(0), m_key_revision(0xFFFF),
      m_tail(), m_1f0(0), m_section()
{
    m_image.m_sce = (const sce_header *)g_part_area;
    m_image.m_size = sizeof(g_part_area);
    clear_parts();
}

void authenticator::clear_parts()
{
    memset(g_part_area, 0, sizeof(g_part_area));
    memset(g_part_digest, 0, sizeof(g_part_digest));
}

void authenticator::set_offset(u64 off)
{
    m_offset = off;
}

long auth_block::get(u64 *out)
{
    memcpy(out, v, 32);
    return 0;
}

long authenticator::read_elf_header(Elf64_Ehdr *out)
{
    long rc = -4;

    if (m_loaded)
        rc = m_elf.get_header(out) != 0 ? -2 : 0;
    return rc;
}

long authenticator::read_program_header(unsigned int i, Elf64_Phdr *out)
{
    if (!m_loaded)
        return -4;
    return m_elf.get_program_header(i, out);
}

long authenticator::check_app_info(const auth_app_info *app, unsigned int type)
{
    if (type != app->type)
        return 28;
    if (app->vendor[1] != 0)
        return 19;
    if (app->vendor[2] != 0)
        return 19;
    if (app->reserved != 0)
        return 19;
    return 0;
}

long authenticator::check_app(sce_image *image, unsigned int type)
{
    if (check_ext_header(image))
        return 19;

    u32 app = 0;

    if (image->get_app_info(&app))
        return 19;
    return check_app_info((const auth_app_info *)app, type);
}

long authenticator::load_header(unsigned int type, const unsigned char *key,
                                const unsigned char *iv, const unsigned char *pub,
                                const unsigned int *curve, dma_buffer **bufs,
                                unsigned int nbufs)
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
    if (check_sce_header((const vec_uchar16 *)m_image.m_sce, 0, 1, m_image.m_size))
        return 19;

    if (m_queue.open(0, 0, m_offset + 32, m_sce->header_len - 32))
        return 21;
    for (i = 0; i != nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return 21;
    if (read_parts())
        return 21;
    m_queue.close();

    m_info = (unsigned char *)m_image.m_sce + m_sce->metadata_offset + 32;
    g_aes256_cbc.set_key(key, iv);
    if (g_aes256_cbc.decrypt(m_info, 64, m_info))
        return 21;
    off = m_sce->metadata_offset + 96;
    end = ((const unsigned int *)&m_sce->header_len)[1] - off;
    g_aes128_ctr.set_key(m_info, m_info + 32);
    p = (unsigned char *)m_image.m_sce + off;
    if (g_aes128_ctr.decrypt(p, end, p))
        return 21;

    m_meta = (const auth_meta *)((const unsigned char *)m_image.m_sce
                                 + m_sce->metadata_offset + 96);
    if (check_meta_header((const vec_uchar16 *)m_sce, m_meta, 1))
        return 19;
    if (check_sections(&m_image))
        return 19;

    ecdsa_verifier v(pub, *curve);

    rc = 14;
    if (v.verify((const unsigned char *)m_image.m_sce, m_meta->signed_len,
                 (const unsigned char *)m_image.m_sce + m_meta->signed_len) == 0) {
        rc = check_app(&m_image, type);
        if (rc == 0) {
            const unsigned char *base = (const unsigned char *)m_image.m_sce;

            m_ext = (const sce_ext_header *)(base + 32);
            m_app = (const auth_app_info *)(base + m_ext->app_offset);
            m_section_info = base + m_ext->section_info_offset;
            m_version = base + m_ext->version_offset;
            m_control = (const auth_entry *)(base + m_ext->control_offset);
            if (m_block.load(m_control, m_ext->control_size))
                rc = 19;
            else {
                m_sections = (const auth_section *)((const unsigned char *)m_image.m_sce
                                                    + m_sce->metadata_offset + 128);
                m_keys = (const unsigned char *)(m_sections + m_meta->section_count);
                m_opt = (const auth_entry *)(m_keys + (m_meta->key_count << 4));
                if (m_state.load(m_opt, m_meta->opt_header_size))
                    rc = 19;
                else {
                    rc = m_elf.load((const unsigned char *)m_image.m_sce + m_ext->elf_offset);
                    if (rc)
                        rc = 19;
                    else
                        m_loaded = true;
                }
            }
        }
    }
    return rc;
}
