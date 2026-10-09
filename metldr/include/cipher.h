#ifndef MLDR_CIPHER_H
#define MLDR_CIPHER_H

#include "aes.h"

struct cipher_iv {
    unsigned long long a;
    unsigned long long b;
} __attribute__((aligned(16)));

class cipher {
public:
    cipher(unsigned int type, unsigned int bits) : m_type(type), m_bits(bits) { }
    virtual ~cipher() { }
    virtual void set_key(const unsigned char *key, const cipher_iv *iv) = 0;
    virtual int decrypt(const void *in, int len, void *out) = 0;

protected:
    unsigned int m_type;
    unsigned int m_bits;
};

class aes256_cbc_cipher : public cipher {
public:
    aes256_cbc_cipher();
    virtual void set_key(const unsigned char *key, const cipher_iv *iv);
    virtual int decrypt(const void *in, int len, void *out);

private:
    const unsigned char *m_key;
    cipher_iv m_iv;
};

class aes128_ctr_cipher : public cipher {
public:
    aes128_ctr_cipher();
    virtual void set_key(const unsigned char *key, const cipher_iv *iv);
    virtual int decrypt(const void *in, int len, void *out);

private:
    const unsigned char *m_key;
    cipher_iv m_iv;
};

extern aes256_cbc_cipher g_aes256_cbc;
extern aes128_ctr_cipher g_aes128_ctr;

#endif
