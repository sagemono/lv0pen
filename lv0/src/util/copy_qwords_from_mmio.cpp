#include "syscon.h"

unsigned long copy_qwords_from_mmio(char *src, char *dst, unsigned int n)
{
    unsigned int i = 0;
    while (i < n) {
        *(long *)(dst + i) = *(long *)(src + i);
        i += 8;
    }
    return n;
}
