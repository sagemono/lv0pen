#include <spu_mfcio.h>
#include "types.h"

unsigned int timebase_frequency = 79800000;

void set_timebase_frequency(unsigned int freq)
{
    timebase_frequency = freq;
}

unsigned int read_decrementer(void)
{
    return spu_readch(SPU_RdDec);
}

void write_decrementer(unsigned int count)
{
    unsigned int mask = spu_readch(SPU_RdEventMask);
    spu_writech(SPU_WrEventMask, mask & ~MFC_DECREMENTER_EVENT);
    spu_writech(SPU_WrEventAck, MFC_DECREMENTER_EVENT);
    spu_sync_c();
    spu_writech(SPU_WrDec, count);
    spu_writech(SPU_WrEventMask, mask | MFC_DECREMENTER_EVENT);
}

void start_decrementer(unsigned int count)
{
    write_decrementer(count);
}

unsigned int get_decrementer(void)
{
    return spu_readch(SPU_RdDec);
}

u64 decrementer_ns(void)
{
    unsigned int dec = spu_readch(SPU_RdDec);
    return dec * 1000 / (timebase_frequency / 1000000);
}

u64 decrementer_us(void)
{
    unsigned int dec = spu_readch(SPU_RdDec);
    return dec / (timebase_frequency / 1000000);
}

u64 decrementer_ms(void)
{
    unsigned int dec = spu_readch(SPU_RdDec);
    return dec / (timebase_frequency / 1000);
}

void delay_dec_ticks(int ticks)
{
    int start, end, now;
    start = spu_readch(SPU_RdDec);
    if (ticks <= 0)
        return;
    end = start - ticks;
    if (end < start) {
        for (;;) {
            now = spu_readch(SPU_RdDec);
            if (now <= end || now > start)
                break;
        }
    } else {
        for (;;) {
            now = spu_readch(SPU_RdDec);
            if (now <= end && now > start)
                break;
        }
    }
}

void delay_ms(unsigned int ms)
{
    delay_dec_ticks((timebase_frequency / 1000) * ms);
}

void delay_us(unsigned int us)
{
    delay_dec_ticks((timebase_frequency / 1000000) * us);
}

void delay_ns(unsigned int ns)
{
    delay_dec_ticks((timebase_frequency / 1000000) * ns / 1000);
}
