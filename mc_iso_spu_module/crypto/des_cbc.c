#include "des.h"

struct des3_cbc_state {
    unsigned int sk[96];
    unsigned char t[8] __attribute__((aligned(16)));
    unsigned char c[8] __attribute__((aligned(16)));
};

int des3_cbc_encrypt(unsigned char *out, const unsigned char *in, int len,
                     const unsigned char *key, int bits, const unsigned char *iv)
{
    struct des3_cbc_state s;
    unsigned char c0, c1, c2, c3, c4, c5, c6, c7;
    int n, i;

    if (len < 0)
        return -2;
    n = des3_set_encrypt_key(s.sk, key, bits);
    if (n == -1)
        return -1;
    c0 = iv[0];
    c1 = iv[1];
    c2 = iv[2];
    c3 = iv[3];
    c4 = iv[4];
    c5 = iv[5];
    c6 = iv[6];
    c7 = iv[7];
    while (len > 7) {
        s.t[0] = in[0] ^ c0;
        s.t[1] = in[1] ^ c1;
        s.t[2] = in[2] ^ c2;
        s.t[3] = in[3] ^ c3;
        s.t[4] = in[4] ^ c4;
        s.t[5] = in[5] ^ c5;
        s.t[6] = in[6] ^ c6;
        s.t[7] = in[7] ^ c7;
        des3_crypt_block(out, s.t, s.sk, n);
        c0 = out[0];
        c1 = out[1];
        c2 = out[2];
        c3 = out[3];
        c4 = out[4];
        c5 = out[5];
        c6 = out[6];
        c7 = out[7];
        len -= 8;
        in += 8;
        out += 8;
    }
    if (len > 0) {
        s.c[0] = c0;
        s.c[1] = c1;
        s.c[2] = c2;
        s.c[3] = c3;
        s.c[4] = c4;
        s.c[5] = c5;
        s.c[6] = c6;
        s.c[7] = c7;
        des3_crypt_block(s.t, s.c, s.sk, n);
        for (i = 0; i < len; i++) {
            unsigned char *d = out + i;
            const unsigned char *a = in + i;
            *d = *a ^ s.t[i];
        }
    }
    return 0;
}

struct des3_cbc_dec_state {
    unsigned int sk[96];
    unsigned char c[8] __attribute__((aligned(16)));
    unsigned char t[8] __attribute__((aligned(16)));
};

int des3_cbc_decrypt(unsigned char *out, const unsigned char *in, int len,
                     const unsigned char *key, int bits, const unsigned char *iv)
{
    struct des3_cbc_dec_state s;
    unsigned char c0, c1, c2, c3, c4, c5, c6, c7;
    int n, i;

    if (len < 0)
        return -2;
    n = des3_set_decrypt_key(s.sk, key, bits);
    if (n == -1)
        return -1;
    for (i = 0; i < 8; i++) {
        const unsigned char *p = iv + i;
        s.c[i] = *p;
    }
    while (len > 7) {
        c0 = in[0];
        c1 = in[1];
        c2 = in[2];
        c3 = in[3];
        c4 = in[4];
        c5 = in[5];
        c6 = in[6];
        c7 = in[7];
        des3_crypt_block(out, in, s.sk, n);
        out[0] ^= s.c[0];
        out[1] ^= s.c[1];
        out[2] ^= s.c[2];
        out[3] ^= s.c[3];
        out[4] ^= s.c[4];
        out[5] ^= s.c[5];
        out[6] ^= s.c[6];
        out[7] ^= s.c[7];
        s.c[0] = c0;
        s.c[1] = c1;
        s.c[2] = c2;
        s.c[3] = c3;
        s.c[4] = c4;
        s.c[5] = c5;
        s.c[6] = c6;
        s.c[7] = c7;
        len -= 8;
        in += 8;
        out += 8;
    }
    if (len > 0) {
        des3_set_encrypt_key(s.sk, key, bits);
        des3_crypt_block(s.t, s.c, s.sk, n);
        for (i = 0; i < len; i++) {
            unsigned char *d = out + i;
            const unsigned char *a = in + i;
            *d = *a ^ s.t[i];
        }
    }
    return 0;
}
