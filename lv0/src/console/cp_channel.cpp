#include "console.h"
#include "gbe.h"
#include "mmio_accessor.h"
#include "pci.h"
#include "memory.h"

typedef unsigned int __attribute__((may_alias)) u32_ma;

extern cp_channel g_cp_channel;
extern cp_devparam g_cp_channel_devparam;

int cp_channel::find_lowest_set_bit(unsigned int bits)
{
    int i;
    for (i = 0; i < 4; i++)
        if ((bits >> i) & 1) return i;
    return -1;
}

long cp_channel::vslot7()
{
    return 0;
}

int cp_channel::connect()
{
    return 0;
}

long cp_channel::vslot6()
{
    return 0;
}

void cp_channel::set_device(pci_devparam *src, cp_bridge *bridge, unsigned int channel)
{
    if (bridge) {
        this->bridge = bridge;
    } else {
        devparam = &g_cp_channel_devparam;
        g_cp_channel_devparam.cfg_base = src->cfg_base;
        g_cp_channel_devparam.bus      = src->bus;
        g_cp_channel_devparam.dev      = src->dev;
        g_cp_channel_devparam.fn       = src->fn;
        g_cp_channel_devparam.pad      = src->pad;
        g_cp_channel_devparam.vendor   = src->vendor;
        g_cp_channel_devparam.device   = src->device;
    }
    this->channel = channel;
}

cp_channel *get_cp_channel(void)
{
    return &g_cp_channel;
}

unsigned int cp_channel::read_bar0(unsigned int off)
{
    if (bridge)
        return bridge->read_ctl(off + bar0);
    return get_mmio_accessor()->read32(off + bar0);
}

void cp_channel::write_bar0(unsigned int off, unsigned int val)
{
    if (bridge)
        bridge->write_ctl(off + bar0, val);
    else
        get_mmio_accessor()->write32(off + bar0, val);
}

void cp_channel::write_ch(unsigned int off, unsigned int val)
{
    unsigned long reg = off + ((channel << 20) + bar0 + 0x2000000);

    if (bridge)
        bridge->write_ctl(reg, val);
    else
        get_mmio_accessor()->write32(reg, val);
}

unsigned int cp_channel::read_bar1(unsigned int off)
{
    unsigned long reg = (channel << 12) + bar1 + off;

    if (bridge)
        return bridge->read32(reg);
    return get_mmio_accessor()->read32(reg);
}

void cp_channel::read_config(unsigned int *out_)
{
    u32_ma *out = (u32_ma *)out_;

    out[0] = read_bar1(0x28);
    out[1] = read_bar1(0x2C);
    out[2] = read_bar1(0x30);
    out[3] = read_bar1(0x34);
}

void cp_channel::write_bar1(unsigned int off, unsigned int val)
{
    unsigned long reg = (channel << 12) + bar1 + off;

    if (bridge)
        bridge->write32(reg, val);
    else
        get_mmio_accessor()->write32(reg, val);
}

unsigned long cp_channel::cfg_read32(unsigned int reg)
{
    if (bridge)
        return bridge->cfg_read32(reg);
    return pci_config_read32(devparam, reg);
}

void cp_channel::cfg_write32(unsigned int reg, unsigned int val)
{
    if (bridge)
        bridge->cfg_write32(reg, val);
    else
        pci_config_write32(devparam, reg, val);
}

long cp_channel::reset()
{
    write_bar0(8, 0);
    delay_ms(50);
    return 0;
}

long cp_channel::disconnect()
{
    reset();
    return 0;
}

int cp_channel::initialize(pci_devparam *src, cp_bridge *bridge, unsigned int channel)
{
    unsigned int size;
    long bar;

    set_device(src, bridge, channel);
    if (bar0_size && bar1_size)
        return 0;
    if (this->bridge)
        this->bridge->window_high();

    bar = cfg_read32(PCI_BASE_ADDRESS_0);
    bar0 = bar;
    if ((bar & 1) || (bar & 6))
        return -13;
    cfg_write32(PCI_BASE_ADDRESS_0, 0xFFFFFFFF);
    size = cfg_read32(PCI_BASE_ADDRESS_0) & 0xFFFFFFF0;
    cfg_write32(PCI_BASE_ADDRESS_0, bar0);
    bar0 &= 0xFFFFFFF0;
    bar0_size = -size;

    bar = cfg_read32(PCI_BASE_ADDRESS_1);
    bar1 = bar;
    if ((bar & 1) || (bar & 6))
        return -13;
    cfg_write32(PCI_BASE_ADDRESS_1, 0xFFFFFFFF);
    size = cfg_read32(PCI_BASE_ADDRESS_1) & 0xFFFFFFF0;
    cfg_write32(PCI_BASE_ADDRESS_1, bar1);
    bar1 &= 0xFFFFFFF0;
    bar1_size = -size;

    cfg_write32(4, 6);
    if (this->bridge)
        this->bridge->window_low();
    return 0;
}

