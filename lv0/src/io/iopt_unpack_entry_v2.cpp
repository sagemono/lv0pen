#include "iommu.h"

struct chan_status { unsigned int w[12]; };

long iopt_unpack_entry_v2(long dev, unsigned int id, volatile struct chan_status *out)
{
    int i = id;
    unsigned int w0, w2;

    if ((unsigned)i > 3)
        return dev;
    long base = dev + (long)(i << 5);
    volatile unsigned int *reg = (volatile unsigned int *)(base + 0x1000000) - 1016;

    out->w[11] = 0;
    out->w[4] = 0;
    out->w[6] = 0;
    out->w[8] = 0;
    w0 = reg[0];
    w2 = reg[2];
    *(volatile unsigned long *)out->w = reg[3];
    out->w[2] = 12 - (w0 & 0x1F);
    out->w[7] = (w2 >> 24) & 1;
    out->w[3] = (w2 >> 8) & 0x3F;
    out->w[9] = (w2 >> 2) & 1;
    out->w[5] = (w2 >> 1) & 1;
    out->w[10] = w2 & 1;
    return dev;
}
