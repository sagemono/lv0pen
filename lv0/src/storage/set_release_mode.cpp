#include "config.h"
#include "log.h"

static unsigned char s_release_mode;

void set_release_mode(unsigned char release)
{
    unsigned char mode;

    s_release_mode = release;
    int rc = read_eeprom_release_mode(&mode);
    if (rc == 0) {
        if (s_release_mode) {
            if (!(mode & 1))
                return;
            rc = write_eeprom_release_mode(mode & ~1);
        } else {
            if (mode & 1)
                return;
            rc = write_eeprom_release_mode(mode | 1);
        }
        if (__builtin_expect(rc == 0, 1))
            return;
    }
    log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x set_release_mode %d\n", rc);
}
