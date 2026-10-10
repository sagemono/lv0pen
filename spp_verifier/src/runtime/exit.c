int run_atexit(void);
void _exit(int status) __attribute__((noreturn));

void exit(int status)
{
    run_atexit();
    _exit(status);
}
