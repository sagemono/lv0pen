#include "sc_proxy_hdr.h"
#include "sc_log.h"
#include "util.h"

sc_proxy_hdr::sc_proxy_hdr()
{
}

sc_proxy_hdr::~sc_proxy_hdr()
{
}

unsigned int sc_proxy_hdr::verify_hdr(const quad_ptr &addr, const quad_uint &size, quad_uint &a,
                                      quad_uint &b)
{
    unsigned int err = 0x81030200;
    u64 va, vb;

    if (addr == 0) {
        SC_ERR(err, 0x80);
        return 9;
    }
    if (size != sizeof(sc_proxy)) {
        SC_ERR(err, 0x81);
        return 9;
    }
    va = ((const sc_proxy *)addr)->a;
    if (va > 0xFFFFFFFF) {
        SC_ERR(err, 0x82);
        return 9;
    }
    vb = ((const sc_proxy *)addr)->b;
    if (vb > 0xFFFFFFFF) {
        SC_ERR(err, 0x83);
        return 9;
    }
    a = va;
    b = vb;
    return 0;
}

unsigned int sc_proxy_hdr::make_hdr(const quad_uint &a, const quad_uint &b, const quad_ptr &addr,
                                    const quad_uint &size)
{
    unsigned int err = 0x81030100;
    sc_proxy hdr;

    if (addr == 0) {
        SC_ERR(err, 0x80);
        return 9;
    }
    if (size != sizeof(hdr)) {
        SC_ERR(err, 0x81);
        return 9;
    }
    if (a == 0 || b == 0) {
        SC_ERR(err, 0x82);
        return 9;
    }
    hdr.a = a;
    hdr.b = b;
    memcpy(addr, &hdr, sizeof(hdr));
    return 0;
}
