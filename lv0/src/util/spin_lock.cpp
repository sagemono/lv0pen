#include "intrinsics.h"

static inline unsigned int xchg32(volatile unsigned int *p, unsigned int v)
{
    unsigned int old;
    __asm__ volatile("1: lwarx %0,0,%1\n\tstwcx. %2,0,%1\n\tbne- 1b"
                     : "=&r"(old) : "r"(p), "r"(v) : "cr0", "memory");
    return old;
}

void spin_lock_guard::acquire()
{
    while (xchg32(&word, 1) != 0) {
        __asm__ volatile("or 1,1,1" ::: "memory");
        while (word)
            ;
        __asm__ volatile("or 3,3,3" ::: "memory");
    }
    __asm__ volatile("isync" ::: "memory");
}

spin_lock_guard::spin_lock_guard() : word(0)
{
    acquire();
}

spin_lock_guard::~spin_lock_guard()
{
    __asm__ volatile("lwsync" ::: "memory");
    word = 0;
}
