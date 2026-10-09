#include "syscon.h"

unsigned long copy_qwords_to_mmio(char *dest, char *src, unsigned int count)
{
    unsigned int i = 0;
    while (i < count) {
        *(long *)(dest + i) = *(long *)(src + i);
        i += 8;
    }
    return count;
}
