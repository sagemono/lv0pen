#include <spu_intrinsics.h>

unsigned int g_exit_status __attribute__((section(".data.exit_status")));

void exit_stop(void) __attribute__((section(".text.exit_stop")));
void exit_stop(void)
{
    switch (g_exit_status) {
    case 12:
        spu_stop(0x0c);
        break;
    case 13:
        spu_stop(0x0d);
        break;
    case 14:
        spu_stop(0x0e);
        break;
    case 15:
        spu_stop(0x0f);
        break;
    case 16:
        spu_stop(0x10);
        break;
    case 19:
        spu_stop(0x13);
        break;
    case 22:
        spu_stop(0x16);
        break;
    case 18:
        spu_stop(0x12);
        break;
    case 29:
        spu_stop(0x1d);
        break;
    case 23:
        spu_stop(0x17);
        break;
    case 17:
        spu_stop(0x11);
        break;
    case 28:
        spu_stop(0x1c);
        break;
    case 37:
        spu_stop(0x25);
        break;
    case 40:
        spu_stop(0x28);
        break;
    case 41:
        spu_stop(0x29);
        break;
    case 42:
        spu_stop(0x2a);
        break;
    case 43:
        spu_stop(0x2b);
        break;
    case 48:
        spu_stop(0x30);
        break;
    default:
        spu_stop(0x15);
        break;
    }
    for (;;)
        ;
}
