#include "intrinsics.h"

unsigned int atomic_xchg32(unsigned int *ptr, unsigned int val)
{
    u32 old;
    __asm__ volatile(
        "1: lwarx   %0, 0, %2\n"
        "   stwcx.  %3, 0, %2\n"
        "   bne-    1b\n"
        : "=&r"(old), "+m"(*ptr)
        : "r"(ptr), "r"(val)
        : "cc", "memory");
    return old;
}
