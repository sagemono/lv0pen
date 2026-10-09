#include "platform.h"
#include "storage.h"
#include "component.h"
#include "memory.h"
#include "spu.h"
#include "clock.h"
#include "config.h"
#include "syscon.h"
#include "log.h"
#include "printf.h"
#include "mmio.h"

extern componnet_manager *g_componnet_manager_ptr;

void fill_boot_parm_header(long parm)
{
    unsigned long addr, size;
    *(int *)parm = 10;
    *(int *)(parm + 0x4) = 96;
    *(long *)(parm + 0x8) = 0;
    if (g_componnet_manager_ptr->get_component(COMPONENT_PARM_TXT, &addr, &size)) {
        addr = 0;
        size = 0;
    }
    *(long *)(parm + 0x10) = addr;
    *(long *)(parm + 0x20) = 0;
    *(long *)(parm + 0x18) = size;
    *(long *)(parm + 0x28) = 0;
    *(long *)(parm + 0x30) = 0;
    *(long *)(parm + 0x38) = 0;
    if (g_componnet_manager_ptr->get_component(COMPONENT_SYSCTL_TXT, &addr, &size)) {
        addr = 0;
        size = 0;
    }
    unsigned int alt_parm_len = *(unsigned int *)(parm + 0x860);
    *(long *)(parm + 0x48) = size;
    *(long *)(parm + 0x58) = alt_parm_len;
    *(long *)(parm + 0x40) = addr;
    *(long *)(parm + 0x50) = parm + 0x60;
}

int create_alt_sys_parm(struct boot_parm *parm, const char *key, u64 val)
{
    FUNCTION_NAME("create_alt_sys_parm");
    char line[56];
    unsigned int off;
    unsigned long dst;

    if (lv0_strnlen(key, 33) > 32) {
        log_message("%s: parm_name too long\n", function_name);
        return -2;
    }
    lv0_snprintf(line, 55, "%s = 0x%llx\n", key, val);
    off = parm->alt_sys_parm_len;
    if (off + lv0_strlen(line) > sizeof(parm->alt_sys_parm) - 1) {
        log_message("%s: alt_sys_parm full\n", function_name);
        return -1;
    }
    dst = (unsigned long)off + 0x60;
    lv0_strncpy((char *)parm + dst, line, 55);
    parm->alt_sys_parm_len = lv0_strlen(line) + parm->alt_sys_parm_len;
    return 0;
}

