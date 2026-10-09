#include "memory.h"
unsigned long lv0_strnlen(const char *s, unsigned long n)
{
    const char *p = s;

    while (*p != 0 && n != 0) {
        p++;
        n--;
    }
    return p - s;
}
