#include "mmio.h"

u32 read32(u64 ea)
{
    volatile u32 *ls = (volatile u32 *)(u32)(DMA_BUF + (ea & 15));
    mfc_get(ls, ea, 4, DMA_TAG, 0, 0);
    dma_wait();
    return *ls;
}

void write64(u64 ea, u64 val)
{
    volatile u64 *ls = (volatile u64 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 8, DMA_TAG, 0, 0);
    dma_wait();
}

unsigned long copy_qwords_to_mmio(u64 dest, char *src, unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i += sizeof(long))
        *(long *)(DMA_BUF + i) = *(long *)(src + i);
    mfc_put((void *)DMA_BUF, dest, count, DMA_TAG, 0, 0);
    dma_wait();
    return count;
}
