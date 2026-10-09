#include "platform.h"
#include "lv0.h"
#include "spu.h"
#include "component.h"
#include "config.h"
#include "memory.h"
#include "log.h"

extern componnet_manager *g_componnet_manager_ptr;

extern void boot_debug_hook(void) __attribute__((weak));

#define rol64(v, n) ({ u64 v_ = (v); (v_ << (n)) | (v_ >> (64 - (n))); })
#define ror64(v, n) ({ u64 v_ = (v); (v_ >> (n)) | (v_ << (64 - (n))); })

inline long boot_loader::run()
{
    platform *plat;
    unsigned char buf[256];

    if (__builtin_expect(init_device_0(), 0)) {
        log_error(LV0_ERR_INTERNAL, "init_device_0 fail");
    } else {
        plat = g_platform_ptr;
        plat->post_init_done();
        if (__builtin_expect(init_device_1(), 0))
            log_error(LV0_ERR_INTERNAL, "init_device_1 fail\n");
    }

    if (__builtin_expect(g_componnet_manager_ptr->initialize(g_component_loader_ptr), 0))
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x component_manager::initialize\n", LV0_ERR_INTERNAL);
    if (__builtin_expect(component_init(), 0))
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x component_init\n", LV0_ERR_INTERNAL);

    print_boot_banner();

    plat->print_system_info();
    if (!get_flash_format()
        && !plat->stub_slot4(buf, sizeof(buf)))
        plat->stub_slot5(buf);

    if (boot_debug_hook)
        boot_debug_hook();
    return load_and_boot_lv1();
}

boot_loader::boot_loader()
{
    g_application_ptr = this;
}

int boot_loader::component_init()
{
    unsigned long addr, size;
    int rc;

    if (get_component_info_table()->clear())
        return -1;
    unsigned int ids[] = { COMPONENT_LV1LDR, COMPONENT_LV2LDR, COMPONENT_APPLDR, COMPONENT_ISOLDR,
                           COMPONENT_EID0, 19 };
    if (g_componnet_manager_ptr->get_component(COMPONENT_METLDR_2, &addr, &size) == 0) {
        rc = g_componnet_manager_ptr->overwrite_2nd_component();
        if (__builtin_expect(rc != 0, 0)) {
            log_message("[ERROR]: 0x%08x overwrite_2nd_component %d\n", LV0_ERR_INTERNAL, rc);
            return -1;
        }
    } else {
        rc = g_componnet_manager_ptr->load_2nd_component();
        if (rc) {
            log_message("[ERROR]: 0x%08x load_2nd_component %d\n", LV0_ERR_INTERNAL, rc);
            return -1;
        }
        rc = g_componnet_manager_ptr->get_component(COMPONENT_METLDR, &addr, &size);
        if (__builtin_expect(rc != 0, 0)) {
            log_message("[ERROR]: 0x%08x get_component %d\n", LV0_ERR_INTERNAL, rc);
            return -1;
        }
    }
    get_component_info_table()->set(COMPONENT_METLDR, addr, size);

    for (unsigned int i = 0; ids[i] != 19; i++) {
        if (g_componnet_manager_ptr->get_component(ids[i], &addr, &size))
            log_message("[WARRING]: 0x%08x get_component %d\n", LV0_ERR_INTERNAL, ids[i]);
        else
            get_component_info_table()->set(ids[i], addr, size);
    }

    if (get_component_info_table()->get(COMPONENT_LV1LDR, &addr, &size)) {
        log_message("[ERROR]: 0x%08x lv1ldr get fail.\n", LV0_ERR_INTERNAL);
        return -1;
    }
    decrypt_lv1ldr((void *)addr, size);

    unsigned long *qa_flag = (unsigned long *)g_memory_budget_low_ptr->allocate(8, 0x10);
    if (__builtin_expect(!qa_flag, 0)) {
        log_message("[ERROR]: 0x%08x allocate is_qa_flag\n", LV0_ERR_INTERNAL);
        return -1;
    }
    *qa_flag = is_qaf_enabled();
    size = 8;
    get_component_info_table()->set(COMPONENT_LV1LDR_2, (unsigned long)qa_flag, size);

    void *qa_token = g_memory_budget_low_ptr->allocate(0x80, 0x10);
    if (__builtin_expect(!qa_token, 0)) {
        log_message("[ERROR]: 0x%08x allocate get_qa_flag\n", LV0_ERR_INTERNAL);
        return -1;
    }
    if (read_qa_token(qa_token)) {
        log_message("[WARRING]: 0x%08x get_qa_flag\n", LV0_ERR_CONFIG);
    } else {
        size = 0x80;
        get_component_info_table()->set(COMPONENT_LV2LDR_2, (unsigned long)qa_token, size);
    }

    unsigned long *trace_level = (unsigned long *)g_memory_budget_low_ptr->allocate(8, 0x10);
    if (__builtin_expect(!trace_level, 0)) {
        log_message("[ERROR]: 0x%08x allocate trace_level\n", LV0_ERR_INTERNAL);
        return -1;
    }
    *trace_level = log_get_threshold();
    size = 8;
    get_component_info_table()->set(COMPONENT_APPLDR_2, (unsigned long)trace_level, size);
    return 0;
}

