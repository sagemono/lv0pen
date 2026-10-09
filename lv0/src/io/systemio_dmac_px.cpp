#include "sb.h"
#include "syscon.h"
#include "log.h"

int systemio_dmac_px::configure(unsigned long base, unsigned long dev_addr, unsigned long desc,
                                unsigned int desc_io)
{
    if (!(desc & 0xF) && !(desc_io & 0xF)) {
        this->base = base;
        this->dev_addr = dev_addr;
        this->desc = desc;
        this->desc_io = desc_io;
        return 0;
    }
    return -12;
}

bool systemio_dmac_px::is_ebus_interrupt_asserted()
{
    if (!base) {
        syscon_printf("[ERROR]: 0x%08x %s not configured\n", LV0_ERR_INTERNAL, __FUNCTION__);
        return 0;
    }
    return (*(unsigned int *)(base + 0xF000) >> 1) & 1;
}

int systemio_dmac_px::kick_dma(unsigned long dst, unsigned int len)
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
            copy_qwords_to_mmio((char *)desc, (char *)tags, 8);

            unsigned long regs = base + 0x10000;
            *(volatile unsigned int *)(regs - 0xF00) = desc_io | 0x80000000;
            result = 0;
            *(volatile unsigned int *)(regs - 0xEF4) = desc_io | 0x80000000;
            *(volatile unsigned int *)(regs - 0x1000) = -1;
            *(volatile unsigned int *)(regs - 0xEFC) = (dev & 0x1FFFFF00) | 0x80000008;
        }
    }
    return result;
}

int systemio_dmac_px::get_dma_status()
{
    if (!base)
        return -11;
    unsigned int status = *(unsigned int *)(base + 0xF000);
    if ((status & 0xE0070000) != 0) {
        syscon_printf("[ERROR]: 0x%08x %s err status %08x\n", LV0_ERR_DMA, __FUNCTION__, status);
        return -1;
    }
    return status & 1;
}

systemio_dmac_px *get_systemio_dmac_px()
{
    static systemio_dmac_px dmac;
    return &dmac;
}

int poll_ebus_interrupt()
{
    return get_systemio_dmac_px()->is_ebus_interrupt_asserted();
}
