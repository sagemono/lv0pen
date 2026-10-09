#include "syscon.h"

int syscon_get_hw_config(unsigned long *out, unsigned long *out2)
{
    unsigned char buf[32];
    buf[0] = 0x22;
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_CONFIG, &buf[0], 1, &buf[8], 16, (int *)&buf[4], 0);
    *out = *(unsigned long *)&buf[16];
    if (out2)
        *out2 = buf[12];
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
