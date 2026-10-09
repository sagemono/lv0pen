#include "syscon.h"

int get_sc_version(unsigned char arg, unsigned short *out)
{
    unsigned char buf[16];
    buf[0] = 1;
    buf[1] = arg;
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_VERSION, &buf[0], 2, &buf[2], 4, (int *)&buf[8], 0);
    *out = (buf[4] << 8) | buf[5];
    if (rc != 0)
        return rc;
    unsigned int status = buf[2];
    switch (status) {
    case 0: return rc;
    case 1: return -10002;
    }
    return -99999;
}
