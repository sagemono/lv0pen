#include "config.h"
#include "syscon.h"
#include "log.h"

extern rsx_rdcy_reader_fn *const rsx_rdcy_reader_table[8] = {
    read_eeprom_rsx_rdcy_1, read_eeprom_rsx_rdcy_2,
    read_eeprom_rsx_rdcy_3, read_eeprom_rsx_rdcy_4,
    read_eeprom_rsx_rdcy_5, read_eeprom_rsx_rdcy_6,
    read_eeprom_rsx_rdcy_7, read_eeprom_rsx_rdcy_8,
};

static unsigned long s_hw_config = -1;
static unsigned long s_hw_config2 = -1;
static bool s_hw_config_read;

unsigned long get_hw_config(int which)
{
    if (!s_hw_config_read) {
        unsigned short sc_ver;
        int rc = get_sc_version(18, &sc_ver);
        if (rc) {
            log_message("[ERRPR]: sc_version %d\n", rc);
            return -1;
        }
        if (sc_ver <= 0x107)
            return -1;
        rc = syscon_get_hw_config(&s_hw_config, &s_hw_config2);
        if (rc) {
            log_message("[ERRPR]: get H/W config %d\n", rc);
            return -1;
        }
        s_hw_config_read = true;
    }
    return which ? s_hw_config2 : s_hw_config;
}
