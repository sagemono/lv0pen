#define ATEXIT_MAX      80

static unsigned short g_atexit_count __attribute__((aligned(16)));
static void (*g_atexit_handlers[ATEXIT_MAX])(void) __attribute__((aligned(16)));

int run_atexit(void)
{
    while (g_atexit_count != 0)
        g_atexit_handlers[--g_atexit_count]();
    return 0;
}
