#include "syscon.h"
#include "util.h"

u32 sc_nv_storage_read_request_template[65] = { 0x20000000 };
u32 sc_nv_storage_write_request_template[65] = { 0x10000000 };

long sc_nv_storage::check_status(unsigned char status)
{
    switch (status) {
    case 0:
        return 0;
    case 1: case 2: case 3: case 4:
        uart_printf("sc_nv_storage error status %d\n", status);
        return -10003;
    case 5:
        uart_printf("sc_nv_storage error status %d\n", 5);
        return -10002;
    default:
        uart_printf("sc_nv_storage error status %d\n", status);
        return -99999;
    }
}

long sc_nv_storage::write(unsigned int region, unsigned int off, unsigned int size, const void *data)
{
    unsigned int stat;
    unsigned char buf[268];
    unsigned int cnt = size;
    if (off == 0 && size == 256)
        cnt = 0;
    if (off + cnt > 256)
        return -10008;
    if (region > 3)
        return -10008;
    memcpy(buf, sc_nv_storage_write_request_template, 260);
    buf[1] = region;
    buf[2] = off;
    buf[3] = cnt;
    memcpy(buf + 4, data, size);
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_NV_STORAGE, buf, size + 4, buf, 260, (int *)&stat, 0);
    if (rc != 0)
        return rc;
    return check_status(buf[0]);
}

long sc_nv_storage::read(unsigned int region, unsigned int off, unsigned int size, void *dst, unsigned int *outlen)
{
    unsigned int stat;
    unsigned char buf[268];
    unsigned int cnt = size;
    if (off == 0 && size == 256)
        cnt = 0;
    if (off + cnt > 256)
        return -10008;
    if (region > 3)
        return -10008;
    memcpy(buf, sc_nv_storage_read_request_template, 260);
    buf[1] = region;
    buf[2] = off;
    buf[3] = cnt;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_NV_STORAGE, buf, 4, buf, 260, (int *)&stat, 0);
    if (rc != 0)
        return rc;
    unsigned int resp_len = stat;
    if (resp_len <= 3)
        return -99999;
    unsigned int avail = resp_len - 4;
    unsigned int n = avail;
    if (avail > size)
        n = size;
    memcpy(dst, buf + 4, n);
    *outlen = n;
    return check_status(buf[0]);
}
