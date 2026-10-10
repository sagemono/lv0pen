extern "C" void free(void *p);

void operator delete(void *p)
{
    if (p)
        free(p);
}
