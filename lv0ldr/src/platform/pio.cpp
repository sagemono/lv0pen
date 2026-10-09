#include "sb.h"

void syscon_printf(const char *fmt, ...);

#define PIO_OUTPUT      0xFFF500
#define PIO_STATUS      0xFFF504
#define PIO_CFG_508     0xFFF508
#define PIO_CFG_530     0xFFF530

void pio::configure(u64 base, unsigned short cfg_508, unsigned short cfg_530)
{
    sb_base = base;
    write32(sb_base + PIO_CFG_508, cfg_508);
    write32(sb_base + PIO_CFG_530, cfg_530);
}

int pio::write_fff500(unsigned short value)
{
    u64 base = sb_base;
    if (!base)
        return -1;
    write32(base + PIO_OUTPUT, value);
    return 0;
}

unsigned char pio::reverse_bits8(unsigned char value)
{
    unsigned char rev = 0;
    int i;
    for (i = 0; i < 8; i++)
        if ((value >> i) & 1)
            rev |= 1 << (7 - i);
    return rev;
}

int pio::write_output_byte(unsigned char value)
{
    return write_fff500(reverse_bits8(~value) << 8);
}

int pio::read_fff504(unsigned short *out)
{
    u64 base = sb_base;
    if (!base)
        return -1;
    *out = read32(base + PIO_STATUS);
    return 0;
}

inline pio *get_pio()
{
    static pio block;
    return &block;
}

int pio::is_ss2_interrupt_asserted()
{
    unsigned short status[8];

    int ret = get_pio()->read_fff504(status);
    if (ret != 0) {
        syscon_printf("[ERROR] %s(%d) pio is not contifured\n", __FUNCTION__, 96);
        return 0;
    }
    if (~status[0] & 0x20)
        ret = 1;
    return ret;
}

u8 poll_ss2_interrupt()
{
    return get_pio()->is_ss2_interrupt_asserted();
}
