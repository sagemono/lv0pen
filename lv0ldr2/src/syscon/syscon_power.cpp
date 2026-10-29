#include "syscon.h"
#include "util.h"

int g_sc_wake_info_format = 1;

long syscon_get_wake_info(unsigned char *out1, unsigned char *out2, unsigned char *out3, unsigned char *out4, unsigned char *out5, unsigned int *out6)
{
    unsigned char req[1];
    unsigned int fmt;
    unsigned char resp[16];
    req[0] = 16;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_POWER, req, 1, resp, 16, (int *)&fmt, 0);
    if (rc != 0)
        return rc;
    unsigned char status = resp[0];
    if (status != 0)
        return -99999;
    if (fmt == 8) {
        g_sc_wake_info_format = 0;
        *out1 = resp[1];
        *out2 = resp[2];
        *out3 = status;
        *out4 = status;
        *out5 = resp[3];
        *out6 = *(unsigned int *)&resp[4];
        return 0;
    }
    if (fmt == 16) {
        g_sc_wake_info_format = 1;
        *out1 = resp[8];
        *out2 = resp[9];
        *out3 = resp[10];
        *out4 = resp[11];
        *out5 = resp[12];
        *out6 = *(unsigned int *)&resp[4];
        return 0;
    }
    g_sc_wake_info_format = -1;
    return -99999;
}

void syscon_power_off_with_code(int p1, int p2, int p3)
{
    unsigned char c1, c2, c3, c4, c5;
    unsigned char req2[2];
    unsigned char req4[4];
    unsigned int w;
    if (g_sc_wake_info_format == -1) {
        long rc = syscon_get_wake_info(&c1, &c2, &c3, &c4, &c5, &w);
        if (rc != 0)
            return;
    }
    switch (g_sc_wake_info_format) {
    case 0:
        req2[0] = 0x11;
        req2[1] = p1;
        get_syscon_device()->send_and_receive(SC_CMD_POWER, req2, 2, 0, 0, (int *)&w, 0);
        break;
    case 1:
        req4[0] = 0x11;
        req4[1] = p1;
        req4[2] = p2;
        req4[3] = p3;
        get_syscon_device()->send_and_receive(SC_CMD_POWER, req4, 4, 0, 0, (int *)&w, 0);
        break;
    }
    for (;;)
        ;
}

long syscon_power_query_33(unsigned int *out)
{
    unsigned char req[1];
    int resp_len;
    unsigned char resp[8];
    req[0] = 0x33;
    syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_POWER, req, 1, resp, 8, &resp_len, 0);
    switch (resp[0]) {
    case 0:
        *out = *(unsigned int *)&resp[4];
        return 0;
    case 1:
        return -10003;
    case 254:
        return -10002;
    default:
        return -99999;
    }
}

long syscon_set_wake_source(unsigned int source)
{
    unsigned char resp[4];
    unsigned char status[4];
    unsigned char req[8];
    *(unsigned long long *)&req[0] = 0;
    req[0] = 0x12;
    *(unsigned int *)&req[4] = source;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_POWER, req, 8, resp, 1, (int *)status, 0);
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
