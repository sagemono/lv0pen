#ifndef ISO_CIPHER_H
#define ISO_CIPHER_H

#include "aes.h"

struct cipher_iv {
    unsigned long long a;
    unsigned long long b;
} __attribute__((aligned(16)));

class cipher {
public:
    cipher(unsigned int type, unsigned int bits) : m_type(type), m_bits(bits) { }
    virtual ~cipher() { }
    virtual void set_key(const unsigned char *key, const unsigned char *iv) = 0;
    virtual int decrypt(const void *in, int len, void *out) = 0;
    virtual int vslot6(const void *state) { return 1; }
    virtual int vslot7(void *state) { return 1; }

protected:
    unsigned int m_type;
    unsigned int m_bits;
};

class aes256_cbc_cipher : public cipher {
public:
    aes256_cbc_cipher() : cipher(4, 256), m_key(0), m_iv() { }
    virtual void set_key(const unsigned char *key, const unsigned char *iv);
    virtual int decrypt(const void *in, int len, void *out);

private:
    const unsigned char *m_key;
    cipher_iv m_iv;
};

class aes128_ctr_cipher : public cipher {
public:
    aes128_ctr_cipher() : cipher(3, 128), m_key(0), m_iv() { }
    virtual void set_key(const unsigned char *key, const unsigned char *iv);
    virtual int decrypt(const void *in, int len, void *out);

private:
    const unsigned char *m_key;
    cipher_iv m_iv;
};

class encryptor {
public:
    encryptor(unsigned int type, unsigned int bits) : m_type(type), m_bits(bits) { }
    virtual ~encryptor() { }
    virtual void set_key(const unsigned char *key, const cipher_iv *iv) = 0;
    virtual int encrypt(const void *in, int len, void *out) = 0;

protected:
    unsigned int m_type;
    unsigned int m_bits;
};

class aes256_cbc_encryptor : public encryptor {
public:
    aes256_cbc_encryptor() : encryptor(4, 256), m_key(0), m_iv() { }
    virtual void set_key(const unsigned char *key, const cipher_iv *iv);
    virtual int encrypt(const void *in, int len, void *out);

private:
    const unsigned char *m_key;
    cipher_iv m_iv;
};

extern aes256_cbc_cipher g_aes256_cbc;
extern aes128_ctr_cipher g_aes128_ctr;
extern aes256_cbc_encryptor g_aes256_cbc_encryptor;

#endif
