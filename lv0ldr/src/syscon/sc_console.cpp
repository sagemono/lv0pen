#include "syscon.h"
#include "util.h"

unsigned char g_sc_console_buffer[257];

long syscon_write_string(const char *str)
{
    unsigned char stat[16];
    unsigned char *buf = g_sc_console_buffer;
    unsigned char *dst = buf + 1;
    unsigned int len = 1;
    unsigned int left = 256;
    unsigned char ch;
    buf[0] = 0;
    while ((ch = *str++) != 0) {
        *dst++ = ch;
        len++;
        if (--left == 0)
            break;
    }
    syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_CONSOLE, buf, len, buf, 257, (int *)stat, 0);
    return 0;
}
