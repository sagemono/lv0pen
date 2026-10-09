#include "svc.h"

int svc_entry0::unsupported(int arg)
{
    return -16;
}

svc_entry0 g_svc_entry0_target;
svc_entry0 *g_svc_entry0_target_ptr = &g_svc_entry0_target;
