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

long syscon_set_console_mode(unsigned char mode)
{
    unsigned char stat[16];
    g_sc_console_buffer[0] = 1;
    g_sc_console_buffer[1] = mode;
    syscon *dev = get_syscon_device();
    dev->send_and_receive(SC_CMD_CONSOLE, g_sc_console_buffer, 2, g_sc_console_buffer, 257, (int *)stat, 0);
    return 0;
}

long syscon_write_buffer(const unsigned char *data, u64 size)
{
    unsigned char resp[2];
    int resp_len;
    unsigned int rem = size;
    unsigned int off = 0;
    long rc;
    g_sc_console_buffer[0] = 0;
    do {
        unsigned int chunk = rem > 256 ? 256 : rem;
        memcpy(&g_sc_console_buffer[1], data + off, chunk);
        syscon *dev = get_syscon_device();
        rc = dev->send_and_receive(SC_CMD_CONSOLE, g_sc_console_buffer, chunk + 1, resp, 2, &resp_len, 0);
        off += chunk;
        rem -= chunk;
        if (rc != 0)
            break;
    } while (rem != 0);
    return rc;
}
