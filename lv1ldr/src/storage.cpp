#include <spu_intrinsics.h>
#include "types.h"

extern "C" int prng_generate(unsigned char *out, unsigned char *xkey, const unsigned char *xseed);

unsigned char g_xkey[20] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};
static const unsigned char xseed[20] = {
    0xAB, 0x19, 0x50, 0x25, 0x86, 0xA3, 0x81, 0xE6, 0x70, 0xD3,
    0x4F, 0x56, 0x0E, 0xAA, 0xF3, 0x1A, 0x20, 0x47, 0x59, 0x03,
};

void prng_seed(void)
{
    for (int i = 0; i < 20; i++)
        g_xkey[i] = spu_readch(74) >> 24;
}

int prng_random(unsigned char *out)
{
    prng_generate(out, g_xkey, xseed);
    return 0;
}