long boot_loader::load_and_boot_lv1()
{
    unsigned long lv1_addr;
    unsigned int stop_code;
    volatile u64 hid1;
    const char *lv1_name;
    int rc;
    platform *plat = g_platform_ptr;

    if (g_componnet_manager_ptr->get_component(COMPONENT_LV1_SELF, &lv1_addr, 0)) {
        if (__builtin_expect(g_componnet_manager_ptr->get_component_name(COMPONENT_LV1_SELF, &lv1_addr, &lv1_name) != 0, 1))
            log_error(LV0_ERR_BOOT_LV1, "[ERROR]: 0x%08x unknown lv1 file not found\n", LV0_ERR_BOOT_LV1);
        log_error(LV0_ERR_BOOT_LV1, "[ERROR]: 0x%08x %s not found\n", LV0_ERR_BOOT_LV1, lv1_name);
    }

    plat->post_load_lv1_start();
    stop_code = 0;
    rc = boot_cell_os(lv1_addr, &stop_code);
    if (rc) {
        plat->post_load_lv1_failed();
        log_error(stop_code ? LV0_ERR_LV1LDR_STOP : LV0_ERR_BOOT_LV1, "load lv1 fail\n");
    }
    plat->post_load_lv1_done();

    setup_boot_parm(get_boot_parm());
    boot_parm_nop(get_boot_parm());
    install_ext_file_hooks();
    finalize_devices();

    __asm__ volatile ("mfspr %0, 0x3F1" : "=r"(hid1));
    hid1 = ror64(rol64(hid1, 6) & 0x7FFFFFFFFFFFFFFFULL, 6);
    __asm__ volatile ("mtspr 0x3F1, %0" :: "r"(hid1));
    hid1 |= 0x0200000000000000ULL;
    __asm__ volatile ("mtspr 0x3F1, %0" :: "r"(hid1));
    hid1 = ror64(rol64(hid1, 6) & 0x7FFFFFFFFFFFFFFFULL, 6);
    __asm__ volatile ("mtspr 0x3F1, %0" :: "r"(hid1));
    __asm__ volatile ("sync");

    __asm__ volatile ("li 3,256\n\tmtctr 3\n\tbctr\n\tnop" ::: "r3");
}

void boot_loader::print_boot_banner()
{
    char buf[64];
    long used_size, free_size;

    log_message("%s%s%s\n", "Boot Loader SE Version 4.9.3 ", "(Build ID: 5382,50774, ", "Build Date: 2026-01-08_15:01:05)");
    if (read_sdk_version(buf, sizeof(buf)))
        buf[0] = 0;
    log_message("SDK Version: %s\n", buf);
    log_message("%s\n", "Copyright(C) 2026 Sony Computer Entertainment Inc.All Rights Reserved.");
    g_memory_budget_low_ptr->get_used_size(&used_size);
    g_memory_budget_low_ptr->get_free_size(&free_size);
}

boot_loader g_boot_loader;

long g_unused_41648 = -1;
