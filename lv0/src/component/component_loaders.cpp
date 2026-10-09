#include "component.h"
#include "config.h"
#include "log.h"

extern component_record g_component_table[9];
extern component_alt_record g_component_alt_table[5];
extern embedded_component g_embedded_components[8];

flash_format_1_component_loader g_flash_format_1_component_loader(0, "ros", "asecure_loader",
                                                                  g_embedded_components, 8, 0x40000);
flash_format_1_component_loader g_flash_format_2_component_loader(0, "ros", "asecure_loader",
                                                                  g_embedded_components, 8, 0);
flash_bank_component_loader g_flash_format_3_component_loader(0, "ros", "asecure_loader",
                                                              g_embedded_components, 8, "ros0", "ros1");
host_file_loader g_host_file_loader;
boot_order_component_loader g_boot_order_component_loader_1(&g_flash_format_1_component_loader, &g_host_file_loader);
boot_order_component_loader g_boot_order_component_loader_2(&g_flash_format_2_component_loader, &g_host_file_loader);
boot_order_component_loader g_boot_order_component_loader_3(&g_flash_format_3_component_loader, &g_host_file_loader);
componnet_manager g_componnet_manager(g_component_table, 9, g_component_alt_table, 5);

component_record g_component_table[9] = {
    { COMPONENT_METLDR_2,    "metldr.2",    0x40000,   0, 0x80, 0 },
    { COMPONENT_LV2LDR,      "lv2ldr",      0x24000,   0, 0x80, 0 },
    { COMPONENT_APPLDR,      "appldr",      0x28700,   0, 0x80, 0 },
    { COMPONENT_ISOLDR,      "isoldr",      0x20000,   0, 0x80, 0 },
    { COMPONENT_PARM_TXT,    "parm.txt",    0x1000,    0, 0x80, 0 },
    { COMPONENT_LV1LDR,      "lv1ldr",      0x2F000,   2, 0x80, 0 },
    { COMPONENT_LV1_SELF,    "lv1.self",    0x2000000, 2, 0x10, 0 },
    { COMPONENT_EID0,        "eid0",        0x1000,    0, 0x80, 0 },
    { COMPONENT_SDK_VERSION, "sdk_version", 0x40,      0, 0x80, 0 },
};

component_alt_record g_component_alt_table[5] = {
    { { COMPONENT_METLDR,   "metldr",   0x40000, 0, 0x80, 0 }, COMPONENT_METLDR_2, 1 },
    { { COMPONENT_LV2LDR_2, "lv2ldr.2", 0x24000, 0, 0x80, 0 }, COMPONENT_LV2LDR,   0 },
    { { COMPONENT_APPLDR_2, "appldr.2", 0x28700, 0, 0x80, 0 }, COMPONENT_APPLDR,   0 },
    { { COMPONENT_ISOLDR_2, "isoldr.2", 0x20000, 0, 0x80, 0 }, COMPONENT_ISOLDR,   0 },
    { { COMPONENT_LV1LDR_2, "lv1ldr.2", 0x2F000, 2, 0x80, 0 }, COMPONENT_LV1LDR,   0 },
};

#define EMBEDDED(name) \
    extern char embedded_##name[], embedded_##name##_end[], embedded_##name##_size[]
EMBEDDED(lv1ldr); EMBEDDED(lv2ldr); EMBEDDED(appldr); EMBEDDED(isoldr);
EMBEDDED(lv1ldr_2); EMBEDDED(lv2ldr_2); EMBEDDED(appldr_2); EMBEDDED(isoldr_2);
#undef EMBEDDED
#define EMBEDDED(id, name) \
    { id, (unsigned long)embedded_##name, (unsigned long)embedded_##name##_end, \
      (unsigned long)embedded_##name##_size }
embedded_component g_embedded_components[8] = {
    EMBEDDED(COMPONENT_LV1LDR, lv1ldr),
    EMBEDDED(COMPONENT_LV2LDR, lv2ldr),
    EMBEDDED(COMPONENT_APPLDR, appldr),
    EMBEDDED(COMPONENT_ISOLDR, isoldr),
    EMBEDDED(COMPONENT_LV1LDR_2, lv1ldr_2),
    EMBEDDED(COMPONENT_LV2LDR_2, lv2ldr_2),
    EMBEDDED(COMPONENT_APPLDR_2, appldr_2),
    EMBEDDED(COMPONENT_ISOLDR_2, isoldr_2),
};
#undef EMBEDDED

componnet_manager *g_componnet_manager_ptr = &g_componnet_manager;
component_loader *g_component_loader_ptr;

void select_component_loader(void)
{
    if (get_flash_format() == 1)
        g_component_loader_ptr = &g_boot_order_component_loader_1;
    else if (get_flash_format() == 2)
        g_component_loader_ptr = &g_boot_order_component_loader_2;
    else if (__builtin_expect(get_flash_format() == 3, 1))
        g_component_loader_ptr = &g_boot_order_component_loader_3;
    else
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x unknown flash format\n", LV0_ERR_INTERNAL);
}
