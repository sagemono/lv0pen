#ifndef LV0_PCI_H
#define LV0_PCI_H

#define PCI_VENDOR_ID           0x00
#define PCI_DEVICE_ID           0x02
#define PCI_COMMAND             0x04
#define PCI_CACHE_LINE_SIZE     0x0C
#define PCI_BASE_ADDRESS_0      0x10
#define PCI_BASE_ADDRESS_1      0x14
#define PCI_BASE_ADDRESS_5      0x24

#include "lv0.h"

struct pci_devparam {
    unsigned long cfg_base;
    unsigned char bus;
    unsigned char dev;
    unsigned char fn;
    unsigned char pad;
    unsigned short vendor;
    unsigned short device;
};

extern struct pci_devparam g_pci_devparams[32];

#ifndef PCI_MINI_DRIVER_UNIT
#define PCI_MINI_DRIVER_UNIT
#endif

#ifdef __cplusplus
class bus_driver {
public:
    int get_devparam(unsigned short vendor, unsigned short device, int nth, pci_devparam **out);

    pci_devparam *devparams[32];
    int ndev;
};

class pci_mini_driver : public bus_driver {
public:
    pci_mini_driver();
    void delay_ms(unsigned long ms) PCI_MINI_DRIVER_UNIT;
    void init_controller();
    int scan_bus(int window) PCI_MINI_DRIVER_UNIT;
    int initialize();
    int set_bus_power(long unused);
    static long get_sc_version(unsigned short *version);
};
#endif

struct pci_mini_driver *get_pci_mini_driver(void);

unsigned long pci_make_config_address(pci_devparam *rec, long reg);
void pci_config_write16(pci_devparam *rec, long reg, short val);
unsigned short pci_config_read16(pci_devparam *rec, long reg);
unsigned long pci_config_read32(pci_devparam *rec, long reg);
void pci_config_write32(pci_devparam *rec, long reg, unsigned int val);

#endif
