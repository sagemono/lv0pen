#include "auth.h"
#include "loader.h"

long loader_base::check_config_ring(void)
{
    config_ring ring;

    return ring.verify() != 0 ? -5 : 0;
}

authenticator g_auth;
