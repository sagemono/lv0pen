#ifndef LV0_PRINTF_H
#define LV0_PRINTF_H

#include "lv0.h"

int default_print_hook(const char *msg);
int snprintf_put_char(char **pbuf, unsigned int *prem, long ctxv, int c);
long lv0_snprintf(char *buf, unsigned long size, const char *format, ...);
long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, void *ap);

#endif
