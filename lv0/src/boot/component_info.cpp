#include "component.h"

component_info_table *get_component_info_table(void)
{
    return (component_info_table *)0x10100;
}

int component_info_table::set(unsigned int id, unsigned long addr, unsigned long size)
{
    switch (id) {
    case COMPONENT_LV1LDR:
        lv1ldr.addr = addr;
        lv1ldr.size = size;
        break;
    case COMPONENT_METLDR:
        metldr.addr = addr;
        metldr.size = size;
        break;
    case COMPONENT_LV2LDR:
        lv2ldr.addr = addr;
        lv2ldr.size = size;
        break;
    case COMPONENT_APPLDR:
        appldr.addr = addr;
        appldr.size = size;
        break;
    case COMPONENT_ISOLDR:
        isoldr.addr = addr;
        isoldr.size = size;
        break;
    case COMPONENT_EID0:
        eid0.addr = addr;
        eid0.size = size;
        break;
    case COMPONENT_LV1LDR_2:
        lv1ldr_2.addr = addr;
        lv1ldr_2.size = size;
        break;
    case COMPONENT_LV2LDR_2:
        lv2ldr_2.addr = addr;
        lv2ldr_2.size = size;
        break;
    case COMPONENT_APPLDR_2:
        appldr_2.addr = addr;
        appldr_2.size = size;
        break;
    default:
        return -55;
    }
    return 0;
}

int component_info_table::clear()
{
    for (unsigned long id = 0; id != 18; id++)
        set(id, 0, 0);
    return 0;
}

int component_info_table::copy_entry(unsigned long *out_addr, unsigned long *out_size, unsigned long addr,
                                     unsigned long size)
{
    if (!addr || !size)
        return -55;
    *out_addr = addr;
    if (out_size)
        *out_size = size;
    return 0;
}

int component_info_table::get(unsigned int id, unsigned long *out_addr, unsigned long *out_size)
{
    switch (id) {
    case COMPONENT_LV1LDR:
        return copy_entry(out_addr, out_size, lv1ldr.addr, lv1ldr.size);
    case COMPONENT_METLDR:
        return copy_entry(out_addr, out_size, metldr.addr, metldr.size);
    case COMPONENT_LV2LDR:
        return copy_entry(out_addr, out_size, lv2ldr.addr, lv2ldr.size);
    case COMPONENT_APPLDR:
        return copy_entry(out_addr, out_size, appldr.addr, appldr.size);
    case COMPONENT_ISOLDR:
        return copy_entry(out_addr, out_size, isoldr.addr, isoldr.size);
    case COMPONENT_EID0:
        return copy_entry(out_addr, out_size, eid0.addr, eid0.size);
    case COMPONENT_LV1LDR_2:
        return copy_entry(out_addr, out_size, lv1ldr_2.addr, lv1ldr_2.size);
    case COMPONENT_LV2LDR_2:
        return copy_entry(out_addr, out_size, lv2ldr_2.addr, lv2ldr_2.size);
    case COMPONENT_APPLDR_2:
        return copy_entry(out_addr, out_size, appldr_2.addr, appldr_2.size);
    }
    return -55;
}
