#include "aes_sbox.h"

#define ENC_SUB(i) do { \
        SUBBYTES(s, b##i); \
        hi = spu_and((vec_uchar16)spu_rlmaskqw((vec_uint4)s, -5), (unsigned char)7); \
        x2_##i = spu_xor(spu_and((vec_uchar16)spu_slqw((vec_uint4)s, 1), (unsigned char)0x3e), spu_shuffle(T2, T2, hi)); \
        x3_##i = spu_xor(x2_##i, s); \
        u_##i = spu_xor(spu_shuffle(s, ZERO, sr2), spu_shuffle(s, ZERO, sr3)); \
    } while (0)
#define ENC_ROT(i) x2_##i = spu_shuffle(x2_##i, ZERO, sr0)
#define ENC_MIX(i) b##i = spu_xor(k, spu_xor(spu_xor(x2_##i, spu_shuffle(x3_##i, ZERO, sr1)), u_##i))
#define ENC_LAST(o, x) do { \
        SUBBYTES(s, x); \
        o = spu_xor(spu_shuffle(s, ZERO, sr0), k); \
    } while (0)

int aes_encrypt8(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr)
{
    vec_uchar16 s0 = SB0, s1 = SB1, s2 = SB2, s3 = SB3;
    vec_uchar16 s4 = SB4, s5 = SB5, s6 = SB6, s7 = SB7;
    vec_uchar16 s8 = SB8, s9 = SB9, s10 = SB10, s11 = SB11;
    vec_uchar16 s12 = SB12, s13 = SB13, s14 = SB14, s15 = SB15;
    vec_uchar16 sr0 = SR0, sr1 = SR1, sr2 = SR2, sr3 = SR3;
    vec_uchar16 b0, b1, b2, b3, b4, b5, b6, b7;
    vec_uchar16 x2_0, x2_1, x2_2, x2_3, x2_4, x2_5, x2_6, x2_7;
    vec_uchar16 x3_0, x3_1, x3_2, x3_3, x3_4, x3_5, x3_6, x3_7;
    vec_uchar16 u_0, u_1, u_2, u_3, u_4, u_5, u_6, u_7;
    vec_uchar16 k, s, hi;
    int r;

    k = *rk++;
    b0 = spu_xor(in[0], k);
    b1 = spu_xor(in[1], k);
    b2 = spu_xor(in[2], k);
    b3 = spu_xor(in[3], k);
    b4 = spu_xor(in[4], k);
    b5 = spu_xor(in[5], k);
    b6 = spu_xor(in[6], k);
    b7 = spu_xor(in[7], k);
    for (r = 1; r < nr; r++) {
        k = *rk++;
        ENC_SUB(0); ENC_SUB(1); ENC_SUB(2); ENC_SUB(3);
        ENC_SUB(4); ENC_SUB(5); ENC_SUB(6); ENC_SUB(7);
        ENC_ROT(0); ENC_ROT(1); ENC_ROT(2); ENC_ROT(3);
        ENC_ROT(4); ENC_ROT(5); ENC_ROT(6); ENC_ROT(7);
        ENC_MIX(0); ENC_MIX(1); ENC_MIX(2); ENC_MIX(3);
        ENC_MIX(4); ENC_MIX(5); ENC_MIX(6); ENC_MIX(7);
    }
    k = *rk;
    ENC_LAST(out[0], b0);
    ENC_LAST(out[1], b1);
    ENC_LAST(out[2], b2);
    ENC_LAST(out[3], b3);
    ENC_LAST(out[4], b4);
    ENC_LAST(out[5], b5);
    ENC_LAST(out[6], b6);
    ENC_LAST(out[7], b7);
    return 0;
}
