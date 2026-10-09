#include "sb.h"

void sb_dx_write32_1f60(u64 base, u32 val)
{
    if (!is_sb_product_dx())
        return;
    write32(base + 0x1F60, val);
}

void sb_dx_write32_1c70(u64 base, u32 val)
{
    if (!is_sb_product_dx())
        return;
    write32(base + 0x1C70, val);
}

void sb_dx_write32_1c74(u64 base, u32 val)
{
    if (!is_sb_product_dx())
        return;
    write32(base + 0x1C74, val);
}

void write8(u64 ea, u8 val)
{
    volatile u8 *ls = (volatile u8 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 1, DMA_TAG, 0, 0);
    dma_wait();
}

long sb_device::write8_100000a(int idx, unsigned char val)
{
    if (base == 0)
        return -1;
    write8(base + idx + 0x100000A, val);
    return 0;
}

long sb_device::write8_100003a(unsigned char b)
{
    return write8_100000a(48, b);
}
