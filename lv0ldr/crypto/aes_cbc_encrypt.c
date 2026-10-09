#include "aes.h"

struct aes_cbc_state {
    vec_uchar16 rk[15];
    vec_uchar16 tmp;
    vec_uchar16 c;
};

int aes_cbc_encrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                    const vec_uchar16 *iv)
{
    struct aes_cbc_state s;
    vec_uchar16 c;
    int nr, i;

    if (len < 0)
        return -2;
    nr = aes_set_encrypt_key(s.rk, key, bits);
    if (nr == -1)
        return -1;
    c = *iv;
    s.c = c;
    while (len > 15) {
        vec_uchar16 t;

        s.tmp = spu_xor(*in, c);
        aes_encrypt_block(out, &s.tmp, s.rk, nr);
        len -= 16;
        in++;
        t = *out;
        s.c = t;
        c = t;
        out++;
    }
    if (len > 0) {
        unsigned char *p = (unsigned char *)&s.tmp;

        aes_encrypt_block(&s.tmp, &s.c, s.rk, nr);
        for (i = 0; i < len; i++) {
            unsigned char *d = (unsigned char *)out + i;
            const unsigned char *a = (const unsigned char *)in + i;
            const unsigned char *b = p + i;
            *d = *a ^ *b;
        }
    }
    return 0;
}
