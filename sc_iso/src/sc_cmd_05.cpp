#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_05::sc_cmd_05()
    : m_a(0), m_b(0)
{
}

sc_cmd_05::~sc_cmd_05()
{
}

unsigned int sc_cmd_05::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x81050200;
    unsigned int n;
    sc_buf req;
    int r;

    len = 0;
    reply_len = 0;
    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    n = 0;
    req.addr = buf;
    req.size = size;
    req.len = &n;
    r = sc_make_05(&req, id, ctx);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_05::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x81050300;
    u8 a, b;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    a = 0;
    b = 0;
    reply.addr = buf;
    reply.size = size;
    r = sc_check_05(&reply, id, ctx, &a, &b);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    m_a = a;
    m_b = b;
    return 0;
}

unsigned int sc_cmd_05::set()
{
    return 0;
}

unsigned int sc_cmd_05::get(u8 &a, u8 &b)
{
    a = m_a;
    b = m_b;
    return 0;
}
