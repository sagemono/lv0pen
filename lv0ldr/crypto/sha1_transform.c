#include "sha1.h"

#define CH(b, c, d) spu_sel(d, c, b)
#define PARITY(b, c, d) spu_xor(spu_xor(b, c), d)
#define MAJ(b, c, d) spu_or(spu_and(b, c), spu_and(spu_or(b, c), d))

#define ROUND(a, b, c, d, e, f, wk) do { \
        vec_uint4 f_ = f(b, c, d); \
        e = spu_add(spu_add(e, spu_rlqw(a, 5)), spu_add(f_, wk)); \
        b = spu_rl(b, 30); \
    } while (0)
#define ROUND2(a, b, c, d, e, f, wk) do { \
        vec_uint4 f_ = f(b, c, d); \
        e = spu_add(spu_add(spu_add(e, wk), f_), spu_rlqw(a, 5)); \
        b = spu_rl(b, 30); \
    } while (0)
#define ROUND3(a, b, c, d, e, f, wk) do { \
        vec_uint4 f_ = f(b, c, d); \
        e = spu_add(spu_add(spu_add(e, wk), spu_rlqw(a, 5)), f_); \
        b = spu_rl(b, 30); \
    } while (0)

#define NEXT_T(w0, w1, w2) t = spu_xor(spu_shuffle(w0, w1, P8), spu_xor(w0, w2))
#define NEXT_W(w0, w3) w0 = spu_rl(spu_xor(t, spu_shuffle(w3, spu_rl(spu_xor(t, spu_rlqwbyte(w3, 4)), 1), P4)), 1)

#define P8 ((vec_uchar16){8,9,10,11, 12,13,14,15, 16,17,18,19, 20,21,22,23})
#define P4 ((vec_uchar16){4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19})
#define K1 spu_splats(0x5A827999u)
#define K2 spu_splats(0x6ED9EBA1u)
#define K3 spu_splats(0x8F1BBCDCu)
#define K4 spu_splats(0xCA62C1D6u)
#define P0 0x00010203u
#define P1 0x04050607u
#define P2 0x08090A0Bu
#define P3 0x0C0D0E0Fu
#define WORD(v, i) spu_shuffle(v, spu_splats(0u), (vec_uchar16)spu_splats(P##i))

