#include "config.h"
#include "log.h"

unsigned char get_eeprom_flash_ext_format(void)
{
    unsigned char val;
    long rc = read_eeprom_flash_ext_format(&val);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_flash_ext_format %d\n", LV0_ERR_CONFIG, rc);
    return val;
}