cp_channel::cp_channel()
    : bar0(0), bar0_size(0), bar1(0), bar1_size(0), tx_buf(0), rx_buf(0),
      status_buf(0), channel(0), devparam(0), bridge(0)
{
}

void cp_channel::setup()
{
    reset();
    write_bar0(0x184, read_bar0(0x184));
    write_bar0(0x18C, read_bar0(0x18C));
    write_bar0(0x194, read_bar0(0x194));
    write_bar0(0x19C, read_bar0(0x19C));
    write_bar0(0x1A4, read_bar0(0x1A4));

    if (bridge) {
        rx_buf = 0x10000;
        tx_buf = 0;
    } else {
        rx_buf = get_gbe_work()->allocate(0x10000, 128);
        tx_buf = (volatile u64 *)get_gbe_work()->allocate(0x10000, 128);
        status_buf = get_gbe_work()->allocate(0x1000, 128);
    }

    write_bar0(0x190, 1);
    write_bar0(0x188, 0);
    write_bar0(0x1A0, 0);
    write_bar0(0x198, 1);
    write_bar0(0x1A8, 0);

    if (bridge) {
        write_bar1(8, (unsigned int)rx_buf);
        write_bar1(0x14, (unsigned int)(unsigned long)tx_buf);
    } else {
        gbe_work *work = get_gbe_work();
        long io = work->io_addr - 0x80000000L;
        write_bar1(8, rx_buf - (long)work->region + io);
        work = get_gbe_work();
        io = work->io_addr - 0x80000000L;
        write_bar1(0x14, (long)tx_buf - (long)work->region + io);
    }
}

int cp_channel::receive(void *buf, long size, unsigned short *out_len)
{
    unsigned int pending;
    int len;

    *out_len = 0;
    if (!(read_bar0(0x180) & 0x40000))
        return -10;

    if (!bridge) {
        gbe_work *work = get_gbe_work();
        long io = work->io_addr - 0x80000000L;
        unsigned long dma_addr = status_buf - (long)work->region + io;
        get_mmio_accessor()->write32(0xD814, (unsigned int)dma_addr);
        get_mmio_accessor()->read32(0xD810);
    }

    pending = read_bar0(0x194);
    if (find_lowest_set_bit((unsigned short)pending)) {
        write_bar0(0x194, pending);
        return -10;
    }
    write_bar0(0x194, pending);

    if (bridge) {
        unsigned int rx = (unsigned int)rx_buf;
        len = (unsigned short)bridge->read_raw(rx);
        if (len > (unsigned short)size)
            len = (unsigned short)size;
        for (int i = 0; i < len; i += 4) {
            unsigned int w = bridge->read_raw(rx + i);
            *(unsigned int *)((char *)buf + i) = ld_le32(&w);
        }
    } else {
        unsigned char *src = (unsigned char *)rx_buf;
        len = src[0] | (src[1] << 8);
        if (len > (unsigned short)size)
            len = (unsigned short)size;
        lv0_memmove((char *)buf, (const char *)src, len);
    }

    __asm__ __volatile__("sync" ::: "memory");
    write_bar1(0, 0);
    write_bar0(0x1C4, 1UL << channel);
    *out_len = len;
    return 0;
}

int cp_channel::send(void *buf, unsigned short len, unsigned short *out_len)
{
    *out_len = 0;
    if (read_bar1(0x10) & 0xC0000000)
        return -9;
    int n = len;
    if (bridge) {
        long tx = (long)tx_buf;
        for (int i = 0; i < n; i += 4)
            bridge->write_raw(tx + i, ld_le32((char *)buf + i));
    } else {
        lv0_memmove((char *)tx_buf, (const char *)buf, n);
    }

    __asm__ __volatile__("sync" ::: "memory");
    write_bar1(0x1C, n);
    write_bar0(0x1C0, 1UL << channel);
    for (;;) {
        if (read_bar0(0x18C) & (unsigned int)(1UL << channel))
            break;
        delay_us(10);
    }
    write_bar0(0x18C, 1UL << channel);
    *out_len = n;
    return 0;
}

cp_channel::~cp_channel()
{
}

cp_channel g_cp_channel;
cp_devparam g_cp_channel_devparam;
