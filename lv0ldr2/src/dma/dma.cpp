#include "mmio.h"

void dma_wait(void)
{
    static bool first = true;
    if (first) {
        mfc_write_tag_update_immediate();
        while (spu_readchcnt(MFC_WrTagUpdate) != 1)
            ;
        mfc_read_tag_status();
        first = false;
    }
    mfc_write_tag_mask(1 << DMA_TAG);
    mfc_read_tag_status_all();
}

unsigned long copy_qwords_from_mmio(u64 src, char *dst, unsigned int n)
{
    unsigned int i;

    mfc_get((void *)DMA_BUF, src, n, DMA_TAG, 0, 0);
    dma_wait();
    for (i = 0; i < n; i += sizeof(long))
        *(long *)(dst + i) = *(long *)(DMA_BUF + i);
    return n;
}
