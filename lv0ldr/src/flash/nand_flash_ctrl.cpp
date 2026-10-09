#include "nand_flash.h"
#include "errors.h"

void syscon_printf(const char *fmt, ...);

int nand_flash_ctrl::set_base(u64 base)
{
    this->base = base;
    return 0;
}

unsigned short nand_flash_ctrl::get_status()
{
    u64 base = this->base;
    if (!base)
        return 0xFFFF;
    return read16(base + 0x4400E);
}

unsigned short nand_flash_ctrl::get_intr()
{
    u64 base = this->base;
    if (!base)
        return 0xFFFF;
    return read16(base + 0x44010);
}

long nand_flash_ctrl::idle()
{
    u64 base = this->base;
    if (!base)
        return -1;
    unsigned int raw = read16(base + 0x44010);
    unsigned int st = raw & 0xC;
    if (st != 4) {
        syscon_printf("[ERROR] 0x%08x %s(%d) intr %08x\n", LV0_ERR_FLASH, __FUNCTION__, 99, raw);
        return -2;
    }
    write16(this->base + 0x44010, st);
    return 0;
}

long nand_flash_ctrl::start_read_sector(unsigned int sector, unsigned short count, unsigned short cmd)
{
    u64 base = this->base;
    if (base == 0)
        return -1;
    write16(base + 0x44012, cmd);
    write16(this->base + 0x44006, 0);
    write16(this->base + 0x44008, sector >> 16);
    write16(this->base + 0x4400A, sector);
    write16(this->base + 0x4400C, 24);
    return 0;
}

long nand_flash_ctrl::start_idle()
{
    u64 base = this->base;
    if (base == 0)
        return -1;
    write16(base + 0x44010, 15);
    write16(this->base + 0x44012, 4);
    write16(this->base + 0x44006, 0);
    write16(this->base + 0x44008, 0);
    write16(this->base + 0x4400A, 0);
    write16(this->base + 0x4400C, 12);
    return 0;
}

nand_flash_ctrl *get_nand_flash_ctrl(void)
{
    static nand_flash_ctrl ctrl;
    return &ctrl;
}
