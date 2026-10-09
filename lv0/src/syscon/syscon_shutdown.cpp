#include "syscon.h"

long syscon_shutdown(void)
{
    unsigned char req = 0;
    int stat;

    struct syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_POWER, &req, 1, 0, 0, &stat, 0);
    for (;;)
        ;
}
