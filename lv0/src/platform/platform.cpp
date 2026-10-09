#define PLATFORM_UNIT __attribute__((visibility("hidden")))

#include "platform.h"
#include "sb.h"
#include "console.h"
#include "iommu.h"
#include "config.h"
#include "clock.h"
#include "spu.h"
#include "log.h"
#include "syscon.h"
#include "static_init.h"
#include "component.h"
#include "memory.h"
#include "mmio.h"
#include "storage.h"

#define BE_PMD_TBR      0x509890
#define BE_PMD_C90      0x509C90
#define BE_IOC_REGS     0x510000

struct pio;

extern platform g_platform;
unsigned long g_unused_416e8[2] = { 0x2000000000000000, 0x1100000000000000 };
const unsigned char tbr_divider_lfsr_table[256] = {
    0x00, 0xFF, 0x7F, 0x3F, 0x9F, 0x4F, 0x27, 0x13, 0x09, 0x84, 0x42, 0xA1,
    0x50, 0x28, 0x94, 0xCA, 0xE5, 0xF2, 0xF9, 0x7C, 0xBE, 0x5F, 0xAF, 0x57,
    0xAB, 0x55, 0xAA, 0xD5, 0xEA, 0x75, 0x3A, 0x1D, 0x0E, 0x07, 0x83, 0xC1,
    0x60, 0x30, 0x18, 0x8C, 0x46, 0xA3, 0x51, 0xA8, 0xD4, 0x6A, 0x35, 0x9A,
    0xCD, 0x66, 0x33, 0x99, 0x4C, 0xA6, 0xD3, 0xE9, 0xF4, 0xFA, 0xFD, 0x7E,
    0xBF, 0xDF, 0xEF, 0xF7, 0x7B, 0x3D, 0x9E, 0xCF, 0x67, 0xB3, 0xD9, 0xEC,
    0x76, 0xBB, 0xDD, 0xEE, 0x77, 0x3B, 0x9D, 0x4E, 0xA7, 0x53, 0xA9, 0x54,
    0x2A, 0x95, 0x4A, 0xA5, 0x52, 0x29, 0x14, 0x8A, 0x45, 0x22, 0x91, 0x48,
    0xA4, 0xD2, 0x69, 0xB4, 0x5A, 0x2D, 0x16, 0x8B, 0xC5, 0x62, 0x31, 0x98,
    0xCC, 0xE6, 0x73, 0x39, 0x9C, 0xCE, 0xE7, 0xF3, 0x79, 0x3C, 0x1E, 0x8F,
    0xC7, 0x63, 0xB1, 0xD8, 0x6C, 0x36, 0x1B, 0x0D, 0x86, 0x43, 0x21, 0x10,
    0x88, 0x44, 0xA2, 0xD1, 0xE8, 0x74, 0xBA, 0x5D, 0xAE, 0xD7, 0xEB, 0xF5,
    0x7A, 0xBD, 0xDE, 0x6F, 0xB7, 0xDB, 0xED, 0xF6, 0xFB, 0x7D, 0x3E, 0x1F,
    0x0F, 0x87, 0xC3, 0x61, 0xB0, 0x58, 0x2C, 0x96, 0xCB, 0x65, 0xB2, 0x59,
    0xAC, 0xD6, 0x6B, 0xB5, 0xDA, 0x6D, 0xB6, 0x5B, 0xAD, 0x56, 0x2B, 0x15,
    0x0A, 0x05, 0x82, 0x41, 0x20, 0x90, 0xC8, 0xE4, 0x72, 0xB9, 0xDC, 0x6E,
    0x37, 0x9B, 0x4D, 0x26, 0x93, 0x49, 0x24, 0x92, 0xC9, 0x64, 0x32, 0x19,
    0x0C, 0x06, 0x03, 0x81, 0xC0, 0xE0, 0x70, 0xB8, 0x5C, 0x2E, 0x97, 0x4B,
    0x25, 0x12, 0x89, 0xC4, 0xE2, 0x71, 0x38, 0x1C, 0x8E, 0x47, 0x23, 0x11,
    0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0xA0, 0xD0, 0x68, 0x34, 0x1A, 0x8D,
    0xC6, 0xE3, 0xF1, 0x78, 0xBC, 0x5E, 0x2F, 0x17, 0x0B, 0x85, 0xC2, 0xE1,
    0xF0, 0xF8, 0xFC, 0xFE,
};

int platform::set_post_code(long mask, long code)
{
    unsigned char pio_val = get_pio()->read_output_byte();
    get_pio()->write_output_byte((unsigned char)((pio_val & mask) | code));
    unsigned char sb_val = get_sb_device()->read8_1000039();
    return get_sb_device()->write8_100003a((unsigned char)((sb_val & mask) | code));
}

void platform::post_load_lv1_failed() { set_post_code(0xC0, 0x32); }

void platform::post_load_lv1_done() { set_post_code(0xC0, 0x10); }

void platform::post_load_lv1_start() { set_post_code(0xC0, 0xE); }

void platform::post_init_done() { set_post_code(0xC0, 9); }

