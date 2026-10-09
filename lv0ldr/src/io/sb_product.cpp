#include "sb.h"

bool is_sb_product_dx(void)
{
    return (read32(SB_MMIO_BASE + 0x87000) & 0x7F000000) == 0x01000000;
}
