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

long syscon_get_core_clock_multiplier(s64 *out)
{
    unsigned char req[2];
    int resp_len;
    unsigned char resp[16];
    req[0] = 3;
    req[1] = 0;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_CONFIG, req, 2, resp, 16, &resp_len, 0);
    switch (resp[4]) {
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
    SETCFG_STATUS(rc, resp);
}

long syscon_get_clock_value(unsigned int *out, unsigned char req0, unsigned char req1, bool scale)
{
    unsigned char req[2];
    int resp_len;
    unsigned char resp[16];
    req[0] = req0;
    req[1] = req1;
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

long syscon_get_xdr_clock(unsigned int *out, bool scale)
{
    return syscon_get_clock_value(out, 0, 0x10, scale);
}
