#ifndef LV0_STATIC_INIT_H
#define LV0_STATIC_INIT_H

#include "lv0.h"

#define DEFAULT_INIT_PRIORITY 0xFFFF

#define GLOBAL_CTOR __attribute__((constructor))

EXTERN_C long c_entry(void);
void run_global_constructors(void);

#endif
