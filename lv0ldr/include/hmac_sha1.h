#ifndef LDR_HMAC_SHA1_H
#define LDR_HMAC_SHA1_H

#include "sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sha1_ctx sha;
    vec_uchar16 key[4];
} hmac_sha1_ctx;

int sha1_digest(unsigned char *md, const unsigned char *data, int len);
int hmac_sha1_init(hmac_sha1_ctx *ctx, const unsigned char *key, unsigned int keylen);
int hmac_sha1_update(hmac_sha1_ctx *ctx, const unsigned char *data, int len);
int hmac_sha1_final(unsigned char *md, hmac_sha1_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif
