#include "syscon.h"

int syscon_get_platform_id(unsigned char *out)
{
    unsigned char buf[24];
    buf[0] = 0x20;
    buf[1] = 0x10;
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_CONFIG, &buf[0], 2, &buf[8], 16, (int *)&buf[4], 0);
    out[0] = buf[12];
    out[1] = buf[13];
    out[2] = buf[14];
    out[3] = buf[15];
    out[4] = buf[16];
    out[5] = buf[17];
    out[6] = buf[18];
    out[7] = buf[19];
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
