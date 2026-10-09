#include "config.h"
#include "log.h"

unsigned int get_eeprom_cellos_spu_configure(void)
{
    unsigned int val;
    long rc = read_eeprom_cellos_spu_configure(&val);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_cellos_spu_configure %d\n", LV0_ERR_CONFIG, rc);
    return val;
}
