#include "syscon.h"
#include "sb.h"
#include "storage.h"
#include "iommu.h"
#include "iopt.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "loader.h"
#include "uart.h"
#include "nand_flash.h"
#include "systemio_dmac.h"
#include "xdr.h"

u64 g_ss2_work_offset = 0x80;

#define BE (0x20000000000ULL)

void sb_dx_write32_1c30(u64 base, u32 val)
{
    if (!is_sb_product_dx())
        return;
    write32(base + 0x1C30, val);
}

void sb_dx_write32_1c34(u64 base, u32 val)
{
    if (!is_sb_product_dx())
        return;
    write32(base + 0x1C34, val);
}

int init_nand_flash_dma(u64 flash_base, u64 buf, u64 size)
{
    if (g_dma_io_addr == (u64)-1) {
        iopt_codec codec;
        iopt_desc d;
        g_dma_io_addr = get_iommu_context()->allocate_io_address(0, buf, size, 3, 3, 1, 3, 1);
        d.addr = flash_base;
        d.w[2] = 0;
        d.w[3] = 2;
        d.w[4] = 2;
        d.w[5] = 1;
        d.w[6] = 0;
        d.w[7] = 0;
        d.w[8] = 0;
        d.w[9] = 0;
        d.w[10] = 1;
        d.w[11] = 0;
        codec.pack_entry(SB_MMIO_BASE, 0, &d);
        get_nand_flash_ctrl()->set_base(flash_base);
        if (is_sb_product_dx()) {
            write32(SB_MMIO_BASE + 0xFFE058, 0x22000);
            d.addr = 0x200000000ULL;
            d.w[2] = 0;
            d.w[3] = 63;
            d.w[5] = 0;
            d.w[6] = 3;
            d.w[10] = 1;
            codec.pack_entry(SB_MMIO_BASE, 7, &d);
            sb_dx_write32_1c30(SB_MMIO_BASE, 0x5000);
            sb_dx_write32_1c34(SB_MMIO_BASE, 0x80000003);
            set_nand_flash_ready_callback((nand_flash_ready_fn)poll_ss2_interrupt);
            get_systemio_dmac_dx()->configure(SB_MMIO_BASE + 0xFF0000, buf, g_dma_io_addr,
                                              0x500000000ULL, 0x200000000ULL);
        } else {
            set_nand_flash_ready_callback(poll_ebus_interrupt);
            get_systemio_dmac_px()->configure(SB_MMIO_BASE + 0xFF0000, flash_base + 0x44000, buf, g_dma_io_addr);
        }
    }
    return 0;
}

#define SB_ROM_BASE (SB_MMIO_BASE + 0x1FC00000)
#define SS2_WORK_BASE 0x1010000ULL

u64 loader_base::copy_to_main_memory(u64 src, unsigned int size)
{
    FUNCTION_NAME("copy_to_main_memory");
    u64 dst = src;
    if (is_boot_memory_type_nand()) {
        init_nand_flash_dma(SB_ROM_BASE, SS2_WORK_BASE, 0x1000000);
        if (src < SB_ROM_BASE) {
            log_message("[ERROR]: 0x%08x %s: ERROR src < config::sb_rom_base\n", LV0_ERR_INTERNAL, function_name);
            return -1;
        }
        u64 count = ((size + 511ULL) & 0xFFFFFE00U) >> 9;
        u64 sector = (src - SB_ROM_BASE) >> 9;
        if (g_ss2_work_offset > 0xFFFFFF) {
            log_message("[ERROR]: 0x%08x %s: ERROR SS2_WORK FULL offset(%016llx)\n", LV0_ERR_BOOT_LV1, function_name, g_ss2_work_offset);
            return -1;
        }
        long rc = nand_flash_ctrl::read_sector_dma(g_dma_io_addr + g_ss2_work_offset, sector, count);
        if (rc) {
            log_message("[ERROR]: 0x%08x %s: read_sectors_dma fail %d\n", LV0_ERR_FLASH, function_name, rc);
            return -1;
        }
        u64 off = g_ss2_work_offset;
        g_ss2_work_offset = off + (count << 9);
        dst = SS2_WORK_BASE + off + (src - SB_ROM_BASE) - (sector << 9);
    }
    return dst;
}

void write32_masked(u64 addr, u32 val, u32 mask)
{
    write32(addr, (read32(addr) & ~mask) | (val & mask));
}

