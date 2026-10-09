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

long syscon_read_xdr_config(void *out)
{
    unsigned char req[2];
    int resp_len;
    unsigned char resp[132];
    req[0] = 0;
    req[1] = 0;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_CONFIG, req, 2, resp, 132, &resp_len, 0);
    memcpy(out, resp + 4, 128);
    SETCFG_STATUS(rc, resp);
}

long syscon_set_xdr_rail(unsigned char a, unsigned char b)
{
    unsigned char req[4];
    unsigned char resp[4];
    int resp_len;
    memset(req, 0, 4);
    req[1] = 2;
    req[2] = a;
    req[3] = b;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_CONFIG, req, 4, resp, 4, &resp_len, 0);
    SETCFG_STATUS(rc, resp);
}

long syscon_write_xdr_config(unsigned int a, unsigned int b)
{
    unsigned char req[3];
    unsigned char resp[4];
    int resp_len;
    memset(req, 0, 3);
    req[1] = 1;
    req[2] = a | b;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_CONFIG, req, 3, resp, 4, &resp_len, 0);
    SETCFG_STATUS(rc, resp);
}

long syscon_read_config_2(unsigned int *out)
{
    unsigned char req[2];
    int resp_len;
    unsigned char resp[16];
    req[0] = 2;
    req[1] = 0;
    syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_CONFIG, req, 2, resp, 16, &resp_len, 0);
    *out = resp[4];
    switch ((unsigned int)resp[1]) {
    case 0: return 0;
    case 1: return -10001;
    case 2: return -10002;
    case 254: return -10003;
    }
    return -99999;
}
