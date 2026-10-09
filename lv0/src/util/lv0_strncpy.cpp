#include "memory.h"
char *lv0_strncpy(char *dst, const char *src, long n)
{
    char *d = dst;

    while (n != 0 && *src != 0) {
        *d++ = *src++;
        n--;
    }
    while (n-- != 0)
        *d++ = 0;
    return dst;
}
