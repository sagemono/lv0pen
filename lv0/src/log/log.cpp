#include "log.h"
#include "printf.h"

log_handler_fn *g_log_handlers[4];
int g_log_threshold = 1;
long (*g_log_final_handler)(unsigned int code);

int log_dispatch(int cat, int level, const char *fmt, void *va)
{
    if (g_log_threshold < level)
        return 0;
    if (cat > 3)
        return -1;
    if (!g_log_handlers[cat])
        return 0;

    char buf[256];
    lv0_vsnprintf(buf, sizeof(buf), fmt, va);
    buf[sizeof(buf) - 1] = 0;
    g_log_handlers[cat](buf);
    return 0;
}

int log_printf(int cat, int level, const char *fmt, ...)
{
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    int rc = log_dispatch(cat, level, fmt, ap);
    __builtin_va_end(ap);
    return rc;
}

int log_message(const char *fmt, ...)
{
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    int rc = log_dispatch(0, 1, fmt, ap);
    __builtin_va_end(ap);
    return rc;
}

void log_error(unsigned int code, const char *fmt, ...)
{
    __builtin_va_list va;
    __builtin_va_start(va, fmt);
    log_dispatch(0, 0, fmt, va);
    if (g_log_final_handler)
        g_log_final_handler(code);
    for (;;)
        ;
}

int log_set_handler(int cat, log_handler_fn *handler)
{
    if (cat > 3)
        return -1;
    g_log_handlers[cat] = handler;
    return 0;
}

int log_set_threshold(int threshold)
{
    if (threshold > 3)
        return -1;
    g_log_threshold = threshold;
    return 0;
}

unsigned int log_get_threshold(void)
{
    return g_log_threshold;
}

int log_set_final_handler(long (*handler)(unsigned int code))
{
    g_log_final_handler = handler;
    return 0;
}
