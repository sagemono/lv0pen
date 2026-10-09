#include "clock.h"
#include "gbe.h"
#include "pci.h"
#include "config.h"
#include "platform.h"
#include "log.h"
#include "console.h"
#include "sb.h"

bool link_to_cp_ch4(cp_link **link_out, pci_devparam *devparam, cp_bridge *bridge)
{
    cp_ch4_channel *ch = get_cp_ch4_channel();

    *link_out = ch;
    if (ch && !ch->initialize(devparam, bridge, 4)) {
        unsigned long deadline;

        ch->setup();
        deadline = get_time_ms() + 5000;
        while (get_time_ms() < deadline) {
            cp_link *link = *link_out;
            if (!link->connect()) {
                g_physical_console_frame_size = 0x8000;
                return 1;
            }
        }
    }
    return 0;
}

bool link_to_cp_ch0(cp_link **link_out, pci_devparam *devparam, cp_bridge *bridge)
{
    cp_channel *ch = get_cp_channel();

    *link_out = ch;
    if (ch && !ch->initialize(devparam, bridge, 0)) {
        unsigned long deadline;

        ch->setup();
        deadline = get_time_ms() + 5000;
        while (get_time_ms() < deadline) {
            cp_link *link = *link_out;
            if (!link->connect()) {
                g_physical_console_frame_size = 0x8000;
                return 1;
            }
        }
    }
    return 0;
}

int init_debug_interface(physical_console *console)
{
    pci_devparam *devparam;
    union {
        unsigned int w[4];
        unsigned char b[16];
    } cp_cfg;
    cp_bridge *bridge;
    int mode;
    int rc;

    console->link = 0;
    mode = get_eeprom_select_net_device();
    switch (mode) {
    case 5:
        goto record;
    case 6:
        set_debug_interface(6);
        return 0;
    case 3:
record:
        console->link = 0;
        set_debug_interface(mode);
        return 0;
    }

    bridge = get_cp_bridge();
    devparam = 0;
    if ((mmio_sb_product_code & 0x7F000000) != 0x4000000)
        goto init_pci;
    rc = -13;
probe:
    if (!bridge && rc) {
        log_message("[ERROR]: 0x%08x Debug interface CP initialize fail\n", LV0_ERR_DEBUG_INTERFACE);
        return -13;
    }

    switch (mode) {
    case 2:
        if (get_cp_channel()->initialize(devparam, bridge, 0))
            goto cp_fail;
        get_cp_channel()->read_config(cp_cfg.w);
        g_sys_hw_model_emulate = cp_cfg.b[9];
        if (cp_cfg.w[0] || cp_cfg.w[1] > 0x102FF) {
            if (cp_cfg.w[2] & 0x8000)
                set_release_mode(1);
            else
                set_release_mode(0);
        }
        if (cp_cfg.b[8] == 2) {
            if (link_to_cp_ch4(&console->link, devparam, bridge)) {
                set_debug_interface(4);
                return 0;
            }
        } else {
            if (link_to_cp_ch0(&console->link, devparam, bridge)) {
                set_debug_interface(2);
                return 0;
            }
        }
cp_fail:
        log_message("[ERROR]: 0x%08x Debug interface CP initialize fail\n", LV0_ERR_DEBUG_INTERFACE);
        break;
    case 4:
        if (link_to_cp_ch4(&console->link, devparam, bridge)) {
            set_debug_interface(4);
            return 0;
        }
        log_message("[ERROR]: 0x%08x Debug interface CP Ch4 initialize fail\n", LV0_ERR_DEBUG_INTERFACE);
        break;
    default:
        log_message("[ERROR]: 0x%08x Debug interface Configuration fail\n", LV0_ERR_DEBUG_INTERFACE);
        break;
    }
    return -13;

init_pci:
    {
        pci_mini_driver *pci = get_pci_mini_driver();
        pci->initialize();
        get_gbe_work()->initialize(0x100000);
        rc = pci->get_devparam(0x104D, 0x81FF, 0, &devparam);
    }
    goto probe;
}
