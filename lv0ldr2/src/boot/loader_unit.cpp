#include "sb.h"
#include "syscon.h"
#include "storage.h"
#include "loader.h"
#include "iommu.h"
#include "iopt.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "mmio.h"
#include "xdr.h"
#include "uart.h"
#include "nand_flash.h"
#include "systemio_dmac.h"

int detect_flash_type(u64 dev);

u64 g_first_spu_priv1_20 = (u64)-1;
u64 g_dma_io_addr = (u64)-1;
bool g_boot_memory_type_checked;
int g_boot_memory_type;
bool g_flash_format_window_mapped;
bool g_post_flag_80;
bool g_post_flag_40;
struct flash_format_header g_flash_format;

#define SET_POST_CODE(code) do {               \
        get_pio()->write_output_byte(code);     \
        get_sb_device()->write8_100003a(code);  \
    } while (0)
#define SET_POST_CODE_FLAGS(code) do {                                         \
        int c_ = (g_post_flag_80 ? 0x80 : 0) | (code) | (g_post_flag_40 ? 0x40 : 0); \
        SET_POST_CODE(c_);                                                     \
    } while (0)

#define BE (0x20000000000ULL)

extern const unsigned char tbr_divider_lfsr_table[256] __attribute__((aligned(16))) = {
    0x00, 0xff, 0x7f, 0x3f, 0x9f, 0x4f, 0x27, 0x13, 0x09, 0x84, 0x42, 0xa1, 0x50, 0x28, 0x94, 0xca,
    0xe5, 0xf2, 0xf9, 0x7c, 0xbe, 0x5f, 0xaf, 0x57, 0xab, 0x55, 0xaa, 0xd5, 0xea, 0x75, 0x3a, 0x1d,
    0x0e, 0x07, 0x83, 0xc1, 0x60, 0x30, 0x18, 0x8c, 0x46, 0xa3, 0x51, 0xa8, 0xd4, 0x6a, 0x35, 0x9a,
    0xcd, 0x66, 0x33, 0x99, 0x4c, 0xa6, 0xd3, 0xe9, 0xf4, 0xfa, 0xfd, 0x7e, 0xbf, 0xdf, 0xef, 0xf7,
    0x7b, 0x3d, 0x9e, 0xcf, 0x67, 0xb3, 0xd9, 0xec, 0x76, 0xbb, 0xdd, 0xee, 0x77, 0x3b, 0x9d, 0x4e,
    0xa7, 0x53, 0xa9, 0x54, 0x2a, 0x95, 0x4a, 0xa5, 0x52, 0x29, 0x14, 0x8a, 0x45, 0x22, 0x91, 0x48,
    0xa4, 0xd2, 0x69, 0xb4, 0x5a, 0x2d, 0x16, 0x8b, 0xc5, 0x62, 0x31, 0x98, 0xcc, 0xe6, 0x73, 0x39,
    0x9c, 0xce, 0xe7, 0xf3, 0x79, 0x3c, 0x1e, 0x8f, 0xc7, 0x63, 0xb1, 0xd8, 0x6c, 0x36, 0x1b, 0x0d,
    0x86, 0x43, 0x21, 0x10, 0x88, 0x44, 0xa2, 0xd1, 0xe8, 0x74, 0xba, 0x5d, 0xae, 0xd7, 0xeb, 0xf5,
    0x7a, 0xbd, 0xde, 0x6f, 0xb7, 0xdb, 0xed, 0xf6, 0xfb, 0x7d, 0x3e, 0x1f, 0x0f, 0x87, 0xc3, 0x61,
    0xb0, 0x58, 0x2c, 0x96, 0xcb, 0x65, 0xb2, 0x59, 0xac, 0xd6, 0x6b, 0xb5, 0xda, 0x6d, 0xb6, 0x5b,
    0xad, 0x56, 0x2b, 0x15, 0x0a, 0x05, 0x82, 0x41, 0x20, 0x90, 0xc8, 0xe4, 0x72, 0xb9, 0xdc, 0x6e,
    0x37, 0x9b, 0x4d, 0x26, 0x93, 0x49, 0x24, 0x92, 0xc9, 0x64, 0x32, 0x19, 0x0c, 0x06, 0x03, 0x81,
    0xc0, 0xe0, 0x70, 0xb8, 0x5c, 0x2e, 0x97, 0x4b, 0x25, 0x12, 0x89, 0xc4, 0xe2, 0x71, 0x38, 0x1c,
    0x8e, 0x47, 0x23, 0x11, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0xa0, 0xd0, 0x68, 0x34, 0x1a, 0x8d,
    0xc6, 0xe3, 0xf1, 0x78, 0xbc, 0x5e, 0x2f, 0x17, 0x0b, 0x85, 0xc2, 0xe1, 0xf0, 0xf8, 0xfc, 0xfe,
};

