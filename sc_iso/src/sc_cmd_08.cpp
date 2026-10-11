#include "sc_cmd.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"

unsigned int sc_cmd_08::check(const quad_ptr &buf, const quad_uint &size, const unsigned int &id,
                              void *ctx)
{
    unsigned int err = 0x81080300;
    unsigned int offset, blocks, seq;
    sc_buf reply;
    int r;

    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    offset = 0;
    blocks = 0;
    seq = 0;
    reply.addr = buf;
    reply.size = size;
    r = sc_check_08(&reply, id, ctx, &offset, &blocks, &seq, m_data, sizeof(m_data));
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    if (offset == m_offset >> 4 && blocks == BLOCKS(m_len))
        goto check_seq;
    err |= 0x80;
    get_log()->put(err, 16);
    get_log()->puts(", ");
    get_log()->put((u64)m_offset, 16);
    get_log()->puts(", ");
    get_log()->put((u64)m_len, 16);
    get_log()->puts(", ");
    get_log()->put((u64)offset, 16);
    get_log()->puts(", ");
    get_log()->put((u64)blocks, 16);
    get_log()->puts("\n");
    return 15;
bad_seq:
    err |= 0x81;
    get_log()->put(err, 16);
    get_log()->puts(", ");
    get_log()->put((u64)m_seq, 16);
    get_log()->puts(", ");
    get_log()->put((u64)seq, 16);
    get_log()->puts("\n");
    return 15;
check_seq:
    if (seq != m_seq)
        goto bad_seq;
    return 0;
}

unsigned int sc_cmd_08::make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                             quad_uint &reply_len, const unsigned int &id, void *ctx)
{
    unsigned int err = 0x81080200;
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
    if (!m_have_seq) {
        m_seq = id;
        m_have_seq = true;
    }
    r = sc_make_08(&req, id, ctx, m_offset >> 4, BLOCKS(m_len), m_seq);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    len = n;
    reply_len = REPLY_LEN;
    return 0;
}

sc_cmd_08::sc_cmd_08()
    : m_offset(0), m_len(0)
{
    memset(m_data, 0, sizeof(m_data));
    m_seq = 0;
    m_have_seq = false;
}

sc_cmd_08::sc_cmd_08(const unsigned int &seq)
    : m_offset(0), m_len(0)
{
    memset(m_data, 0, sizeof(m_data));
    m_seq = seq;
    m_have_seq = true;
}

sc_cmd_08::~sc_cmd_08()
{
}

unsigned int sc_cmd_08::get(const quad_ptr &buf, const quad_uint &max, quad_uint &len)
{
    unsigned int err = 0x81080400;

    len = 0;
    if (buf == 0) {
        SC_ERR(err, 0x80);
        return 9;
    }
    if (m_len > max) {
        SC_ERR(err, 0x81);
        return 9;
    }
    memcpy(buf, m_data, m_len);
    len = m_len;
    return 0;
}

unsigned int sc_cmd_08::set(const quad_uint &offset, const quad_uint &len)
{
    m_offset = offset;
    m_len = len;
    return 0;
}
