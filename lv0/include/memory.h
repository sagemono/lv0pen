#ifndef LV0_MEMORY_H
#define LV0_MEMORY_H

#include "lv0.h"

long lv0_memcmp(const unsigned char *s1, const unsigned char *s2, long n);
void *lv0_memmove(char *dst, const char *src, unsigned long n);

void *lv0_memset(void *dst, int c, unsigned long n);
int lv0_strncmp(const char *s1, const char *s2, unsigned long n);
char *lv0_strncpy(char *dst, const char *src, long n);
unsigned long lv0_strnlen(const char *s, unsigned long n);
unsigned long lv0_strlen(const char *s) __attribute__((pure));

#ifdef __cplusplus
class memory_budget {
public:
    memory_budget();
    virtual ~memory_budget();
    long initialize(unsigned long addr, unsigned long size);
    void *allocate(unsigned long size, unsigned long align);
    int get_used_size(long *out);
    int get_free_size(long *out);

    unsigned long addr;
    unsigned long size;
    unsigned long cursor;
    unsigned char initialized;
};

extern memory_budget *g_memory_budget_low_ptr;
extern memory_budget *g_memory_budget_high_ptr;
#endif

#endif
