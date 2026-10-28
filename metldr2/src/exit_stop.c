#include <spu_intrinsics.h>

unsigned int g_exit_status __attribute__((section(".data.exit_status")));

void exit_stop(void) __attribute__((section(".text.exit_stop")));
void exit_stop(void)
{
    switch (g_exit_status) {
    case 35:
        spu_stop(0x23);
        break;
    case 31:
        spu_stop(0x1F);
        break;
    case 32:
        spu_stop(0x20);
        break;
    case 34:
        spu_stop(0x22);
        break;
    case 33:
        spu_stop(0x21);
        break;
    case 30:
        spu_stop(0x1E);
        break;
    case 36:
        spu_stop(0x24);
        break;
    default:
        spu_stop(0x15);
        break;
    }
    for (;;)
        ;
}
