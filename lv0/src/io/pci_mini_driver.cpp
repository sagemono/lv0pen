#define PCI_MINI_DRIVER_UNIT __attribute__((visibility("hidden")))

#include "pci.h"
#include "sb.h"
#include "platform.h"
#include "clock.h"
#include "log.h"
#include "mmio_accessor.h"

extern pci_mini_driver g_pci_mini_driver;

unsigned char pci_mini_driver_sc_version_subcmd[2] __attribute__((aligned(2))) = { 0x01, 0x00 };

#define mmio_pci_ctrl_1990        (*(volatile unsigned int *)0x24000001990UL)
#define mmio_pci_ctrl_1998        (*(volatile unsigned int *)0x24000001998UL)
#define mmio_pci_ctrl_3160        (*(volatile unsigned int *)0x24000003160UL)
#define mmio_pci_ctrl_3168        (*(volatile unsigned int *)0x24000003168UL)
#define mmio_pci_ctrl_2d68        (*(volatile unsigned int *)0x24000002D68UL)
#define mmio_pci_ctrl_2d70        (*(volatile unsigned int *)0x24000002D70UL)
#define mmio_pci_ctrl_1810        (*(volatile unsigned int *)0x24000001810UL)
#define mmio_pci_ctrl_1814        (*(volatile unsigned int *)0x24000001814UL)
#define mmio_pci_ctrl_1818        (*(volatile unsigned int *)0x24000001818UL)
#define mmio_pci_ctrl_181c        (*(volatile unsigned int *)0x2400000181CUL)
#define mmio_pci_ctrl_1820        (*(volatile unsigned int *)0x24000001820UL)
#define mmio_pci_ctrl_1830        (*(volatile unsigned int *)0x24000001830UL)
#define mmio_pci_ctrl_1834        (*(volatile unsigned int *)0x24000001834UL)
#define mmio_pci_ctrl_1838        (*(volatile unsigned int *)0x24000001838UL)
#define mmio_pci_ctrl_183c        (*(volatile unsigned int *)0x2400000183CUL)
#define mmio_pci_ctrl_cfg_window  (*(volatile unsigned int *)0x24000001968UL)
#define mmio_pci_ctrl_mem_window  (*(volatile unsigned int *)0x24000001960UL)
#define mmio_pci_host_df00        (*(volatile unsigned int *)0x2400000DF00UL)
#define mmio_pci_host_dff0        (*(volatile unsigned int *)0x2400000DFF0UL)
#define mmio_pci_host_d110        (*(volatile unsigned int *)0x2400000D110UL)
#define mmio_pci_host_d81c        (*(volatile unsigned int *)0x2400000D81CUL)
#define mmio_pci_host_d800        (*(volatile unsigned int *)0x2400000D800UL)
#define mmio_pci_host_d80c        (*(volatile unsigned int *)0x2400000D80CUL)
#define mmio_pci_host_d100        (*(volatile unsigned int *)0x2400000D100UL)
#define mmio_pci_host_d104        (*(volatile unsigned int *)0x2400000D104UL)
#define mmio_pci_host_d108        (*(volatile unsigned int *)0x2400000D108UL)
#define mmio_pci_host_d12c        (*(volatile unsigned int *)0x2400000D12CUL)
#define mmio_pci_host_d134        (*(volatile unsigned int *)0x2400000D134UL)
#define mmio_pci_host_d13c        (*(volatile unsigned int *)0x2400000D13CUL)
#define mmio_pci_host_d804        (*(volatile unsigned int *)0x2400000D804UL)
#define mmio_pci_host_d114        (*(volatile unsigned int *)0x2400000D114UL)
#define mmio_pci_host_d124        (*(volatile unsigned int *)0x2400000D124UL)
#define mmio_pci_host_command     (*(volatile unsigned int *)0x2400000D004UL)
#define mmio_pci_host_bar0        (*(volatile unsigned int *)0x2400000D010UL)
#define mmio_pci_host_bar1        (*(volatile unsigned int *)0x2400000D014UL)
#define mmio_pci_host_bar2        (*(volatile unsigned int *)0x2400000D018UL)

static inline void eieio(void) { __asm__ volatile("eieio"); }