int sha1_transform(sha1_ctx *ctx, const unsigned char *data, int blocks)
{
    vec_uint4 h0 = ctx->h[0], h1 = ctx->h[1], h2 = ctx->h[2], h3 = ctx->h[3], h4 = ctx->h[4];
    vec_uint4 a, b, c, d, e, w0, w1, w2, w3, wk, t;
    int i;

    for (i = 0; i < blocks; i++) {
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        w0 = ((const vec_uint4 *)data)[0];
        w1 = ((const vec_uint4 *)data)[1];
        w2 = ((const vec_uint4 *)data)[2];
        w3 = ((const vec_uint4 *)data)[3];
        data += 64;
        NEXT_T(w0, w1, w2);
        wk = spu_add(w0, K1);
        NEXT_W(w0, w3);
        ROUND(a, b, c, d, e, CH, WORD(wk, 0));
        ROUND(e, a, b, c, d, CH, WORD(wk, 1));
        ROUND(d, e, a, b, c, CH, WORD(wk, 2));
        ROUND(c, d, e, a, b, CH, WORD(wk, 3));
        NEXT_T(w1, w2, w3);
        wk = spu_add(w1, K1);
        NEXT_W(w1, w0);
        ROUND(b, c, d, e, a, CH, WORD(wk, 0));
        ROUND(a, b, c, d, e, CH, WORD(wk, 1));
        ROUND(e, a, b, c, d, CH, WORD(wk, 2));
        ROUND(d, e, a, b, c, CH, WORD(wk, 3));
        NEXT_T(w2, w3, w0);
        wk = spu_add(w2, K1);
        NEXT_W(w2, w1);
        ROUND(c, d, e, a, b, CH, WORD(wk, 0));
        ROUND(b, c, d, e, a, CH, WORD(wk, 1));
        ROUND(a, b, c, d, e, CH, WORD(wk, 2));
        ROUND(e, a, b, c, d, CH, WORD(wk, 3));
        NEXT_T(w3, w0, w1);
        wk = spu_add(w3, K1);
        NEXT_W(w3, w2);
        ROUND(d, e, a, b, c, CH, WORD(wk, 0));
        ROUND(c, d, e, a, b, CH, WORD(wk, 1));
        ROUND(b, c, d, e, a, CH, WORD(wk, 2));
        ROUND(a, b, c, d, e, CH, WORD(wk, 3));
        NEXT_T(w0, w1, w2);
        wk = spu_add(w0, K1);
        NEXT_W(w0, w3);
        ROUND(e, a, b, c, d, CH, WORD(wk, 0));
        ROUND(d, e, a, b, c, CH, WORD(wk, 1));
        ROUND(c, d, e, a, b, CH, WORD(wk, 2));
        ROUND(b, c, d, e, a, CH, WORD(wk, 3));
        NEXT_T(w1, w2, w3);
        wk = spu_add(w1, K2);
        NEXT_W(w1, w0);
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 0));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 1));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 2));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 3));
        NEXT_T(w2, w3, w0);
        wk = spu_add(w2, K2);
        NEXT_W(w2, w1);
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 0));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 1));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 2));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 3));
        NEXT_T(w3, w0, w1);
        wk = spu_add(w3, K2);
        NEXT_W(w3, w2);
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 0));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 1));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 2));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 3));
        NEXT_T(w0, w1, w2);
        wk = spu_add(w0, K2);
        NEXT_W(w0, w3);
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 0));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 1));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 2));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 3));
        NEXT_T(w1, w2, w3);
        wk = spu_add(w1, K2);
        NEXT_W(w1, w0);
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 0));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 1));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 2));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 3));
        NEXT_T(w2, w3, w0);
        wk = spu_add(w2, K3);
        NEXT_W(w2, w1);
        ROUND3(a, b, c, d, e, MAJ, WORD(wk, 0));
        ROUND3(e, a, b, c, d, MAJ, WORD(wk, 1));
        ROUND3(d, e, a, b, c, MAJ, WORD(wk, 2));
        ROUND3(c, d, e, a, b, MAJ, WORD(wk, 3));
        NEXT_T(w3, w0, w1);
        wk = spu_add(w3, K3);
        NEXT_W(w3, w2);
        ROUND3(b, c, d, e, a, MAJ, WORD(wk, 0));
        ROUND3(a, b, c, d, e, MAJ, WORD(wk, 1));
        ROUND3(e, a, b, c, d, MAJ, WORD(wk, 2));
        ROUND3(d, e, a, b, c, MAJ, WORD(wk, 3));
        NEXT_T(w0, w1, w2);
        wk = spu_add(w0, K3);
        NEXT_W(w0, w3);
        ROUND3(c, d, e, a, b, MAJ, WORD(wk, 0));
        ROUND3(b, c, d, e, a, MAJ, WORD(wk, 1));
        ROUND3(a, b, c, d, e, MAJ, WORD(wk, 2));
        ROUND3(e, a, b, c, d, MAJ, WORD(wk, 3));
        NEXT_T(w1, w2, w3);
        wk = spu_add(w1, K3);
        NEXT_W(w1, w0);
        ROUND3(d, e, a, b, c, MAJ, WORD(wk, 0));
        ROUND3(c, d, e, a, b, MAJ, WORD(wk, 1));
        ROUND3(b, c, d, e, a, MAJ, WORD(wk, 2));
        ROUND3(a, b, c, d, e, MAJ, WORD(wk, 3));
        NEXT_T(w2, w3, w0);
        wk = spu_add(w2, K3);
        NEXT_W(w2, w1);
        ROUND3(e, a, b, c, d, MAJ, WORD(wk, 0));
        ROUND3(d, e, a, b, c, MAJ, WORD(wk, 1));
        ROUND3(c, d, e, a, b, MAJ, WORD(wk, 2));
        ROUND3(b, c, d, e, a, MAJ, WORD(wk, 3));
        NEXT_T(w3, w0, w1);
        wk = spu_add(w3, K4);
        NEXT_W(w3, w2);
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 0));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 1));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 2));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 3));
        wk = spu_add(w0, K4);
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 0));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 1));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 2));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 3));
        wk = spu_add(w1, K4);
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 0));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 1));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 2));
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 3));
        wk = spu_add(w2, K4);
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 0));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 1));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 2));
        ROUND2(a, b, c, d, e, PARITY, WORD(wk, 3));
        wk = spu_add(w3, K4);
        ROUND2(e, a, b, c, d, PARITY, WORD(wk, 0));
        ROUND2(d, e, a, b, c, PARITY, WORD(wk, 1));
        ROUND2(c, d, e, a, b, PARITY, WORD(wk, 2));
        ROUND2(b, c, d, e, a, PARITY, WORD(wk, 3));
        h0 = spu_add(a, h0);
        h1 = spu_add(b, h1);
        h2 = spu_add(c, h2);
        h3 = spu_add(d, h3);
        h4 = spu_add(e, h4);
    }
    ctx->h[0] = h0;
    ctx->h[1] = h1;
    ctx->h[2] = h2;
    ctx->h[3] = h3;
    ctx->h[4] = h4;
    return 0;
}
