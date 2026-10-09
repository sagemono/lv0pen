#include "syscon.h"

int syscon_get_core_clock_multiplier(long *out)
{
    unsigned char buf[24];
    buf[0] = 3;
    buf[1] = 0;
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_CONFIG, &buf[0], 2, &buf[8], 16, (int *)&buf[4], 0);
    switch (buf[12]) {
    case 0:  *out = 2;  break;
    case 1:  *out = 4;  break;
    case 2:  *out = 6;  break;
    case 3:  *out = 8;  break;
    case 4:  *out = 10; break;
    case 5:  *out = 12; break;
    case 6:  *out = 16; break;
    case 7:  *out = 20; break;
    default: *out = 0;  break;
    }
    if (rc != 0)
        return rc;
    unsigned int status = buf[9];
    switch (status) {
    case 0: return rc;
    case 1: return -10001;
    case 2: return -10002;
    case 254: return -10003;
    }
    return -99999;
}
