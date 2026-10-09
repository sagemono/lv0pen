#include "config.h"
#include "log.h"

unsigned int get_eeprom_dgbe_ip_gateway(void)
{
    unsigned int buf[4];
    long rc = read_eeprom_dgbe_ip_gateway(buf);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_dgbe_ip_gateway %d\n", LV0_ERR_CONFIG, rc);
    unsigned int gateway = buf[0];
    if (gateway == 0xFFFFFFFF)
        gateway = 0xC0A80001;
    return gateway;
}
