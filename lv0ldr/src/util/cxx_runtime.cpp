extern "C" void __cxa_pure_virtual(void);

void operator delete(void *p)
{
    for (;;)
        ;
}

extern "C" void __cxa_pure_virtual(void)
{
    for (;;)
        ;
}
