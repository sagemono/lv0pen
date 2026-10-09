#include "config.h"
#include "log.h"

unsigned char get_eeprom_core_os_bank_indicator(void)
{
    unsigned char val;
    long rc = read_eeprom_core_os_bank_indicator(&val);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_core_os_bank_indicator %d\n", LV0_ERR_CONFIG, rc);
    return val;
}
