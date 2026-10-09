#include "syscon.h"
#include "log.h"

static unsigned char s_boot_gos;
static unsigned char s_boot_gos_read;

int get_boot_gos(void)
{
    if (!s_boot_gos_read) {
        unsigned int len;
        int rc = sc_nv_storage::read(255, 0, 1, &s_boot_gos, &len);
        if (__builtin_expect(rc != 0, 0))
            log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x sc_nv_storage %d\n", rc);
        else if (__builtin_expect(len != 1, 0))
            log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x sc_nv_storage size %d\n", len);
        s_boot_gos_read = 1;
    }
    return s_boot_gos & 3;
}
