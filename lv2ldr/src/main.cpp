#include "loader.h"
#include <spu_intrinsics.h>

int main(void)
{
    switch (g_loader.run()) {
    case 11: spu_stop(0xB); break;
    case 10: spu_stop(0xA); break;
    case 50: spu_stop(0x32); break;
    case 17: spu_stop(0x11); break;
    case 12: spu_stop(0xC); break;
    case 13: spu_stop(0xD); break;
    case 14: spu_stop(0xE); break;
    case 15: spu_stop(0xF); break;
    case 16: spu_stop(0x10); break;
    case 19: spu_stop(0x13); break;
    case 29: spu_stop(0x1D); break;
    case 22: spu_stop(0x16); break;
    case 23: spu_stop(0x17); break;
    case 28: spu_stop(0x1C); break;
    case 37: spu_stop(0x25); break;
    case 48: spu_stop(0x30); break;
    default: spu_stop(0x15); break;
    }
    return 0;
}
