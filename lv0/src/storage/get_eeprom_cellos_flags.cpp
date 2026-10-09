#include "config.h"
#include "log.h"

long g_flash_type = -1;
int g_flash_format_cache = -1;

unsigned short get_eeprom_cellos_flags(void)
{
    unsigned short flags[8];
    long rc = read_eeprom_cellos_flags(flags);
    if (rc)
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_cellos_flags %d\n", LV0_ERR_CONFIG, rc);
    return flags[0];
}

DEAD_STRING(get_eeprom_gx_config_msg, "[ERROR]: 0x%08x get_eeprom_gx_config %d\n");
