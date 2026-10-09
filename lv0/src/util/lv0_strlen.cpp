#include "lv0.h"
#include "memory.h"

unsigned long lv0_strlen(const char *s)
{
    const char *p = s;

    while (*p)
        p++;
    return p - s;
}
