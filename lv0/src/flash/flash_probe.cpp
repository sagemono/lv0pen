#include "printf.h"
#include "sb.h"
#include "syscon.h"
#include "intrinsics.h"
#include "clock.h"
#include "nand_flash.h"
#include "log.h"

void uart_printf(const char *fmt, ...)
{
    char buf[256];
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    lv0_vsnprintf(buf, sizeof buf, fmt, ap);
    __builtin_va_end(ap);
    buf[sizeof buf - 1] = 0;
    get_uart()->puts(buf);
}

void syscon_printf(const char *fmt, ...)
{
    char buf[256];
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    lv0_vsnprintf(buf, sizeof buf, fmt, ap);
    __builtin_va_end(ap);
    buf[sizeof buf - 1] = 0;
    syscon_write_string(buf);
}

static unsigned short bswap16_lo(unsigned short x)
{
    return (x >> 8) | (x << 8);
}

static inline int nor_read16(volatile unsigned short *p)
{
    return *p;
}

int identify_nor_flash(volatile unsigned short *flash)
{
    unsigned char need_swap;
    unsigned short id, type, size;

    *flash = 0xF0F0;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x55] = 0x9898;
    __asm__ __volatile__("eieio" ::: "memory");

    {
        unsigned short s16, s17, s18;
        s16 = nor_read16(&flash[16]);
        __asm__ __volatile__("eieio" ::: "memory");
        s17 = nor_read16(&flash[17]);
        __asm__ __volatile__("eieio" ::: "memory");
        s18 = nor_read16(&flash[18]);
        __asm__ __volatile__("eieio" ::: "memory");
        if ((unsigned char)(s16 >> 8) == 'Q' && (unsigned char)(s17 >> 8) == 'R' && (unsigned char)(s18 >> 8) == 'Y')
            need_swap = 1;
        else if ((unsigned char)s16 == 'Q' && (unsigned char)s17 == 'R' && (unsigned char)s18 == 'Y')
            need_swap = 0;
        else
            return -1;
    }

    *flash = 0xF0F0;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x555] = 0xAAAA;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x2AA] = 0x5555;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x555] = 0x9090;
    __asm__ __volatile__("eieio" ::: "memory");
    (void)*flash;
    *flash = 0xF0F0;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x555] = 0xAAAA;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x2AA] = 0x5555;
    __asm__ __volatile__("eieio" ::: "memory");
    flash[0x555] = 0x9090;
    __asm__ __volatile__("eieio" ::: "memory");
    id = nor_read16(&flash[1]);
    type = nor_read16(&flash[14]);
    size = nor_read16(&flash[15]);
    *flash = 0xF0F0;
    __asm__ __volatile__("eieio" ::: "memory");

    if (need_swap) {
        id = bswap16_lo(id);
        type = bswap16_lo(type);
        size = bswap16_lo(size);
    }

    if ((unsigned char)id == 0x7E && (unsigned char)type == 0x02 && (unsigned char)size == 0x01)
        return 256;
    if ((unsigned char)id == 0x7E && (unsigned char)type == 0x12 && (unsigned char)size == 0x00)
        return 272;
    if ((unsigned char)id == 0x7E && (unsigned char)type == 0x21 && (unsigned char)size == 0x01)
        return 273;
    if ((unsigned char)id == 0x7E && (unsigned char)type == 0x12 && (unsigned char)size == 0x01)
        return 288;
    if (id == 0x257E && type == 0x2506 && size == 0x2501)
        return 400;
    if (id == 0x227E && type == 0x2266 && size == 0x2260)
        return 416;
    return 417;
}

int probe_nand_flash(long dev)
{
    unsigned short *regs = (unsigned short *)(dev + 0x40000);
    volatile unsigned short status;

    regs[0x4010 / 2] = 15;
    if (!*(volatile unsigned short *)&regs[0x4010 / 2]) {
        regs[0x4012 / 2] = 8;
        __asm__ __volatile__("eieio" ::: "memory");
        regs[0x400C / 2] = 0x98;
        __asm__ __volatile__("eieio" ::: "memory");

        unsigned int cnt = 100;
        for (;;) {
            status = regs[0x4010 / 2];
            cnt--;
            if ((status >> 3) & 1) {
                regs[0x4010 / 2] = 8;
                regs[0x4012 / 2] = 4;
                *(unsigned short *)(dev + 0x44006) = 0;
                __asm__ __volatile__("eieio" ::: "memory");
                regs[0x400C / 2] = 12;
                __asm__ __volatile__("eieio" ::: "memory");
                do
                    __asm__ __volatile__("" ::: "memory");
                while (!((regs[0x4010 / 2] >> 2) & 1));
                regs[0x4010 / 2] = 4;
                return 0;
            }
            delay_ms(1);
            if (cnt == 0)
                break;
        }
    }
    return -1;
}

int detect_flash_type(long base)
{
    int type = probe_nand_flash(base);
    if (type == -1) {
        type = identify_nor_flash((volatile unsigned short *)base);
        if (type == -1)
            type = 512;
    }
    return type;
}
