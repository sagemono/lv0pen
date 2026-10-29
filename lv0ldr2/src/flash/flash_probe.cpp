#include "mmio.h"
#include "syscon.h"
#include "uart.h"
#include "util.h"

long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, __builtin_va_list ap);
void delay_ms(unsigned int ms);

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

int identify_nor_flash(u64 flash)
{
    bool need_swap;
    int result;
    u16 raw_id, raw_type, raw_size;

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

    if ((raw_id & 0xFF) == 126) {
        if ((raw_type & 0xFF) == 2) {
            result = 256;
            if ((raw_size & 0xFF) == 1)
                goto out;
        } else if ((raw_type & 0xFF) == 18) {
            result = 272;
            if ((raw_size & 0xFF) == 0)
                goto out;
            goto size_1;
        } else if ((raw_type & 0xFF) == 33) {
            result = 273;
            if ((raw_size & 0xFF) == 1)
                goto out;
        }
        goto other;
size_1:
        result = 288;
        if ((raw_size & 0xFF) == 1)
            goto out;
    }
other:
    result = 400;
    if (raw_id != 0x257E || raw_type != 0x2506 || raw_size != 0x2501) {
        result = 416;
        if (raw_id != 0x227E || raw_type != 0x2266 || raw_size != 0x2260)
            result = 417;
    }
out:    return result;
}

int probe_nand_flash(u64 dev)
{
    volatile unsigned short status;

    write16(dev + 0x44010, 15);
    if (!read16(dev + 0x44010)) {
        write16(dev + 0x44012, 8);
        write16(dev + 0x4400C, 0x98);

        unsigned int cnt = 100;
        for (;;) {
            status = read16(dev + 0x44010);
            if (status & 8) {
                write16(dev + 0x44010, 8);
                write16(dev + 0x44012, 4);
                write16(dev + 0x44006, 0);
                write16(dev + 0x4400C, 12);
                while (!(read16(dev + 0x44010) & 4))
                    ;
                write16(dev + 0x44010, 4);
                if ((read16(dev + 0x4400E) >> 8) == 0)
                    return 0;
                break;
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
