#include "console.h"
#include "sb.h"
#include "memory.h"

int uart::putc(int ch)
{
    if (!base)
        return -1;
    while (((*(volatile unsigned int *)(base + 0xFFF308) >> 8) & 1) == 0)
        ;
    *(volatile unsigned int *)(base + 0xFFF31C) = ch;
    return ch;
}

long uart::puts(const char *str)
{
    unsigned int i;
    const unsigned char *cur = (const unsigned char *)str;

    for (i = 0; i < lv0_strlen(str); ++i, ++cur) {
        if (*cur) {
            putc(*cur);
            if (*cur == '\n')
                putc('\r');
        }
    }
    return 0;
}

uart *uart::init(volatile unsigned int *regs, int baud_div, unsigned int mode, int a5, int a6, int a7)
{
    unsigned int baud_reg;
    int m;

    m = mode;
    base = (long)regs;
    regs[0xFFF310 / 4] = 0x808F;
    regs[0xFFF310 / 4] = 0x8E;
    regs[0xFFF300 / 4] = ((m & 1) ? 0x4030 : 0x4020) | ((m >> 1) != 0 ? 8 : 0)
                | (a5 ? 4 : 0) | (a7 ? 3 : 0);
    regs[0xFFF304 / 4] = 0;
    regs[0xFFF314 / 4] = (a6 ? 0x1000 : 0) | (a6 ? 0x800 : 0) | 2;
    baud_reg = baud_div;
    regs[0xFFF318 / 4] = baud_reg;
    return this;
}

inline __attribute__((used)) uart *get_uart()
{
    static uart console;
    return &console;
}

long uart_console_puts(const char *str)
{
    return get_uart()->puts(str);
}
