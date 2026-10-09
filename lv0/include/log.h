#ifndef LV0_LOG_H
#define LV0_LOG_H

#include "lv0.h"

typedef long log_handler_fn(const char *msg);

typedef long (*region_fn)(long, long);

#ifdef __cplusplus
class log_ring {
public:
    log_ring() : base(0), size(0), flush(0), cursor(0) {}
    int initialize(unsigned long base, unsigned long size, region_fn flush);
    int write_string(const char *str);
    int dump(region_fn flush);

    unsigned long base;
    unsigned long size;
    region_fn flush;
    unsigned long cursor;
};
#endif

struct log_ring *get_log_ring(void);
int write_log_to_log_ring(const char *msg);

long handle_fatal_error(void);
int log_dispatch(int cat, int level, const char *fmt, void *va);
int log_printf(int cat, int level, const char *fmt, ...);
int log_message(const char *fmt, ...);
void log_error(unsigned int code, const char *fmt, ...) __attribute__((noreturn));
int log_set_handler(int cat, log_handler_fn *handler);
int log_set_threshold(int threshold);
int log_set_final_handler(long (*handler)(unsigned int code));
int log_to_internal_console(const char *s);
unsigned int log_get_threshold(void);
void uart_printf(const char *fmt, ...);
void syscon_printf(const char *fmt, ...);

#endif
