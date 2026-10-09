#include "mmio.h"
#include "syscon.h"
#include "uart.h"
#include "util.h"

long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, __builtin_va_list ap);
void delay_ms(unsigned int ms);

void uart_printf(const char *fmt, ...)
{
    char buf[256];
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    lv0_vsnprintf(buf, 0x100, fmt, ap);
    __builtin_va_end(ap);
    buf[255] = 0;
    get_uart()->puts(buf);
}

void syscon_printf(const char *fmt, ...)
{
    char buf[256];
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    lv0_vsnprintf(buf, 0x100, fmt, ap);
    __builtin_va_end(ap);
    buf[255] = 0;
    syscon_write_string(buf);
}

int probe_nand_flash(u64 dev)
{
    u64 regs = dev + 0x44000;
    volatile unsigned short status;

    write16(regs + 0x10, 15);
    if (!read16(regs + 0x10)) {
        write16(regs + 0x12, 8);
        write16(regs + 0x0C, 255);

        unsigned int cnt = 100;
        for (;;) {
            status = read16(regs + 0x10);
            if (status & 8) {
                write16(regs + 0x10, 8);
                return 257;
            }
            delay_ms(1);
            if (--cnt == 0)
                break;
        }
    }
    return -1;
}