static const unsigned short xio_regs_7[6] = { 0xC438, 0xC475, 0xC4B1, 0xC50D, 0xC40D, 0xFFFF };
static const unsigned short xio_regs_11[6] = { 0xC439, 0xC475, 0xC4B1, 0xC50D, 0xC40D, 0xC539 };
static const unsigned short xio_regs_12[6] = { 0xC439, 0xC475, 0xC4B1, 0xC50D, 0xC40D, 0xC539 };

u64 g_ss2_work_offset = 0x80;

#define SB_ROM_BASE (SB_MMIO_BASE + 0x1FC00000)
#define SS2_WORK_BASE 0x1010000ULL
void configure_be_errors(bool enable, bool livelock);
void configure_sb_errors(bool enable);
void write32_masked(u64 addr, u32 val, u32 mask);
void start_decrementer(unsigned int count);

extern const unsigned char tbr_divider_lfsr_table[256];

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
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_bootrom_trace_leve fail %d\n", LV0_ERR_CONFIG, rc);
    switch (trace) {
    case 0: log_set_threshold(0); break;
    case 2: log_set_threshold(2); break;
    case 3: log_set_threshold(3); break;
    default: log_set_threshold(1); break;
    }
    if (syscon_get_ref_clock(&ref, true))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_reference_clock fail\n", LV0_ERR_CONFIG);
    u64 ref_div = (u64)ref / 0x4C1A6C0;
    if (ref_div > 0xFF) {
        get_pio()->write_output_byte(0x98);
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x Invalid be_ref_clk %08x or ref_div %08llx\n", LV0_ERR_CONFIG, ref, ref_div);
    }
    write64(BE + 0x509890, tbr_divider_lfsr_table[ref_div]);
    start_decrementer(0x7FFFFFFF);
    get_syscon_device()->set_f8(1);
    if (is_sb_product_dx())
        get_sb_device()->initialize(SB_MMIO_BASE, true);
    if (get_syscon_device()->get_protocol_version() == 0)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x sc_protocol_version %d\n", LV0_ERR_CONFIG, get_syscon_device()->get_protocol_version());
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

long loader_base::init_memory(void)
{
    xdr x;
    xdr_config cfg;
    unsigned char second;
    unsigned char mode;
    unsigned char setting;
    sc_config_info::xdr q;

    q.initialize();
    if (q.query_config(&cfg))
        log_error(LV0_ERROR_BASE, "[ERROR]: 0x%08x (FATAL) unable to set up memory configuration parameter.\n", LV0_ERROR_BASE);
    if (get_bootrom_diag(&mode, &second))
        cfg.basic[7] = 0;
    long rc = x.set_config(&cfg);
    if (rc)
        log_error(rc, "[ERROR]: 0x%08x (FATAL) XDR Init not performed due to invalid parameter.\n", rc);
    rc = x.initialize();
    if (rc) {
        log_message("[ERROR]: 0x%08x (FATAL) XDR Link not initilized.\n", rc);
        x.dump(0);
        x.dump(1);
        x.dump_regs();
        x.prepare_iow_dump();
        x.dump(2);
        get_eeprom_48c0e(&setting);
        if (setting)
            setting--;
        set_eeprom_48c0e(setting);
        log_error(rc, "[ERROR]: 0x%08x (FATAL) XDR Link not initilized.\n", rc);
    }
    log_info("[INFO]: XDR Link successfully initilized.\n");
    if (get_bootrom_diag(&mode, &second)) {
        u64 size = get_memory_size();
        log_message("[INFO]: memory diag (mode %d, %d)\n", mode, second);
        bool ok = memory_diag(size, 0, 0, mode);
        if (!ok)
            log_message("[ERROR]: 0x%08x (FATAL) memory diag fail\n", LV0_ERROR_BASE | 7);
        if (second == 0xFF) {
            if (!ok) {
                get_eeprom_48c0d(&setting);
                setting &= ~1;
                set_eeprom_48c0d(setting);
                if (mode == 1)
                    log_error(LV0_ERROR_BASE, "[ERROR]: 0x%08x (FATAL) memory_diag fail.\n", LV0_ERROR_BASE);
            } else {
                get_eeprom_48c0d(&setting);
                setting |= 1;
                set_eeprom_48c0d(setting);
                get_eeprom_bootrom_diag(&setting);
                setting |= 1;
                set_eeprom_bootrom_diag(setting);
            }
            int code = 0;
            switch (mode) {
            case 1:
                code = 1;
                break;
            }
            syscon_power_off_with_code(0, 0, code);
        } else {
            get_eeprom_48c0d(&setting);
            setting &= ~8;
            if (!ok)
                setting &= ~4;
            else
                setting |= 4;
            set_eeprom_48c0d(setting);
            get_eeprom_bootrom_diag(&setting);
            setting |= 4;
            set_eeprom_bootrom_diag(setting);
            syscon_power_off_with_code(0, 0, 1);
        }
    }
    get_iommu_context()->initialize(BE, 0x1C000, 1024, 0x1D000, 4096);
    write64(BE + 0x509C90, 0x8001);
    if (is_boot_memory_type_nand())
        init_nand_flash_dma(0x2401FC00000ULL, 0x1010000, 0x1000000);
    if (is_boot_memory_type_nor()) {
        map_flash_format_window(0x2401F000000ULL, 4);
        copy_qwords_from_mmio(0x2401F000200ULL, (char *)&g_flash_format, 16);
    }
    bool cold = !cfg.str;
    return !cold;
}

