#include <spu_intrinsics.h>
#include "types.h"
#include "dma_channel.h"
#include "util.h"

#define LS_BUF          0x3E000
#define LS_BUF_SIZE     0x2000
#define TAG_PUT         3

bool is_valid_ea(u64 ea)
{
    if (ea == 0)
        return false;
    return (ea & 15) == 0;
}

unsigned int put_value(u64 value, u64 ea, unsigned int size)
{
    unsigned int r = 9;

    if (!is_valid_ea(ea))
        return r;
    if (size > LS_BUF_SIZE)
        return r;
    if (size < 8)
        return r;
    memset((void *)LS_BUF, 0, size);
    *(u64 *)LS_BUF = value;
    r = 15;
    {
        dma_channel dma;

        if (dma.issue(LS_BUF, ea, size, TAG_PUT, 0, MFC_PUT) == 0) {
            r = 0;
            dma.wait(TAG_PUT);
        }
    }
    return r;
}

struct quad_arg {
    u64 hi;
    u64 lo;
};

int main(u64 arg0, u64 ea, u64 size, u64 arg3,
         quad_arg a4, quad_arg a5, quad_arg a6, quad_arg a7, quad_arg a8, quad_arg a9,
         quad_arg a10, quad_arg a11, quad_arg a12, quad_arg a13, quad_arg a14, quad_arg a15,
         quad_arg a16, quad_arg a17, quad_arg a18, quad_arg a19, quad_arg a20, u64 value)
{
    unsigned int r;

    if ((unsigned int)arg0 != 0)
        r = 9;
    else
        r = put_value(value, ea, (unsigned int)size);
    switch (r) {
    case 0:
        spu_stop(0x100);
        break;
    case 9:
        spu_stop(0x101);
        break;
    default:
        spu_stop(0x103);
        break;
    }
    return -1;
}
