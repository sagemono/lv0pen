#include "console.h"
#include "iommu.h"
#include "mmio.h"
#include "sb.h"
#include "pci.h"

#define CP_BRIDGE_BASE  0x2401C000000L
#define CP_CFG          0x20000
#define BRIDGE_CFG      0x80000

cp_bridge g_cp_bridge;

cp_bridge::cp_bridge() : base(0), window(0)
{
}

unsigned short cp_bridge::cfg_read16(unsigned int reg)
{
    return *(volatile unsigned int *)(base + CP_CFG + (reg & ~3)) >> ((reg & 2) * 8);
}

void cp_bridge::cfg_write32(unsigned int reg, unsigned int val)
{
    *(volatile unsigned int *)(base + CP_CFG + reg) = val;
}

unsigned long cp_bridge::cfg_read32(unsigned int reg)
{
    return *(volatile unsigned int *)(base + CP_CFG + reg);
}

void cp_bridge::bridge_cfg_write32(unsigned int reg, unsigned int val)
{
    *(volatile unsigned int *)(base + BRIDGE_CFG + reg) = val;
}

unsigned int cp_bridge::bridge_cfg_read32(unsigned int reg)
{
    return *(volatile unsigned int *)(base + BRIDGE_CFG + reg);
}

unsigned long cp_bridge::map(unsigned int addr)
{
    return ((addr >> 2) & 0x1FC0000) | (addr & 0x3FFFF);
}

unsigned int cp_bridge::read32(unsigned int addr)
{
    return *(volatile unsigned int *)(base + (unsigned int)map(addr));
}

void cp_bridge::write32(unsigned int addr, unsigned int val)
{
    *(volatile unsigned int *)(base + (unsigned int)map(addr)) = val;
}

void cp_bridge::set_window(unsigned int id, unsigned int addr)
{
    iopt_codec codec;

    if (window != addr) {
        codec.set_address(0x24000000000L, id, addr);
        window = addr;
    }
}

void cp_bridge::window_low()
{
    set_window(2, 0);
}

void cp_bridge::window_high()
{
    set_window(2, 0x2000000);
}

bool cp_bridge::init()
{
    iopt_codec codec;
    unsigned int mode;

    base = CP_BRIDGE_BASE;
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        mode = 0;
    else if ((mmio_sb_product_code & 0x7F000000) != 0x4000000)
        mode = 2;
    else
        mode = 1;
    if (!mode)
        return 0;

    struct iopt_window win;
    __builtin_memset(&win, 0, sizeof win);
    win.addr = base;
    win.w[0] = 5;
    win.w[1] = mode;
    win.w[2] = 2;
    win.w[3] = 1;
    win.w[7] = 1;
    win.w[8] = 1;
    codec.pack_entry(0x24000000000L, 2, &win);
    __asm__ volatile ("eieio" ::: "memory");
    set_window(2, 0x2000000);

    unsigned int id = bridge_cfg_read32(0);
    if (id != 0x905D104D && id != 0x104D)
        return 0;
    if (cfg_read16(PCI_VENDOR_ID) != 0x104D || cfg_read16(PCI_DEVICE_ID) != 0x81FF)
        return 0;
    bridge_cfg_write32(PCI_BASE_ADDRESS_0, 0);
    bridge_cfg_write32(PCI_BASE_ADDRESS_1, 0x10000);
    bridge_cfg_write32(PCI_COMMAND, bridge_cfg_read32(PCI_COMMAND) | 0xD42);
    cfg_write32(PCI_BASE_ADDRESS_0, 0x4000000);
    cfg_write32(PCI_BASE_ADDRESS_1, 0x20000);
    cfg_write32(PCI_CACHE_LINE_SIZE, cfg_read32(PCI_CACHE_LINE_SIZE) | 0x4000);
    return 1;
}

cp_bridge *get_cp_bridge(void)
{
    static bool present = true;
    static bool ready;

    if (!present)
        return 0;
    if (ready)
        return &g_cp_bridge;
    if (g_cp_bridge.init()) {
        ready = 1;
        return &g_cp_bridge;
    }
    present = 0;
    return 0;
}

void cp_bridge::write_raw(unsigned int off, unsigned int val)
{
    *(volatile unsigned int *)(base + off) = val;
}

void cp_bridge::write_ctl(unsigned int addr, unsigned int val)
{
    *(volatile unsigned int *)(base + (unsigned int)map(addr)) = val;
}

unsigned int cp_bridge::read_raw(unsigned int off)
{
    return *(volatile unsigned int *)(base + off);
}

unsigned int cp_bridge::read_ctl(unsigned int addr)
{
    return *(volatile unsigned int *)(base + (unsigned int)map(addr));
}
