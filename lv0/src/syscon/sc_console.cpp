#include "syscon.h"
#include "memory.h"

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
    struct syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_CONSOLE, buf, len, buf, 257, (int *)stat, 0);
    return 0;
}

long syscon_write_buffer(const unsigned char *data, unsigned long size)
{
    unsigned char stat[16];
    unsigned int rem = size;
    unsigned int off = 0;
    int rc;
    g_sc_console_buffer[0] = 0;
    do {
        unsigned int chunk = rem > 256 ? 256 : rem;
        lv0_memmove((char *)&g_sc_console_buffer[1], (const char *)(data + off), chunk);
        struct syscon *dev = get_syscon_device();
        rc = dev->send_and_receive(SC_CMD_CONSOLE, g_sc_console_buffer, chunk + 1, &stat[0], 2, (int *)&stat[4], 0);
        off += chunk;
        rem -= chunk;
    } while (rc == 0 && rem != 0);
    return rc;
}
