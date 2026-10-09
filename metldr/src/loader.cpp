#include "auth.h"
#include "loader.h"
#include "local_buffer.h"
#include "mailbox.h"

extern const unsigned char mldr_header_key[32] = {
    0xc0, 0xce, 0xfe, 0x84, 0xc2, 0x27, 0xf7, 0x5b,
    0xd0, 0x7a, 0x7e, 0xb8, 0x46, 0x50, 0x9f, 0x93,
    0xb2, 0x38, 0xe7, 0x70, 0xda, 0xcb, 0x9f, 0xf4,
    0xa3, 0x88, 0xf8, 0x12, 0x48, 0x2b, 0xe2, 0x1b,
};
extern const unsigned char mldr_header_iv[16] = {
    0x47, 0xee, 0x74, 0x54, 0xe4, 0x77, 0x4c, 0xc9,
    0xb8, 0x96, 0x0c, 0x7b, 0x59, 0xf4, 0xc1, 0x4d,
};
extern const unsigned int mldr_curve = 32;
unsigned char mldr_public_key[40] = {
    0xc2, 0xd4, 0xaa, 0xf3, 0x19, 0x35, 0x50, 0x19,
    0xaf, 0x99, 0xd4, 0x4e, 0x2b, 0x58, 0xca, 0x29,
    0x25, 0x2c, 0x89, 0x12, 0x3d, 0x11, 0xd6, 0x21,
    0x8f, 0x40, 0xb1, 0x38, 0xca, 0xb2, 0x9b, 0x71,
    0x01, 0xf3, 0xae, 0xb7, 0x2a, 0x97, 0x50, 0x19,
};

inline void *operator new(__SIZE_TYPE__, void *p) throw() { return p; }

loader::loader()
    : m_ea(0), m_auth(0)
{
}

long loader::read_request(void)
{
    mbox_in_read64(&m_ea);
    return (m_ea & 0x7F) ? 30 : 0;
}

long loader::load_segments(unsigned int *entry)
{
    Elf32_Phdr ph;
    tagged_dma_buffer b0(0);
    tagged_dma_buffer b1(1);
    local_buffer seg;
    dma_buffer *in[2];
    dma_buffer *out[1];
    Elf32_Ehdr eh;
    unsigned short i;
    unsigned int n;
    long rc;

    rc = m_auth->read_elf_header(&eh);
    if (rc)
        return rc;
    *entry = eh.e_entry;
    b0.set_buffer(0x3E000, 0x1000);
    b1.set_buffer(0x3F000, 0x1000);
    in[0] = &b0;
    in[1] = &b1;
    for (i = 0; i < (n = eh.e_phnum); i++) {
        if (m_auth->read_program_header(i, &ph))
            return 32;
        if (ph.p_filesz == 0)
            continue;
        if (((u64)ph.p_offset & 15) != ((u64)ph.p_vaddr & 15))
            return 33;
        if (ph.p_type != 1)
            continue;
        seg.set_buffer(ph.p_vaddr, ph.p_filesz);
        out[0] = &seg;
        if (ph.p_vaddr < 0x12C00)
            return 34;
        rc = m_auth->load_segment(2, i, ph.p_vaddr, in, 2, out, 1);
        if (rc)
            return rc;
    }
    return 0;
}

long loader::load_header(void)
{
    tagged_dma_buffer b(0);
    dma_buffer *bufs[1];

    b.set_buffer(0x3E000, 0x2000);
    bufs[0] = &b;
    return m_auth->load_header(6, mldr_header_key, mldr_header_iv, mldr_public_key,
                               &mldr_curve, bufs, 1);
}

long loader::load(unsigned int *entry)
{
    auth_app_info info;
    long rc;

    m_auth->set_offset(m_ea);
    rc = load_header();
    if (rc)
        return rc;
    if (m_auth->get_app_info(&info))
        return 32;
    return load_segments(entry);
}

long loader::run(unsigned int *entry)
{
    long rc;

    rc = read_request();
    if (rc)
        return rc;
    m_auth = new (g_auth_area) authenticator;
    rc = load(entry);
    if (rc)
        return rc;
    if (mbox_out_write(1))
        return 21;
    if (mbox_out_intr_write(1))
        return 31;
    return 0;
}
