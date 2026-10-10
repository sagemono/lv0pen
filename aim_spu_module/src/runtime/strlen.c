#include <spu_intrinsics.h>

typedef unsigned int size_t;

size_t strlen(const char *s)
{
    unsigned int skip = (unsigned int)s & 15;
    const vec_uchar16 *p = (const vec_uchar16 *)(s - skip);
    unsigned int cmp = spu_extract(spu_gather(spu_cmpeq(*p++, 0)), 0) & (0xFFFFu >> skip);

    while (cmp == 0) {
        unsigned int hi = spu_extract(spu_gather(spu_cmpeq(p[0], 0)), 0) << 16;
        unsigned int lo = spu_extract(spu_gather(spu_cmpeq(p[1], 0)), 0);

        p += 2;
        cmp = hi | lo;
    }
    return spu_extract(spu_cntlz(spu_promote(cmp, 0)), 0) + ((const char *)p - s) - 32;
}
