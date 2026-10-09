#include "config.h"
#include "platform.h"
#include "storage.h"

int read_eeprom_nv4_u64_at5(unsigned long *out)
{
    long rc = load_nv_entry(4);
    if (!rc) *out = *(unsigned long *)(g_nv_entry_table[4].data.buf + 5);
    return rc;
}

int get_eeprom_bootrom_trace_leve(unsigned char *out_level)
{
    long rc = load_nv_entry(2);
    if (!rc) *out_level = g_nv_entry_table[2].data.buf[17];
    return rc;
}
