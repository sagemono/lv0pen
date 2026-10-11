#include <spu_intrinsics.h>
#include "sb.h"
#include "util.h"

static const quad_uint s_error = STOP_ERROR;
static const quad_uint s_ok = STOP_OK;

static const unsigned char s_key[24] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67,
};

void session_c::init(ss_iso_dma_data *t, unsigned int version)
{
    unsigned int i;

    m_t = t;
    m_version = version;
    for (i = 0; i < 24; i++)
        m_key[i] = s_key[i];
}

int session_c::check(unsigned int token, unsigned int cmd, const unsigned char *msg,
                     unsigned int len, unsigned int size)
{
    int r = -1;

    if (token == 3)
        return -1;
    if (token != 2)
        return -1;
    if (cmd - CMD_PEER_OK <= 1)
        r = (msg[17] | msg[16] << 8) == size ? 0 : -1;
    return r;
}

int session_c::check_answer(unsigned int token, unsigned int cmd, const unsigned char *msg,
                            unsigned int len)
{
    return check(token, cmd, msg, len, 8) != 0 ? -1 : 0;
}

int session_c::put_done(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len)
{
    unsigned char msg[128] __attribute__((aligned(16)));

    *(unsigned int *)msg = 16;
    *(unsigned int *)(msg + 4) = 16;
    *(unsigned short *)(msg + 16) = 6;
    *(unsigned short *)(msg + 18) = 0;
    return put_reply(token, cmd, out, len, msg);
}

int session_c::put_key(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len)
{
    token = 0;
    cmd = CMD_START_C;
    memcpy(out, m_key, 24);
    len = 32;
    return 0;
}

unsigned int session_c::put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                                  quad_uint &len, const unsigned char *msg)
{
    len = GET_BE32(msg) + 16;
    memcpy(out, msg, len);
    token = 129;
    cmd = CMD_ANSWER;
    return 0;
}

int session_c::run()
{
    dma_channel dma;
    quad_uint token = 0;
    quad_uint c = CMD_REPLY;
    quad_ptr buf;
    quad_uint len = 0;
    quad_uint in_len = 0;
    buf = (unsigned char *)0x3E000;
    quad_uint max = 128;
    session_b b;

    b.init(m_t, buf, CMD_START_C, m_version, m_key);
    if (b.run() != 0) {
        c = CMD_START_C;
        m_t->finish(s_error, c, 0, 0);
    }
    put_done(token, c, buf, len);
    if ((int)m_t->put(token, c, len, buf) < 0) {
        c = CMD_START_C;
        m_t->finish(s_error, c, 0, 0);
    }
    if ((int)m_t->get(buf, max, token, c, in_len) < 0) {
        c = CMD_START_C;
        m_t->finish(s_error, c, 0, 0);
    }
    if (check_answer(token, c, buf, in_len) < 0) {
        c = CMD_START_C;
        m_t->finish(s_error, c, 0, 0);
    }
    put_key(token, c, buf, in_len);
    c = CMD_START_C;
    m_t->finish(s_ok, c, in_len, buf);
    spu_stop(0x103);
    return 0;
}
