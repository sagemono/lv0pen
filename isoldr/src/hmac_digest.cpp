#include "verifier.h"

hmac_digest g_hmac;

hmac_digest::hmac_digest()
    : m_type(2), m_ctx()
{
}

void hmac_digest::init(const unsigned char *key, unsigned int keylen)
{
    hmac_sha1_init(&m_ctx, key, keylen);
}

int hmac_digest::final(unsigned char *md)
{
    return hmac_sha1_final(md, &m_ctx);
}

int hmac_digest::update(const unsigned char *data, int len)
{
    return hmac_sha1_update(&m_ctx, data, len) < 0;
}

int hmac_digest::digest(const unsigned char *data, int len, unsigned char *md, const unsigned char *key,
                        unsigned int keylen)
{
    return hmac_sha1(md, data, len, key, keylen) < 0;
}
