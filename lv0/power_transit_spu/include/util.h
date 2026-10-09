#ifndef PT_UTIL_H
#define PT_UTIL_H

#include "types.h"

extern "C" void *memcpy(void *dst, const void *src, unsigned long n);
extern "C" void *memset(void *dst, int c, unsigned long n);
extern "C" unsigned long strlen(const char *s);

void uart_printf(const char *fmt, ...);
int log_message(const char *fmt, ...);

#define FUNCTION_NAME(n) static const char function_name[] = n

#define CXX_DROPPED __attribute__((used, section(".dropped")))

#endif
