#include "memory.h"
#include "syscon.h"
extern const unsigned char sc_console_request_template[257] = { 0 };

long sc_write_cmd32_chunked(const unsigned char *data, unsigned int size)
{
    unsigned char resp[4];
    unsigned char status[4];
    unsigned char req[257];
    unsigned int rem = size;
    unsigned int off = 0;
    long rc;
    lv0_memmove((char *)req, (const char *)sc_console_request_template, 257);
    do {
        unsigned int chunk = rem > 256 ? 256 : rem;
        lv0_memmove((char *)(&req[1]), (const char *)(data + off), chunk);
        struct syscon *dev = get_syscon_device();
        rc = dev->send_and_receive_safe(SC_CMD_CONSOLE, req, 257, resp, 2, (int *)status, 0);
        off += chunk;
        rem -= chunk;
        if (rc != 0)
            return -20;
    } while (rem != 0);
    return 0;
}
