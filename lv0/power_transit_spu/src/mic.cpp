#include "mmio.h"

void delay_ns(unsigned int ns);
void delay_us(unsigned int us);

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

class be_mmio {
public:
    void delay_ns(unsigned int ns);
    void delay_us(unsigned int us);
    u64 read64(unsigned int off);
    int poll64(unsigned int off, unsigned int mask_hi, unsigned int mask_lo,
               unsigned int val_hi, unsigned int val_lo, unsigned int tries);
    void write64(unsigned int off, unsigned int val_hi, unsigned int val_lo);
    void set_mic_timing(unsigned int ch, unsigned int a, unsigned int b);
};

#define BE_MMIO_BASE 0x20000000000ULL

void be_mmio::delay_ns(unsigned int ns)
{
    ::delay_ns(ns);
}

void be_mmio::delay_us(unsigned int us)
{
    ::delay_us(us);
}

u64 read64(u64 ea)
{
    volatile u64 *ls = (volatile u64 *)(u32)(DMA_BUF + (ea & 15));
    mfc_get(ls, ea, 8, DMA_TAG, 0, 0);
    dma_wait();
    return *ls;
}

u64 be_mmio::read64(unsigned int off)
{
    return ::read64(BE_MMIO_BASE | off);
}

int be_mmio::poll64(unsigned int off, unsigned int mask_hi, unsigned int mask_lo,
                    unsigned int val_hi, unsigned int val_lo, unsigned int tries)
{
    u64 mask = (u64)mask_hi << 32 | mask_lo;
    u64 val = ((u64)val_hi << 32 | val_lo) & mask;
    unsigned int i = 0;
    for (;;) {
        i++;
        if ((read64(off) & mask) == val)
            return 1;
        delay_us(1);
        if (i == tries)
            return 0;
    }
}

void write64(u64 ea, u64 val)
{
    volatile u64 *ls = (volatile u64 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 8, DMA_TAG, 0, 0);
    dma_wait();
}

void be_mmio::write64(unsigned int off, unsigned int val_hi, unsigned int val_lo)
{
    ::write64(BE_MMIO_BASE | off, (u64)val_hi << 32 | val_lo);
}

static const unsigned int mic_timing_reg[2] = { 0x50A100, 0x50A140 };

void be_mmio::set_mic_timing(unsigned int ch, unsigned int a, unsigned int b)
{
    write64(mic_timing_reg[ch], (a & 0xFFF) << 16 | (b & 0xFFFF), 0);
}

void quiesce_memory_controller(void)
{
    be_mmio be;
    be.set_mic_timing(0, 17, 0);
    be.set_mic_timing(1, 17, 0);
    be.write64(0x50A208, 0x50000000, 0);
    be.poll64(0x50A208, 0x50000000, 0, 0x50000000, 0, 10000);
    for (int i = 8; i != 0; i--) {
        be.write64(0x50A208, 0x56000000, 0);
        be.poll64(0x50A208, 0x04000000, 0, 0, 0, 10000);
    }
    be.write64(0x50A208, 0x54000000, 0);
    be.poll64(0x50A208, 0x04000000, 0, 0, 0, 10000);
    be.delay_ns(40);
    be.write64(0x50A128, 0x04005000, 0);
    be.write64(0x50A168, 0x04005000, 0);
    be.set_mic_timing(0, 7, 0);
    be.set_mic_timing(1, 7, 0);
    be.set_mic_timing(0, 6, 0);
    be.set_mic_timing(1, 6, 0);
    be.set_mic_timing(0, 1003, 2);
    be.set_mic_timing(1, 1003, 2);
}
