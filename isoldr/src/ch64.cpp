#include <spu_intrinsics.h>

long ch64_request(void)
{
    if (spu_readch(SPU_RdMachStat) & 0x40000)
        return -1;
    spu_writech(64, 0x60000);
    return 0;
}
