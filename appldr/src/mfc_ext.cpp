#include "dma_stream.h"
#include "util.h"

#define WRCH(ch, v)     __asm__ __volatile__ ("wrch $ch" #ch ",%0" : : "r" (v))
#define RDCH(ch, v)     __asm__ __volatile__ ("rdch %0,$ch" #ch : "=r" (v))

unsigned int mfc_atomic_status(void)
{
    unsigned int s;

    RDCH(27, s);
    return s;
}

void ll_getllar(unsigned int ls, u64 ea)
{
    mfc_issue(ls, ea, 128, 0, 0, 0xD0);
}

void ll_putllc(unsigned int ls, u64 ea)
{
    mfc_issue(ls, ea, 128, 0, 0, 0xB4);
}

long event_status(unsigned int *out)
{
    unsigned int s;

    RDCH(0, s);
    *out = s;
    return 0;
}

long event_mask(unsigned int m)
{
    WRCH(1, m);
    return 0;
}

long event_ack(unsigned int m)
{
    WRCH(2, m);
    return 0;
}
