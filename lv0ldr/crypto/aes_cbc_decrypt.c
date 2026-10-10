#include "aes.h"

int aes_cbc_decrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                    const vec_uchar16 *iv)
{
    vec_uchar16 rk[15], t, prev;
    vec_uchar16 c0, c1, c2, c3, c4, c5, c6, c7;
    unsigned char *tb, *d;
    const unsigned char *s, *b;
    int nr, n, groups, blocks, rest, i;

    if (len < 0)
        return -2;
    nr = aes_set_decrypt_key(rk, key, bits);
    if (nr == -1)
        return -1;
    prev = *iv;
    n = len >> 4;
    groups = n / 8;
    blocks = n % 8;
    rest = len % 16;
    for (i = 0; i < groups; i++) {
        c7 = in[7];
        c0 = in[0];
        c1 = in[1];
        c2 = in[2];
        c3 = in[3];
        c4 = in[4];
        c5 = in[5];
        c6 = in[6];
        aes_decrypt8(out, in, rk, nr);
        in += 8;
        out[0] = spu_xor(out[0], prev);
        out[1] = spu_xor(out[1], c0);
        out[2] = spu_xor(out[2], c1);
        out[3] = spu_xor(out[3], c2);
        out[4] = spu_xor(out[4], c3);
        out[5] = spu_xor(out[5], c4);
        out[6] = spu_xor(out[6], c5);
        out[7] = spu_xor(out[7], c6);
        prev = c7;
        out += 8;
    }
    for (i = 0; i < blocks; i++) {
        c0 = *in;
        aes_decrypt_block(out, in, rk, nr);
        in++;
        *out = spu_xor(*out, prev);
        prev = c0;
        out++;
    }
    if (rest > 0) {
        tb = (unsigned char *)&t;
        aes_set_encrypt_key(rk, key, bits);
        aes_encrypt_block(&t, &prev, rk, nr);
        for (i = 0; i < rest; i++) {
            d = (unsigned char *)out + i;
            s = (const unsigned char *)in + i;
            b = tb + i;
            *d = *s ^ *b;
        }
    }
    return 0;
}
