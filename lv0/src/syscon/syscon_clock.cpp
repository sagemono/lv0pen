#include "syscon.h"

int syscon_get_clock_value(unsigned char *out, unsigned char req0, unsigned char req1, int scale)
{
    unsigned char req[2] = {req0, req1};
    int resp_len;
    unsigned char buf[16];
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_CONFIG, req, 2, buf, 16, &resp_len, 0);
    out[0] = buf[4];
    out[1] = buf[5];
    out[2] = buf[6];
    out[3] = buf[7];
    if (scale) {
        unsigned int val = *(unsigned int *)out;
        *(unsigned int *)out = val - val / 400;
    }
    if (rc != 0)
        return rc;
    unsigned int status = buf[1];
    switch (status) {
    case 0: return rc;
    case 1: return -10001;
    case 2: return -10002;
    case 254: return -10003;
    }
    return -99999;
}

int syscon_get_ref_clock(unsigned int *out, int scale)
{
    return syscon_get_clock_value((unsigned char *)out, 3, 0x10, scale);
}
