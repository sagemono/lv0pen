#include "config.h"
#include "platform.h"
#include "spu.h"
#include "memory.h"
#include "syscon.h"

#include "storage.h"

unsigned char sc_get_version_subcmd = 1;

long read_eeprom_os_boot_order_flag(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[0];
    return rc;
}

int read_eeprom_select_dgbe_device(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[3];
    return rc;
}

int read_eeprom_48c08(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[8];
    return rc;
}

int read_eeprom_qa_flag(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[10];
    return rc;
}

int read_eeprom_cellos_flags(unsigned short *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = *(unsigned short *)(g_nv_entry_table[2].data.buf + 15);
    return rc;
}

int read_eeprom_core_clock_multiplier(unsigned char *out)
{
    long rc = load_nv_entry(3);
    if (!rc) *out = g_nv_entry_table[3].data.buf[0];
    return rc;
}

int read_eeprom_ref_clock(unsigned char *out)
{
    long rc = load_nv_entry(3);
    if (!rc) *out = g_nv_entry_table[3].data.buf[1];
    return rc;
}

int read_eeprom_restrict_spu_flag(unsigned char *out)
{
    long rc = load_nv_entry(4);
    if (!rc) *out = g_nv_entry_table[4].data.buf[0];
    return rc;
}

int read_eeprom_rsx_rdcy_1(unsigned long *out)
{
    long rc = load_nv_entry(6);
    if (!rc) *out = *(unsigned long *)g_nv_entry_table[6].data.buf;
    return rc;
}

int read_eeprom_rsx_rdcy_2(unsigned long *out)
{
    long rc = load_nv_entry(6);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[6].data.buf + 8);
    return rc;
}

int read_eeprom_rsx_rdcy_3(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)g_nv_entry_table[7].data.buf;
    return rc;
}

int read_eeprom_rsx_rdcy_4(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[7].data.buf + 8);
    return rc;
}

int read_eeprom_rsx_rdcy_5(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[7].data.buf + 16);
    return rc;
}

int read_eeprom_rsx_rdcy_6(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[7].data.buf + 24);
    return rc;
}

int read_eeprom_rsx_rdcy_7(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[7].data.buf + 32);
    return rc;
}

int read_eeprom_rsx_rdcy_8(unsigned long *out)
{
    long rc = load_nv_entry(7);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[7].data.buf + 40);
    return rc;
}

int read_eeprom_dgbe_ip_address(unsigned int *out)
{
    long rc = load_nv_entry(8);
    if (!rc) *out = *(unsigned int *)g_nv_entry_table[8].data.buf;
    return rc;
}

int read_eeprom_dgbe_ip_netmask(unsigned int *out)
{
    long rc = load_nv_entry(8);
    if (!rc) *out = *(unsigned int *)(g_nv_entry_table[8].data.buf + 4);
    return rc;
}

int read_eeprom_dgbe_ip_gateway(unsigned int *out)
{
    long rc = load_nv_entry(8);
    if (!rc) *out = *(unsigned int *)(g_nv_entry_table[8].data.buf + 8);
    return rc;
}

int read_eeprom_gbe_macaddr(unsigned long *out)
{
    long rc = load_nv_entry(9);
    if (!rc) *out = *(unsigned long *)g_nv_entry_table[9].data.buf;
    return rc;
}

int read_eeprom_sata_param(unsigned int *out)
{
    long rc = load_nv_entry(4);
    if (!rc) *out = *(unsigned int *)(g_nv_entry_table[4].data.buf + 1);
    return rc;
}

int read_eeprom_core_os_bank_indicator(unsigned char *out)
{
    long rc = load_nv_entry(3);
    if (!rc) *out = g_nv_entry_table[3].data.buf[2];
    return rc;
}

int read_eeprom_cellos_spu_configure(unsigned int *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = *(unsigned int *)(g_nv_entry_table[2].data.buf + 20);
    return rc;
}

int read_eeprom_flash_ext_format(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[19];
    return rc;
}

int read_eeprom_release_mode(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[6];
    return rc;
}

int write_eeprom_release_mode(unsigned char mode)
{
    struct nv_entry *entry = &g_nv_entry_table[2];
    int rc = nv_storage::write(entry->block, entry->offset + 6, 1, &mode, entry->base);
    if (!rc)
        entry->data.buf[6] = mode;
    return rc;
}

int read_eeprom_qa_token(void *dst)
{
    long rc = load_nv_entry(10);
    if (!rc)
        lv0_memmove((char *)dst, (const char *)g_nv_entry_table[10].data.buf, g_nv_entry_table[10].data.size);
    return rc;
}

int write_eeprom_restrict_spu_flag(unsigned char flag)
{
    struct nv_entry *entry = &g_nv_entry_table[4];
    int rc = nv_storage::write(entry->block, entry->offset, 1, &flag, entry->base);
    if (!rc)
        *entry->data.buf = flag;
    return rc;
}
