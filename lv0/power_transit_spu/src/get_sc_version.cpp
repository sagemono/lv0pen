#include "syscon.h"

long get_sc_version(unsigned char arg, unsigned short *out)
{
    static const unsigned char request[2] = { 1, 3 };
    unsigned char req[2];
    unsigned char resp[4];
    int resp_len;
    req[0] = request[0];
    req[1] = arg;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_VERSION, req, 2, resp, 4, &resp_len, 0);
    *out = (resp[2] << 8) | resp[3];
    if (rc != 0)
        return rc;
    unsigned int status = resp[0];
    switch (status) {
    case 0: return rc;
    case 1: return -10002;
    }
    return -99999;
}
