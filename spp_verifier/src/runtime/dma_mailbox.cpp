#include "dma_channel.h"

#define WRCH(ch, v)     __asm__ __volatile__ ("wrch $ch" #ch ",%0" : : "r" (v))
#define RDCH(ch, v)     __asm__ __volatile__ ("rdch %0,$ch" #ch : "=r" (v))
#define RCHCNT(ch, v)   __asm__ __volatile__ ("rchcnt %0,$ch" #ch : "=r" (v))

unsigned int dma_channel::write_out_intr_mbox(unsigned int v)
{
    unsigned int n;

    RCHCNT(30, n);
    if (n == 0)
        return MBOX_FULL;
    WRCH(30, v);
    return 0;
}

unsigned int dma_channel::read_in_mbox(unsigned int &v)
{
    unsigned int n, w;

    RCHCNT(29, n);
    if (n == 0)
        return MBOX_EMPTY;
    RDCH(29, w);
    v = w;
    return 0;
}
