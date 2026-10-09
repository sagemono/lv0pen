#include "hmac_sha1.h"

int sha1_digest(unsigned char *md, const unsigned char *data, int len)
{
    sha1_ctx ctx;

    if (len < 0)
        return -2;
    sha1_init(&ctx);
    sha1_update(&ctx, data, len);
    sha1_final(md, &ctx);
    return 0;
}