void configure_sb_errors(bool enable)
{
    if (enable) {
        write32(SB_MMIO_BASE + 0x80310, 0x4EFFFFFE);
        write32(SB_MMIO_BASE + 0x80260, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x80264, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x80268, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x8026C, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x80270, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x80274, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x80278, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x8027C, 0x0FFFFF00);
        write32(SB_MMIO_BASE + 0x800C8, 0x0FFF0000);
        write32(SB_MMIO_BASE + 0x800CC, 0x10000);
        write32(SB_MMIO_BASE + 0x800D0, 0);
        write32(SB_MMIO_BASE + 0x800D4, 1);
        write32(SB_MMIO_BASE + 0x800D8, 0);
        write32(SB_MMIO_BASE + 0x800DC, 1);
        write32(SB_MMIO_BASE + 0x800E0, 0);
    } else {
        write32(SB_MMIO_BASE + 0x80310, 0);
        write32(SB_MMIO_BASE + 0x80260, 0);
        write32(SB_MMIO_BASE + 0x80264, 0);
        write32(SB_MMIO_BASE + 0x80268, 0);
        write32(SB_MMIO_BASE + 0x8026C, 0);
        write32(SB_MMIO_BASE + 0x80270, 0);
        write32(SB_MMIO_BASE + 0x80274, 0);
        write32(SB_MMIO_BASE + 0x80278, 0);
        write32(SB_MMIO_BASE + 0x8027C, 0);
        write32(SB_MMIO_BASE + 0x800CC, 0);
        write32(SB_MMIO_BASE + 0x800D0, 0);
        write32(SB_MMIO_BASE + 0x800D4, 0);
        write32(SB_MMIO_BASE + 0x800D8, 0);
        write32(SB_MMIO_BASE + 0x800DC, 0);
        write32(SB_MMIO_BASE + 0x800E0, 0);
    }
    u32 mask = is_sb_product_dx() ? 6 : 4;
    write32(SB_MMIO_BASE + 0x87020, read32(SB_MMIO_BASE + 0x87020) | mask);
    write32(SB_MMIO_BASE + 0x87030, read32(SB_MMIO_BASE + 0x87030) | mask);
    while ((read32(SB_MMIO_BASE + 0x87030) & mask) != mask)
        ;
    if (enable) {
        write32_masked(SB_MMIO_BASE + 0x11A8, 112, 112);
        write32_masked(SB_MMIO_BASE + 0x2FA8, 96, 112);
        if (is_sb_product_dx())
            write32_masked(SB_MMIO_BASE + 0xFA8, 96, 112);
    } else {
        write32(SB_MMIO_BASE + 0x11A8, 0);
        write32(SB_MMIO_BASE + 0x2FA8, 0);
        if (is_sb_product_dx())
            write32(SB_MMIO_BASE + 0xFA8, 0);
    }
}

void configure_be_errors(bool enable, bool livelock)
{
    write64(BE + 0x500910, 0);
    write64(BE + 0x500920, (u64)-1);
    write64(BE + 0x500918, 0);
    write64(BE + 0x500928, (u64)-1);
    if (enable) {
        if (livelock)
            write64(BE + 0x500930, 0x2C00000000000000ULL);
        else
            write64(BE + 0x500930, 0x3FE0000000000000ULL);
    } else {
        write64(BE + 0x500930, 0);
    }
    write64(BE + 0x500938, 0xC000000000000000ULL);
    write64(BE + 0x500B10, 0);
    write64(BE + 0x500B20, (u64)-1);
    write64(BE + 0x500B18, 0);
    write64(BE + 0x500B28, (u64)-1);
    if (enable)
        write64(BE + 0x500B30, 0xFF);
    else
        write64(BE + 0x500B30, 0);
    write64(BE + 0x500810, 0);
    write64(BE + 0x500820, (u64)-1);
    write64(BE + 0x500818, 64);
    write64(BE + 0x500828, (u64)-1);
    if (enable) {
        if (livelock)
            write64(BE + 0x500830, 0xEB781);
        else
            write64(BE + 0x500830, 0xEFF81);
    } else {
        write64(BE + 0x500830, 0);
    }
    write64(BE + 0x500848, read64(BE + 0x500848) | 4);
    if (enable) {
        write64(BE + 0x50A230, 0xFD7E00000000ULL);
        write64(BE + 0x50A238, 0x28000000000ULL);
    } else {
        write64(BE + 0x50A230, 0);
        write64(BE + 0x50A238, 0);
    }
    write64(BE + 0x508508, 0);
    write64(BE + 0x508500, (u64)-1);
    write64(BE + 0x508518, 0);
    if (enable) {
        switch (get_first_spu_priv1_20()) {
        case 0x100:
        case 0x200:
            if (livelock)
                write64(BE + 0x508510, 0x070FB0FB03D03D1FULL);
            else
                write64(BE + 0x508510, 0x070FB0FB03F03F1FULL);
            write64(BE + 0x512010, 0xFFFF00000000F000ULL);
            write64(BE + 0x513010, 0xFFFF00000000F000ULL);
            break;
        default:
            if (livelock)
                write64(BE + 0x508510, 0x070FF0FF03D03D1FULL);
            else
                write64(BE + 0x508510, 0x070FF0FF03F03F1FULL);
            write64(BE + 0x512010, 0xFFFF000000001000ULL);
            write64(BE + 0x513010, 0xFFFF000000001000ULL);
            break;
        }
    } else {
        write64(BE + 0x508510, 0);
    }
    u32 mask = read32(BE + 0x509C38);
    u64 spu_base = BE + 0x400000;
    int i;
    for (i = 0; i < 8; i++) {
        if (mask & (128 >> i)) {
            write64(spu_base + 0x388, 0);
            write64(spu_base + 0x390, (u64)-1);
            write64(spu_base + 0x3A0, 0);
            write64(spu_base + 0x3A8, (u64)-1);
            if (get_first_spu_priv1_20() == 0x100)
                write64(spu_base + 0x3B0, 2006);
            else
                write64(spu_base + 0x3B0, 1412);
        }
        spu_base += 0x2000;
    }
    write32(BE + 0x509C18, 304);
    write32(BE + 0x509C20, (u32)-4);
}

