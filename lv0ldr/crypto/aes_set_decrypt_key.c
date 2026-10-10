#include "aes_sbox.h"

int aes_set_decrypt_key(vec_uchar16 *rk, const unsigned char *key, int bits)
{
    vec_uchar16 tmp[15];
    int nr, i;

    nr = aes_set_encrypt_key(tmp, key, bits);
    if (nr < 0)
        return nr;
    rk[0] = tmp[0];
    rk[1] = spu_shuffle(tmp[1], ZERO, SR0);
    rk[2] = spu_shuffle(tmp[2], ZERO, SR0);
    rk[3] = spu_shuffle(tmp[3], ZERO, SR0);
    rk[4] = spu_shuffle(tmp[4], ZERO, SR0);
    rk[5] = spu_shuffle(tmp[5], ZERO, SR0);
    rk[6] = spu_shuffle(tmp[6], ZERO, SR0);
    rk[7] = spu_shuffle(tmp[7], ZERO, SR0);
    rk[8] = spu_shuffle(tmp[8], ZERO, SR0);
    rk[9] = spu_shuffle(tmp[9], ZERO, SR0);
    rk[nr] = tmp[nr];
    for (i = 10; i < nr; i++)
        rk[i] = spu_shuffle(tmp[i], ZERO, SR0);
    return nr;
}
