#include "memory.h"
#include "syscon.h"
#include "log.h"

struct sc_nvs_request {
    unsigned char cmd, region, off, cnt;
    unsigned char data[256];
};

int sc_nv_storage::check_status(unsigned int status)
{
    switch (status) {
    case 0:
        return 0;
    case 1: case 2: case 3: case 4:
        uart_printf("sc_nv_storage error status %d\n", (int)status);
        return -10003;
    case 5:
        uart_printf("sc_nv_storage error status %d\n", 5);
        return -10002;
    default:
        uart_printf("sc_nv_storage error status %d\n", (int)status);
        return -99999;
    }
}

int sc_nv_storage::write(unsigned int region, unsigned int off, unsigned int size, const void *data)
{
    unsigned char stat[4];
    struct sc_nvs_request req;
    unsigned int cnt;
    int ret = -10008;
    if (off == 0 && size == 256) {
        cnt = 0;
    } else {
        if (off + size > 256)
            return ret;
        cnt = size;
    }
    if (region != 255 && region > 3)
        return ret;
    lv0_memset(&req, 0, sizeof req);
    req.cmd = 0x10;
    req.region = region;
    req.off = off;
    req.cnt = cnt;
    lv0_memmove((char *)req.data, (const char *)data, size);
    syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_NV_STORAGE, &req, size + 4, &req, sizeof req, (int *)stat, 0);
    if (rc != 0)
        return rc;
    return check_status(req.cmd);
}

int sc_nv_storage::read(unsigned int region, unsigned int off, unsigned int size, void *dst, unsigned int *outlen)
{
    unsigned char stat[4];
    struct sc_nvs_request req;
    unsigned int cnt;
    int ret = -10008;
    if (off == 0 && size == 256) {
        cnt = 0;
    } else {
        if (off + size > 256)
            return ret;
        cnt = size;
    }
    if (region != 255 && region > 3)
        return ret;
    lv0_memset(&req, 0, sizeof req);
    req.cmd = 0x20;
    req.region = region;
    req.off = off;
    req.cnt = cnt;
    syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_NV_STORAGE, &req, 4, &req, sizeof req, (int *)stat, 0);
    if (rc != 0)
        return rc;
    unsigned int resp_len = *(unsigned int *)stat;
    if (resp_len <= 3)
        return -99999;
    unsigned int avail = resp_len - 4;
    unsigned int n = avail;
    if (avail > size)
        n = size;
    lv0_memmove((char *)dst, (const char *)req.data, n);
    *outlen = n;
    return check_status(req.cmd);
}
