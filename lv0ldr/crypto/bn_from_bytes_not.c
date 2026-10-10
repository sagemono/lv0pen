#include "bn.h"

void bn_from_bytes_not(vec_uint4 *d, const unsigned char *p)
{
    unsigned char buf[20];
    int i;

    for (i = 0; i < 20; i++) {
        unsigned char *q = buf + i;
        *q = ~*p++;
    }
    bn_from_bytes(d, buf);
}

void bn_from_bytes21_not(vec_uint4 *d, const unsigned char *p)
{
    unsigned char buf[21];
    int i;

    for (i = 0; i < 21; i++) {
        unsigned char *q = buf + i;
        *q = ~*p++;
    }
    bn_from_bytes21(d, buf);
}
