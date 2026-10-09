#include "mac.h"
#include "aes.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

struct cmac_state {
    unsigned char key[16];
    unsigned char last[16];
    unsigned char iv[16];
    unsigned int last_len;
};

mac_aes_cmac g_mac_aes_cmac;

mac_aes_cmac::mac_aes_cmac() : mac(4, 128, 16), m_last_len(0)
{
    memset(m_key, 0, sizeof m_key);
    memset(m_last, 0, sizeof m_last);
    memset(m_iv, 0, sizeof m_iv);
}

int mac_aes_cmac::save(void *state)
{
    cmac_state *s = (cmac_state *)state;

    memcpy(s->key, m_key, sizeof s->key);
    memcpy(s->last, m_last, sizeof s->last);
    memcpy(s->iv, m_iv, sizeof s->iv);
    s->last_len = m_last_len;
    return 0;
}

int mac_aes_cmac::restore(const void *state)
{
    const cmac_state *s = (const cmac_state *)state;

    memcpy(m_key, s->key, sizeof m_key);
    memcpy(m_last, s->last, sizeof m_last);
    memcpy(m_iv, s->iv, sizeof m_iv);
    m_last_len = s->last_len;
    return 0;
}

void mac_aes_cmac::init(const unsigned char *key, unsigned int flags)
{
    memcpy(m_key, key, sizeof m_key);
    memset(m_last, 0, sizeof m_last);
    memset(m_iv, 0, sizeof m_iv);
    m_last_len = 0;
}

int mac_aes_cmac::update(const void *data, unsigned int len)
{
    unsigned char out[16];
    unsigned char buf[2048];

    if (len == 0)
        return 0;
    if (len & 15)
        return 1;
    if (m_last_len) {
        if (aes_cbc_encrypt((vec_uchar16 *)out, (const vec_uchar16 *)m_last, 16, m_key, m_key_size,
                            (const vec_uchar16 *)m_iv) < 0)
            return 1;
        memcpy(m_iv, out, sizeof m_iv);
    }
    m_last_len = 16;
    len -= 16;
    memcpy(m_last, (const unsigned char *)data + len, sizeof m_last);
    if (len == 0)
        return 0;
    if (len > sizeof buf)
        return 1;
    unsigned char *iv = m_iv;

    if (aes_cbc_encrypt((vec_uchar16 *)buf, (const vec_uchar16 *)data, len, m_key, m_key_size,
                        (const vec_uchar16 *)iv) < 0)
        return 1;
    memcpy(iv, buf + len - 16, sizeof m_iv);
    return 0;
}

int mac_aes_cmac::verify(const unsigned char *expected)
{
    unsigned char out[16];
    int i;

    memset(out, 0, sizeof out);
    if (m_last_len) {
        unsigned char *last = m_last;
        const unsigned char *iv = m_iv;

        for (i = 0; i < 16; i++)
            last[i] ^= iv[i];
        if (aes_cmac((vec_uchar16 *)out, (const vec_uchar16 *)m_last, m_last_len, m_key, m_key_size) < 0)
            return 1;
    }
    return bytes_differ(out, expected, m_mac_size) != 0;
}
