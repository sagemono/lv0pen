#ifndef ISO_VERIFIER_H
#define ISO_VERIFIER_H
#define LDR_VERIFIER_H

#include "hmac_sha1.h"

class verifier {
public:
    verifier(unsigned int type) : m_type(type) { }
    virtual ~verifier() { }
    virtual int verify(const unsigned char *data, int len, const unsigned char *sig) = 0;

protected:
    unsigned int m_type;
};

class ecdsa_verifier : public verifier {
public:
    ecdsa_verifier(const unsigned char *pub, unsigned int curve);
    virtual int verify(const unsigned char *data, int len, const unsigned char *sig);

private:
    const unsigned char *m_pub;
    unsigned int m_curve;
};

class hmac_digest {
public:
    hmac_digest();
    ~hmac_digest() { }
    void init(const unsigned char *key, unsigned int keylen);
    int update(const unsigned char *data, int len);
    int final(unsigned char *md);
    int digest(const unsigned char *data, int len, unsigned char *md, const unsigned char *key,
               unsigned int keylen);

private:
    unsigned int m_type;
    hmac_sha1_ctx m_ctx;
};

extern hmac_digest g_hmac;

#endif
