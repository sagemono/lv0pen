#include "config.h"
#include "platform.h"
#include "console.h"

int g_debug_interface = -1;

long get_debug_interface(void)
{
    return g_debug_interface;
}

void set_debug_interface(int iface)
{
    g_debug_interface = iface;
}

bool is_host_file_io_unavailable(void)
{
    switch (g_debug_interface + 1) {
    case 0: case 4: case 5: case 6: case 7:
        return true;
    }
    return false;
}
