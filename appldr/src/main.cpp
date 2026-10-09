#include "loader.h"
#include <spu_intrinsics.h>

void stop_with(long code)
{
    switch (code) {
    case 60: spu_stop(0x3C); break;
    case 44: spu_stop(0x2C); break;
    case 10: spu_stop(0xA); break;
    case 47: spu_stop(0x2F); break;
    case 46: spu_stop(0x2E); break;
    case 45: spu_stop(0x2D); break;
    case 23: spu_stop(0x17); break;
    case 17: spu_stop(0x11); break;
    case 12: spu_stop(0xC); break;
    case 13: spu_stop(0xD); break;
    case 14: spu_stop(0xE); break;
    case 15: spu_stop(0xF); break;
    case 16: spu_stop(0x10); break;
    case 19: spu_stop(0x13); break;
    case 22: spu_stop(0x16); break;
    case 29: spu_stop(0x1D); break;
    case 28: spu_stop(0x1C); break;
    case 37: spu_stop(0x25); break;
    case 39: spu_stop(0x27); break;
    case 40: spu_stop(0x28); break;
    case 41: spu_stop(0x29); break;
    case 42: spu_stop(0x2A); break;
    case 43: spu_stop(0x2B); break;
    case 48: spu_stop(0x30); break;
    case 51: spu_stop(0x33); break;
    case 52: spu_stop(0x34); break;
    case 53: spu_stop(0x35); break;
    default: spu_stop(0x15); break;
    }
}

int main(void)
{
    for (;;)
        stop_with(g_loader.run());
}
