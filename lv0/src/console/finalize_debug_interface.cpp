#include "console.h"
#include "gbe.h"
#include "platform.h"

void finalize_debug_interface(char *iface)
{
    cp_link *link = ((physical_console *)iface)->link;
    if (link)
        link->disconnect();
    get_gbe_work()->finalize();
}
