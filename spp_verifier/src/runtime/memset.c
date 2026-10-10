#include <spu_intrinsics.h>

typedef unsigned int size_t;

void *memset(void *s, int c, size_t n)
{
    vec_uchar16 *p;
    vec_uchar16 v = spu_splats((unsigned char)c);
    unsigned int off, end, head;

    if (n == 0)
        return s;
    off = (unsigned int)s & 15;
    p = (vec_uchar16 *)s;
    if (off == 0) {
        unsigned int nq = n & ~15;

        n &= 15;
        while (nq) {
            *p++ = v;
            nq -= 16;
        }
    } else {
        vec_uchar16 m;

        end = off + n;
        head = 16 - off;
        m = spu_slqwbyte(spu_splats((unsigned char)0xff), head);
        if (end <= 15) {
            *p = spu_sel(v, *p, spu_or(m, spu_rlmaskqwbyte(spu_splats((unsigned char)0xff), -end)));
            return s;
        }
        n -= head;
        *p = spu_sel(v, *p, m);
        p = (vec_uchar16 *)((char *)s + head);
        while (n > 15) {
            *p++ = v;
            n -= 16;
        }
    }
    if (n)
        *p = spu_sel(v, *p, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -n));
    return s;
}
