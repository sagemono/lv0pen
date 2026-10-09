#include <spu_intrinsics.h>

typedef unsigned int size_t;

extern const vec_uchar16 shuffle_table[17];

int strncmp(const char *a, const char *b, size_t n)
{
    const vec_uchar16 *pa = (const vec_uchar16 *)a;
    const vec_uchar16 *pb = (const vec_uchar16 *)b;
    vec_uchar16 pata = shuffle_table[(unsigned int)a & 15];
    vec_uchar16 patb = shuffle_table[(unsigned int)b & 15];
    unsigned int left = n + 32;
    vec_uchar16 x, y;
    vec_uint4 ne, nul;
    unsigned int i, j, z;

    do {
        vec_uchar16 a0 = *pa++;
        vec_uchar16 b0 = *pb++;

        left -= 16;
        y = spu_shuffle(b0, *pb, patb);
        x = spu_shuffle(a0, *pa, pata);
        ne = spu_xor(spu_gather(spu_cmpeq(x, y)), 0xFFFF);
        nul = spu_gather(spu_cmpeq(x, 0));
        if (spu_extract(ne, 0) != 0)
            break;
        if (spu_extract(nul, 0) != 0)
            break;
    } while (left > 32);

    i = spu_extract(spu_cntlz(ne), 0);
    z = spu_extract(spu_cntlz(nul), 0);
    j = spu_extract(spu_cntlz(spu_gather(spu_cmpgt(x, y))), 0);
    if (__builtin_expect(left <= i, 1))
        return 0;
    if (__builtin_expect(i > z, 1))
        return 0;
    return j == i ? 1 : -1;
}
