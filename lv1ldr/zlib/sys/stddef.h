/* Altered from zlib 1.2.3: comments removed, changes for the loader. */
#ifndef LV1LDR_STDDEF_H
#define LV1LDR_STDDEF_H
typedef unsigned int size_t;
typedef int ptrdiff_t;
#define NULL ((void *)0)
#define offsetof(t, m) __builtin_offsetof(t, m)
#endif
