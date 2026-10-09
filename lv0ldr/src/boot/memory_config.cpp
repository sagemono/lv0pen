#include "syscon.h"
#include "log.h"
#include "loader.h"
#include "xdr.h"

long ata_activation(u64 addr, u64 size)
{
    return 0;
}

void memory_config_query::initialize(void)
{
}

long memory_config_query::is_str(bool *str)
{
    unsigned char requested_os, current_os, requested_gr, current_gr, last_shutdown;
    unsigned int wake_source;
    long rc = syscon_get_wake_info(&requested_os, &current_os, &requested_gr, &current_gr,
                                   &last_shutdown, &wake_source);
    if (rc) {
        log_message("[INFO]: query_system_power_up_cause failed.\n");
        *str = false;
        return -1;
    }
    log_info("[INFO]: query_system_power_up_cause returns successfully.\n");
    log_info("[INFO]: requested_os_context: 0x%02x\n", requested_os);
    log_info("[INFO]: current_os_context  : 0x%02x\n", current_os);
    log_info("[INFO]: requested_gr_context: 0x%02x\n", requested_gr);
    log_info("[INFO]: current_gr_context  : 0x%02x\n", current_gr);
    log_info("[INFO]: last_shutdown_cause : 0x%02x\n", last_shutdown);
    log_info("[INFO]: wake_source         : 0x%08x\n", wake_source);
    if (requested_os == 1 && current_os == 1)
        *str = true;
    else
        *str = false;
    return 0;
}

long memory_config_query::query_config(memory_config *cfg)
{
    long rc = syscon_read_xdr_config(cfg);
    log_info("[INFO]: xdr::query_config (basic) returns 0x%08lx\n", rc);
    if (rc) {
        log_message("[INFO]: (0x%08x) sc_config_info::xdr::query_config failed.\n", rc);
        return -1;
    }
    rc = is_str(&cfg->str);
    if (rc) {
        log_message("[INFO]: (0x%08x) is_str() failed\n", rc);
        return -1;
    }
    cfg->f_84 = 0x10000;
    rc = syscon_get_xdr_clock(&cfg->xio_ref_clk, false);
    if (rc) {
        log_message("[INFO]: (0x%08x) sc_config_info::xdr::get_reference_clock() failed\n", rc);
        return -1;
    }
    rc = syscon_get_ref_clock(&cfg->be_ref_clk, false);
    if (rc) {
        log_message("[INFO]: (0x%08x) sc_config_info::be::get_reference_clock() failed\n", rc);
        return -1;
    }
    rc = syscon_get_core_clock_multiplier(&cfg->be_pll_multiplier);
    if (rc) {
        log_message("[INFO]: (0x%08x) sc_config_info::be::get_be_pll_multiply() failed\n", rc);
        return -1;
    }
    log_info("[INFO]: b_str: bool(%d)\n", cfg->str);
    log_info("[INFO]: xio_ref_clk: %d MHz\n", cfg->xio_ref_clk / 1000000);
    log_info("[INFO]: be_ref_clk: %d MHz\n", cfg->be_ref_clk / 1000000);
    log_info("[INFO]: be_pll_multiplier: %lld\n", cfg->be_pll_multiplier);
    log_debug("[INFO]: dump basic_config byte stream: size %d\n", 128);
    unsigned char *b = cfg->basic;
    for (int i = 0; i < 128; i++) {
        log_debug("%02x:", *b++);
        if ((i & 15) == 15)
            log_debug("\n");
    }
    log_debug("[INFO]: ------------------------------- dump end\n");
    return 0;
}
