#include "iommu.h"

void iopt_unpack_entry_v1(long dev, unsigned int id, unsigned int *out)
{
    unsigned long entry = *(volatile unsigned long *)(dev + ((long)(int)id << 3) + 0xFF9000);

    *(unsigned long *)out = (entry >> 32) << 4;
    out[2] = (entry >> 8) & 0xF;
    out[3] = (entry >> 12) & 0x3F;
    out[4] = (entry >> 20) & 3;
    out[5] = (entry >> 6) & 1;
    out[6] = (entry >> 4) & 3;
    out[7] = (entry >> 24) & 1;
    out[8] = (entry >> 22) & 3;
    out[9] = (entry >> 7) & 1;
    out[10] = (entry >> 3) & 1;
    out[11] = entry & 7;
}
