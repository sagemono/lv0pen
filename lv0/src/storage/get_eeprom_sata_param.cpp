#include "config.h"
#include "log.h"

unsigned int get_eeprom_sata_param(void)
{
    unsigned int val;
    long rc = read_eeprom_sata_param(&val);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_sata_param %d\n", LV0_ERR_CONFIG, rc);
    return val;
}
