#include "component.h"
#include "config.h"

bool g_boot_order_checked;

boot_order_component_loader::boot_order_component_loader(component_loader *primary, component_loader *secondary)
    : primary(primary), secondary(secondary)
{
}

boot_order_component_loader::~boot_order_component_loader()
{
}

void boot_order_component_loader::apply_boot_order()
{
    if (!g_boot_order_checked) {
        if ((unsigned char)get_eeprom_os_boot_order_flag()) {
            component_loader *tmp = primary;
            primary = secondary;
            secondary = tmp;
        }
        g_boot_order_checked = 1;
    }
}

int boot_order_component_loader::load_component_to(unsigned int id, const char *name, void *dst, unsigned long size)
{
    apply_boot_order();
    int result = primary->load_component_to(id, name, dst, size);
    if (result)
        return secondary->load_component_to(id, name, dst, size);
    return result;
}

int boot_order_component_loader::load_component(unsigned int id, const char *name, memory_budget *budget,
                                                unsigned int region, unsigned long max_size, unsigned long align,
                                                unsigned long *out_addr, unsigned long *out_size)
{
    apply_boot_order();
    int result = primary->load_component(id, name, budget, region, max_size, align, out_addr, out_size);
    if (result)
        return secondary->load_component(id, name, budget, region, max_size, align, out_addr, out_size);
    return result;
}
