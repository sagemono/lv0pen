#include "auth.h"
#include "loader.h"
#include "mmio.h"
#include "be_mmio.h"
#include "util.h"

const unsigned char lv0_header_key[32] = {
    0x38, 0x41, 0x7a, 0x8a, 0xa9, 0xe1, 0x4e, 0x48, 0x5e, 0x2b, 0xb0, 0x05, 0x8f, 0x97, 0xd9, 0xf2,
    0x4c, 0x13, 0x8c, 0xdb, 0x2a, 0x2e, 0xf3, 0xc1, 0x4d, 0xf8, 0x87, 0x00, 0xce, 0x3a, 0xa8, 0xcd,
};
const unsigned char lv0_header_iv[16] = {
    0x08, 0x1a, 0x42, 0x7e, 0x02, 0x31, 0xca, 0x0c, 0x71, 0xcd, 0xb7, 0x48, 0x05, 0xc2, 0xb1, 0x47,
};
const unsigned int lv0_curve = 32;
unsigned char lv0_public_key[40] = {
    0x87, 0x33, 0x26, 0x2c, 0x88, 0xf2, 0x19, 0xdf, 0x75, 0x9c, 0x64, 0xfc, 0x75, 0xb5, 0x80, 0x9b,
    0x25, 0xe3, 0xae, 0x52, 0x2b, 0x4a, 0x42, 0x58, 0x3a, 0xaf, 0x8c, 0xdb, 0x9b, 0xf6, 0x37, 0xb2,
    0x01, 0xfc, 0xea, 0xc4, 0x24, 0x40, 0x00, 0xb7,
};

authenticator g_auth;

long loader::load_lv0(u64 lv0, u64 lv0_2)
{
    long rc;

    g_auth.set_offset(lv0_2);
    {
        tagged_dma_buffer buf(0);
        dma_buffer *bufs[1];

        bufs[0] = &buf;
        buf.set_buffer(0x3E000, 8192);
        rc = g_auth.load_header(1, lv0_header_key, lv0_header_iv, lv0_public_key,
                                &lv0_curve, bufs, 1);
        if (rc)
            return rc;
    }
    g_auth.set_offset(lv0);
    if (g_auth.key_revision() != 0)
        return 19;

    auth_app_info info;

    if (g_auth.get_app_info(&info))
        return 19;
    if (info.version < 0x0003006500000000ULL)
        return 19;
    {
        tagged_dma_buffer b0(0), b1(1), b2(2), b3(3);
        dma_buffer *in[2] = { &b0, &b1 };
        dma_buffer *out[2] = { &b2, &b3 };
        Elf64_Ehdr ehdr;
        Elf64_Phdr phdr;

        rc = g_auth.read_elf_header(&ehdr);
        if (rc)
            return rc;
        if (ehdr.e_type != 2)
            return 29;
        for (unsigned int i = 0; i < ehdr.e_phnum; i++) {
            long r = g_auth.read_program_header(i, &phdr);

            if (r)
                return r;
            if (phdr.p_filesz == 0 || phdr.p_type != 1)
                continue;
            if (phdr.p_align > 1 && phdr.p_offset % phdr.p_align != phdr.p_vaddr % phdr.p_align)
                return 29;
            b0.set_buffer(0x3E000, 2048);
            b1.set_buffer(0x3E800, 2048);
            b2.set_buffer(0x3F000, 2048);
            b3.set_buffer(0x3F800, 2048);
            r = g_auth.load_segment(2, i, phdr.p_vaddr, in, 2, out, 2);
            if (r)
                return r;
        }
    }
    return rc;
}

void loader::start(void)
{
    spu_writech(64, 0x20000);
    write32(be_mmio_base + 0x509C20, ~1);
    spu_writech(64, 2);
}

void loader::handoff(void)
{
    u32 v;
    unsigned char *p = (unsigned char *)&v;

    write32(0, 0x3600);
    int ver1 = is_sc_protocol_ver1();
    v = 0;
    p[0] = ver1;
    p[1] = 1;
    write32(4, v);
}

void loader::halt(void)
{
    spu_writech(64, 0);
    spu_writech(64, 2);
}

long loader_base::check_config_ring(void)
{
    config_ring ring;

    return ring.verify() != 0 ? -5 : 0;
}
