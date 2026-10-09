#include "sb.h"
#include "log.h"

#define PIO_OUTPUT      0xFFF500
#define PIO_STATUS      0xFFF504
#define PIO_CFG_508     0xFFF508
#define PIO_CFG_530     0xFFF530

int pio::read_fff500(unsigned short *out)
{
    char *base = sb_base;
    if (!base)
        return -1;
    *out = (unsigned short)*(unsigned int *)(base + PIO_OUTPUT);
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

int pio::read_output_byte()
{
    unsigned char buf[16];
    if (read_fff500((unsigned short *)buf))
        return 0;
    unsigned short hw = *(unsigned short *)buf;
    unsigned char hi = hw >> 8;
    unsigned char inv = ~hi;
    *(unsigned short *)buf = hi;
    return reverse_bits8(inv);
}

void pio::configure(char *base, unsigned int cfg_508, unsigned int cfg_530)
{
    sb_base = base;
    *(unsigned int *)(base + PIO_CFG_530) = cfg_530;
    *(unsigned int *)(base + PIO_CFG_508) = cfg_508;
}

int pio::write_fff500(unsigned short value)
{
    char *base = sb_base;
    if (!base)
        return -1;
    *(unsigned int *)(base + PIO_OUTPUT) = value;
    return 0;
}

int pio::read_fff504(unsigned short *out)
{
    char *base = sb_base;
    if (!base)
        return -1;
    *out = (unsigned short)*(unsigned int *)(base + PIO_STATUS);
    return 0;
}

inline __attribute__((used)) pio *get_pio()
{
    static pio block;
    return &block;
}

bool pio::is_ss2_interrupt_asserted()
{
    unsigned short status[8];

    if (get_pio()->read_fff504(status) != 0) {
        syscon_printf("[ERROR] %s(%d) pio is not contifured\n", __FUNCTION__, 96);
        return 0;
    }
    return (~(unsigned long)status[0] >> 5) & 1;
}

long poll_ss2_interrupt()
{
    return (int)get_pio()->is_ss2_interrupt_asserted();
}

int pio::write_output_byte(unsigned char value)
{
    return write_fff500(reverse_bits8(~value) << 8);
}

bool pio::is_emmcbridge_interrupt_asserted()
{
    unsigned short status[8];

    if (get_pio()->read_fff504(status) != 0) {
        syscon_printf("[ERROR] %s(%d) pio is not contifured\n", __FUNCTION__, 112);
        return 0;
    }
    return ~status[0] & 1;
}

long poll_emmcbridge_interrupt()
{
    return (int)get_pio()->is_emmcbridge_interrupt_asserted();
}