long xdr::set_config(const xdr_config *c)
{
    unsigned short shift;
    unsigned short mask;

    memcpy(&m_cfg, c, sizeof m_cfg);
    m_mcp = &m_cfg;
    m_b0 = m_cfg.basic[109];
    unsigned int ratio = (*(unsigned int *)&m_cfg.basic[20] >> 24) & 63;
    switch (m_cfg.cbe_kind) {
    case 12:
        m_ae = 0;
        m_c8 = true;
        m_b4 = 256;
        m_b8 = 0;
        memcpy(m_bc, xio_regs_12, sizeof m_bc);
        shift = 6;
        mask = 252;
        break;
    case 11:
        m_ae = 0;
        m_c8 = true;
        m_b4 = 256;
        m_b8 = 0;
        memcpy(m_bc, xio_regs_11, sizeof m_bc);
        shift = 6;
        mask = 252;
        break;
    case 7:
        m_ae = 7;
        m_c8 = false;
        m_b4 = 64;
        m_b8 = 1;
        memcpy(m_bc, xio_regs_7, sizeof m_bc);
        shift = 8;
        mask = 255;
        break;
    default:
        info("[ERROR]: CBE version unknown.\n");
        error("[ERROR]: >>> XDR Init: Not Configured <<<\n");
        return 0xB0002015;
    }
    m_ac = (m_b0 & mask) << shift | 0x39;
    m_b2 = m_b0 - m_ae;
    if (m_b2 == 0) {
        info("[ERROR]: Designated target FB_ADJ is invalid.\n");
        error("[ERROR]: >>> XDR Init: Not Configured <<<\n");
        return 0xB0002016;
    }
    if (m_b8 != 0)
        m_b2 = m_b4 - m_b2;
    m_b2 &= m_b4 - 1;

    switch (ratio) {
    case 4:
        m_mcp->f_ac = 2;
        break;
    case 8:
        m_mcp->f_ac = 3;
        break;
    case 16:
        m_mcp->f_ac = 4;
        break;
    case 32:
        m_mcp->f_ac = 5;
        break;
    default:
        info("[ERROR]: Designated XDR DRAM Prog.Width is invalid.\n");
        error("[ERROR]: >>> XDR Init: Not Configured <<<\n");
        return 0xB0002017;
    }
    m_mcp->f_b0 = (m_mcp->basic[3] << 2) / 2;
    m_mcp->f_b4 = m_mcp->basic[2] / ratio;
    m_mcp->f_b8 = m_mcp->f_b0 / m_mcp->f_b4;
    if (m_mcp->basic[5] == 1) {
        m_mcp->ecc_off_ch0 = false;
        m_mcp->ecc_off_ch1 = false;
        m_mcp->ecc_on = true;
    } else {
        m_mcp->ecc_off_ch0 = true;
        m_mcp->ecc_off_ch1 = true;
        m_mcp->ecc_on = false;
    }
    m_mcp->xio_ch0_on = false;
    m_mcp->xio_ch1_on = false;
    return 0;
}

