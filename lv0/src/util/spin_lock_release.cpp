#include "intrinsics.h"

void spin_lock_guard::release()
{
    __asm__ volatile("lwsync" ::: "memory");
    word = 0;
}
