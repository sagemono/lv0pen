#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_0b::sc_cmd_0b()
    : m_data()
{
}

sc_cmd_0b::~sc_cmd_0b()
{
}

unsigned int sc_cmd_0b::set(const quad_uint &data, const quad_uint &len)
{
    unsigned int err = 0x810B0100;

    if (data == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    if (len != sizeof(m_data)) {
        err |= 0x80;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((u64)len, 10);
        get_log()->puts("\n");
        return 9;
    }
    memcpy(m_data, (const void *)data, sizeof(m_data));
    return 0;
}

unsigned int sc_cmd_0b::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x810B0200;
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
    r = sc_make_0b(&req, id, ctx, m_data);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_0b::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x810B0300;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    reply.addr = buf;
    reply.size = size;
    r = sc_check_0b(&reply, id, ctx);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    return 0;
}

unsigned int sc_cmd_0b::get()
{
    return 0;
}
