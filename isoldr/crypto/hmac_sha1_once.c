#include "hmac_sha1.h"

int hmac_sha1(unsigned char *md, const unsigned char *data, int len, const unsigned char *key,
              unsigned int keylen)
{
    hmac_sha1_ctx ctx;

    if (len < 0)
        return -2;
    if (hmac_sha1_init(&ctx, key, keylen))
        return -1;
    hmac_sha1_update(&ctx, data, len);
    hmac_sha1_final(md, &ctx);
    return 0;
}
