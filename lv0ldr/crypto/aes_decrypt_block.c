#include "aes_inv_sbox.h"

int aes_decrypt_block(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr)
{
    vec_uchar16 s0 = ISB0, s1 = ISB1, s2 = ISB2, s3 = ISB3;
    vec_uchar16 s4 = ISB4, s5 = ISB5, s6 = ISB6, s7 = ISB7;
    vec_uchar16 s8 = ISB8, s9 = ISB9, s10 = ISB10, s11 = ISB11;
    vec_uchar16 s12 = ISB12, s13 = ISB13, s14 = ISB14, s15 = ISB15;
    vec_uchar16 isr0 = ISR0, isr1 = ISR1, isr2 = ISR2, isr3 = ISR3;
    vec_uchar16 x, s, t, hi, x2, x4, x8, x9;
    int r;

    x = spu_xor(*in, rk[nr]);
    for (r = nr - 1; r > 0; r--) {
        SUBBYTES(s, x);
        t = spu_xor(s, rk[r]);
        hi = spu_and((vec_uchar16)spu_rlmaskqw((vec_uint4)t, -5), (unsigned char)7);
        x2 = spu_xor(spu_and((vec_uchar16)spu_slqw((vec_uint4)t, 1), (unsigned char)0x3e), spu_shuffle(T2, T2, hi));
        x4 = spu_xor(spu_and((vec_uchar16)spu_slqw((vec_uint4)t, 2), (unsigned char)0x7c), spu_shuffle(T4, T4, hi));
        x8 = spu_xor(spu_and((vec_uchar16)spu_slqw((vec_uint4)t, 3), (unsigned char)0xf8), spu_shuffle(T8, T8, hi));
        x9 = spu_xor(x8, t);
        x = spu_xor(spu_xor(spu_shuffle(spu_xor(x8, spu_xor(x4, x2)), ZERO, isr0), spu_shuffle(spu_xor(x9, x2), ZERO, isr1)),
                    spu_xor(spu_shuffle(spu_xor(x9, x4), ZERO, isr2), spu_shuffle(x9, ZERO, isr3)));
    }
    SUBBYTES(s, x);
    *out = spu_xor(spu_shuffle(s, ZERO, isr0), *rk);
    return 0;
}
