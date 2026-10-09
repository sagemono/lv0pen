#include "memory.h"
#include "platform.h"

boot_parm::boot_parm()
    : alt_sys_parm_len(0), reserved_868(0), reserved_870(0)
{
    lv0_memset(body, 0, sizeof(body));
}

boot_parm *get_boot_parm()
{
    static boot_parm parm;
    return &parm;
}
