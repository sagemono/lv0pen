#include "config.h"
#include "log.h"

int get_eeprom_select_net_device(void)
{
    unsigned char buf[16];
    int rc = read_eeprom_select_net_device(buf);

    if (__builtin_expect(rc != 0, 0))
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_select_net_device %d\n", LV0_ERR_CONFIG, rc);

    switch (buf[0]) {
    case 2:  return 3;
    case 1:  return 2;
    case 3:  return 4;
    case 4:  return 5;
    default: return 6;
    }
}
