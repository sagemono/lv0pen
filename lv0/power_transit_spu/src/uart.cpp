#include "mmio.h"
#include "uart.h"
#include "util.h"

enum uart_reg {
    UART_CTRL    = 0xFFF300,
    UART_CTRL2   = 0xFFF304,
    UART_STATUS  = 0xFFF308,
    UART_CMD     = 0xFFF310,
    UART_MODE    = 0xFFF314,
    UART_BAUD    = 0xFFF318,
    UART_TX      = 0xFFF31C,
};

uart::uart() : base(0)
{
}

unsigned long strlen(const char *s)
{
    unsigned int skip = (unsigned int)s & 15;
    const qword *p = (const qword *)(s - skip);
    unsigned int cmp = si_to_uint(si_gbb(si_ceqbi(*p++, 0))) & (0xFFFFu >> skip);
    while (cmp == 0) {
        unsigned int c0 = si_to_uint(si_gbb(si_ceqbi(p[0], 0)));
        unsigned int c1 = si_to_uint(si_gbb(si_ceqbi(p[1], 0)));
        p += 2;
        cmp = (c0 << 16) | c1;
    }
    return ((const char *)p - s) + si_to_uint(si_clz(si_from_uint(cmp))) - 32;
}

long uart::putc(long ch)
{
    if (!base)
        return -1;
    while ((read32(base + UART_STATUS) & 0x100) == 0)
        ;
    write32(base + UART_TX, ch);
    return ch;
}

long uart::puts(const char *str)
{
    unsigned int i;
    const char *cur = str;

    for (i = 0; i < strlen(str); ++i, ++cur) {
        if (*cur) {
            putc(*cur);
            if (*cur == 10)
                putc(13);
        }
    }
    return 0;
}

void uart::init(u64 regs, int baud_div, int mode, long a5, bool a6, long a7)
{
    base = regs;
    write32(base + UART_CMD, 32911);
    write32(base + UART_CMD, 142);
    write32(base + UART_CTRL, ((mode & 1) ? 16432 : 16416) | ((mode >> 1) != 0 ? 8 : 0)
            | (a5 ? 4 : 0) | (a7 ? 3 : 0));
    write32(base + UART_CTRL2, 0);
    write32(base + UART_MODE, (a6 ? 4096 : 0) | (a6 ? 2048 : 0) | 2);
    write32(base + UART_BAUD, baud_div);
}

uart *get_uart()
{
    static uart console;
    return &console;
}

long uart_putc(long ch)
{
    return get_uart()->putc(ch);
}

long uart_console_puts(const char *str)
{
    return get_uart()->puts(str);
}
