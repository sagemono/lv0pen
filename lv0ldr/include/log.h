#ifndef LDR_LOG_H
#define LDR_LOG_H

typedef long log_handler_fn(const char *msg);

int log_message(const char *fmt, ...);
int log_info(const char *fmt, ...);
int log_debug(const char *fmt, ...);
void log_error(const char *fmt, ...) __attribute__((noreturn));
int log_set_handler(int cat, log_handler_fn *handler);
int log_set_threshold(int threshold);
int log_set_final_handler(long (*handler)(void));

#endif
