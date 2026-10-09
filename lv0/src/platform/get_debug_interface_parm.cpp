#include "config.h"

int get_debug_interface_parm(void)
{
    long iface = get_debug_interface();
    switch ((unsigned)iface) {
    case 0: return 1;
    case 1: return 2;
    case 3: return 3;
    case 2: return 0;
    case 4: return 4;
    case 5: return 5;
    case 6: return 6;
    default: return -1;
    }
}
