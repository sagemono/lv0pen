#include "sb.h"

extern const long sb_device_iopt_desc_template[6] = {
    0x0000000001000000, 3, 0x0000000300000000, 0, 0, 0x0000000100000002,
};

void sb_dx_write32_1f60(long base, long val)
{
    if ((mmio_sb_product_code & 0x7F000000) != 0x01000000)
        return;
    *(unsigned int *)(base + 0x1F60) = val;
}

int sb_device::write8_100000a(unsigned int idx, long val)
{
    if (base == 0)
        return -1;
    *(volatile unsigned char *)(base + 0x100000A + (int)idx) = val;
    return 0;
}

int sb_device::read8_1000009(unsigned int idx, unsigned char *out)
{
    if (base == 0)
        return -1;
    *out = *(volatile unsigned char *)(base + 0x1000009 + (int)idx);
    return 0;
}

int sb_device::write8_100003a(long b)
{
    return write8_100000a(48, b);
}

int sb_device::read8_1000039()
{
    unsigned char buf[16];
    if (read8_1000009(48, buf) == 0)
        return buf[0];
    return 0;
}

void sb_dx_write32_1c74(long base, long val)
{
    if ((mmio_sb_product_code & 0x7F000000) != 0x01000000)
        return;
    *(unsigned int *)(base + 0x1C74) = val;
}

void sb_dx_write32_1c70(long base, long val)
{
    if ((mmio_sb_product_code & 0x7F000000) != 0x01000000)
        return;
    *(unsigned int *)(base + 0x1C70) = val;
}
