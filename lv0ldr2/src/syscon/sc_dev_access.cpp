#include "syscon.h"
#include "util.h"

struct sc_dev_access_hdr {
    unsigned char flag;
    unsigned char pad[3];
    unsigned int addr;
    unsigned int size;
};

struct sc_dev_access_write_msg {
    sc_dev_access_hdr hdr;
    unsigned char data[256];
};

long sc_dev_access::write(unsigned int addr, const void *data, unsigned int size)
{
    unsigned char stat[4];
    sc_dev_access_write_msg msg;
    if (size > 256)
        return -7;
    msg.hdr.flag = 1;
    msg.hdr.pad[0] = msg.hdr.pad[1] = msg.hdr.pad[2] = 0;
    msg.hdr.addr = addr;
    msg.hdr.size = size;
    memcpy(msg.data, data, size);
    syscon *dev = get_syscon_device();
    return dev->send_and_receive(SC_CMD_DEV_ACCESS, &msg, size + 12, &msg, 268, (int *)stat, 0);
}

long sc_dev_access::read(unsigned int addr, void *dst, unsigned int size)
{
    unsigned char stat[4];
    sc_dev_access_hdr hdr;
    unsigned char reply[260];
    if (size > 256)
        return -7;
    hdr.flag = 0;
    hdr.pad[0] = hdr.pad[1] = hdr.pad[2] = 0;
    hdr.addr = addr;
    hdr.size = size;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_DEV_ACCESS, &hdr, 12, reply, 260, (int *)stat, 0);
    if (rc != 0)
        return rc;
    memcpy(dst, reply + 1, size);
    return rc;
}
