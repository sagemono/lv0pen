#include "nand_flash.h"
#include "log.h"

long g_unused_4ac58;

int nand_flash_ctrl::set_base(long base)
{
    this->base = base;
    return 0;
}

int nand_flash_ctrl::start_read_sector(unsigned int sector, unsigned short count, unsigned short cmd)
{
    long base = this->base;
    unsigned short hi = sector >> 16;
    unsigned short lo = sector;
    if (base == 0)
        return -1;
    char *regs = (char *)(base + 0x40000);
    *(volatile unsigned short *)(regs + 0x4012) = cmd;
    *(volatile unsigned short *)(regs + 0x4006) = 0;
    *(volatile unsigned short *)(regs + 0x4008) = hi;
    *(volatile unsigned short *)(regs + 0x400A) = lo;
    __asm__ __volatile__("eieio" ::: "memory");
    *(volatile unsigned short *)(regs + 0x400C) = 24;
    __asm__ __volatile__("eieio" ::: "memory");
    return 0;
}

int nand_flash_ctrl::start_idle()
{
    long base = this->base;
    if (base == 0)
        return -1;
    char *regs = (char *)(base + 0x40000);
    *(volatile unsigned short *)(regs + 0x4010) = 15;
    *(volatile unsigned short *)(regs + 0x4012) = 4;
    *(volatile unsigned short *)(regs + 0x4006) = 0;
    *(volatile unsigned short *)(regs + 0x4008) = 0;
    *(volatile unsigned short *)(regs + 0x400A) = 0;
    __asm__ __volatile__("eieio" ::: "memory");
    *(volatile unsigned short *)(regs + 0x400C) = 12;
    __asm__ __volatile__("eieio" ::: "memory");
    return 0;
}

int nand_flash_ctrl::start_write_sector(unsigned int sector, unsigned short count)
{
    long base = this->base;
    unsigned short hi = sector >> 16;
    unsigned short lo = sector;
    if (base == 0)
        return -1;
    char *regs = (char *)(base + 0x40000);
    *(volatile unsigned short *)(regs + 0x4012) = 14;
    *(volatile unsigned short *)(regs + 0x4006) = count;
    *(volatile unsigned short *)(regs + 0x4008) = hi;
    *(volatile unsigned short *)(regs + 0x400A) = lo;
    __asm__ __volatile__("eieio" ::: "memory");
    *(volatile unsigned short *)(regs + 0x400C) = 56;
    __asm__ __volatile__("eieio" ::: "memory");
    return 0;
}

int nand_flash_ctrl::start_erase_sector(unsigned int sector, unsigned short count)
{
    long base = this->base;
    unsigned short hi = sector >> 16;
    unsigned short lo = sector;
    if (base == 0)
        return -1;
    char *regs = (char *)(base + 0x40000);
    *(volatile unsigned short *)(regs + 0x4012) = 12;
    *(volatile unsigned short *)(regs + 0x4006) = count;
    *(volatile unsigned short *)(regs + 0x4008) = hi;
    *(volatile unsigned short *)(regs + 0x400A) = lo;
    __asm__ __volatile__("eieio" ::: "memory");
    *(volatile unsigned short *)(regs + 0x400C) = 64;
    __asm__ __volatile__("eieio" ::: "memory");
    return 0;
}

int nand_flash_ctrl::start_identify_device()
{
    short *regs = (short *)base;
    if (!regs)
        return -1;
    regs[0x44012 / 2] = 5;
    regs[0x44008 / 2] = 0;
    __asm__ __volatile__("eieio" ::: "memory");
    regs[0x4400C / 2] = 8;
    __asm__ __volatile__("eieio" ::: "memory");
    return 0;
}

int nand_flash_ctrl::read_sector(unsigned short *buf, int unused)
{
    long base = this->base;
    if (!base)
        return -1;
    unsigned short *ctl = (unsigned short *)(base + 0x44010);
    int raw = *ctl;
    unsigned short st = raw & 9;
    if (st != 1) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 60, raw);
        return -2;
    }
    *ctl = st;
    {
        unsigned short *dst = buf;
        int n = 256;
        do {
            *dst++ = *(volatile unsigned short *)(base + 0x44000);
        } while (--n);
    }
    return 0;
}

int nand_flash_ctrl::idle()
{
    long base = this->base;
    if (!base)
        return -1;
    unsigned short *ctl = (unsigned short *)(base + 0x44010);
    int raw = *ctl;
    unsigned short st = raw & 0xC;
    if (st != 4) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 99, raw);
        return -2;
    }
    *ctl = st;
    return 0;
}

unsigned short nand_flash_ctrl::get_status()
{
    long base = this->base;
    if (!base)
        return 0xFFFF;
    return *(unsigned short *)(base + 0x4400E);
}

int nand_flash_ctrl::get_reg_44016()
{
    long base = this->base;
    if (!base)
        return 0xFFFF;
    return *(unsigned short *)(base + 0x44016);
}

unsigned char is_nand_flash_boot(void)
{
    long reg = get_nand_flash_ctrl()->get_reg_44016();
    return ((reg >> 8) & 0xFF) == 0x5E;
}

unsigned short nand_flash_ctrl::get_intr()
{
    long base = this->base;
    if (!base)
        return 0xFFFF;
    return *(unsigned short *)(base + 0x44010);
}

int nand_flash_ctrl::write_sector(const unsigned short *buf, int unused)
{
    long base = this->base;
    if (base == 0)
        return -1;
    unsigned short *ctl = (unsigned short *)(base + 0x44010);
    unsigned short st = *ctl;
    if (st & 8) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 157, st);
        return -2;
    }
    if (st & 4) {
        *ctl = 4;
        return 1;
    }
    if (st & 2) {
        *ctl = 2;
        const unsigned short *p = buf;
        for (int n = 0; n < 256; n++)
            *(volatile unsigned short *)(base + 0x44000) = *p++;
        return 0;
    }
    syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 178, st);
    return -3;
}

int nand_flash_ctrl::erase_sector()
{
    long base = this->base;
    if (!base)
        return -1;
    unsigned short *ctl = (unsigned short *)(base + 0x44010);
    int raw = *ctl;
    unsigned short st = raw & 0xC;
    if (st != 4) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 206, raw);
        return -2;
    }
    *ctl = st;
    return 0;
}

int nand_flash_ctrl::identify_device(unsigned short *buf)
{
    long base = this->base;
    if (base == 0)
        return -1;
    unsigned short *ctl = (unsigned short *)(base + 0x44010);
    unsigned short st = *ctl;
    if (st & 8) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 238, st);
        return -2;
    }
    if (st & 4) {
        *ctl = 4;
        return 1;
    }
    if (st & 1) {
        *ctl = 1;
        unsigned short *p = buf;
        for (int n = 0; n < 256; n++)
            *p++ = *(volatile unsigned short *)(base + 0x44000);
        return 0;
    }
    syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 255, st);
    return -3;
}

CXX_DROPPED void nand_flash_ctrl::low_level_format() { DEAD_STRING(function_name, "low_level_format"); }
CXX_DROPPED void nand_flash_ctrl::erase_flash() { DEAD_STRING(function_name, "erase_flash"); }
CXX_DROPPED void nand_flash_ctrl::read_page() { DEAD_STRING(function_name, "read_page"); }
CXX_DROPPED void nand_flash_ctrl::read_block_condition() { DEAD_STRING(function_name, "read_block_condition"); }
