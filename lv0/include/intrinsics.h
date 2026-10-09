#ifndef LV0_INTRINSICS_H
#define LV0_INTRINSICS_H

#include "lv0.h"

#ifdef __cplusplus
class spin_lock_guard {
public:
    spin_lock_guard();
    ~spin_lock_guard();
    void release();
    void acquire();
    volatile unsigned int word;
};
#endif

EXTERN_C void __cxa_pure_virtual(void);

unsigned int atomic_xchg32(unsigned int *ptr, unsigned int val);

#endif
