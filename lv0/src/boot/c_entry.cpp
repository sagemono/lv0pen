#include "static_init.h"
#include "platform.h"

application *g_application_ptr;

long c_entry()
{
    run_global_constructors();
    return g_application_ptr->run();
}
