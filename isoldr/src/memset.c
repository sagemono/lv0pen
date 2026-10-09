#include <spu_intrinsics.h>

typedef unsigned int size_t;

void *memset(void *s, int c, size_t n)
{
    vec_uchar16 *p;
    vec_uchar16 v = spu_splats((unsigned char)c);
    unsigned int off, nq, rem, end, head;

    if (n == 0)
        return s;
    off = (unsigned int)s & 15;
    p = (vec_uchar16 *)s;
    nq = n & ~15;
    if (off == 0) {
        rem = n & 15;
        while (nq) {
            *p++ = v;
            nq -= 16;
        }
        if (rem)
            *p = spu_sel(v, *p, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -rem));
        return s;
    }
    end = off + n;
    head = 16 - off;
    {
        vec_uchar16 old = *p;
        vec_uchar16 m = spu_slqwbyte((vec_uchar16)spu_splats(-1), head);
        vec_uchar16 *q;

        if (end <= 15) {
            *p = spu_sel(v, old, spu_or(m, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -end)));
            return s;
        }
        *p = spu_sel(v, old, m);
        q = (vec_uchar16 *)((char *)s + head);
        n -= head;
        while (n > 15) {
            *q++ = v;
            n -= 16;
        }
        if (n)
            *q = spu_sel(v, *q, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -n));
    }
    return s;
}
