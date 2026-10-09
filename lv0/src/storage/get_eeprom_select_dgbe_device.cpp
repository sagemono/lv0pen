#include "config.h"
#include "log.h"

long get_eeprom_select_dgbe_device(void)
{
    unsigned char dev_sel[16];
    long rc = read_eeprom_select_dgbe_device(dev_sel);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_select_dgbe_device %d\n", LV0_ERR_CONFIG, rc);
    if ((unsigned char)(dev_sel[0] - 1) <= 0xFD)
        return dev_sel[0];
    return 0;
}
