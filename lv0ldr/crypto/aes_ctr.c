#include "aes.h"

#define CTR_ONE ((vec_uint4){0, 0, 0, 1})
#define CTR_WORD ((vec_uint4){0, 0, 0, 0xffffffff})

int aes_ctr(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
            vec_uchar16 *iv, int ctr_bits)
{
    vec_uchar16 rk[15], cb[8], ks[8];
    vec_uint4 iv0, ctr, mask;
    int nr, n, groups, blocks, rest, i;
    unsigned char *d, *k, *b;
    const unsigned char *s;

    mask = CTR_WORD;
    if (len < 0)
        return -2;
    nr = aes_set_encrypt_key(rk, key, bits);
    if (nr == -1)
        return -1;
    iv0 = (vec_uint4)*iv;
    n = len >> 4;
    groups = n / 8;
    blocks = n % 8;
    rest = len % 16;
    for (i = 0; i < 8; i++)
        cb[i] = (vec_uchar16)iv0;
    ctr = iv0;
    if (ctr_bits > 0 && ctr_bits < 32)
        mask = spu_xor(spu_sl(mask, spu_splats((unsigned int)ctr_bits)), CTR_WORD);
    for (i = 0; i < groups; i++) {
        cb[0] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[1] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[2] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[3] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[4] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[5] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[6] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        cb[7] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        aes_encrypt8(ks, cb, rk, nr);
        out[0] = spu_xor(in[0], ks[0]);
        out[1] = spu_xor(in[1], ks[1]);
        out[2] = spu_xor(in[2], ks[2]);
        out[3] = spu_xor(in[3], ks[3]);
        out[4] = spu_xor(in[4], ks[4]);
        out[5] = spu_xor(in[5], ks[5]);
        out[6] = spu_xor(in[6], ks[6]);
        out[7] = spu_xor(in[7], ks[7]);
        in += 8;
        out += 8;
    }
    for (i = 0; i < blocks; i++) {
        cb[0] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        ctr = spu_add(ctr, CTR_ONE);
        aes_encrypt_block(ks, cb, rk, nr);
        *out = spu_xor(*in, ks[0]);
        in++;
        out++;
    }
    if (rest > 0) {
        cb[0] = (vec_uchar16)spu_sel(iv0, ctr, mask);
        aes_encrypt_block(ks, cb, rk, nr);
        k = (unsigned char *)ks;
        for (i = 0; i < rest; i++) {
            d = (unsigned char *)out + i;
            s = (const unsigned char *)in + i;
            b = k + i;
            *d = *s ^ *b;
        }
        ctr = spu_add(ctr, CTR_ONE);
    }
    *iv = (vec_uchar16)spu_sel(iv0, ctr, mask);
    return 0;
}