pci_mini_driver::pci_mini_driver()
{
    ndev = 0;
    for (int i = 0; i < 32; i++)
        devparams[i] = 0;
}

pci_mini_driver *get_pci_mini_driver(void)
{
    return &g_pci_mini_driver;
}

pci_mini_driver g_pci_mini_driver;
pci_devparam g_pci_devparams[32];

void pci_mini_driver::delay_ms(unsigned long ms)
{
    unsigned long deadline = get_time_ms() + ms;
    while (deadline > get_time_ms())
        ;
}

int pci_mini_driver::scan_bus(int window)
{
    struct pci_devparam rec;
    mmio_accessor *acc = get_mmio_accessor();
    unsigned int sb_id = acc->read32(0x87000);
    int base;
    int nslots;
    char *mem_cur, *io_cur;
    int slot, found;

    acc->write32(0x1998, 0x10000);
    do
        delay_ms(5);
    while ((int)acc->read32(0x1998) != 0x10000);

    rec.cfg_base = 0;
    rec.bus = 0;
    rec.dev = 0;
    rec.fn = 0;
    rec.pad = 0;
    rec.vendor = 0;
    rec.device = 0;

    unsigned long cfg_reg = get_mmio_accessor()->read32(0x1968);
    unsigned long mem_reg = get_mmio_accessor()->read32(0x1960);
    unsigned long bus_window = (long)window << 31;
    mem_cur = (char *)(mem_reg & 0xFFFFF000);
    unsigned long cfg_base = cfg_reg & 0xFFFFF000;
    rec.cfg_base = bus_window + cfg_base;
    io_cur = (char *)(cfg_base + 0x3000000);
    g_platform_ptr->set_post_code(192, 10);

    switch (sb_id) {
    case 0x1000101:
    case 0x1000102:
        base = 6;
        nslots = 1;
        break;
    default:
        base = 0;
        nslots = 32;
        break;
    }

    found = 0;
    for (slot = 0; slot < nslots; slot++) {
        rec.dev = slot + base;
        unsigned short vendor = pci_config_read16(&rec, PCI_VENDOR_ID);
        if (vendor == 0xFFFF)
            continue;
        rec.vendor = vendor;
        rec.device = pci_config_read16(&rec, PCI_DEVICE_ID);
        pci_devparam *ent = &g_pci_devparams[found];
        ent->cfg_base = rec.cfg_base;
        unsigned short vendor_id = rec.vendor;
        devparams[found] = ent;
        unsigned char bus = rec.bus, dev = rec.dev, fn = rec.fn, pad = rec.pad;
        ent->vendor = vendor_id;
        ent->bus = bus;
        ent->dev = dev;
        ent->fn = fn;
        ent->pad = pad;
        ent->device = rec.device;
        for (long bar = PCI_BASE_ADDRESS_0; bar != PCI_BASE_ADDRESS_5 + 4; bar += 4) {
            unsigned int orig = pci_config_read32(&rec, bar);
            pci_config_write32(&rec, bar, 0xFFFFFFFF);
            unsigned int probe = pci_config_read32(&rec, bar);
            pci_config_write32(&rec, bar, orig);
            if (probe == 0)
                continue;
            if (probe & 1) {
                unsigned int mask = probe & ~3u;
                unsigned int size = -mask;
                if ((unsigned long)io_cur & (size - 1))
                    io_cur = (char *)(((unsigned long)io_cur + size) & mask);
                pci_config_write32(&rec, bar, (unsigned long)io_cur);
                io_cur += size;
            } else {
                unsigned int mask = probe & ~15u;
                unsigned int size = -mask;
                if ((unsigned long)mem_cur & (size - 1))
                    mem_cur = (char *)(((unsigned long)mem_cur + size) & mask);
                pci_config_write32(&rec, bar, (unsigned long)mem_cur);
                mem_cur += size;
            }
        }
        found++;
    }

    ndev = found;
    acc->write32(0xD110, 0x30000);
    acc->write32(0xD118, 4);
    acc->write32(0x1998, 0);
    do
        delay_ms(5);
    while ((int)acc->read32(0x1998));

    return found;
}

