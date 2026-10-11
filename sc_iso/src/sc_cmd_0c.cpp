#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_0c::sc_cmd_0c()
    : m_v(0)
{
}

sc_cmd_0c::~sc_cmd_0c()
{
}

unsigned int sc_cmd_0c::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x810C0200;
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
    r = sc_make_0c(&req, id, ctx, m_v);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_0c::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x810C0300;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    reply.addr = buf;
    reply.size = size;
    r = sc_check_0c(&reply, id, ctx);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    return 0;
}

unsigned int sc_cmd_0c::set(const u8 &v)
{
    m_v = v;
    return 0;
}

unsigned int sc_cmd_0c::get()
{
    return 0;
}
