#include "syscon.h"
#include "loader.h"

long set_livelock_detection_mode(int mode)
{
    unsigned char resp[1];
    unsigned char req[2];
    int resp_len;
    req[0] = 16;
    req[1] = mode;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(27, req, 2, resp, 1, &resp_len, 0);
    if (rc != 0)
        return rc;
    switch (resp[0]) {
    case 0: return rc;
    case 1: return -10008;
    }
    return -99999;
}
