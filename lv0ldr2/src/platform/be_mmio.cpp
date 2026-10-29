#include "be_mmio.h"

void delay_ns(unsigned int ns);
void delay_us(unsigned int us);

void be_mmio::delay_ns(unsigned int ns)
{
    ::delay_ns(ns);
}

void be_mmio::delay_us(unsigned int us)
{
    ::delay_us(us);
}

void be_mmio::write64(unsigned int off, unsigned int val_hi, unsigned int val_lo)
{
    ::write64(BE_MMIO_BASE | off, (u64)val_hi << 32 | val_lo);
}

extern const u64 be_mmio_base = 0x20000000000ULL;

extern const unsigned int mic_timing_reg[2] = { 0x50A100, 0x50A140 };
static const unsigned int mic_timing_b_reg[2] = { 0x50A108, 0x50A148 };

void be_mmio::set_mic_timing(unsigned int ch, unsigned short a, unsigned short b)
{
    write64(mic_timing_reg[ch], (a & 0xFFF) << 16 | (b & 0xFFFF), 0);
}

void be_mmio::set_mic_timing_b(unsigned int ch, unsigned int a, unsigned short b)
{
    write64(mic_timing_b_reg[ch], a << 8 | (b & 0xFF), 0);
}
