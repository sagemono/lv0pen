#include "hmac_sha1.h"

int hmac_sha1_init(hmac_sha1_ctx *ctx, const unsigned char *key, unsigned int keylen)
{
    vec_uchar16 pad[4];
    unsigned char *k;
    const unsigned char *s;
    unsigned char *d;
    int i;

    k = (unsigned char *)ctx->key;
    if (keylen > 64)
        return -1;
    for (i = 0; i < 4; i++)
        ((vec_uchar16 *)k)[i] = spu_splats((unsigned char)0);
    for (i = 0; i < (int)keylen; i++) {
        s = key + i;
        d = k + i;
        *d = *s;
    }
    for (i = 0; i < 4; i++)
        pad[i] = spu_xor(ctx->key[i], (unsigned char)0x36);
    sha1_init(&ctx->sha);
    return sha1_update(&ctx->sha, (unsigned char *)pad, 64);
}

int hmac_sha1_update(hmac_sha1_ctx *ctx, const unsigned char *data, int len)
{
    return sha1_update(&ctx->sha, data, len);
}

int hmac_sha1_final(unsigned char *md, hmac_sha1_ctx *ctx)
{
    vec_uchar16 buf[6];
    int i;

    if (sha1_final((unsigned char *)&buf[4], &ctx->sha) != 0)
        return -1;
    for (i = 0; i < 4; i++)
        buf[i] = spu_xor(ctx->key[i], (unsigned char)0x5C);
    return sha1_digest(md, (unsigned char *)buf, 84);
}
