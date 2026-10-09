#ifndef LV1LDR_PARAMS_H
#define LV1LDR_PARAMS_H

#include "types.h"

class param_block {
public:
    void init();

    long open(const unsigned char *in, u32 size,
              const unsigned char *key1, const unsigned char *iv1,
              const unsigned char *key2, const unsigned char *iv2,
              u32 *revision, unsigned char *id, unsigned char *body);

    long verify(const unsigned char *body, u32 size);
};

#endif
