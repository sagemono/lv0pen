#ifndef LDR_SB_H
#define LDR_SB_H

#include "mmio.h"

#define SB_MMIO_BASE    0x24000000000ULL

bool is_sb_product_dx(void);

void sb_dx_write32_1f60(u64 base, u32 val);
void sb_dx_write32_1c70(u64 base, u32 val);
void sb_dx_write32_1c74(u64 base, u32 val);

class sb_device {
public:
    sb_device() : base(0) {}
    long write8_100000a(int idx, unsigned char val);
    long write8_100003a(unsigned char b);
    void initialize(u64 sb_base, bool init_hw);

    u64 base;
    unsigned int reserved;
};

sb_device *get_sb_device(void);

class pio {
public:
    pio() : sb_base(0) {}
    void configure(u64 base, unsigned short cfg_508, unsigned short cfg_530);
    int write_fff500(unsigned short value);
    unsigned char reverse_bits8(unsigned char value);
    int write_output_byte(unsigned char value);
    int read_fff504(unsigned short *out);
    int is_ss2_interrupt_asserted();

    u64 sb_base;
    unsigned int reserved;
};

pio *get_pio(void);
u8 poll_ss2_interrupt(void);

#endif
