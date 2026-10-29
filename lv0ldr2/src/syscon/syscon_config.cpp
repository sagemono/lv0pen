#include "syscon.h"
#include "util.h"

#define SETCFG_STATUS(rc, resp)                         \
    if (rc != 0)                                        \
        return rc;                                      \
    switch ((unsigned int)(resp)[1]) {                  \
    case 0: return rc;                                  \
    case 1: return -10001;                              \
    case 2: return -10002;                              \
    case 254: return -10003;                            \
    }                                                   \
    return -99999

long syscon_get_clock_value(unsigned int *out, unsigned char req0, unsigned char req1, bool scale)
{
    unsigned char req[2] = { req0, req1 };
    int resp_len;
    unsigned char resp[16];
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_CONFIG, req, 2, resp, 16, &resp_len, 0);
    memcpy(out, resp + 4, 4);
    if (scale) {
        unsigned int val = *out;
        *out = val - val / 400;
    }
    SETCFG_STATUS(rc, resp);
}

long syscon_get_ref_clock(unsigned int *out, bool scale)
{
    return syscon_get_clock_value(out, 3, 0x10, scale);
}
