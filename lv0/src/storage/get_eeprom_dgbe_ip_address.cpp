#include "config.h"
#include "log.h"

unsigned int get_eeprom_dgbe_ip_address(void)
{
    unsigned int buf[4];
    long rc = read_eeprom_dgbe_ip_address(buf);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_dgbe_ip_address %d\n", LV0_ERR_CONFIG, rc);
    unsigned int addr = buf[0];
    if (addr == 0xFFFFFFFF)
        addr = 0xC0A80002;
    return addr;
}
