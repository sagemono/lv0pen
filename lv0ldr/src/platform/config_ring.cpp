#include "loader.h"
#include "log.h"

#include <spu_intrinsics.h>

extern const unsigned char config_ring_expected[338];
extern const unsigned short config_ring_vary[56];

bool config_ring::cmp_bits(const unsigned char *a, const unsigned char *b, unsigned short start, unsigned short count)
{
    unsigned int i = start >> 3;
    unsigned short bit = start & 7;

    while (count) {
        int m = 1 << (7 - bit);
        if ((a[i] & m) != (b[i] & m)) {
            log_message("[ERROR]: config_ring cmp fail current %d, start %d, num %d, index %d, s1 %02x, s2 %02x\n", bit, start, count, i, a[i], b[i]);
            return 0;
        }
        bit++;
        if (bit > 7) {
            bit = 0;
            i++;
        }
        count--;
    }
    return 1;
}

long config_ring::verify(void)
{
    unsigned char ring[338];
    int i;

    for (i = 23; i != 0; i--)
        spu_readch(67);
    for (i = 337; i != -1; i--)
        ring[i] = spu_readch(67);
    const unsigned short *p = &config_ring_vary[1];
    for (;;) {
        if (p[0] == 0)
            return 0;
        unsigned short end = p[-1] + p[0];
        unsigned short n = p[1] - end;
        p += 2;
        if (!cmp_bits(ring, config_ring_expected, end + 3, n))
            return -1;
    }
    return 0;
}