void loader_base::finalize(bool fatal, long rc)
{
    release_dma_io_address();
    write64(0x20000509C90ULL, 0);
    get_iommu_context()->disable();
    if (fatal) {
        unsigned int code;

        switch (rc) {
        case -1: code = LV0_ERR_BOOT_LV1; break;
        case -5: code = LV0_ERR_NO_SPU; break;
        default: code = 0; break;
        }
        if (__builtin_expect(!code, 0))
            code = LV0_ERR_INTERNAL;
        log_error(code, "");
    }
    log_message("[INFO]: Connecting to Debug Device (%s)\n", get_debug_device_name());
}

u64 loader_base::copy_to_main_memory(u64 src, unsigned int size)
{
    FUNCTION_NAME("copy_to_main_memory");
    u64 dst = src;
    if (is_boot_memory_type_nand()) {
        if (src < SB_ROM_BASE) {
            log_message("[ERROR]: 0x%08x %s: ERROR src < config::sb_rom_base\n", LV0_ERR_INTERNAL, function_name);
            return -1;
        }
        u64 count = (size + 511ULL + ((src - SB_ROM_BASE) & 511) & 0xFFFFFE00U) >> 9;
        u64 sector = (src - SB_ROM_BASE) >> 9;
        if (g_ss2_work_offset + (count << 9) > 0xFFFFFF) {
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

int loader_base::is_sc_protocol_ver1(void)
{
    return si_to_uint(si_ceqi(si_from_uint(get_syscon_device()->get_protocol_version()), 1)) & 1;
}

u64 loader_base::get_flash_rom_base(void)
{
    if (get_flash_layout() == 1)
        return 0x2401FC40200ULL;
    if (get_flash_layout() == 2)
        return 0x2401F000200ULL;
    return get_flash_layout() == 3 ? 0x2401F000400ULL : 0x2401FC00000ULL;
}

int loader_base::get_flash_layout(void)
{
    if (is_boot_memory_type_nor()) {
        if (g_flash_format.magic == 0x49464900) {
            if (g_flash_format.version == 1 && g_flash_format.format == 2) {
                log_info("[INFO]: flash format 2\n");
                return 3;
            }
            log_error(LV0_ERR_BOOT_LV1,
                      "[ERROR]: 0x%08x invalid flash format version %04x, format_version %04x\n",
                      LV0_ERR_BOOT_LV1, g_flash_format.version, g_flash_format.format);
        }
        log_info("[INFO]: flash format 1_1\n");
        return 2;
    }
    log_info("[INFO]: flash format 1\n");
    return 1;
}

bool is_boot_memory_type_nand(void)
{
    return get_boot_memory_type() == 0;
}

bool is_boot_memory_type_nor(void)
{
    return get_boot_memory_type() == 256 || get_boot_memory_type() == 272 ||
           get_boot_memory_type() == 273 || get_boot_memory_type() == 288 ||
           get_boot_memory_type() == 400 || get_boot_memory_type() == 416;
}

int get_boot_memory_type(void)
{
    FUNCTION_NAME("get_boot_memory_type");
    if (!g_boot_memory_type_checked) {
        iopt_codec codec;
        iopt_desc d;
        codec.unpack_entry(SB_MMIO_BASE, 0, &d);
        g_boot_memory_type = detect_flash_type(SB_MMIO_BASE + d.addr);
        g_boot_memory_type_checked = true;
        log_info("[INFO] %s type %d\n", function_name, g_boot_memory_type);
    }
    return g_boot_memory_type;
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
    unsigned int i;
    for (i = 0; i < 8; i++) {
        if (mask & (128 >> i)) {
            u64 spu_base = BE + 0x400000 + (u64)i * 0x2000;
            write64(spu_base + 0x388, 0);
            write64(spu_base + 0x390, (u64)-1);
            write64(spu_base + 0x3A0, 0);
            write64(spu_base + 0x3A8, (u64)-1);
            if (get_first_spu_priv1_20() == 0x100)
                write64(spu_base + 0x3B0, 2006);
            else
                write64(spu_base + 0x3B0, 1412);
        }
    }
    write32(BE + 0x509C18, 304);
    write32(BE + 0x509C20, (u32)-4);
}

u64 get_first_spu_priv1_20(void)
{
    u32 mask = read32(0x20000509C38ULL);
    if (g_first_spu_priv1_20 == (u64)-1) {
        unsigned int i;
        for (i = 0; i < 8; i++) {
            if (mask & (128 >> i)) {
                g_first_spu_priv1_20 = read64(0x20000400020ULL + (u64)i * 0x2000);
                break;
            }
        }
    }
    return g_first_spu_priv1_20;
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
        if (is_sb_product_dx() || is_sb_product_2())
            write32_masked(SB_MMIO_BASE + 0x2FA8, 96, 112);
        if (is_sb_product_dx())
            write32_masked(SB_MMIO_BASE + 0xFA8, 96, 112);
    } else {
        write32(SB_MMIO_BASE + 0x11A8, 0);
        if (is_sb_product_dx() || is_sb_product_2())
            write32(SB_MMIO_BASE + 0x2FA8, 0);
        if (is_sb_product_dx())
            write32(SB_MMIO_BASE + 0xFA8, 0);
    }
}

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

bool is_sb_product_dx(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x01000000;
}

bool is_sb_product_2(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x02000000;
}

void write32_masked(u64 addr, u32 val, u32 mask)
{
    write32(addr, (read32(addr) & ~mask) | (val & mask));
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

void release_dma_io_address(void)
{
    if (g_dma_io_addr != (u64)-1) {
        get_iommu_context()->free_io_address(g_dma_io_addr);
        g_dma_io_addr = (u64)-1;
    }
}

int map_flash_format_window(u64 flash_base, unsigned int w2)
{
    if (!g_flash_format_window_mapped) {
        iopt_codec codec;
        iopt_desc d;
        d.addr = flash_base;
        d.w[2] = w2;
        d.w[3] = 4;
        d.w[4] = 2;
        d.w[5] = 0;
        d.w[6] = 0;
        d.w[7] = 0;
        d.w[8] = 0;
        d.w[9] = 0;
        d.w[10] = 1;
        d.w[11] = 0;
        codec.pack_entry(SB_MMIO_BASE, 0, &d);
        g_flash_format_window_mapped = true;
    }
    return 0;
}

bool is_config_2_zero(void)
{
    unsigned int v;
    if (syscon_read_config_2(&v) == 0 && v == 0)
        return 1;
    return 0;
}

bool setup_livelock_detection(void)
{
    unsigned short ver;
    long rc = get_sc_version(27, &ver);
    if (__builtin_expect(rc != 0, 1)) {
        log_debug("[DEBUG]: get livelock version fail %d\n", rc);
        return false;
    }
    if (ver < 0x100) {
        log_debug("[DEBUG]: get livelock version %04x is lower than %04x\n", ver, 0x100);
        return false;
    }
    rc = set_livelock_detection_mode(0);
    if (rc) {
        log_debug("[DEBUG]: set livelock detection mode fail %d\n", rc);
        return false;
    }
    return true;
}

long set_livelock_detection_mode(int mode)
{
    unsigned char resp[1];
    unsigned char req[2] = {0};
    int resp_len;
    req[0] = 16;
    req[1] = mode;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(27, req, 2, resp, 1, &resp_len, 0);
    if (rc != 0)
        return rc;
    switch (resp[0]) {
    case 0: return rc;
    case 1: return -10008;
    }
    return -99999;
}

long get_sc_version(unsigned char arg, unsigned short *out)
{
    static const unsigned char request[2] = { 1, 3 };
    unsigned char req[2];
    unsigned char resp[4];
    int resp_len;
    req[0] = request[0];
    req[1] = arg;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_VERSION, req, 2, resp, 4, &resp_len, 0);
    *out = (resp[2] << 8) | resp[3];
    if (rc != 0)
        return rc;
    unsigned int status = resp[0];
    switch (status) {
    case 0: return rc;
    case 1: return -10002;
    }
    return -99999;
}

const char *get_debug_device_name(void)
{
    unsigned char v;
    long rc = get_eeprom_select_net_device(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_select_net_device %d\n", LV0_ERR_CONFIG, rc);
    switch (v) {
    case 1: return "CP";
    case 3: return "CP ch4";
    }
    return "SB UART";
}

long get_eeprom_select_net_device(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[2];
    return rc;
}

unsigned char get_boot_fir_config(void)
{
    unsigned char v;
    long rc = get_eeprom_boot_fir_config(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_boot_fir_config %d\n", LV0_ERR_CONFIG, rc);
    return v;
}

long get_eeprom_boot_fir_config(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[9];
    return rc;
}

bool get_bootrom_diag(unsigned char *mode, unsigned char *second)
{
    unsigned char v;
    long rc = get_eeprom_bootrom_diag(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_bootrom_diag fail %d\n", LV0_ERR_CONFIG, rc);
    unsigned int bits = v;
    if (!(bits & 1)) {
        *mode = (bits >> 1) & 1;
        *second = 0xFF;
        return true;
    }
    if (!(bits & 4)) {
        *mode = (bits >> 3) & 1;
        *second = *mode;
        return true;
    }
    return false;
}

long get_eeprom_bootrom_diag(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[12];
    return rc;
}

long get_eeprom_48c0d(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[13];
    return rc;
}

long get_eeprom_48c0e(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[14];
    return rc;
}

long get_eeprom_bootrom_trace_level(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[17];
    return rc;
}

long get_eeprom_os_bank_indicator(unsigned char *out)
{
    long rc = load_nv_entry(3);
    if (!rc)
        *out = g_nv_entry_table[3].data.buf[2];
    return rc;
}

int loader_base::get_os_bank_indicator(void)
{
    unsigned char v;
    long rc = get_eeprom_os_bank_indicator(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_os_bank_indicator %d\n",
                  LV0_ERR_CONFIG, rc);
    if (v == 0xFF)
        return 0;
    return 1;
}

long handle_fatal_error(unsigned int code)
{
    if (code != LV0_ERR_CONFIG) {
        unsigned int pm = 0;
        syscon_power_query_33(&pm);
        write_eeprom_nv1_u32_at8(pm);
        write_eeprom_nv1_u32_at4((code << 24) | 0xFFFFFF);
    }
    syscon_set_wake_source(820);
    syscon_power_off_with_code(0, 0, 3);
}

int write_eeprom_nv1_u32_at4(unsigned int value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 4, 4, &value, entry->base);
    if (!rc)
        *(unsigned int *)(entry->data.buf + 4) = value;
    return rc;
}

int write_eeprom_nv1_u32_at8(unsigned int value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 8, 4, &value, entry->base);
    if (!rc)
        *(unsigned int *)(entry->data.buf + 8) = value;
    return rc;
}

long set_eeprom_bootrom_diag(unsigned char val)
{
    struct nv_entry *entry = &g_nv_entry_table[2];
    long rc = nv_storage::write(entry->block, entry->offset + 12, 1, &val, entry->base);
    if (!rc)
        entry->data.buf[12] = val;
    return rc;
}

long set_eeprom_48c0d(unsigned char val)
{
    struct nv_entry *entry = &g_nv_entry_table[2];
    long rc = nv_storage::write(entry->block, entry->offset + 13, 1, &val, entry->base);
    if (!rc)
        entry->data.buf[13] = val;
    return rc;
}

long set_eeprom_48c0e(unsigned char val)
{
    struct nv_entry *entry = &g_nv_entry_table[2];
    long rc = nv_storage::write(entry->block, entry->offset + 14, 1, &val, entry->base);
    if (!rc)
        entry->data.buf[14] = val;
    return rc;
}

void loader_base::print(const char *msg)
{
    log_message(msg);
}

void post_lv0_auth_fail(void)
{
    SET_POST_CODE_FLAGS(0x31);
    log_message("[ERROR]: 0x%08x lv0 authentication fail\n", LV0_ERR_BOOT_LV1);
}

void post_lv0_not_found(void)
{
    SET_POST_CODE_FLAGS(0x33);
    log_message("[ERROR]: 0x%08x lv0 not found\n", LV0_ERR_BOOT_LV1);
}

void post_code_aa(void) { SET_POST_CODE(0xAA); }

void post_code_01(void) { SET_POST_CODE(1); }

void post_code_02(void) { SET_POST_CODE(2); }

void post_code_03(void) { SET_POST_CODE(3); }

void post_code_05(void) { SET_POST_CODE(5); }

void post_code_06(void) { SET_POST_CODE(6); }

void post_code_07(bool f80, bool f40)
{
    g_post_flag_80 = f80;
    g_post_flag_40 = f40;
    SET_POST_CODE_FLAGS(7);
}

void post_code_08(void) { SET_POST_CODE_FLAGS(8); }

void post_code_30(void) { SET_POST_CODE_FLAGS(0x30); }
