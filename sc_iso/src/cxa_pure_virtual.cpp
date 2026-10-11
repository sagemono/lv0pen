#include <spu_intrinsics.h>

extern "C" void __cxa_pure_virtual(void)
{
    spu_stop(0x3FFF);
}
