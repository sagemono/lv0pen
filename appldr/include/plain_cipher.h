#ifndef APPLDR_PLAIN_CIPHER_H
#define APPLDR_PLAIN_CIPHER_H

#include "cipher.h"

class plain_cipher : public cipher {
public:
    plain_cipher() : cipher(1, 0) { }
    virtual void set_key(const unsigned char *key, const unsigned char *iv);
    virtual int decrypt(const void *in, int len, void *out);
    virtual int vslot6(const void *state);
    virtual int vslot7(void *state);
};

extern plain_cipher g_plain_cipher;

#endif
