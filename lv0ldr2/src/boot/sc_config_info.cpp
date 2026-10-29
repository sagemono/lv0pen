#include "syscon.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "loader.h"
#include "xdr.h"

unsigned int get_first_spu_vr(void);
bool is_spu_enabled(u64 spu);

void sc_config_info::xdr::initialize(void)
{
}

long sc_config_info::xdr::is_waking_from_suspend_to_ram_mode(unsigned char *str)
{
    unsigned char req_os, cur_os, req_gr, cur_gr, last_shutdown;
    unsigned int wake_source;

    *str = false;
    if (syscon_get_wake_info(&req_os, &cur_os, &req_gr, &cur_gr, &last_shutdown, &wake_source)) {
        log_message("[INFO]: query_system_power_up_cause failed.\n");
        return -1;
    }
    log_info("[INFO]: query_system_power_up_cause returns successfully.\n");
    log_info("[INFO]: requested_os_context: 0x%02x\n", req_os);
    log_info("[INFO]: current_os_context  : 0x%02x\n", cur_os);
    log_info("[INFO]: requested_gr_context: 0x%02x\n", req_gr);
    log_info("[INFO]: current_gr_context  : 0x%02x\n", cur_gr);
    log_info("[INFO]: last_shutdown_cause : 0x%02x\n", last_shutdown);
    log_info("[INFO]: wake_source         : 0x%08x\n", wake_source);
    if (req_os == 1 && cur_os == 1)
        *str = true;
    else
        *str = false;
    return 0;
}

long sc_config_info::xdr::identify_cbe_version_from_spu_vr(unsigned int *kind,
                                                          unsigned int *level)
{
    int vr = get_first_spu_vr();

    switch (vr) {
    case 0x100:
        *kind = 7;
        *level = 2;
        break;
    case 0x200:
    case 0x201:
    case 0x202:
        *kind = 7;
        *level = 3;
        break;
    case 0x1000:
    case 0x1001:
        *kind = 11;
        *level = 1;
        break;
    case 0x2000:
        *kind = 12;
        *level = 1;
        break;
    case 0x2100:
        *kind = 12;
        *level = 2;
        break;
    default:
        return -1;
    }
    return 0;
}

long sc_config_info::xdr::query_config(xdr_config *cfg)
{
    long rc;

    rc = syscon_read_xdr_config(cfg);
    if (rc) {
        log_message("[INFO]: (0x%08x) sc_config_info::xdr::query_config failed.\n", rc);
        return -1;
    }
    rc = is_waking_from_suspend_to_ram_mode(&cfg->str);
    if (rc) {
        log_message("[INFO]: (0x%08x) is_waking_from_suspend_to_ram_mode() failed\n", rc);
        return -1;
    }
    log_info("[INFO]: b_str: bool(%d)\n", cfg->str);
    cfg->f_84 = 0x10000;
    rc = identify_cbe_version_from_spu_vr(&cfg->cbe_kind, &cfg->f_8c);
    if (rc) {
        log_message("[INFO]: (0x%08x) identify_cbe_version_from_spu_vr() failed\n", rc);
        return -1;
    }
    log_info("[INFO]: CBE process technology id: 0x%08x\n", cfg->cbe_kind);
    log_info("[INFO]: CBE design level: 0x%08x\n", cfg->f_8c);
    return 0;
}
