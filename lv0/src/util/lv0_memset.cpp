#include "memory.h"
void *lv0_memset(void *dst, int c, unsigned long n)
{
    unsigned char *p = (unsigned char *)dst;
    unsigned char *end = p + n;
    unsigned char v = c;

    if (n > 7) {
        unsigned char b = c;
        unsigned short h = b | (b << 8);
        unsigned int w = (h << 16) | h;
        unsigned long d = ((unsigned long)w << 32) | w;
        unsigned char *a2 = (unsigned char *)(((unsigned long)p + 1) & ~1UL);
        unsigned char *a4 = (unsigned char *)(((unsigned long)a2 + 2) & ~3UL);
        unsigned long *a8 = (unsigned long *)(((unsigned long)a4 + 4) & ~7UL);
        unsigned long *e8 = (unsigned long *)((unsigned long)end & ~7UL);

        *p = v;
        *(unsigned short *)a2 = h;
        *(unsigned int *)a4 = w;
        while (a8 <= e8 - 8) {
            a8[7] = d;
            a8[6] = d;
            a8[5] = d;
            a8[4] = d;
            a8[3] = d;
            a8[2] = d;
            a8[1] = d;
            a8[0] = d;
            a8 += 8;
        }
        while (a8 < e8)
            *a8++ = d;
        p = (unsigned char *)a8;
    }
    while (p < end)
        *p++ = c;

    return dst;
}
