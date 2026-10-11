#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

sc_cmd_04::sc_cmd_04()
    : m_data()
{
}

sc_cmd_04::~sc_cmd_04()
{
}

unsigned int sc_cmd_04::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x81060200;
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
    r = sc_make_04(&req);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

unsigned int sc_cmd_04::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x81040300;
    u8 data[128];
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    reply.addr = buf;
    reply.size = size;
    r = sc_check_04(&reply, data);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    memcpy(m_data, data, sizeof(m_data));
    return 0;
}

unsigned int sc_cmd_04::get(u8 *data)
{
    memcpy(data, m_data, sizeof(m_data));
    return 0;
}

unsigned int sc_cmd_04::set()
{
    return 0;
}
