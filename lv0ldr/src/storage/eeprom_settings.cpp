#include "syscon.h"
#include "storage.h"

long get_eeprom_select_net_device(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[2];
    return rc;
}

long get_eeprom_update_flag(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[5];
    return rc;
}

long get_eeprom_boot_fir_config(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc)
        *out = g_nv_entry_table[2].data.buf[9];
    return rc;
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
