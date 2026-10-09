#include "cxx.h"
#include "syscon.h"
#include "clock.h"
#include "pci.h"
#include "log.h"

struct pci_bus {
    static const unsigned char set_subcmd = 0x31;
    static const unsigned char get_subcmd = 0x30;
};

struct req {
    unsigned char sub, len, pad[2];
    unsigned int out;
    unsigned char resp[4];
    unsigned char st[4];
};

CXX_DROPPED long pci_mini_driver::get_sc_version(unsigned short *version)
{
    req msg;
    msg.sub = 1;
    msg.len = 16;
    syscon *dev = get_syscon_device();
    long rc = dev->send_and_receive(SC_CMD_VERSION, &msg, 2, msg.resp, 4, (int *)msg.st, 0);
    if (rc == 0) {
        switch (msg.resp[0]) {
        case 0: break;
        case 1: rc = -10002; break;
        default: rc = 0xFFFFFFFFFFFEA073L - 10002; break;
        }
    }
    *version = (msg.resp[2] << 8) | msg.resp[3];
    return rc;
}

template <class T> int set_power_status(unsigned char state)
{
    unsigned char buf[16];
    buf[1] = T::set_subcmd;
    buf[2] = state;
    syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_POWER_STATUS, &buf[1], 2, &buf[0], 1, (int *)&buf[4], 0);
    if (rc != 0) return rc;
    unsigned int status = buf[0];
    switch (status) {
    case 0: return rc;
    case 1: return -10002;
    case 2: return -10003;
    }
    return 0xFFFFFFFFFFFEA074L - 10003;
}

template <class T> int get_power_status(unsigned int *out)
{
    unsigned char buf[16];
    buf[0] = T::get_subcmd;
    syscon *dev = get_syscon_device();
    int rc = dev->send_and_receive(SC_CMD_POWER_STATUS, &buf[0], 1, &buf[1], 2, (int *)&buf[4], 0);
    if (rc != 0) return rc;
    *out = buf[2];
    unsigned int status = buf[1];
    switch (status) {
    case 0: return rc;
    case 1: return -10002;
    case 2: return -10003;
    }
    return 0xFFFFFFFFFFFEA074L - 10003;
}

int pci_mini_driver::set_bus_power(long unused)
{
    req msg;
    msg.sub = 1;
    msg.len = 16;
    syscon *dev = get_syscon_device();
    int xfer_rc = dev->send_and_receive(SC_CMD_VERSION, &msg, 2, msg.resp, 4, (int *)msg.st, 0);
    unsigned short version = (msg.resp[2] << 8) | msg.resp[3];
    if (xfer_rc == 0) {
        switch (msg.resp[0]) {
        case 0: break;
        case 1: xfer_rc = -10002; break;
        default: xfer_rc = 0xFFFFFFFFFFFEA073L - 10002; break;
        }
    }
    if (xfer_rc != 0) return 0;
    if (version <= 0x100) return 0;
    if (get_power_status<pci_bus>(&msg.out) != 0) return 0;
    if (msg.out == 1) return 0;
    int rc = set_power_status<pci_bus>(1);
    if (rc != 0) {
        log_message("[ERROR]: 0x%08x fatal. set_power_status<pci_bus> fail %d", LV0_ERR_CONFIG, rc);
        return -11;
    }
    for (;;) {
        rc = get_power_status<pci_bus>(&msg.out);
        if (rc != 0) {
            log_message("[ERROR]: 0x%08x fatal. get_power_status<pci_bus> fail %d\n", LV0_ERR_CONFIG, rc);
            return -11;
        }
        if (msg.out == 1) break;
    }
    ::delay_ms(650);
    return 0;
}