void pci_mini_driver::init_controller()
{
    if (mmio_pci_ctrl_1990 & 0x1800000) {
        mmio_pci_ctrl_1998 = 0x20;
        while (mmio_pci_ctrl_1998 != 0x20) ;
        mmio_pci_ctrl_1998 = 0;
    }
    mmio_pci_ctrl_1990 &= ~0x2000000u;
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000
        || (mmio_sb_product_code & 0x7F000000) == 0x2000000) {
        mmio_pci_ctrl_3160 = 0x30004B2;
        mmio_pci_ctrl_3168 = 0x38004B2;
    }
    mmio_pci_ctrl_2d68 = 0x3006459;
    mmio_pci_ctrl_2d70 = 0x3005459;
    mmio_pci_ctrl_1998 = 0x10;
    while (mmio_pci_ctrl_1998 != 0x10) ;
    mmio_pci_ctrl_1998 = 0;
    while (mmio_pci_ctrl_1998) ;

    mmio_pci_ctrl_1810 = 0;
    mmio_pci_ctrl_1814 = 0x1F;
    mmio_pci_ctrl_1818 = 0x1F;
    mmio_pci_ctrl_181c = 0x1F;
    mmio_pci_ctrl_1820 = 0x1F;
    mmio_pci_ctrl_1830 = 0;
    mmio_pci_ctrl_1834 = 0x1F;
    mmio_pci_ctrl_1838 = 0x1F;
    mmio_pci_ctrl_183c = 0x1F;
    mmio_pci_ctrl_cfg_window = 0x40004C9;
    mmio_pci_ctrl_mem_window = 0x600004E2;
    mmio_pci_host_df00 = 1;
    mmio_pci_host_dff0 = 0;
    while (mmio_pci_host_dff0) ;
    eieio();
    mmio_pci_host_dff0 = 0x101;
    while (mmio_pci_host_dff0 != 0x101) ;
    eieio();
    mmio_pci_host_dff0 = 0x10101;
    while (mmio_pci_host_dff0 != 0x10101) ;
    eieio();
    mmio_pci_host_d110 = 0x3F0;
    mmio_pci_host_d81c = 5;
    mmio_pci_host_d800 = 0;
    mmio_pci_host_d80c = 0xF1F001F;

    if (set_bus_power(0)) {
        log_message("[ERROR]: 0x%08x pci_mini_driver::set_bus_power fail\n", LV0_ERR_CONFIG);
        return;
    }

    mmio_pci_host_d800 = 0x200;
    mmio_pci_host_d800 = 0x201;
    while (mmio_pci_host_d800 != 0x201) ;
    mmio_pci_host_dff0 = 0x30101;
    while (mmio_pci_host_dff0 != 0x30101) ;
    eieio();
    mmio_pci_host_d100 = 0x20000000;
    mmio_pci_host_d104 = 0;
    mmio_pci_host_d108 = 0;
    mmio_pci_host_d12c = 0xF0000000;
    mmio_pci_host_d134 = 0;
    mmio_pci_host_d13c = 0;
    mmio_pci_host_d804 = 0xFFFFFFFF;
    mmio_pci_host_d114 = 0xF;
    mmio_pci_host_d124 = 1;
    mmio_pci_host_bar0 = 0x20000008;
    mmio_pci_host_bar1 = 8;
    mmio_pci_host_bar2 = 8;
    mmio_pci_host_command = 0x2880002;
    mmio_pci_host_command = 0x2880006;
    mmio_pci_host_d800 = 0x301;
    while (mmio_pci_host_d800 != 0x301) ;
    ::delay_ms(0);
}

int pci_mini_driver::initialize()
{
    init_controller();
    return scan_bus(0);
}

int bus_driver::get_devparam(unsigned short vendor, unsigned short device, int nth, pci_devparam **out)
{
    g_platform_ptr->set_post_code(192, 11);
    unsigned short v = vendor, d = device;
    for (int i = 0; i < 32; i++) {
        volatile pci_devparam *entry = devparams[i];
        unsigned short id = entry->vendor;
        if (v == id && d == entry->device) {
            if (nth == 0) {
                *out = (pci_devparam *)entry;
                return 0;
            }
            nth--;
        }
    }
    return -10;
}
