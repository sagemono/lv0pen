#include "auth.h"
#include "loader.h"
#include "local_buffer.h"
#include "mailbox.h"
#include "util.h"

extern const unsigned int mldr_curve = 16;
unsigned char mldr_header_key[32] = {
    0x5b, 0x54, 0x8e, 0x7d, 0x5b, 0x37, 0x5b, 0x9c,
    0x43, 0xc1, 0xf1, 0xd8, 0x0b, 0x77, 0x12, 0x4b,
    0xf4, 0xc2, 0xae, 0x80, 0x19, 0xf9, 0x18, 0xee,
    0xa2, 0xa7, 0xfc, 0x21, 0x45, 0x9f, 0xdc, 0xa7,
};
unsigned char mldr_header_iv[16] = {
    0x3c, 0xaf, 0x87, 0xa6, 0xc0, 0x03, 0x9b, 0xa2,
    0x0f, 0xf0, 0x26, 0xc1, 0xc3, 0x0b, 0xd6, 0x66,
};
unsigned char mldr_public_key[40] = {
    0x19, 0x6a, 0x08, 0xdd, 0x39, 0x49, 0x98, 0xdd,
    0x39, 0xe2, 0x01, 0x49, 0x9f, 0x3d, 0x52, 0x6c,
    0x97, 0x51, 0x90, 0xf2, 0xe4, 0x76, 0xc4, 0xe3,
    0xed, 0x71, 0x6d, 0x2c, 0x5e, 0x05, 0x9e, 0xc3,
    0x32, 0x2a, 0x2b, 0x04, 0x46, 0x13, 0x30, 0xf8,
};

inline void *operator new(__SIZE_TYPE__, void *p) throw() { return p; }

loader::loader()
    : m_ea(0), m_auth(0)
{
}

long loader::check_version(void)
{
    auth_app_info info;

    if (m_auth->key_revision() != 0)
        return 32;
    if (m_auth->get_app_info(&info))
        return 32;
    if (info.version < 0x0003006500000000ULL)
        return 32;
    return 0;
}

long loader::load_header(void)
{
    tagged_dma_buffer b(0);
    dma_buffer *bufs[1];
    long rc;

    b.set_buffer(0x3E000, 0x2000);
    bufs[0] = &b;
    rc = m_auth->load_header(6, mldr_header_key, mldr_header_iv, mldr_public_key,
                             &mldr_curve, bufs, 1);
    memset(mldr_header_key, 0, sizeof(mldr_header_key));
    memset(mldr_header_iv, 0, sizeof(mldr_header_iv));
    return rc;
}

long loader::load_segments(unsigned int *entry)
{
    tagged_dma_buffer b0(0), b1(1);
    local_buffer lb;
    Elf32_Phdr ph;
    Elf32_Ehdr eh;
    u16 i;
    long r;

    r = m_auth->read_elf_header(&eh);
    if (r)
        return r;
    if (eh.e_type != 2)
        return 32;
    if (eh.e_machine != 23)
        return 32;
    if (eh.e_entry < 0x12C00 || eh.e_entry > 0x3C000)
        return 34;
    *entry = eh.e_entry;
    b0.set_buffer(0x3E000, 4096);
    b1.set_buffer(0x3F000, 4096);
    dma_buffer *in[2] = { &b0, &b1 };
    for (i = 0; i < eh.e_phnum; i++) {
        if (m_auth->read_program_header(i, &ph))
            return 32;
        if (ph.p_filesz == 0 || ph.p_type != 1)
            continue;
        if (ph.p_align > 1 && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
            return 33;
        if (ph.p_vaddr < 0x12C00 || ph.p_vaddr + ph.p_filesz > 0x3C000)
            return 34;
        lb.set_buffer(ph.p_vaddr & ~15, ph.p_filesz + (ph.p_vaddr & 15));
        dma_buffer *out[1] = { &lb };
        r = m_auth->load_segment(2, i, ph.p_vaddr, ph.p_filesz, in, 2, out, 1);
        if (r)
            return r;
    }
    return 0;
}

long loader::load(unsigned int *entry)
{
    long rc;

    m_auth->set_offset(m_ea);
    rc = load_header();
    if (rc)
        return rc;
    rc = check_version();
    if (rc)
        return rc;
    return load_segments(entry);
}

long loader::read_request(void)
{
    mbox_in_read64(&m_ea);
    return (m_ea & 0x7F) ? 30 : 0;
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
