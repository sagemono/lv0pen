#include <spu_intrinsics.h>
#include "sha1.h"

void prng_add160(vec_uint4 *r, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 s, c, t;

    s = spu_add(a[1], b[1]);
    c = spu_genc(a[1], b[1]);
    r[1] = s;
    c = spu_rlmaskqwbyte(c, -12);
    t = spu_gencx(a[0], b[0], c);
    s = spu_addx(a[0], b[0], c);
    c = spu_slqwbyte(t, 4);
    t = spu_genc(s, c);
    s = spu_add(s, c);
    c = spu_slqwbyte(t, 4);
    t = spu_genc(s, c);
    s = spu_add(s, c);
    c = spu_slqwbyte(t, 4);
    r[0] = spu_add(s, c);
}

static const vec_uchar16 prng_pick01 = { 0, 1, 2, 3, 16, 17, 18, 19,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
static const vec_uchar16 prng_pick0 = { 0, 1, 2, 3, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
static const vec_uchar16 prng_pick0011 = { 0, 1, 2, 3, 4, 5, 6, 7,
    16, 17, 18, 19, 20, 21, 22, 23 };
static const vec_uint4 prng_one = { 1, 0, 0, 0 };

typedef union {
    vec_uint4 v[2];
    vec_uchar16 b[2];
} prng160;

int prng_generate(unsigned char *out, unsigned char *xkey, const unsigned char *xseed)
{
    prng160 k, s, v;
    vec_uint4 block[4];
    sha1_ctx ctx;
    const unsigned char *p;
    unsigned char *d;
    int i;

    k.v[0] = spu_splats(0u);
    k.v[1] = spu_splats(0u);
    s.v[0] = spu_splats(0u);
    s.v[1] = spu_splats(0u);
    v.v[0] = spu_splats(0u);
    v.v[1] = spu_splats(0u);
    for (i = 0; i < 16; i++) {
        p = xkey + i;
        k.v[0] = (vec_uint4)spu_insert(*p, k.b[0], i);
    }
    for (i = 0; i < 4; i++) {
        p = xkey + i + 16;
        k.v[1] = (vec_uint4)spu_insert(*p, k.b[1], i);
    }
    for (i = 0; i < 16; i++) {
        p = xseed + i;
        s.v[0] = (vec_uint4)spu_insert(*p, s.b[0], i);
    }
    for (i = 0; i < 4; i++) {
        p = xseed + i + 16;
        s.v[1] = (vec_uint4)spu_insert(*p, s.b[1], i);
    }
    prng_add160(v.v, k.v, s.v);
    block[0] = v.v[0];
    block[1] = v.v[1];
    block[2] = spu_splats(0u);
    block[3] = spu_splats(0u);
    sha1_init(&ctx);
    sha1_transform(&ctx, (unsigned char *)block, 1);
    s.v[0] = spu_shuffle(ctx.h[0], ctx.h[1], prng_pick01);
    s.v[1] = spu_shuffle(ctx.h[2], ctx.h[3], prng_pick01);
    s.v[0] = (vec_uint4)spu_shuffle(s.b[0], s.b[1], prng_pick0011);
    s.v[1] = spu_shuffle(ctx.h[4], spu_splats(0u), prng_pick0);
    for (i = 0; i < 16; i++) {
        d = out + i;
        *d = spu_extract(s.b[0], i);
    }
    for (i = 0; i < 4; i++) {
        d = out + i + 16;
        *d = spu_extract(s.b[1], i);
    }
    prng_add160(v.v, s.v, k.v);
    s.v[0] = spu_splats(0u);
    s.v[1] = prng_one;
    prng_add160(k.v, s.v, v.v);
    for (i = 0; i < 16; i++) {
        d = xkey + i;
        *d = spu_extract(k.b[0], i);
    }
    for (i = 0; i < 4; i++) {
        d = xkey + i + 16;
        *d = spu_extract(k.b[1], i);
    }
    return 0;
}
