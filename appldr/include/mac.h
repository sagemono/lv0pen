#ifndef APPLDR_MAC_H
#define APPLDR_MAC_H

#include "hmac_sha1.h"

class mac {
public:
    mac(unsigned int type, unsigned int key_size, unsigned int mac_size)
        : m_type(type), m_key_size(key_size), m_mac_size(mac_size) { }
    virtual ~mac() { }
    virtual void init(const unsigned char *key, unsigned int flags) = 0;
    virtual int update(const void *data, unsigned int len) = 0;
    virtual int verify(const unsigned char *expected) = 0;
    virtual int restore(const void *state) { return 1; }
    virtual int save(void *state) { return 1; }
    virtual int set_mac_size(unsigned int n) { return 1; }

protected:
    unsigned int m_type;
    unsigned int m_key_size;
    unsigned int m_mac_size;
};

class mac_type1 : public mac {
public:
    mac_type1();
    virtual void init(const unsigned char *key, unsigned int flags);
    virtual int update(const void *data, unsigned int len);
    virtual int verify(const unsigned char *expected);

private:
    sha1_ctx m_sha;
    unsigned int m_a;
    int m_b;
};

class mac_hmac_sha1 : public mac {
public:
    mac_hmac_sha1();
    virtual void init(const unsigned char *key, unsigned int flags);
    virtual int update(const void *data, unsigned int len);
    virtual int verify(const unsigned char *expected);
    virtual int restore(const void *state);
    virtual int save(void *state);
    virtual int set_mac_size(unsigned int n)
    {
        if (n != 16 && n != 20)
            return 1;
        m_mac_size = n;
        return 0;
    }

private:
    hmac_sha1_ctx m_ctx;
};

class mac_aes_cmac : public mac {
public:
    mac_aes_cmac();
    virtual void init(const unsigned char *key, unsigned int flags);
    virtual int update(const void *data, unsigned int len);
    virtual int verify(const unsigned char *expected);
    virtual int restore(const void *state);
    virtual int save(void *state);

private:
    unsigned char m_key[16];
    unsigned char m_last[16];
    unsigned char m_iv[16];
    unsigned int m_last_len;
} __attribute__((aligned(16)));

extern mac_type1 g_mac_type1;
extern mac_hmac_sha1 g_mac_hmac_sha1;
extern mac_aes_cmac g_mac_aes_cmac;

#endif
