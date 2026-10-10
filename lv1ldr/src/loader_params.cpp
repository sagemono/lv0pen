#include "params.h"
#include "aes.h"
#include "ec.h"
#include "hmac_sha1.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

extern const vec_uchar16 params_seed;
extern const unsigned char params_pub[40];
extern const unsigned char params_curve[121];

void param_block::init()
{
}

long param_block::open(const unsigned char *in, u32 size,
                       const unsigned char *key1, const unsigned char *iv1,
                       const unsigned char *key2, const unsigned char *iv2,
                       u32 *revision, unsigned char *id, unsigned char *body)
{
    vec_uchar16 key;
    vec_uchar16 mac;
    vec_uchar16 out[12];
    vec_uchar16 rk[32];
    int nr;

    if (size <= 223)
        goto bad;
    nr = aes_set_encrypt_key(rk, key2, 256);
    aes_encrypt_block(&key, &params_seed, rk, nr);
    aes_cbc_decrypt(out, (const vec_uchar16 *)(in + 32), 192,
                    (const unsigned char *)&key, 128, (const vec_uchar16 *)iv2);

    memcpy(&mac, (const unsigned char *)out + 168, 16);
    aes_cmac(&mac, out, 168, (const unsigned char *)&key, 128);
    if (body != 0)
        memcpy(body, out, 192);
    if (bytes_differ((const unsigned char *)&mac, (const unsigned char *)out + 168, 16))
        goto bad;
    *revision = *(const unsigned short *)((const unsigned char *)out + 4);
    *(vec_uchar16 *)id = out[0];
    return 0;
bad:
    *revision = 0xFFFF;
    return -1;
}

long param_block::verify(const unsigned char *body, u32 size)
{
    unsigned char hash[20];
    unsigned char sig[42];

    if (size <= 233)
        return -1;
    if (sha1_digest(hash, body, 56))
        return -1;

    sig[21] = 0;
    sig[0] = 0;
    memcpy(sig + 1, body + 56, 20);
    memcpy(sig + 22, body + 76, 20);
    if (ecdsa_verify_curve(sig, hash, params_pub, params_curve))
        return -1;
    return 0;
}
