#include "loader.h"
#include "mmio.h"
#include "be_mmio.h"
#include "util.h"
#include "auth.h"

loader::loader()
{
}

void loader::finalize(bool fatal, long rc)
{
    loader_base::finalize(fatal, rc);
}

long loader::find_lv0_ros(u64 base, u64 *lv0, u64 *lv0_2)
{
    u64 ros;
    long rc;

    u64 toc = copy_to_main_memory(base, 992);
    if (toc == (u64)-1)
        return -1;
    if (get_flash_layout() == 3) {
        u64 off, size;

        rc = find_file(toc, get_os_bank_indicator() == 0 ? "ros0" : "ros1", &off, &size);
        if (rc != 0)
            return rc;
        ros = off;
    } else {
        u64 off, size;
        u64 hdr[4];

        rc = find_file(toc, "ros", &off, &size);
        if (rc != 0)
            return rc;
        off = off - toc + base;
        u64 p = copy_to_main_memory(off, 32);
        if (p == (u64)-1)
            return -1;
        copy_qwords_from_mmio(p, (char *)hdr, 32);
        ros = off + hdr[0];
    }
    u64 q = copy_to_main_memory(ros, 992);
    if (q == (u64)-1)
        return -1;
    q += 16;

    u64 off, size;

    rc = find_file(q, "lv0", &off, &size);
    if (rc != 0)
        return rc;
    off = off - q + ros + 16;
    *lv0 = copy_to_main_memory(off, size);
    if (*lv0 == (u64)-1)
        return -1;

    u64 off2, size2;

    rc = find_file(q, "lv0.2", &off2, &size2);
    if (__builtin_expect(rc != 0, 0))
        return rc;
    off2 = off2 - q + ros + 16;
    *lv0_2 = copy_to_main_memory(off2, size2);
    if (*lv0_2 == (u64)-1)
        return -1;
    return rc;
}

long loader::find_lv0_flash(u64 base, u64 *lv0, u64 *lv0_2)
{
    if (get_flash_layout() > 0)
        return find_lv0_ros(base, lv0, lv0_2);
    return -1;
}

long loader::find_lv0(u64 *lv0, u64 *lv0_2)
{
    return find_lv0_flash(get_flash_rom_base(), lv0, lv0_2) != 0 ? -1 : 0;
}

long loader::run(void)
{
    u64 lv0, lv0_2;
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
    if (find_lv0(&lv0, &lv0_2) != 0) {
        rc = -1;
        post_lv0_not_found();
        return rc;
    }
    post_code_08();
    rc = load_lv0(lv0, lv0_2);
    if (rc != 0) {
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

void loader::print_banner(void)
{
    print("\n");
    print("Boot Loader SE Version 3.6.5 ");
    print("(Build ID: 4447,47393, ");
    print("Build Date: 2011-04-20_12:18:23)");
    print("\n");
    print("Copyright(C) 2011 Sony Computer Entertainment Inc.All Rights Reserved.");
    print("\n");
}
