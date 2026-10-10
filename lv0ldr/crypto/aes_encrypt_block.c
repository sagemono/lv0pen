#include "aes_sbox.h"

int aes_encrypt_block(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr)
{
    vec_uchar16 s0 = SB0, s1 = SB1, s2 = SB2, s3 = SB3;
    vec_uchar16 s4 = SB4, s5 = SB5, s6 = SB6, s7 = SB7;
    vec_uchar16 s8 = SB8, s9 = SB9, s10 = SB10, s11 = SB11;
    vec_uchar16 s12 = SB12, s13 = SB13, s14 = SB14, s15 = SB15;
    vec_uchar16 sr0 = SR0, sr1 = SR1, sr2 = SR2, sr3 = SR3;
    vec_uchar16 x, s, x2, x3, t2, t3;
    int r;

    x = spu_xor(*in, *rk++);
    for (r = 1; r < nr; r++) {
        SUBBYTES(s, x);
        t2 = spu_shuffle(s, ZERO, sr2);
        t3 = spu_shuffle(s, ZERO, sr3);
        x2 = spu_xor(spu_and((vec_uchar16)spu_slqw((vec_uint4)s, 1), (unsigned char)0xfe),
                     spu_and((vec_uchar16)spu_cmpgt(s, (unsigned char)0x7f), (unsigned char)0x1b));
        x3 = spu_xor(x2, s);
        x = spu_xor(*rk++, spu_xor(spu_xor(spu_shuffle(x2, ZERO, sr0), spu_shuffle(x3, ZERO, sr1)),
                                   spu_xor(t2, t3)));
    }
    SUBBYTES(s, x);
    *out = spu_xor(spu_shuffle(s, ZERO, sr0), *rk);
    return 0;
}
