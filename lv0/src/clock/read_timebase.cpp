#include "clock.h"
unsigned long read_timebase(void)
{
    unsigned long tb;
    do {
        __asm__ volatile ("mftb %0" : "=r"(tb));
    } while (__builtin_expect((unsigned int)tb == 0, 0));
    return tb;
}
