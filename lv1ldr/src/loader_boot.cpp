#include "types.h"
#include "loader.h"
#include "auth.h"
#include "util.h"
#include "dma_stream.h"
#include "mmio.h"

extern const unsigned char request_token[16];

long loader::load_segments()
{
    tagged_dma_buffer b0(0), b1(1), b2(2), b3(3);
    dma_buffer *in[2] = { &b0, &b1 };
    dma_buffer *out[2] = { &b2, &b3 };
    Elf64_Ehdr eh;
    Elf64_Phdr ph;
    unsigned int i;
    long rc;

    rc = m_auth->read_elf_header(&eh);
    if (rc)
        return rc;
    if (eh.e_type != 2)
        return 29;
    for (i = 0; i < eh.e_phnum; i++) {
        rc = m_auth->read_program_header(i, &ph);
        if (rc)
            return rc;
        if (ph.p_filesz == 0 || ph.p_type != 1)
            continue;
        if (ph.p_align > 1 && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
            return 29;
        if (ph.p_vaddr < m_req.arg_70) {
            if (ph.p_vaddr + ph.p_filesz >= m_req.arg_70)
                return 12;
        } else if (ph.p_vaddr < m_req.arg_78)
            return 12;
        b0.set_buffer(0x3E000, 0x800);
        b1.set_buffer(0x3E800, 0x800);
        b2.set_buffer(0x3F000, 0x800);
        b3.set_buffer(0x3F800, 0x800);
        rc = m_auth->load_segment(2, i, ph.p_vaddr, ph.p_filesz, in, 2, out, 2);
        if (rc)
            return rc;
    }
    return 0;
}

long loader::check_request_token(void)
{
    long r = 0;
    int i;

    if (m_req.token_len != 16)
        r = 49;
    for (i = 0; i < 16; i++)
        if (request_token[i] != m_req.token[i])
            r = 49;
    memset(m_req.token, 0, 16);
    return r;
}

long loader::auth_request(void)
{
    tagged_dma_buffer buf(0);
    dma_buffer *bufs[1];

    bufs[0] = &buf;
    buf.set_buffer(DMA_BUF, 8192);
    return m_auth->load_header(2, (const unsigned char *)0x37A80, (const unsigned char *)0x37AA0,
                               (const unsigned char *)0x2DE90, (const unsigned int *)0x2DE40,
                               bufs, 1);
}
