#include <spu_intrinsics.h>

typedef unsigned int size_t;

extern const vec_uchar16 shuffle_table[17];

void *memcpy(void *dst, const void *src, size_t n)
{
    vec_uchar16 *d;
    const vec_uchar16 *s;
    unsigned int doff, soff;

    if (n == 0)
        return dst;
    d = (vec_uchar16 *)dst;
    s = (const vec_uchar16 *)src;
    doff = (unsigned int)dst & 15;
    soff = (unsigned int)src & 15;
    if ((soff | doff) == 0) {
        while (n > 15) {
            *d++ = *s++;
            n -= 16;
        }
        if (n)
            *d = spu_sel(*s, *d, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -n));
        return dst;
    }
    {
        int shift = soff - doff;
        vec_uchar16 s0 = s[0];
        vec_uchar16 s1 = s[1];
        unsigned int end = doff + n;
        unsigned int head = 16 - doff;
        vec_uchar16 old = *d;
        vec_uchar16 m = spu_slqwbyte((vec_uchar16)spu_splats(-1), head);
        vec_uchar16 data, pat;

        if (end <= 15)
            m = spu_or(m, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -end));
        if (shift > 0)
            data = spu_shuffle(s0, s1, shuffle_table[shift]);
        else
            data = spu_rlmaskqwbyte(s0, shift);
        *d = spu_sel(data, old, m);
        if (end <= 15)
            return dst;

        s = (const vec_uchar16 *)((const char *)src + head);
        pat = shuffle_table[(unsigned int)s & 15];
        n -= head;
        d = (vec_uchar16 *)((char *)dst + head);
        while (n > 15) {
            vec_uchar16 a = *s++;
            *d++ = spu_shuffle(a, *s, pat);
            n -= 16;
        }
        if (n)
            *d = spu_sel(spu_shuffle(s[0], s[1], pat), *d, spu_rlmaskqwbyte((vec_uchar16)spu_splats(-1), -n));
    }
    return dst;
}
