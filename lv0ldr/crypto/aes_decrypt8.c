#include "aes_inv_sbox.h"

#define L2A ((vec_uchar16){0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e})
#define L2B ((vec_uchar16){0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e})
#define L4A ((vec_uchar16){0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c,0x20,0x24,0x28,0x2c,0x30,0x34,0x38,0x3c})
#define L4B ((vec_uchar16){0x40,0x44,0x48,0x4c,0x50,0x54,0x58,0x5c,0x60,0x64,0x68,0x6c,0x70,0x74,0x78,0x7c})
#define L8A ((vec_uchar16){0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78})
#define L8B ((vec_uchar16){0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8})

#define DEC_ROUND(x) do { \
        SUBBYTES(s, x); \
        t = spu_xor(s, k); \
        lo = spu_and(t, (unsigned char)0x1f); \
        hi = spu_and((vec_uchar16)spu_rlmaskqw((vec_uint4)t, -5), (unsigned char)7); \
        x2 = spu_xor(spu_shuffle(L2A, L2B, lo), spu_shuffle(T2, T2, hi)); \
        x4 = spu_xor(spu_shuffle(L4A, L4B, lo), spu_shuffle(T4, T4, hi)); \
        x8 = spu_xor(spu_shuffle(L8A, L8B, lo), spu_shuffle(T8, T8, hi)); \
        x9 = spu_xor(x8, t); \
        x13 = spu_xor(x9, x4); \
        x = spu_xor(spu_xor(spu_shuffle(spu_xor(x8, spu_xor(x4, x2)), ZERO, isr0), spu_shuffle(spu_xor(x9, x2), ZERO, isr1)), \
                    spu_xor(spu_shuffle(x13, ZERO, isr2), spu_shuffle(x9, ZERO, isr3))); \
    } while (0)
#define DEC_LAST(o, x) do { \
        SUBBYTES(s, x); \
        o = spu_xor(spu_shuffle(s, ZERO, isr0), k); \
    } while (0)

int aes_decrypt8(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr)
{
    vec_uchar16 s0 = ISB0, s1 = ISB1, s2 = ISB2, s3 = ISB3;
    vec_uchar16 s4 = ISB4, s5 = ISB5, s6 = ISB6, s7 = ISB7;
    vec_uchar16 s8 = ISB8, s9 = ISB9, s10 = ISB10, s11 = ISB11;
    vec_uchar16 s12 = ISB12, s13 = ISB13, s14 = ISB14, s15 = ISB15;
    vec_uchar16 isr0 = ISR0, isr1 = ISR1, isr2 = ISR2, isr3 = ISR3;
    vec_uchar16 b0, b1, b2, b3, b4, b5, b6, b7;
    vec_uchar16 k, s, t, lo, hi, x2, x4, x8, x9, x13;
    int r;

    k = rk[nr];
    b0 = spu_xor(in[0], k);
    b1 = spu_xor(in[1], k);
    b2 = spu_xor(in[2], k);
    b3 = spu_xor(in[3], k);
    b4 = spu_xor(in[4], k);
    b5 = spu_xor(in[5], k);
    b6 = spu_xor(in[6], k);
    b7 = spu_xor(in[7], k);
    for (r = nr - 1; r > 0; r--) {
        k = rk[r];
        DEC_ROUND(b0);
        DEC_ROUND(b1);
        DEC_ROUND(b2);
        DEC_ROUND(b3);
        DEC_ROUND(b4);
        DEC_ROUND(b5);
        DEC_ROUND(b6);
        DEC_ROUND(b7);
    }
    k = *rk;
    DEC_LAST(out[0], b0);
    DEC_LAST(out[1], b1);
    DEC_LAST(out[2], b2);
    DEC_LAST(out[3], b3);
    DEC_LAST(out[4], b4);
    DEC_LAST(out[5], b5);
    DEC_LAST(out[6], b6);
    DEC_LAST(out[7], b7);
    return 0;
}
