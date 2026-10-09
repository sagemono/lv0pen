#include "syscon.h"

int g_sc_wake_info_format = 1;

#define V8(i)  (((volatile unsigned char *)buf)[i])
#define V32(i) (*(volatile unsigned int *)&buf[i])

int syscon_get_wake_info(unsigned char *out1, unsigned char *out2, unsigned char *out3, unsigned char *out4, unsigned char *out5, unsigned int *out6) {
    char buf[32];
    buf[0] = 0x10;
    struct syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_POWER, &buf[0], 1, &buf[8], 16, (int *)&buf[4], 0);
    if (rc != 0) return rc;
    unsigned char status = buf[8];
    if (status != 0) return -99999;
    unsigned int fmt = *(unsigned int *)&buf[4];
    volatile int *format;
    if (fmt == 8) {
        unsigned char c10 = V8(10), c11 = V8(11);
        unsigned int w = V32(12);
        format = &g_sc_wake_info_format;
        *format = 0;
        *out1 = V8(9);  *out2 = c10; *out3 = status; *out4 = status;
        *out5 = c11; *out6 = w;
        return 0;
    }
    if (fmt == 16) {
        unsigned char c17 = V8(17);
        unsigned char c18 = V8(18);
        unsigned char c19 = V8(19);
        unsigned char c20 = V8(20);
        unsigned int w = V32(12);
        format = &g_sc_wake_info_format;
        *format = 1;
        *out1 = V8(16); *out2 = c17; *out3 = c18; *out4 = c19;
        *out5 = c20; *out6 = w;
        return 0;
    }
    format = &g_sc_wake_info_format;
    *format = -1;
    return -99999;
}

void syscon_power_off_with_code(int p1, int p2, int p3) {
    unsigned char buf[16];
    if (g_sc_wake_info_format == -1) {
        int rc = syscon_get_wake_info(&buf[0], &buf[4], &buf[3], &buf[2], &buf[1], (unsigned int *)&buf[12]);
        if (rc != 0) return;
    }
    switch (g_sc_wake_info_format) {
    case 0:
        buf[5] = 0x11;
        buf[6] = p1;
        get_syscon_device()->send_and_receive(SC_CMD_POWER, &buf[5], 2, 0, 0, (int *)&buf[12], 0);
        break;
    case 1:
        buf[7] = 0x11;
        buf[8] = p1;
        buf[9] = p2;
        buf[10] = p3;
        get_syscon_device()->send_and_receive(SC_CMD_POWER, &buf[7], 4, 0, 0, (int *)&buf[12], 0);
        break;
    }
    for (;;) {
    }
}
