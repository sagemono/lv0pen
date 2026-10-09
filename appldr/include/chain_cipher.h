#ifndef APPLDR_CHAIN_CIPHER_H
#define APPLDR_CHAIN_CIPHER_H

#include "cipher.h"
#include "util.h"

class aes128_cbc_chain_cipher : public cipher {
public:
    aes128_cbc_chain_cipher() : cipher(2, 128), m_chained(0)
    {
        memset(m_key, 0, sizeof m_key);
        memset(m_iv, 0, sizeof m_iv);
        memset(m_next, 0, sizeof m_next);
    }
    virtual void set_key(const unsigned char *key, const unsigned char *iv);
    virtual int decrypt(const void *in, int len, void *out);
    virtual int vslot6(const void *state);
    virtual int vslot7(void *state);

private:
    unsigned char m_key[16] __attribute__((aligned(16)));
    unsigned char m_iv[16];
    unsigned char m_next[16];
    unsigned int m_chained;
} __attribute__((aligned(16)));

extern aes128_cbc_chain_cipher g_aes128_cbc_chain;

#endif
