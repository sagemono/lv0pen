#include "syscon.h"

int syscon_syspm_get_33(unsigned int *out)
{
    unsigned char req = 0x33;
    int status;
    unsigned char resp[8];

    get_syscon_device()->send_and_receive(SC_CMD_POWER, &req, 1, resp, 8, &status, 0);
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
