#define ATEXIT_MAX      80

void _exit(int status) __attribute__((noreturn));

static unsigned int g_atexit_free = ATEXIT_MAX;
static void (*g_atexit_handlers[ATEXIT_MAX])(void);

int atexit(void (*f)(void))
{
    if (g_atexit_free == 0)
        return -1;
    g_atexit_handlers[--g_atexit_free] = f;
    return 0;
}

void exit(int status)
{
    while (g_atexit_free < ATEXIT_MAX)
        g_atexit_handlers[g_atexit_free++]();
    _exit(status);
}
