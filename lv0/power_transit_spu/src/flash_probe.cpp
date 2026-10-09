#include "mmio.h"
#include "syscon.h"
#include "uart.h"
#include "util.h"

long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, __builtin_va_list ap);
void delay_ms(unsigned int ms);

int probe_no_flash(void)
{
    return -1;
}

unsigned short bswap16_lo(unsigned short x)
{
    unsigned char lo = x;
    unsigned char hi = x >> 8;
    return (lo << 8) | hi;
}

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

void write16(u64 ea, u16 val)
{
    volatile u16 *ls = (volatile u16 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 2, DMA_TAG, 0, 0);
    dma_wait();
}

int identify_nor_flash(u64 flash)
{
    bool need_swap;
    int result;
    u16 raw_id, raw_type, raw_size;
    u16 id, type, size;

    write16(flash, 0xF0F0);
    write16(flash + 170, 0x9898);
    {
        u16 s16, s17, s18;
        s16 = read16(flash + 32);
        s17 = read16(flash + 34);
        s18 = read16(flash + 36);
        if ((s16 >> 8) != 81 || (s17 >> 8) != 82 || (need_swap = true, (s18 >> 8) != 89)) {
            if ((s16 & 0xFF) != 81 || (s17 & 0xFF) != 82 || (s18 & 0xFF) != 89)
                { result = -1; goto out; }
            need_swap = false;
        }
    }

    write16(flash, 0xF0F0);
    write16(flash + 2730, 0xAAAA);
    write16(flash + 1364, 0x5555);
    write16(flash + 2730, 0x9090);
    read16(flash);
    write16(flash, 0xF0F0);
    write16(flash + 2730, 0xAAAA);
    write16(flash + 1364, 0x5555);
    write16(flash + 2730, 0x9090);
    raw_id = read16(flash + 2);
    raw_type = read16(flash + 28);
    raw_size = read16(flash + 30);
    write16(flash, 0xF0F0);

    if (need_swap) {
        raw_id = bswap16_lo(raw_id);
        raw_type = bswap16_lo(raw_type);
        raw_size = bswap16_lo(raw_size);
    }

    id = raw_id & 0xFF; type = raw_type & 0xFF; size = raw_size & 0xFF;
    result = 3;
    if (id == 126 && type == 2 && size == 1)
        result = 0;
    else if (id == 126 && type == 18 && size == 0)
        result = 1;
    else if (id == 126 && type == 18 && size == 1)
        result = 2;
out:
    return result;
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

int detect_flash_type(u64 base)
{
    int type = probe_nand_flash(base);
    if (type == -1) {
        type = identify_nor_flash(base);
        if (type == -1)
            type = 512;
    }
    return type;
}
