#include "static_init.h"

typedef void (*fn_t)(void);
extern "C" fn_t __ctors_start[];
extern "C" fn_t __ctors_end[];

void run_global_constructors()
{
    fn_t *ctor;
    for (ctor = __ctors_start; ctor != __ctors_end; ctor++)
        (*ctor)();
}
