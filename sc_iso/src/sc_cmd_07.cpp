#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_07::sc_cmd_07()
    : m_value(0), m_mode(0), m_value2(0)
{
}

sc_cmd_07::~sc_cmd_07()
{
}

unsigned int sc_cmd_07::set(const u64 &mode)
{
    unsigned int err = 0x81070100;

    switch (mode) {
    case 0:
        m_mode = 0;
        break;
    case 1:
        m_mode = 1;
        break;
    case 0xFF:
        m_mode = 0xFF;
        break;
    default:
        err |= 0x07;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(mode, 10);
        get_log()->puts("\n");
        return 9;
    }
    return 0;
}

unsigned int sc_cmd_07::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x81070200;
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
    r = sc_make_07(&req, id, ctx, m_mode);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_07::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x81070300;
    u8 mode;
    unsigned int value;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    mode = 0;
    value = 0;
    reply.addr = buf;
    reply.size = size;
    r = sc_check_07(&reply, id, ctx, &mode, &value);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    if (mode != m_mode) {
        err |= 0x80;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(r, 10);
        get_log()->puts(", ");
        get_log()->put((unsigned int)mode, 16);
        get_log()->puts(", ");
        get_log()->put(value, 16);
        get_log()->puts("\n");
        return 15;
    }
    m_value = value;
    m_value2 = 0;
    return 0;
}

unsigned int sc_cmd_07::get(u64 &value, u64 &value2)
{
    value = m_value;
    value2 = m_value2;
    return 0;
}
