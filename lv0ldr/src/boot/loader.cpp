#include "loader.h"
#include "mmio.h"
#include "be_mmio.h"
#include "util.h"
#include "auth.h"

loader::loader()
{
}

void loader::finalize(bool fatal)
{
    loader_base::finalize(fatal);
}

long loader::find_lv0(u64 *lv0)
{
    long (loader::*first)(u64, u64 *);
    long (loader::*second)(u64, u64 *);
    u64 base = get_flash_rom_base();

    if (get_update_flag()) {
        first = &loader::find_lv0_update;
        second = &loader::find_lv0_flash;
    } else {
        first = &loader::find_lv0_flash;
        second = &loader::find_lv0_update;
    }
    if (is_force_update()) {
        first = &loader::find_lv0_update;
        second = &loader::find_lv0_flash;
    }
    if ((this->*first)(base, lv0) != 0) {
        if ((this->*second)(base, lv0) != 0)
            return -1;
    }
    return 0;
}

long loader::run(void)
{
    u64 lv0;
    long rc;

    initialize();
    rc = check_config_ring();
    if (rc != 0)
        return rc;
    post_code_aa();
    print_banner();
    rc = init_memory();
    if (rc != 0)
        return rc;
    if (find_lv0(&lv0) != 0) {
        rc = -1;
        post_lv0_not_found();
        return rc;
    }
    post_code_08();
    if (load_lv0(lv0) != 0) {
        rc = -1;
        post_lv0_auth_fail();
        return rc;
    }
    post_code_30();
    handoff();
    return rc;
}

long loader::find_file(u64 toc, const char *name, u64 *off, u64 *size)
{
    ros_toc_header hdr;
    ros_toc_entry e;

    if (name[0] == '\0')
        return -3;
    if (ros_read_header(toc, &hdr) != 0)
        return -1;
    if (ros_find_entry(toc + 16, hdr.count, name, &e) != 0)
        return -1;
    *off = toc + e.offset;
    *size = e.size;
    return 0;
}

long loader::find_lv0_ros(u64 base, u64 *lv0)
{
    u64 ros = base + 0x40200;
    u64 off, size, size2, off2;
    u64 hdr[4];
    long rc;

    u64 toc = copy_to_main_memory(ros, 992);
    if (toc == (u64)-1)
        return -1;
    rc = find_file(toc, "ros", &off, &size);
    if (rc != 0)
        return rc;
    off = off - toc + ros;
    u64 p = copy_to_main_memory(off, 32);
    if (p == (u64)-1)
        return -1;
    copy_qwords_from_mmio(p, (char *)hdr, 32);
    u64 q = copy_to_main_memory(off + hdr[0], 992);
    if (q == (u64)-1)
        return -1;
    q += 16;
    rc = find_file(q, "lv0", &off2, &size2);
    if (rc != 0)
        return rc;
    off2 = off2 - q + off + hdr[0] + 16;
    *lv0 = copy_to_main_memory(off2, size2);
    if (*lv0 == (u64)-1)
        return -1;
    return rc;
}

long loader::find_updater(u64 base, u64 *lv0)
{
    u64 off, size;

    u64 toc = copy_to_main_memory(base, 1024);
    if (toc == (u64)-1)
        return -1;
    if (find_file(toc, "updater", &off, &size) != 0)
        return -1;
    *lv0 = copy_to_main_memory(off, size);
    if (*lv0 == (u64)-1)
        return -1;
    return 0;
}

void loader::print_banner(void)
{
    print("\n");
    print("Boot Loader SE Version 1.0.0 ");
    print("(Build ID: 1673,16934, ");
    print("Build Data: 2006-10-30_12:39:57)");
    print("\n");
    print("Copyright(C) 2006 Sony Computer Entertainment Inc.All Rights Reserved.");
    print("\n");
}

long loader::find_lv0_bank(u64 base, u64 *lv0)
{
    u64 ind_off, ind_size, bank_off, bank_size, off, size;

    u64 toc = copy_to_main_memory(base, 1024);
    if (toc == (u64)-1)
        return -1;
    if (find_file(toc, "bank_indicator", &ind_off, &ind_size) != 0)
        return -1;
    u64 ind = copy_to_main_memory(ind_off, ind_size);
    if (ind == (u64)-1)
        return -1;
    const char *name = (read64(ind) & 0x8000000000000000ULL) ? "lv0_bank0" : "lv0_bank1";
    if (find_file(toc, name, &bank_off, &bank_size) != 0)
        return -1;
    u64 bank = copy_to_main_memory(bank_off, 1024);
    if (bank == (u64)-1)
        return -1;
    if (find_file(bank, "lv0", &off, &size) != 0)
        return -1;
    *lv0 = copy_to_main_memory(off, size);
    if (*lv0 == (u64)-1)
        return -1;
    return 0;
}

long loader::find_lv0_flash(u64 base, u64 *lv0)
{
    if (get_flash_layout() == 1)
        return find_lv0_ros(base, lv0);
    return find_lv0_bank(base, lv0);
}

long loader::find_lv0_update(u64 base, u64 *lv0)
{
    if (get_flash_layout() == 1)
        return -1;
    return find_updater(base, lv0);
}

void loader::halt(void)
{
    spu_writech(64, 0);
    spu_writech(64, 2);
}

void loader::start(void)
{
    spu_writech(64, 0x20000);
    write32(be_mmio_base + 0x509C20, ~1);
    spu_writech(64, 2);
}

void loader::handoff(void)
{
    unsigned char arg[4];

    write32(0, 0x500);
    int ver1 = is_sc_protocol_ver1();
    memset(arg, 0, 4);
    arg[1] = 1;
    arg[0] = ver1;
    write32(4, *(u32 *)arg);
}

long loader::load_lv0(u64 off)
{
    long rc;

    g_auth.set_offset(off);
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
    {
        tagged_dma_buffer b0(0), b1(1), b2(2), b3(3);
        dma_buffer *in[2];
        dma_buffer *out[2];
        Elf64_Ehdr ehdr;
        Elf64_Phdr phdr;
        unsigned int err;

        in[0] = &b0;
        in[1] = &b1;
        out[0] = &b2;
        out[1] = &b3;
        goto read;
    check:
        if (ehdr.e_type != 2) {
            err = 29;
            goto done;
        }
        for (unsigned int i = 0; i < ehdr.e_phnum; i++) {
            rc = g_auth.read_program_header(i, &phdr);
            if (rc)
                return rc;
            if (phdr.p_filesz == 0)
                continue;
            if ((phdr.p_offset & 0xFFFF) != (phdr.p_vaddr & 0xFFFF)) {
                err = 29;
                goto done;
            }
            if (phdr.p_type != 1)
                continue;
            b0.set_buffer(0x3E000, 2048);
            b1.set_buffer(0x3E800, 2048);
            b2.set_buffer(0x3F000, 2048);
            b3.set_buffer(0x3F800, 2048);
            rc = g_auth.load_segment(2, i, phdr.p_vaddr, in, 2, out, 2);
            if (rc)
                return rc;
        }
        err = 0;
    done:
        return err;
    read:
        rc = g_auth.read_elf_header(&ehdr);
        if (rc == 0)
            goto check;
        return rc;
    }
}
