#include "sb.h"
#include "syscon.h"
#include "log.h"
#include "memory.h"

int systemio_dmac_dx::configure(unsigned long base, unsigned long desc, unsigned long desc_io,
                                unsigned long ioid, unsigned long dev_addr)
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
    long base = this->base;
    long regs = base + 0x10000;
    unsigned int status = *(unsigned int *)(regs - 0x4F04);
    int result = 0;
    if (((status >> 8) & 1) == 0) {
        result = 1;
        if (status & 0x40)
            *(int *)(regs - 0x4F0C) = 0x21000034;
        else {
            log_message("[ERROR]: 0x%08x %s fail %08x\n", LV0_ERR_DMA, __FUNCTION__, status);
            return -1;
        }
    }
    return result;
}

int systemio_dmac_dx::start_transfer(unsigned long dst, unsigned long src, unsigned int len)
{
    struct {
        unsigned long next;
        unsigned long src;
        unsigned long dst;
        unsigned int pad;
        unsigned int len;
    } hdr;
    struct {
        unsigned long a, b, c;
        unsigned int d, e;
    } body;

    if (base == 0)
        return -11;
    lv0_memset(&hdr, 0, sizeof hdr);
    hdr.next = desc_io + ioid + 32;
    hdr.src = src;
    hdr.dst = dst;
    hdr.len = len;
    body.a = 0;
    body.b = 0xFFF1F9038;
    body.c = 0xFFF1F9038;
    body.d = 0;
    body.e = 4;
    copy_qwords_to_mmio((char *)desc, (char *)&hdr, 32);
    copy_qwords_to_mmio((char *)(desc + 32), (char *)&body, 32);
    {
        char *regs = (char *)(base + 0x10000);
        *(volatile unsigned int *)(regs - 0x4EAC) |= 0x41;
        *(volatile unsigned int *)(regs - 0x4F24) = 0;
        *(volatile unsigned int *)(regs - 0x4F1C) = 8;
        *(volatile unsigned int *)(regs - 0x4F14) = 8;
        *(volatile unsigned int *)(regs - 0x4F04) = 0;
        *(volatile unsigned int *)(regs - 0x4F0C) = 0x20000034;
        __asm__ volatile ("eieio" ::: "memory");
        *(volatile unsigned int *)(regs - 0x4F40) = (desc_io | ioid) >> 32;
        *(volatile unsigned int *)(regs - 0x4F3C) = desc_io | ioid;
    }
    return 0;
}

int systemio_dmac_dx::kick_dma(unsigned long buf, unsigned int len)
{
    return start_transfer(buf | ioid, dev_addr, len);
}
