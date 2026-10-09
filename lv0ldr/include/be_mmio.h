#ifndef LDR_BE_MMIO_H
#define LDR_BE_MMIO_H

#include "mmio.h"

class be_mmio {
public:
    void delay_ns(unsigned int ns);
    void delay_us(unsigned int us);
    void delay_ms(unsigned int ms);
    u64 read64(unsigned int off);
    bool poll64(unsigned int off, unsigned int mask_hi, unsigned int mask_lo,
                unsigned int val_hi, unsigned int val_lo, unsigned int tries);
    void write64(unsigned int off, unsigned int val_hi, unsigned int val_lo);
    void set_mic_timing(unsigned int ch, unsigned short a, unsigned short b);
    unsigned short get_mic_timing(unsigned int ch, unsigned short a);
    bool poll_mic_timing(unsigned int ch, unsigned short a, unsigned short mask,
                         unsigned short val, unsigned int tries);
    void set_mic_timing_b(unsigned int ch, unsigned int a, unsigned short b);
};

#define BE_MMIO_BASE 0x20000000000ULL

extern const u64 be_mmio_base;

extern const unsigned int mic_timing_reg[2] __attribute__((aligned(16)));

#endif
