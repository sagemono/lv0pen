#include "config.h"
#include "log.h"

int get_eeprom_os_boot_order_flag(void)
{
    unsigned char flag[16];
    int rc = read_eeprom_os_boot_order_flag(flag);

    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_os_boot_order_flag %d\n", LV0_ERR_CONFIG, rc);

    return (unsigned char)(flag[0] - 1) > 0xFD;
}
