#include "dma_channel.h"

#define WRCH(ch, v)     __asm__ __volatile__ ("wrch $ch" #ch ",%0" : : "r" (v))
#define RDCH(ch, v)     __asm__ __volatile__ ("rdch %0,$ch" #ch : "=r" (v))
#define RCHCNT(ch, v)   __asm__ __volatile__ ("rchcnt %0,$ch" #ch : "=r" (v))

dma_channel::dma_channel()
{
}

unsigned int dma_channel::issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
                                unsigned int rid, unsigned int cmd)
{
    if (ls == 0 || ea == 0 || size == 0 || size > 0x4000 || tag > 31 ||
        (cmd != MFC_GET && cmd != MFC_PUT))
        return DMA_EINVAL;
    WRCH(16, ls);
    WRCH(17, (unsigned int)(ea >> 32));
    WRCH(18, (unsigned int)ea);
    WRCH(19, size);
    WRCH(20, tag);
    WRCH(21, rid << 16 | cmd);
    return 0;
}

bool dma_channel::tag_done(unsigned int tag)
{
    unsigned int n, s, mask;

    WRCH(23, 0);
    do
        RCHCNT(23, n);
    while (n != 1);
    RDCH(24, s);
    mask = 1 << tag;
    WRCH(22, mask);
    WRCH(23, 0);
    RDCH(24, s);
    return s == mask;
}

void dma_channel::wait(unsigned int tag)
{
    unsigned int n, s;

    WRCH(23, 0);
    do
        RCHCNT(23, n);
    while (n != 1);
    RDCH(24, s);
    WRCH(22, 1 << tag);
    WRCH(23, 2);
    RDCH(24, s);
}

dma_channel::~dma_channel()
{
}
