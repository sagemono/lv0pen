#include "config.h"
#include "log.h"

unsigned int get_eeprom_dgbe_ip_netmask(void)
{
    unsigned int buf[4];
    long rc = read_eeprom_dgbe_ip_netmask(buf);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_dgbe_ip_netmask %d\n", LV0_ERR_CONFIG, rc);
    unsigned int mask = buf[0];
    if (mask == 0xFFFFFFFF)
        mask = 0xFFFFFF00;
    return mask;
}
