#include "sb.h"

bool is_sb_product_dx(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x01000000;
}

bool is_sb_product_2(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x02000000;
}

bool is_sb_product_3(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x03000000;
}

bool is_sb_product_4(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x04000000;
}
