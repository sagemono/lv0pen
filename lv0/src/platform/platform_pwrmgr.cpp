#include "cxx.h"
#include "log.h"
#include "syscon.h"
#include "platform.h"

class platform_pwrmgr {
public:
    platform_pwrmgr() CXX_DROPPED { g_platform_pwrmgr_ptr = this; }
    virtual int shutdown();
    virtual int reboot();
};

int platform_pwrmgr::shutdown()
{
    log_message("platform_pwrmgr::shutdown\n");
    long rc = syscon_shutdown();
    log_message("fail platform_pwrmgr::shutdown\n");
    return rc;
}

int platform_pwrmgr::reboot()
{
    log_message("platform_pwrmgr::reboot\n");
    long rc = syscon_reboot();
    log_message("fail platform_pwrmgr::reboot\n");
    return rc;
}

platform_pwrmgr g_platform_pwrmgr;