int platform::stub_slot4(void *buf, long size)
{
    return -1;
}

long platform::stub_slot5(void *buf)
{
    return -1;
}

long platform::stub_slot6()
{
    return -1;
}

void platform::finalize()
{
    unsigned long tb;
    unsigned int *tbw = (unsigned int *)&tb;

    if ((int)log_get_threshold() > 1)
        get_log_ring()->dump(0);
    get_iommu_context()->free_io_address(dma_io_addr);
    physical_console::finalize();
    *(long *)(be_mmio_base + BE_PMD_C90) = 0;
    get_iommu_context()->disable();
    read_eeprom_nv4_u64_at5((unsigned long *)tbw);
    if (tb != (unsigned long)-1) {
        tb &= 0x3FFFFFFFFFFFFFFFUL;
        __asm__ volatile ("mttbl %0" : : "r"(tbw[1]));
        __asm__ volatile ("mttbu %0" : : "r"(tbw[0]));
        __asm__ volatile ("mttbl %0" : : "r"(tbw[1]));
    }
}

void platform::print_memory_info(int level)
{
    int sev;
    unsigned long xio_mask, size_mb;

    switch (level) {
    case 0: sev = 0; break;
    case 1: sev = 3; break;
    default: sev = 1; break;
    }

    log_printf(sev, 1, "[INFO]: === eXtreme Data Rate Memory Subsystem ===\n");
    xio_mask = get_xio_channel_mask();
    if (xio_mask & 1)
        size_mb = get_xio_channel_size_mb(0);
    else if (xio_mask & 2)
        size_mb = get_xio_channel_size_mb(1);
    else
        size_mb = 0;
    log_printf(sev, 1, "[INFO]: (Configured Memory Size per single XIO channel: %d MBytes.)\n", size_mb);
    log_printf(sev, 1, "[INFO]: XIO channel[0] is ");
    log_printf(sev, 1, (xio_mask & 1) ? "available.\n" : "*NOT* available.\n");
    log_printf(sev, 1, "[INFO]: XIO channel[1] is ");
    log_printf(sev, 1, (xio_mask & 2) ? "available.\n" : "*NOT* available.\n");
    log_printf(sev, 1, "[INFO]: ---> Total %d MBytes are now in use.\n", get_total_memory_size() >> 20);
}

void platform::print_chip_revisions(long level)
{
    int be_rev = get_be_revision();
    unsigned int sb_rev = get_sb_revision();

    switch ((unsigned int)be_rev) {
    case 0x10:  log(level, "[INFO]: BE:1.0, "); break;
    case 0x11:  log(level, "[INFO]: BE:1.1, "); break;
    case 0x12:  log(level, "[INFO]: BE:1.2, "); break;
    case 0x20:  log(level, "[INFO]: BE:2.0, "); break;
    case 0x30:  log(level, "[INFO]: BE:3.0, "); break;
    case 0x31:  log(level, "[INFO]: BE:3.1, "); break;
    case 0x32:  log(level, "[INFO]: BE:3.2, "); break;
    case 0x110: log(level, "[INFO]: BE:11S DD1.0, "); break;
    case 0x210: log(level, "[INFO]: BE:12S DD1.0, "); break;
    case 0x220: log(level, "[INFO]: BE:12S DD2.0, "); break;
    default:    log(level, "[INFO]: BE:unknown, "); break;
    }

    switch (sb_rev) {
    case 0x10:  log(level, "SB:DX1.0\n"); return;
    case 0x11:  log(level, "SB:DX1.1\n"); return;
    case 0x20:  log(level, "SB:DX2.0\n"); return;
    case 0x30:  log(level, "SB:DX3.0\n"); return;
    case 0x31:  log(level, "SB:DX3.1\n"); return;
    case 0x32:  log(level, "SB:DX3.2\n"); return;
    case 0x110: log(level, "SB:PX1.0\n"); return;
    case 0x120: log(level, "SB:PX1.1\n"); return;
    case 0x210: log(level, "SB:SX1.0\n"); return;
    case 0x220: log(level, "SB:SX1.1\n"); return;
    case 0x230: log(level, "SB:SX1.2\n"); return;
    case 0x310: log(level, "SB:ZX1.0\n"); return;
    case 0x320: log(level, "SB:ZX1.1\n"); return;
    case 0x330: log(level, "SB:ZX1.2\n"); return;
    default:    log(level, "SB:unknown\n"); return;
    }
}

void platform::print_spu_enable(long level)
{
    long fault_mask = calc_spu_faultbm_lo();
    bool first = true;
    long i;

    log(level, "[INFO]: SPU enable [");
    for (i = 0; i != 8; ++i) {
        if (((0x80UL >> i) & fault_mask) == 0) {
            if (first) {
                log(level, "%d", (unsigned int)i);
                first = false;
            } else {
                log(level, ", %d", (unsigned int)i);
            }
        }
    }
    log(level, "] ");
    for (int j = 0; j < 8; j++) {
        if (((0x80UL >> j) & get_spu_fault_mask()) == 0)
            log(level, "1");
        else
            log(level, "0");
    }
    log(level, "\n");
}

