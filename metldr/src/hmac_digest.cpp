#include "verifier.h"

int hmac_digest::init(const unsigned char *key, unsigned int keylen)
{
    return hmac_sha1_init(&m_ctx, key, keylen);
}

int hmac_digest::final(unsigned char *md)
{
    return hmac_sha1_final(md, &m_ctx);
}

int hmac_digest::update(const unsigned char *data, int len)
{
    hmac_sha1_update(&m_ctx, data, len);
    return 0;
}
