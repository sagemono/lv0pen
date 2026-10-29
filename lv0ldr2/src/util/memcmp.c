#include <spu_intrinsics.h>

typedef unsigned int size_t;

extern const vec_uchar16 shuffle_table[17];

int memcmp(const void *a, const void *b, size_t n)
{
    const vec_uchar16 *pa = (const vec_uchar16 *)a;
    const vec_uchar16 *pb = (const vec_uchar16 *)b;
    vec_uchar16 pata = shuffle_table[(unsigned int)a & 15];
    vec_uchar16 patb = shuffle_table[(unsigned int)b & 15];
    unsigned int left = n + 32;
    vec_uchar16 x, y;
    vec_uint4 eq;
    unsigned int i, j;

    do {
        vec_uchar16 a0 = *pa++;
        vec_uchar16 b0 = *pb++;

        left -= 16;
        x = spu_shuffle(a0, *pa, pata);
        y = spu_shuffle(b0, *pb, patb);
        eq = spu_gather(spu_cmpeq(x, y));
        if (spu_extract(eq, 0) != 0xFFFF)
            break;
    } while (left > 32);

    i = spu_extract(spu_cntlz(spu_xor(eq, 0xFFFF)), 0);
    j = spu_extract(spu_cntlz(spu_gather(spu_cmpgt(x, y))), 0);
    return left > i ? (j == i ? 1 : -1) : 0;
}
