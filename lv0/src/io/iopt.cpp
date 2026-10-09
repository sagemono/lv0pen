#include "iommu.h"
#include "sb.h"

void iopt_codec::unpack_entry(long dev, unsigned int id, void *out)
{
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        iopt_unpack_entry_v1(dev, id, (unsigned int *)out);
    else
        iopt_unpack_entry_v2(dev, id, (struct chan_status *)out);
}

inline long iopt_pack_entry_v2(long dev, unsigned int id, unsigned int *desc)
{
    int i = id;
    const volatile unsigned int *vdesc = desc;

    if ((unsigned)i > 3)
        return dev;
    long base = dev + (long)(i << 5);
    volatile unsigned int *reg = (volatile unsigned int *)(base + 0x1000000) - 1016;
    unsigned long w3 = *(unsigned long *)desc;
    unsigned int w0 = (12 - desc[2]) | w3;
    unsigned int d7 = vdesc[7], d3 = vdesc[3], d5 = vdesc[5], d9 = vdesc[9], d10 = vdesc[10];
    unsigned int w2 = (d7 << 24) | (d3 << 8) | (d9 << 2) | (d5 << 1) | d10;
    reg[0] = w0;
    reg[2] = w2;
    reg[3] = w3;
    return dev;
}

void iopt_codec::pack_entry(long dev, unsigned int id, void *desc)
{
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        iopt_pack_entry_v1(dev, id, (const char *)desc);
    else
        iopt_pack_entry_v2(dev, id, (unsigned int *)desc);
}

inline void iopt_set_address_v2(long dev, unsigned int id, unsigned int addr)
{
    int i = id;
    *(volatile unsigned int *)(dev + (long)(i << 5) + 0xFFF02C) = addr;
}

void iopt_codec::set_address(long dev, unsigned int id, unsigned int addr)
{
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        return;
    iopt_set_address_v2(dev, id, addr);
}
