#include "syscon.h"
unsigned long g_unused_41978 = 0x2000000000000000;
unsigned int sc_set_wake_source_request_header = 0x12000000;

int syscon_set_wake_source(int arg)
{
    unsigned char resp[4];
    int status;
    struct { unsigned char sub; unsigned char pad[3]; int arg; } req = { 0x12, { 0 }, arg };
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_POWER, &req, 8, resp, 1, &status, 0);
    if (rc != 0)
        return rc;
    switch (resp[0]) {
    case 0:
        return 0;
    case 1:
        return -10007;
    case 254:
        return -10002;
    default:
        return -99999;
    }
}
