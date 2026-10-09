#include "aes.h"

static const vec_uchar16 cmac_top = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static inline vec_uchar16 cmac_double(vec_uchar16 k)
{
    vec_uchar16 c = spu_and(spu_cmpeq(spu_and(k, cmac_top), (unsigned char)0x80), (unsigned char)0x87);
    return spu_xor(spu_rlqwbyte(c, 1), spu_slqw(k, 1));
}

int aes_cmac(vec_uchar16 *mac, const vec_uchar16 *in, int len, const unsigned char *key, int bits)
{
    vec_uchar16 rk[15];
    vec_uchar16 k, c, x;
    int nr, i;

    if (len < 0)
        return -2;
    nr = aes_set_encrypt_key(rk, key, bits);
    if (nr == -1)
        return -1;
    c = spu_splats((unsigned char)0);
    aes_encrypt_block(&k, &c, rk, nr);
    k = cmac_double(k);
    while (len > 16) {
        len -= 16;
        x = spu_xor(*in++, c);
        aes_encrypt_block(&c, &x, rk, nr);
    }
    for (i = 0; i < len; i++)
        x = spu_insert(*(const unsigned char *)(i + (unsigned int)in), x, i);
    if (len <= 15) {
        x = spu_insert((unsigned char)0x80, x, i);
        for (i++; i < 16; i++)
            x = spu_insert((unsigned char)0, x, i);
        k = cmac_double(k);
    }
    x = spu_xor(x, spu_xor(k, c));
    aes_encrypt_block(mac, &x, rk, nr);
    return 0;
}
