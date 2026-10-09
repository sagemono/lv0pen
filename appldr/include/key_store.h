#ifndef APPLDR_KEY_STORE_H
#define APPLDR_KEY_STORE_H

#include "types.h"

struct app_keyset {
    unsigned char key[32];
    unsigned char iv[16];
    unsigned char pub[40];
    unsigned int curve;
    unsigned int pad;
};

class key_store {
public:
    virtual ~key_store() { }

    long check_version(u64 version);

    long unwrap_keysets();
    long unwrap_keys();

    long request();
};

#endif
