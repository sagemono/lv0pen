#ifndef PT_UART_H
#define PT_UART_H

#include "mmio.h"

class uart {
public:
    uart() : base(0) {}
    long putc(long ch);
    long puts(const char *str);
    void init(u64 regs, int baud_div, int mode, long a5, bool a6, long a7);

    u64 base;
    unsigned int reserved;
};

uart *get_uart();
long uart_console_puts(const char *str);

#endif
