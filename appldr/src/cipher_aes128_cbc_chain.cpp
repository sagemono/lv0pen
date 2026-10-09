#include "chain_cipher.h"
#include "aes.h"

struct chain_state {
    unsigned char key[16];
    unsigned char iv[16];
    unsigned char next[16];
    unsigned int chained;
};

aes128_cbc_chain_cipher g_aes128_cbc_chain;

int aes128_cbc_chain_cipher::vslot7(void *state)
{
    chain_state *s = (chain_state *)state;

    memcpy(s->key, m_key, sizeof s->key);
    memcpy(s->iv, m_iv, sizeof s->iv);
    memcpy(s->next, m_next, sizeof s->next);
    s->chained = m_chained;
    return 0;
}

int aes128_cbc_chain_cipher::vslot6(const void *state)
{
    const chain_state *s = (const chain_state *)state;

    memcpy(m_key, s->key, sizeof m_key);
    memcpy(m_iv, s->iv, sizeof m_iv);
    memcpy(m_next, s->next, sizeof m_next);
    m_chained = s->chained;
    return 0;
}

int aes128_cbc_chain_cipher::decrypt(const void *in, int len, void *out)
{
    unsigned char iv[16];
    const unsigned char *p = m_iv;

    if (len & 15)
        return 1;
    if (m_chained) {
        memcpy(iv, m_next, sizeof iv);
        p = iv;
    }
    memcpy(m_next, (const unsigned char *)in + len - 16, sizeof m_next);
    m_chained = 16;
    return aes_cbc_decrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
                           (const vec_uchar16 *)p) < 0;
}

void aes128_cbc_chain_cipher::set_key(const unsigned char *key, const unsigned char *iv)
{
    memcpy(m_key, key, sizeof m_key);
    memcpy(m_iv, iv, sizeof m_iv);
    memset(m_next, 0, sizeof m_next);
    m_chained = 0;
}
