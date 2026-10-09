#include "sb.h"
#include "config.h"

#define MIC_REG(off) (*(volatile unsigned long *)(0x20000000000UL + (off)))

unsigned long get_xio_channel_mask(void)
{
    return (MIC_REG(0x50A210) >> 49) & 3;
}

long get_xio_channel_size_mb(int channel)
{
    unsigned long cfg = channel == 0 ? MIC_REG(0x50A0C8) : MIC_REG(0x50A188);
    return 32 * (cfg >> 54) + 32;
}

unsigned long get_total_memory_size(void)
{
    unsigned long mask = (MIC_REG(0x50A210) >> 49) & 3;
    long size_mb = 0;
    if (mask & 1)
        size_mb = get_xio_channel_size_mb(0);
    if (mask & 2)
        size_mb += get_xio_channel_size_mb(1);
    return size_mb << 20;
}
