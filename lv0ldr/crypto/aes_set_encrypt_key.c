#include "aes_sbox.h"

static const vec_uint4 rcon[10] = {
    {0x01000000, 0x01000000, 0x01000000, 0x01000000}, {0x02000000, 0x02000000, 0x02000000, 0x02000000},
    {0x04000000, 0x04000000, 0x04000000, 0x04000000}, {0x08000000, 0x08000000, 0x08000000, 0x08000000},
    {0x10000000, 0x10000000, 0x10000000, 0x10000000}, {0x20000000, 0x20000000, 0x20000000, 0x20000000},
    {0x40000000, 0x40000000, 0x40000000, 0x40000000}, {0x80000000, 0x80000000, 0x80000000, 0x80000000},
    {0x1b000000, 0x1b000000, 0x1b000000, 0x1b000000}, {0x36000000, 0x36000000, 0x36000000, 0x36000000},
};

#define W0 ((vec_uchar16)spu_splats(0x00010203u))
#define W1X ((vec_uchar16){0x80,0x80,0x80,0x80, 4,5,6,7, 4,5,6,7, 4,5,6,7})
#define W2X ((vec_uchar16){0x80,0x80,0x80,0x80, 0x80,0x80,0x80,0x80, 8,9,10,11, 8,9,10,11})
#define MASK3 ((vec_uchar16)spu_maskb(0x000f))

int aes_set_encrypt_key(vec_uchar16 *rk, const unsigned char *key, int bits)
{
    vec_uchar16 s0 = SB0, s1 = SB1, s2 = SB2, s3 = SB3;
    vec_uchar16 s4 = SB4, s5 = SB5, s6 = SB6, s7 = SB7;
    vec_uchar16 s8 = SB8, s9 = SB9, s10 = SB10, s11 = SB11;
    vec_uchar16 s12 = SB12, s13 = SB13, s14 = SB14, s15 = SB15;
    vec_uchar16 tmp[16];
    int i;

    switch (bits) {
    case 128: {
        vec_uchar16 k0, t, s, n;
        vec_uchar16 c, w0, w1, w2;

        k0 = *(const vec_uchar16 *)key;
        rk[0] = k0;
        c = (vec_uchar16)spu_splats(0x0d0e0f0cu);
        w0 = W0;
        w1 = W1X;
        w2 = W2X;
        for (i = 0; i < 10; i++) {
            t = spu_shuffle(k0, ZERO, c);
            SUBBYTES(s, t);
            n = spu_xor(spu_xor(spu_xor(spu_shuffle(k0, ZERO, w0), spu_shuffle(k0, ZERO, w1)),
                                spu_xor(spu_shuffle(k0, ZERO, w2), spu_and(k0, MASK3))),
                        spu_xor((vec_uchar16)rcon[i], s));
            rk[i + 1] = n;
            k0 = n;
        }
        return 10;
    }
    case 192: {
        vec_uchar16 k0, k1, t, s, n, u, m, v;
        vec_uchar16 w1, p12, c, w0, w2, m3, p10;

        w1 = W1X;
        c = (vec_uchar16)spu_splats(0x15161714u);
        w0 = W0;
        w2 = W2X;
        m3 = MASK3;
        p10 = ((vec_uchar16){0x10,0x11,0x12,0x13, 0x10,0x11,0x12,0x13, 0x80,0x80,0x80,0x80, 0x80,0x80,0x80,0x80});
        p12 = ((vec_uchar16){12,13,14,15, 12,13,14,15, 0x80,0x80,0x80,0x80, 0x80,0x80,0x80,0x80});
        k0 = ((const vec_uchar16 *)key)[0];
        k1 = ((const vec_uchar16 *)key)[1];
        i = 0;
        for (;;) {
            tmp[2 * i] = k0;
            tmp[2 * i + 1] = k1;
            t = spu_shuffle(k0, k1, c);
            SUBBYTES(s, t);
            n = spu_xor(spu_xor(spu_xor(spu_shuffle(k0, ZERO, w0), spu_shuffle(k0, ZERO, w1)),
                                spu_xor(spu_shuffle(k0, ZERO, w2), spu_and(k0, m3))),
                        spu_xor((vec_uchar16)rcon[i], s));
            t = spu_shuffle(k0, k1, p10);
            m = spu_and(k1, (vec_uchar16)spu_maskb(0x0f00));
            i++;
            if (i == 8)
                break;
            v = spu_xor(t, m);
            k0 = n;
            u = spu_xor(spu_shuffle(n, u, p12), v);
            k1 = u;
        }
        for (i = 0; i < 4; i++) {
            rk[3 * i] = tmp[4 * i];
            rk[3 * i + 1] = spu_shuffle(tmp[4 * i + 1], tmp[4 * i + 2], ((vec_uchar16){0,1,2,3, 4,5,6,7, 0x10,0x11,0x12,0x13, 0x14,0x15,0x16,0x17}));
            rk[3 * i + 2] = spu_shuffle(tmp[4 * i + 2], tmp[4 * i + 3], ((vec_uchar16){8,9,10,11, 12,13,14,15, 0x10,0x11,0x12,0x13, 0x14,0x15,0x16,0x17}));
        }
        rk[12] = n;
        return 12;
    }
    case 256: {
        vec_uchar16 k0, k1, t, s, n, sel;
        vec_uchar16 c1, c2, w0, w1, m3;

        k0 = ((const vec_uchar16 *)key)[0];
        k1 = ((const vec_uchar16 *)key)[1];
        rk[0] = k0;
        rk[1] = k1;
        n = k1;
        sel = ZERO;
        c1 = (vec_uchar16)spu_splats(0x1d1e1f1cu);
        c2 = (vec_uchar16)spu_splats(0x1c1d1e1fu);
        w0 = W0;
        w1 = W1X;
        m3 = MASK3;
        for (i = 0; i < 13; i++) {
            t = spu_sel(spu_shuffle(ZERO, n, c1), spu_shuffle(ZERO, n, c2), sel);
            SUBBYTES(s, t);
            n = spu_xor(spu_xor(spu_xor(spu_shuffle(k0, ZERO, w0), spu_shuffle(k0, ZERO, w1)),
                                spu_xor(spu_shuffle(k0, ZERO, W2X), spu_and(k0, m3))),
                        spu_xor(spu_andc((vec_uchar16)rcon[i / 2], sel), s));
            k0 = k1;
            k1 = n;
            rk[i + 2] = n;
            sel = spu_xor(sel, (unsigned char)0xff);
        }
        return 14;
    }
    }
    return -1;
}
