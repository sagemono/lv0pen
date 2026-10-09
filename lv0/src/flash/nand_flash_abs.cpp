#include "nand_flash.h"

nand_flash_abs::nand_flash_abs()
{
}

int nand_flash_abs::read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read)
{
    if (offset > 0x3FFFF)
        return nand_flash::read(offset - 0x40000, size, buf, bytes_read);
    return -99;
}

int nand_flash_abs::write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written)
{
    if (offset > 0x3FFFF)
        return nand_flash::write(offset - 0x40000, size, src, bytes_written);
    return -99;
}

long nand_flash_abs::get_size(unsigned long *out_size)
{
    int result = nand_flash::get_size(out_size);
    if (!result)
        *out_size += 0x40000;
    return result;
}
