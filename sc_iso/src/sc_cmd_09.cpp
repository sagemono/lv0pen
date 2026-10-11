#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_09::sc_cmd_09()
    : m_value(0), m_mode(0)
{
}

sc_cmd_09::~sc_cmd_09()
{
}

unsigned int sc_cmd_09::set(const u64 &mode, const s64 &value)
{
    unsigned int err = 0x81090100;

    switch (mode) {
    case 0:
        m_mode = 0;
        break;
    case 1:
        m_mode = 1;
        break;
    default:
        err |= 0x07;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(mode, 10);
        get_log()->puts("\n");
        return 9;
    }
    if (value < -0xFFFFFFFFLL || value > 0xFFFFFFFFLL) {
        err |= 0x07;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((int)value, 10);
        get_log()->puts("\n");
        return 9;
    }
    m_value = value;
    return 0;
}

unsigned int sc_cmd_09::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x81090200;
    unsigned int n;
    sc_buf req;
    unsigned int neg;
    unsigned int mag;
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
    if (m_value > 0) {
        neg = 0;
        mag = m_value;
    } else {
        neg = 1;
        mag = -m_value;
    }
    r = sc_make_09(&req, id, ctx, m_mode, neg, mag, 0);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_09::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x81090300;
    u8 mode;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    mode = 0;
    reply.addr = buf;
    reply.size = size;
    r = sc_check_09(&reply, id, ctx, &mode);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    if (mode != m_mode) {
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)mode, 16);
        get_log()->puts("\n");
        return 15;
    }
    return 0;
}

unsigned int sc_cmd_09::get()
{
    return 0;
}