extern const unsigned char tbr_divider_lfsr_table[256] __attribute__((aligned(16)));
void start_decrementer(unsigned int count);

void loader_base::initialize(void)
{
    unsigned char trace;
    unsigned int ref;

    write64(BE + 0x511048, 0x1F26000000000000ULL);
    write64(BE + 0x511448, 0x0826000000000000ULL);
    write64(BE + 0x512008, 0x0400000000000000ULL);
    write64(BE + 0x513008, 0x0400000000000000ULL);
    write64(BE + 0x511800, 0x8806800000000000ULL);
    get_pio()->configure(SB_MMIO_BASE, 0xFF00, 0xFFA0);
    get_uart()->init(SB_MMIO_BASE, 23, 0, 0, 0, 0);
    log_set_handler(1, uart_console_puts);
    log_set_final_handler((long (*)(void))handle_fatal_error);
    get_syscon_device()->set_f8(0);
    get_syscon_device()->initialize(SB_MMIO_BASE, 1, 1);
    log_set_handler(0, syscon_write_string);
    log_set_handler(2, syscon_write_string);
    long rc = get_eeprom_bootrom_trace_level(&trace);
    if (rc)
        log_error("[ERROR]: 0x%08x get_eeprom_bootrom_trace_leve fail %d\n", LV0_ERR_CONFIG, rc);
    switch (trace) {
    case 0: log_set_threshold(0); break;
    case 2: log_set_threshold(2); break;
    case 3: log_set_threshold(3); break;
    default: log_set_threshold(1); break;
    }
    if (syscon_get_ref_clock(&ref, true))
        log_error("[ERROR]: 0x%08x get_reference_clock fail\n", LV0_ERR_CONFIG);
    u64 ref_div = (u64)ref / 0x4C1A6C0;
    if (ref_div > 0xFF) {
        get_pio()->write_output_byte(0x98);
        log_error("[ERROR]: 0x%08x Invalid be_ref_clk %08x or ref_div %08llx\n", LV0_ERR_CONFIG, ref, ref_div);
    }
    write64(BE + 0x509890, tbr_divider_lfsr_table[ref_div]);
    start_decrementer(0x7FFFFFFF);
    get_syscon_device()->set_f8(1);
    if (is_sb_product_dx())
        get_sb_device()->initialize(SB_MMIO_BASE, true);
    if (get_syscon_device()->get_protocol_version() == 0)
        log_error("[ERROR]: 0x%08x sc_protocol_version %d\n", LV0_ERR_CONFIG, get_syscon_device()->get_protocol_version());
    bool livelock = setup_livelock_detection();
    if (get_boot_fir_config() & 1)
        configure_be_errors(true, livelock);
    else
        configure_be_errors(false, livelock);
    if (get_boot_fir_config() & 2)
        configure_sb_errors(true);
    else
        configure_sb_errors(false);
    get_boot_fir_config();
    if (is_config_2_zero())
        write64(BE + 0x511C00, 0x10000800);
    else
        write64(BE + 0x511C00, 0x800);
}
