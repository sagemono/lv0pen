#include "mac.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

mac_hmac_sha1 g_mac_hmac_sha1;

mac_hmac_sha1::mac_hmac_sha1() : mac(2, 16, 16), m_ctx()
{
}

int mac_hmac_sha1::restore(const void *state)
{
    m_ctx = *(const hmac_sha1_ctx *)state;
    return 0;
}

int mac_hmac_sha1::save(void *state)
{
    *(hmac_sha1_ctx *)state = m_ctx;
    return 0;
}

int mac_hmac_sha1::update(const void *data, unsigned int len)
{
    if (len & 15)
        return 1;
    return hmac_sha1_update(&m_ctx, (const unsigned char *)data, len) < 0;
}

int mac_hmac_sha1::verify(const unsigned char *expected)
{
    unsigned char md[20];

    memset(md, 0, sizeof md);
    hmac_sha1_final(md, &m_ctx);
    return bytes_differ(md, expected, m_mac_size) != 0;
}

void mac_hmac_sha1::init(const unsigned char *key, unsigned int flags)
{
    hmac_sha1_init(&m_ctx, key, m_key_size);
}