void build_alt_sys_parm(struct boot_parm *parm)
{
    create_alt_sys_parm(parm, "be.0.bp_base", be_mmio_base);
    create_alt_sys_parm(parm, "be.0.ioif0.addr", ioif0_base);
    create_alt_sys_parm(parm, "be.0.ioif1.addr", sb_mmio_base);
    long value = get_reference_clock();
    create_alt_sys_parm(parm, "be.0.nclk", value * get_core_clock_multiplier());
    create_alt_sys_parm(parm, "be.0.ref_clk", value);
    value = calc_spu_faultbm_lo() | calc_spu_faultbm_hi();
    create_alt_sys_parm(parm, "be.0.spu.faultbm", value);
    create_alt_sys_parm(parm, "be.0.tb_clk", 79800000UL);
    create_alt_sys_parm(parm, "mu.1.size", get_total_memory_size());
    create_alt_sys_parm(parm, "plat.id", get_platform_id());
    create_alt_sys_parm(parm, "sys.load.image.in_rom", (get_eeprom_os_boot_order_flag() ^ 1) & 0xFF);

    u64 flash_size;
    if (g_storage.get_size(0, &flash_size) != 0)
        flash_size = 0;
    create_alt_sys_parm(parm, "sys.rom.addr", flash_rom_base);
    unsigned int dbg_parm = get_debug_interface_parm();
    u64 dbg_if;
    switch (dbg_parm) {
    case 1: dbg_if = 1; break;
    case 2: dbg_if = 0; break;
    case 0: dbg_if = 2; break;
    case 3: dbg_if = 3; break;
    case 4: dbg_if = 4; break;
    case 5: dbg_if = 5; break;
    default:
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x invalid debug_interface was selected\n", LV0_ERR_INTERNAL);
    case 6: dbg_if = 6; break;
    }
    create_alt_sys_parm(parm, "sys.dbgcard.dgbe", dbg_if);

    unsigned int dbg_parm2 = get_debug_interface_parm();
    if (dbg_parm2 == 1 || dbg_parm2 == 5) {
        create_alt_sys_parm(parm, "sys.dbgcard.dgbe.index", get_eeprom_select_dgbe_device());
        create_alt_sys_parm(parm, "sys.dgbe.ipaddr", get_eeprom_dgbe_ip_address());
        create_alt_sys_parm(parm, "sys.dgbe.netmask", get_eeprom_dgbe_ip_netmask());
        create_alt_sys_parm(parm, "sys.dgbe.gateway", get_eeprom_dgbe_ip_gateway());
    }

    u64 macaddr = get_gbe_macaddr(0);
    if ((macaddr & 0xFFFFFFFFFFFFUL) != 0xFFFFFFFFFFFFUL)
        create_alt_sys_parm(parm, "spider.gbe0.macaddr.0", macaddr);
    create_alt_sys_parm(parm, "sys.platform.mode", !is_mambo());
    if (is_mambo() != 0)
        create_alt_sys_parm(parm, "sys.mambo.version", get_mambo_version());

    create_alt_sys_parm(parm, "sys.syscon.protocol_version", get_syscon_protocol_version());
    u64 rdcy[2];
    char key[32];
    read_rsx_rdcy(rdcy, 16);
    unsigned int i = 0;
    unsigned long off = 0;
    do {
        i++;
        lv0_snprintf(key, 32, "rsx.rdcy.%d", i);
        create_alt_sys_parm(parm, key, *(u64 *)(off + (unsigned long)rdcy));
        off += 8;
    } while (i != 2);

    create_alt_sys_parm(parm, "sys.lv0.version", *(const u64 *)"4.9.3");
    create_alt_sys_parm(parm, "sys.lv0.revision", *(const u64 *)"5382");
    create_alt_sys_parm(parm, "sys.lv0.address", 0);
    create_alt_sys_parm(parm, "sys.lv0.size", (lv1ldr_io_page_addr + 0xFFF) & ~0xFFFUL);
    unsigned long flash_fmt = 0;
    switch (get_flash_format()) {
    case 1 ... 2:
        flash_fmt = 1;
        break;
    case 3 ... 0x7FFFFFFF:
        flash_fmt = 2;
        break;
    }
    create_alt_sys_parm(parm, "sys.flash.fmt", flash_fmt);
    create_alt_sys_parm(parm, "sys.flash.ext", get_eeprom_flash_ext_format());
    create_alt_sys_parm(parm, "sys.flash.boot", get_flash_boot());
    create_alt_sys_parm(parm, "sys.wake_source", get_wake_source());
    create_alt_sys_parm(parm, "sys.ac.sd", g_sys_ac_sd);
    create_alt_sys_parm(parm, "sys.ac.misc", g_sys_ac_misc);
    create_alt_sys_parm(parm, "sys.qaf.qafen", is_qaf_enabled());
    create_alt_sys_parm(parm, "sys.hw.config", get_hw_config(0));
    create_alt_sys_parm(parm, "sys.hw.config_version", get_hw_config(1));
    create_alt_sys_parm(parm, "sys.hw.model_emulate", g_sys_hw_model_emulate);
    create_alt_sys_parm(parm, "sys.cellos.flags", get_eeprom_cellos_flags());
    create_alt_sys_parm(parm, "sys.cellos.spu.configure", get_eeprom_cellos_spu_configure());
    create_alt_sys_parm(parm, "sys.ac.misc2", g_sys_ac_misc2);
    create_alt_sys_parm(parm, "sys.sata.param", get_eeprom_sata_param());
    create_alt_sys_parm(parm, "sys.ac.product_code", g_sys_ac_product_code);
    create_alt_sys_parm(parm, "sys.boot.gos", get_boot_gos());
}

int boot_parm_nop(struct boot_parm *parm)
{
    return 0;
}

long setup_boot_parm(struct boot_parm *parm)
{
    build_alt_sys_parm(parm);
    fill_boot_parm_header((long)parm);
    __asm__ volatile ("mtspr 0x131, %0" :: "r"(parm));
    return 0;
}
