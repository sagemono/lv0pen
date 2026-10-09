#include "systemio_dmac.h"
#include "errors.h"

int log_message(const char *fmt, ...);

int systemio_dmac_dx::configure(u64 base, u64 desc, u64 desc_io, u64 ioid, u64 dev_addr)
{
    this->base = base;
    this->desc = desc;
    this->desc_io = desc_io;
    this->ioid = ioid;
    this->dev_addr = dev_addr;
    return 0;
}

int systemio_dmac_dx::get_dma_status()
{
    unsigned int status = read32(base + 0x10000 - 20228);
    int result = 0;
    if (((status >> 8) & 1) == 0) {
        result = 1;
        if ((status >> 6) & 1)
            write32(base + 0x10000 - 20236, 553648180);
        else {
            log_message("[ERROR]: 0x%08x %s fail %08x\n", LV0_ERR_DMA, __FUNCTION__, status);
            return -1;
        }
    }
    return result;
}

long systemio_dmac_dx::start_transfer(u64 dst, u64 src, unsigned int len)
{
    if (base == 0)
        return -11;

    systemio_dmac_dx_desc hdr = { desc_io + ioid + 32, src, dst, 0, len };
    systemio_dmac_dx_desc body = { 0, 0xFFF1F9038ULL, 0xFFF1F9038ULL, 0, 4 };

    copy_qwords_to_mmio(desc, (char *)&hdr, 32);
    copy_qwords_to_mmio(desc + 32, (char *)&body, 32);

    write32(base + 0x10000 - 20140, read32(base + 0x10000 - 20140) | 0x41);
    write32(base + 0x10000 - 20260, 0);
    write32(base + 0x10000 - 20252, 8);
    write32(base + 0x10000 - 20244, 8);
    write32(base + 0x10000 - 20228, 0);
    write32(base + 0x10000 - 20236, 0x20000034);
    unsigned int lo = desc_io | ioid;
    unsigned int hi = (desc_io | ioid) >> 32;
    write32(base + 0x10000 - 20288, hi);
    write32(base + 0x10000 - 20284, lo);
    return 0;
}

long systemio_dmac_dx::kick_dma(u64 buf, unsigned int len)
{
    return start_transfer(buf | ioid, dev_addr, len);
}

systemio_dmac_dx *get_systemio_dmac_dx(void)
{
    static systemio_dmac_dx dmac;
    return &dmac;
}
