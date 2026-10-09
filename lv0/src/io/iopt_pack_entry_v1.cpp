#include "iommu.h"

void iopt_pack_entry_v1(long dev, unsigned int id, const char *desc)
{
    int i = id;
    long slot = dev + ((long)i << 3) + 0x1000000;

    *(volatile unsigned long *)(slot - 0x7000) =
          (*(const unsigned long *)desc << 28) & 0xFFFFFFFF00000000LL
        | (int)(*(const int *)(desc + 8) << 8)
        | (int)(*(const int *)(desc + 12) << 12)
        | (int)(*(const int *)(desc + 16) << 20)
        | (int)(*(const int *)(desc + 20) << 6)
        | (16 * *(const int *)(desc + 24))
        | (int)(*(const int *)(desc + 28) << 24)
        | (int)(*(const int *)(desc + 32) << 22)
        | (int)(*(const int *)(desc + 36) << 7)
        | (8 * *(const int *)(desc + 40))
        | *(const int *)(desc + 44);
}
