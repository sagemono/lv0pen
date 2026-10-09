#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

long elf_image::load(const void *image)
{
    const char *p = (const char *)image;

    if (p[0] != 0x7F || p[1] != 'E' || p[2] != 'L' || p[3] != 'F' || p[6] != 1 || p[5] != 2)
        return -1;
    switch ((unsigned int)p[4]) {
    case 1:
        ehdr = (const Elf32_Ehdr *)p;
        phdr = (const Elf32_Phdr *)(((unsigned int)p + sizeof(Elf32_Ehdr) + 15) & ~15);
        break;
    default:
        return -1;
    }
    if (ehdr->e_type != 2 && ehdr->e_type != 3)
        return -1;
    return 0;
}

authenticator::authenticator()
    : m_sce(0), m_ext(0), m_info(0), m_app(0), m_section_info(0), m_meta(0),
      m_sections(0), m_keys(0), m_offset(0), m_loaded(false), m_58(0), m_section()
{
}

void authenticator::set_offset(u64 off)
{
    m_offset = off;
}

long authenticator::load_header(unsigned int type, const unsigned char *key,
                                const unsigned char *iv, const unsigned char *pub,
                                const unsigned int *curve, dma_buffer **bufs,
                                unsigned int nbufs)
{
    const unsigned char *base;
    unsigned char *p;
    unsigned int off;
    unsigned int end;
    unsigned int i;
    long rc;

    if (m_queue.open(0, 32, m_offset, 32))
        return 21;
    for (i = 0; i < nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return 21;
    base = (const unsigned char *)m_sink.base;
    if (drain_read_queue())
        return 21;
    m_queue.close();
    if (check_sce_header(1))
        return 32;
    if (m_sce->header_len > 0x800)
        return 32;
    m_queue.open(0, m_sce->header_len - 32, m_offset + 32, m_sce->header_len - 32);
    for (i = 0; i < nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return 21;
    if (drain_read_queue())
        return 21;
    m_queue.close();

    m_info = (unsigned char *)base + m_sce->metadata_offset + 32;
    m_meta = (const auth_meta *)(base + m_sce->metadata_offset + 96);
    g_aes256_cbc.set_key(key, (const cipher_iv *)iv);
    if (g_aes256_cbc.decrypt(m_info, 64, m_info))
        return 35;
    off = m_sce->metadata_offset + 96;
    end = ((const unsigned int *)&m_sce->header_len)[1] - off;
    g_aes128_ctr.set_key(m_info, (const cipher_iv *)(m_info + 32));
    p = (unsigned char *)base + off;
    if (g_aes128_ctr.decrypt(p, end, p))
        return 35;
    if (m_meta->sig_type != 1)
        return 32;

    ecdsa_verifier v(pub, *curve);

    rc = 35;
    if (v.verify(base, m_meta->signed_len, base + m_meta->signed_len) == 0) {
        m_ext = (const sce_ext_header *)(base + 32);
        m_app = (const auth_app_info *)(base + m_ext->app_offset);
        m_section_info = base + m_ext->section_info_offset;
        rc = 36;
        if (check_type(type) == 0) {
            m_sections = (const auth_section *)(base + m_sce->metadata_offset + 128);
            m_keys = (const unsigned char *)(m_sections + m_meta->section_count);
            rc = 32;
            if (m_elf.load(base + m_ext->elf_offset) == 0) {
                m_loaded = true;
                rc = 0;
            }
        }
    }
    return rc;
}

aes256_cbc_cipher g_aes256_cbc;
aes128_ctr_cipher g_aes128_ctr;
hmac_digest g_hmac;
unsigned char g_auth_area[sizeof(authenticator)] __attribute__((aligned(16)));
