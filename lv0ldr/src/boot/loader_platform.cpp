#include "syscon.h"
#include "sb.h"
#include "storage.h"
#include "iommu.h"
#include "iopt.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "loader.h"

int probe_nand_flash(u64 dev);

u64 g_first_spu_priv1_20 = (u64)-1;
u64 g_dma_io_addr = (u64)-1;
bool g_boot_nand_checked;
bool g_boot_nand;
bool g_post_flag_80;
bool g_post_flag_40;

unsigned char get_boot_fir_config(void)
{
    unsigned char v;
    long rc = get_eeprom_boot_fir_config(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error("[ERROR]: 0x%08x get_eeprom_boot_fir_config %d\n", LV0_ERR_CONFIG, rc);
    return v;
}

bool get_bootrom_diag(unsigned char *second)
{
    unsigned char v;
    long rc = get_eeprom_bootrom_diag(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error("[ERROR]: 0x%08x get_eeprom_bootrom_diag fail %d\n", LV0_ERR_CONFIG, rc);
    *second = (v & 2) != 0;
    return (v & 1) ? false : true;
}

const char *get_debug_device_name(void)
{
    unsigned char v;
    long rc = get_eeprom_select_net_device(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error("[ERROR]: 0x%08x get_eeprom_select_net_device %d\n", LV0_ERR_CONFIG, rc);
    switch (v) {
    case 1: return "CP";
    case 3: return "CP ch4";
    }
    return "SB UART";
}

u64 get_flash_rom_base(void)
{
    return 0x2401FC00000ULL;
}

int loader_base::is_sc_protocol_ver1(void)
{
    return (get_syscon_device()->get_protocol_version() == 1) & 1;
}

int get_flash_layout(void)
{
    return 1;
}

unsigned char loader_base::get_update_flag(void)
{
    unsigned char v;
    long rc = get_eeprom_update_flag(&v);
    if (__builtin_expect(rc != 0, 0))
        log_error("[ERROR]: 0x%08x get_eeprom_update_flag %d\n", LV0_ERR_CONFIG, rc);
    if (v == 0xFF)
        return 0;
    return 1;
}

bool loader_base::is_force_update(void)
{
    return 0;
}

void loader_base::print(const char *msg)
{
    log_message(msg);
}

bool is_boot_memory_type_nand(void)
{
    FUNCTION_NAME("is_boot_memory_type_nand");
    if (!g_boot_nand_checked) {
        iopt_codec codec;
        iopt_desc d;
        codec.unpack_entry(SB_MMIO_BASE, 0, &d);
        int type = probe_nand_flash(SB_MMIO_BASE + d.addr);
        log_info("[INFO] %s type %d\n", function_name, type);
        if (type == 257)
            g_boot_nand = true;
        else
            g_boot_nand = false;
        g_boot_nand_checked = true;
    }
    return g_boot_nand;
}

void release_dma_io_address(void)
{
    if (g_dma_io_addr != (u64)-1) {
        get_iommu_context()->free_io_address(g_dma_io_addr);
        g_dma_io_addr = (u64)-1;
    }
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

void handle_fatal_error(void)
{
    syscon_set_wake_source(820);
    syscon_power_off_with_code(0, 0, 3);
}

#define SET_POST_CODE(code) do {               \
        get_pio()->write_output_byte(code);     \
        get_sb_device()->write8_100003a(code);  \
    } while (0)

void post_code_aa(void) { SET_POST_CODE(0xAA); }
void post_code_01(void) { SET_POST_CODE(1); }
void post_code_02(void) { SET_POST_CODE(2); }
void post_code_03(void) { SET_POST_CODE(3); }
void post_code_05(void) { SET_POST_CODE(5); }
void post_code_06(void) { SET_POST_CODE(6); }

#define SET_POST_CODE_FLAGS(code) do {                                         \
        int c_ = (g_post_flag_80 ? 0x80 : 0) | (code) | (g_post_flag_40 ? 0x40 : 0); \
        SET_POST_CODE(c_);                                                     \
    } while (0)

void post_code_07(bool f80, bool f40)
{
    g_post_flag_80 = f80;
    g_post_flag_40 = f40;
    SET_POST_CODE_FLAGS(7);
}

void post_code_08(void) { SET_POST_CODE_FLAGS(8); }
void post_code_30(void) { SET_POST_CODE_FLAGS(0x30); }

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

u64 get_first_spu_priv1_20(void)
{
    u32 mask = read32(0x20000509C38ULL);
    if (g_first_spu_priv1_20 == (u64)-1) {
        u64 addr = 0x20000400020ULL;
        int i;
        for (i = 0; i < 8; i++) {
            if (mask & (128 >> i)) {
                g_first_spu_priv1_20 = read64(addr);
                break;
            }
            addr += 0x2000;
        }
    }
    return g_first_spu_priv1_20;
}
