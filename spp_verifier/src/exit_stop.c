#include <spu_intrinsics.h>

void exit_stop(int status)
{
    spu_writech(23, 0);
    while (spu_readchcnt(24) != 1)
        ;
    spu_readch(24);
    spu_writech(22, -1);
    spu_writech(23, 2);
    spu_readch(24);
    spu_writech(28, status);
    spu_stop(0x102);
}
