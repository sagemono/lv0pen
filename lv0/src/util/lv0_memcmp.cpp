#include "memory.h"
long lv0_memcmp(const unsigned char *s1, const unsigned char *s2, long n) {
    long i = 0;
    while (i != n) {
        unsigned char c1 = s1[i], c2 = s2[i];
        i++;
        if (c1 != c2) return c1 < c2 ? -1 : 1;
    }
    return 0;
}