int write_eeprom_nv1_u32_at8(unsigned int value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 8, 4, &value, entry->base);
    if (!rc)
        *(unsigned int *)(entry->data.buf + 8) = value;
    return rc;
}

long handle_fatal_error(unsigned int code)
{
    get_log_ring()->dump(0);
    if (code != LV0_ERR_CONFIG) {
        unsigned int pm = 0;
        syscon_syspm_get_33(&pm);
        write_eeprom_nv1_u32_at8(pm);
        write_eeprom_nv1_u32_at4((code << 24) | 0xFFFFFF);
    }
    syscon_set_wake_source(820);
    syscon_power_off_with_code(0, 0, 3);
}

void platform::log(unsigned int level, const char *fmt, ...)
{
    DEAD_STRING(build_date, "Build Date: 2026-01-08_15:00:58)");
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    int sev;
    switch (level) {
    case 1: sev = 3; break;
    case 2: sev = 1; break;
    default: sev = 0; break;
    }
    log_dispatch(sev, 1, fmt, ap);
    __builtin_va_end(ap);
}

platform g_platform;

long platform::initialize()
{
    char trace_buf[16];
    bool proto_ver = false;

    long log_buf = (long)g_memory_budget_high_ptr->allocate(0x2000, 0x10);
    lv0_memset((void *)log_buf, 0, 0x2000);
    get_log_ring()->initialize(log_buf, 0x2000, (region_fn)syscon_write_buffer);

    log_set_handler(0, write_log_to_log_ring);
    log_set_handler(1, write_log_to_log_ring);
    log_set_final_handler(handle_fatal_error);

    pio *pio = get_pio();
    long sb_base = sb_mmio_base;
    pio->configure((char *)sb_base, 0xFF00, 0xFFA1);
    get_uart()->init((volatile unsigned int *)sb_base, 23, 0, 0, 0, 0);

    if (*(unsigned int *)0 > 0x4FF)
        proto_ver = *(unsigned char *)4 == 1;

    get_syscon_device()->initialize(sb_mmio_base, proto_ver, 0);

    log_set_handler(2, syscon_write_string);

    long rc = get_eeprom_bootrom_trace_leve((unsigned char *)trace_buf);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_bootrom_trace_leve fail %d\n", LV0_ERR_CONFIG, rc);
    switch (trace_buf[0]) {
    case 0: log_set_threshold(0); break;
    case 2: log_set_threshold(2); break;
    case 3: log_set_threshold(3); break;
    default: log_set_threshold(1); break;
    }

    unsigned long ref_clk = get_reference_clock();
    unsigned long ref_div = ref_clk / 79800000;
    if (ref_div > 0xFF)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x Invalid be_ref_clk %08x or ref_div %08llx\n", LV0_ERR_CONFIG, ref_clk, ref_div);
    *(volatile long *)(be_mmio_base + BE_PMD_TBR) = tbr_divider_lfsr_table[ref_div];
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        get_sb_device()->initialize(sb_mmio_base, 1);
    return 0;
}

long platform::print_system_info()
{
    print_memory_info(0);
    print_spu_enable(0);
    print_chip_revisions(0);
    return get_debug_interface();
}

int platform::initialize_io()
{
    long iost_buf, iopt_buf, io_addr;
    char *io_buf;
    iommu_context *iommu;
    unsigned long ioc_base, ioc_regs;
    int mode;

    iost_buf = (long)g_memory_budget_high_ptr->allocate(0x400, 0x1000);
    iopt_buf = (long)g_memory_budget_high_ptr->allocate(0x3000, 0x1000);
    if (!iost_buf || !iopt_buf)
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x iost/iopt buffer allocate fail\n", LV0_ERR_INTERNAL);
    iommu = get_iommu_context();
    ioc_base = (unsigned long)be_mmio_base;
    ioc_regs = ioc_base + BE_IOC_REGS;
    iommu->initialize(ioc_base, iost_buf, 0x400, iopt_buf, 0x3000);
    io_buf = (char *)g_memory_budget_high_ptr->allocate(0x110000, 0x10000);
    io_addr = get_iommu_context()->allocate_io_address(0, (long)io_buf, 0x110000, 3, 3, 1, 3, 1);
    dma_io_addr = io_addr;
    init_nand_flash_storage(io_buf, io_addr, 0x100080);
    *(unsigned long *)(ioc_regs - (BE_IOC_REGS - BE_PMD_C90)) = 1;
    if (physical_console::initialize())
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x physical_console::initialize\n", LV0_ERR_INTERNAL);
    mode = get_debug_interface();
    switch (mode) {
    case -1:
        log_set_handler(0, 0);
        break;
    case 3:
    case 5:
        log_set_handler(0, uart_console_puts);
        break;
    case 6:
        log_set_handler(0, write_log_to_log_ring);
        break;
    default:
        init_log_cons_ctl_and_data();
        internal_console::initialize();
        log_set_handler(3, (log_handler_fn *)log_to_internal_console);
        log_set_handler(0, (log_handler_fn *)log_to_internal_console);
        break;
    }
    select_component_loader();
    return 0;
}
