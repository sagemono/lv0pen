#include "systemio_dmac.h"
#include "errors.h"

void syscon_printf(const char *fmt, ...);

long systemio_dmac_px::configure(u64 base, u64 dev_addr, u64 desc, unsigned int desc_io)
{
    if (desc & 0xF)
        return -12;
    if (desc_io & 0xF)
        return -12;
    this->base = base;
    this->dev_addr = dev_addr;
    this->desc = desc;
    this->desc_io = desc_io;
    return 0;
}

int systemio_dmac_px::get_dma_status()
{
    if (!base)
        return -11;
    unsigned int status = read32(base + 0xF000);
    if ((status & 0xE0070000) != 0) {
        syscon_printf("[ERROR]: 0x%08x %s err status %08x\n", LV0_ERR_DMA, __FUNCTION__, status);
        return -1;
    }
    if (status & 1)
        return 1;
    return 0;
}

bool systemio_dmac_px::is_ebus_interrupt_asserted()
{
    if (!base) {
        syscon_printf("[ERROR]: 0x%08x %s not configured\n", LV0_ERR_INTERNAL, __FUNCTION__);
        return 0;
    }
    if (read32(base + 0xF000) & 2)
        return 1;
    return 0;
}

long systemio_dmac_px::kick_dma(u64 dst, unsigned int len)
{
    long result = -11;

    if (base) {
        unsigned int dev = dev_addr;
        if (((unsigned int)dst & 0x7F) != 0 || (dev & 0x7F) != 0) {
            return -12;
        } else {
            int tags[2];

            tags[0] = (unsigned int)dst | 0x80000000;
            tags[1] = len | 0x80000000;
            copy_qwords_to_mmio(desc, (char *)tags, 8);

            write32(base + 0x10000 - 3840, desc_io | 0x80000000);
            result = 0;
            write32(base + 0x10000 - 3828, desc_io | 0x80000000);
            write32(base + 0x10000 - 4096, -1);
            write32(base + 0x10000 - 3836, (dev & 0x1FFFFF00) | 0x80000008);
        }
    }
    return result;
}

systemio_dmac_px *get_systemio_dmac_px(void)
{
    static systemio_dmac_px dmac;
    return &dmac;
}

bool poll_ebus_interrupt(void)
{
    return get_systemio_dmac_px()->is_ebus_interrupt_asserted();
}
