#include "platform.h"
#include "storage.h"
#include "config.h"

int read_eeprom_select_net_device(unsigned char *out)
{
    long rc = load_nv_entry(2);
    if (!rc) *out = g_nv_entry_table[2].data.buf[2];
    return rc;
}
