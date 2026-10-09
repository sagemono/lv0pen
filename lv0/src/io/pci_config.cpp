#include "pci.h"
#include "mmio_accessor.h"

unsigned long pci_make_config_address(pci_devparam *dp, long reg)
{
    u8 bus = dp->bus;
    u64 cfg_base = dp->cfg_base;
    bool z = (bus == 0);
    u8 flag = (z == 0);
    return (cfg_base & 0x0FD00000)
         | (u64)(flag << 24)
         | (bus << 16)
         | (u64)((dp->dev << 11) & 0x7F800)
         | (u64)(unsigned short)(dp->fn << 8)
         | (u64)(reg & 0xFD);
}

unsigned short pci_config_read16(pci_devparam *rec, long reg)
{
    long addr = pci_make_config_address(rec, reg);
    mmio_accessor *acc = get_mmio_accessor();
    union { int w; unsigned short h[2]; } val;
    val.w = acc->read32(addr);
    return val.h[((unsigned)(reg ^ 2) & 2) >> 1];
}

unsigned long pci_config_read32(pci_devparam *rec, long reg)
{
    long addr = pci_make_config_address(rec, reg);
    mmio_accessor *acc = get_mmio_accessor();
    return (unsigned int)acc->read32(addr);
}

void pci_config_write16(pci_devparam *rec, long reg, short val)
{
    long addr = pci_make_config_address(rec, reg);
    unsigned int word;

    mmio_accessor *rd_acc = get_mmio_accessor();
    word = rd_acc->read32(addr);
    ((short *)&word)[~reg & 1] = val;

    mmio_accessor *wr_acc = get_mmio_accessor();
    wr_acc->write32(addr, word);
}

void pci_config_write32(pci_devparam *rec, long reg, unsigned int val)
{
    long addr = pci_make_config_address(rec, reg);
    mmio_accessor *acc = get_mmio_accessor();
    acc->write32(addr, val);
}
