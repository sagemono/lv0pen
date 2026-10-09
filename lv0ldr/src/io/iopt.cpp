#include "iopt.h"
#include "sb.h"

int log_debug(const char *fmt, ...);

void iopt_unpack_entry_v2(u64 dev, unsigned int id, iopt_desc *out)
{
    int i = id;

    if ((unsigned)i > 3)
        return;
    u64 regs = dev + (i << 5);
    u64 w0 = read32(regs + 0xFFF020);
    out->w[2] = 12 - (w0 & 0x1F);
    unsigned int w2 = read32(regs + 0xFFF028);
    out->w[7] = (w2 & 0x1000000) >> 24;
    out->w[3] = (w2 & 0x3F00) >> 8;
    out->w[9] = (w2 & 4) >> 2;
    out->w[5] = (w2 & 2) >> 1;
    out->w[10] = w2 & 1;
    out->w[4] = 0;
    out->w[6] = 0;
    out->w[8] = 0;
    out->w[11] = 0;
    out->addr = read32(regs + 0xFFF02C);
}

void iopt_pack_entry_v2(u64 dev, unsigned int id, iopt_desc *desc)
{
    int i = id;

    if ((unsigned)i > 3)
        return;
    u64 regs = dev + (i << 5);
    write32(regs + 0xFFF020, desc->w[1] | (12 - desc->w[2]));
    log_debug("PX::EBCADMATCH%d %08x\n", id, read32(regs + 0xFFF020));
    write32(regs + 0xFFF028, (desc->w[7] << 24) | (desc->w[3] << 8) | (desc->w[9] << 2) | (desc->w[5] << 1) | desc->w[10]);
    log_debug("PX::EBCACCTRL%d %08x\n", id, read32(regs + 0xFFF028));
    write32(regs + 0xFFF02C, desc->w[1]);
    log_debug("PX::EBCADBS%d %08x\n", id, read32(regs + 0xFFF02C));
}

void iopt_pack_entry_v1(u64 dev, unsigned int id, iopt_desc *desc)
{
    write64(dev + (id << 3) + 0xFF9000,
              ((desc->addr >> 4) << 32)
            | (int)(desc->w[2] << 8)
            | (int)(desc->w[3] << 12)
            | (int)(desc->w[4] << 20)
            | (int)(desc->w[5] << 6)
            | (int)(desc->w[6] << 4)
            | (int)(desc->w[7] << 24)
            | (int)(desc->w[8] << 22)
            | (int)(desc->w[9] << 7)
            | (int)(desc->w[10] << 3)
            | (int)desc->w[11]);
}

void iopt_codec::pack_entry(u64 dev, unsigned int id, void *desc)
{
    if (is_sb_product_dx())
        iopt_pack_entry_v1(dev, id, (iopt_desc *)desc);
    else
        iopt_pack_entry_v2(dev, id, (iopt_desc *)desc);
}

void iopt_unpack_entry_v1(u64 dev, unsigned int id, iopt_desc *out)
{
    u64 entry = read64(dev + (id << 3) + 0xFF9000);

    out->addr = (entry >> 32) << 4;
    out->w[2] = (entry & 0xF00) >> 8;
    out->w[3] = (entry & 0x3F000) >> 12;
    out->w[4] = (entry & 0x300000) >> 20;
    out->w[5] = (entry & 0x40) >> 6;
    out->w[6] = (entry & 0x30) >> 4;
    out->w[7] = (entry & 0x1000000) >> 24;
    out->w[8] = (entry & 0xC00000) >> 22;
    out->w[9] = (entry & 0x80) >> 7;
    out->w[10] = (entry & 8) >> 3;
    out->w[11] = entry & 7;
}

void iopt_codec::unpack_entry(u64 dev, unsigned int id, void *out)
{
    if (is_sb_product_dx())
        iopt_unpack_entry_v1(dev, id, (iopt_desc *)out);
    else
        iopt_unpack_entry_v2(dev, id, (iopt_desc *)out);
}
