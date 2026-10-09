#include "memory.h"

typedef unsigned long u64_t;
typedef unsigned int u32_t;
typedef unsigned short u16_t;

#define ST4(p, o, v)    (*(u32_t *)((p) + (o)) = (u32_t)(v))
#define ST2(p, o, v)    (*(u16_t *)((p) + (o)) = (u16_t)(v))
#define ST1(p, o, v)    (*(unsigned char *)((p) + (o)) = (unsigned char)(v))

static inline void dcbt(const void *p)
{
    __asm__ volatile ("dcbt 0,%0" :: "r"(p));
}

void *lv0_memmove(char *dst, const char *src, unsigned long n)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    unsigned long sa, da;

    if (((unsigned long)src & 0xFFF) < ((unsigned long)dst & 0xFFF)) {
        s += n;
        d += n;

        if (n <= 7) {
            while (n--)
                *--d = *--s;
            return dst;
        }
        sa = (unsigned long)s & 7;
        da = (unsigned long)d & 7;
        if ((int)sa & 1) {
            *--d = *--s;
            sa -= 1;
            da -= 1;
            n -= 1;
        }
        if (sa & 2) {
            *--d = *--s;
            *--d = *--s;
            sa -= 2;
            da -= 2;
            n -= 2;
        }
        if (sa & 4) {
            *--d = *--s;
            *--d = *--s;
            *--d = *--s;
            *--d = *--s;
            da -= 4;
            n -= 4;
        }
        if (n != 0) {
            da &= 7;
            if (da == 0) {
                const u64_t *ps = (const u64_t *)s;
                u64_t *pd = (u64_t *)d;

                while (n >= 48) {
                    ps -= 6;
                    pd -= 6;
                    n -= 48;
                    u64_t a = ps[5], b = ps[4], c = ps[3], e = ps[2], f = ps[1], g = ps[0];
                    pd[5] = a;
                    pd[4] = b;
                    pd[3] = c;
                    pd[2] = e;
                    pd[1] = f;
                    pd[0] = g;
                    if (n < 48)
                        break;
                    ps -= 6;
                    pd -= 6;
                    n -= 48;
                    a = ps[5];
                    b = ps[4];
                    c = ps[3];
                    e = ps[2];
                    f = ps[1];
                    g = ps[0];
                    pd[5] = a;
                    pd[4] = b;
                    pd[3] = c;
                    pd[2] = e;
                    pd[1] = f;
                    pd[0] = g;
                    dcbt((const char *)ps - 256);
                }
                while (n >= 8) {
                    *--pd = *--ps;
                    n -= 8;
                }
                d = (unsigned char *)pd;
                s = (const unsigned char *)ps;
                while (n--)
                    *--d = *--s;
            } else if (da == 1 || da == 5) {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 16) {
                    u64_t a, b;
                    u32_t w;
                    ps -= 2;
                    pd -= 16;
                    n -= 16;
                    b = ps[1];
                    a = ps[0];
                    ST1(pd, 15, b);
                    ST4(pd, 11, b >> 8);
                    w = b >> 40;
                    w |= a << 24;
                    ST4(pd, 7, w);
                    ST4(pd, 3, a >> 8);
                    ST2(pd, 1, a >> 40);
                    ST1(pd, 0, a >> 56);
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *--pd = *--cs;
                }
            } else if (da == 2 || da == 6) {
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a, b, c, e;
                    u32_t w;
                    ps -= 4;
                    d -= 32;
                    n -= 32;
                    e = ps[3];
                    c = ps[2];
                    b = ps[1];
                    a = ps[0];
                    ST2(d, 30, e);
                    ST4(d, 26, e >> 16);
                    w = e >> 48;
                    w |= c << 16;
                    ST4(d, 22, w);
                    ST4(d, 18, c >> 16);
                    w = c >> 48;
                    w |= b << 16;
                    ST4(d, 14, w);
                    ST4(d, 10, b >> 16);
                    w = b >> 48;
                    w |= a << 16;
                    ST4(d, 6, w);
                    ST4(d, 2, a >> 16);
                    ST2(d, 0, a >> 48);
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *--d = *--cs;
                }
            } else if (da == 3 || da == 7) {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 16) {
                    u64_t a, b;
                    u32_t w;
                    ps -= 2;
                    pd -= 16;
                    n -= 16;
                    b = ps[1];
                    a = ps[0];
                    ST1(pd, 15, b);
                    ST2(pd, 13, b >> 8);
                    ST4(pd, 9, b >> 24);
                    w = b >> 56;
                    w |= a << 8;
                    ST4(pd, 5, w);
                    ST4(pd, 1, a >> 24);
                    ST1(pd, 0, a >> 56);
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *--pd = *--cs;
                }
            } else {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a, b, c, e;
                    ps -= 4;
                    pd -= 32;
                    n -= 32;
                    e = ps[3];
                    c = ps[2];
                    b = ps[1];
                    a = ps[0];
                    ST4(pd, 28, e);
                    ST4(pd, 24, e >> 32);
                    ST4(pd, 20, c);
                    ST4(pd, 16, c >> 32);
                    ST4(pd, 12, b);
                    ST4(pd, 8, b >> 32);
                    ST4(pd, 4, a);
                    ST4(pd, 0, a >> 32);
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *--pd = *--cs;
                }
            }
        }
    } else {
        if (n <= 7) {
            while (n--)
                *d++ = *s++;
            return dst;
        }
        sa = (unsigned long)s & 7;
        da = (unsigned long)d & 7;
        if ((int)sa & 1) {
            *d++ = *s++;
            sa += 1;
            da += 1;
            n -= 1;
        }
        if (sa & 2) {
            *d++ = *s++;
            *d++ = *s++;
            sa += 2;
            da += 2;
            n -= 2;
        }
        if (sa & 4) {
            *d++ = *s++;
            *d++ = *s++;
            *d++ = *s++;
            *d++ = *s++;
            da += 4;
            n -= 4;
        }
        if (n != 0) {
            da &= 7;
            if (da == 0) {
                u64_t *pd = (u64_t *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 48) {
                    u64_t a = ps[0], b = ps[1], c = ps[2], e = ps[3], f = ps[4], g = ps[5];
                    pd[0] = a;
                    pd[1] = b;
                    pd[2] = c;
                    pd[3] = e;
                    pd[4] = f;
                    pd[5] = g;
                    ps += 6;
                    pd += 6;
                    n -= 48;
                    if (n < 48)
                        break;
                    a = ps[0];
                    b = ps[1];
                    c = ps[2];
                    e = ps[3];
                    f = ps[4];
                    g = ps[5];
                    pd[0] = a;
                    pd[1] = b;
                    pd[2] = c;
                    pd[3] = e;
                    pd[4] = f;
                    pd[5] = g;
                    ps += 6;
                    pd += 6;
                    n -= 48;
                    dcbt((const char *)ps + 256);
                }
                while (n >= 8) {
                    *pd++ = *ps++;
                    n -= 8;
                }
                {
                    unsigned char *cd = (unsigned char *)pd;
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *cd++ = *cs++;
                }
            } else if (da == 1 || da == 5) {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a = ps[0], b = ps[1], c = ps[2], e = ps[3];
                    n -= 32;
                    ST1(pd, 0, a >> 56);
                    ST2(pd, 1, a >> 40);
                    ST4(pd, 3, a >> 8);
                    ST4(pd, 7, (b >> 40) | (a << 24));
                    ST4(pd, 11, b >> 8);
                    ST4(pd, 15, (c >> 40) | (b << 24));
                    ST4(pd, 19, c >> 8);
                    ST4(pd, 23, (e >> 40) | (c << 24));
                    ST4(pd, 27, e >> 8);
                    ST1(pd, 31, e);
                    ps += 4;
                    pd += 32;
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *pd++ = *cs++;
                }
            } else if (da == 2 || da == 6) {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a = ps[0], b = ps[1], c = ps[2], e = ps[3];
                    n -= 32;
                    ST2(pd, 0, a >> 48);
                    ST4(pd, 2, a >> 16);
                    ST4(pd, 6, (b >> 48) | (a << 16));
                    ST4(pd, 10, b >> 16);
                    ST4(pd, 14, (c >> 48) | (b << 16));
                    ST4(pd, 18, c >> 16);
                    ST4(pd, 22, (e >> 48) | (c << 16));
                    ST4(pd, 26, e >> 16);
                    ST2(pd, 30, e);
                    ps += 4;
                    pd += 32;
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *pd++ = *cs++;
                }
            } else if (da == 3 || da == 7) {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a = ps[0], b = ps[1], c = ps[2], e = ps[3];
                    n -= 32;
                    ST1(pd, 0, a >> 56);
                    ST4(pd, 1, a >> 24);
                    ST4(pd, 5, (b >> 56) | (a << 8));
                    ST4(pd, 9, b >> 24);
                    ST4(pd, 13, (c >> 56) | (b << 8));
                    ST4(pd, 17, c >> 24);
                    ST4(pd, 21, (e >> 56) | (c << 8));
                    ST4(pd, 25, e >> 24);
                    ST2(pd, 29, e >> 8);
                    ST1(pd, 31, e);
                    ps += 4;
                    pd += 32;
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *pd++ = *cs++;
                }
            } else {
                char *pd = (char *)d;
                const u64_t *ps = (const u64_t *)s;

                while (n >= 32) {
                    u64_t a = ps[0], b = ps[1], c = ps[2], e = ps[3];
                    n -= 32;
                    ST4(pd, 0, a >> 32);
                    ST4(pd, 4, a);
                    ST4(pd, 8, b >> 32);
                    ST4(pd, 12, b);
                    ST4(pd, 16, c >> 32);
                    ST4(pd, 20, c);
                    ST4(pd, 24, e >> 32);
                    ST4(pd, 28, e);
                    ps += 4;
                    pd += 32;
                }
                {
                    const unsigned char *cs = (const unsigned char *)ps;

                    while (n--)
                        *pd++ = *cs++;
                }
            }
        }
    }
    return dst;
}
