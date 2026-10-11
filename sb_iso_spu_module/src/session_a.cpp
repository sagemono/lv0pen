#include <spu_intrinsics.h>
#include "sb.h"
#include "aes.h"
#include "util.h"

static const quad_uint s_error = STOP_ERROR;

void session_a::init(ss_iso_dma_data *t, unsigned int type, unsigned int version, unsigned int len,
                     const unsigned char *key)
{
    m_t = t;
    m_type = type;
    m_version = version;
    if (version == 0x01000201 || version == 0x01000301 || version == 0x01000302 ||
        version == 0x01000303)
        m_key = g_a_key0;
    else if (version == 0x02000101 || version == 0x02000102)
        m_key = g_a_key1;
    else if (version == 0x03000101)
        m_key = g_a_key2;
    else if (version == 0x04000101)
        m_key = g_a_key3;
    else if ((version & 0xFF000000) == 0x01000000)
        m_key = g_a_key0;
    else if ((version & 0xFF000000) == 0x02000000)
        m_key = g_a_key1;
    else if ((version & 0xFF000000) == 0x03000000)
        m_key = g_a_key2;
    else if ((version & 0xFF000000) == 0x04000000)
        m_key = g_a_key3;
    else
        spu_stop(0x219);
    memset(m_data, 0, sizeof(m_data));
    memcpy(m_data, key, 24);
    fill_random(m_iv, 16);
}

int session_a::run(unsigned int cmd)
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

    b.init(m_t, buf, cmd, m_version, m_session_key);
    if (b.run() != 0) {
        c = cmd;
        m_t->finish(s_error, c, 0, 0);
    }
    put_key(token, c, buf, len);
    if ((int)m_t->put(token, c, len, buf) < 0) {
        c = cmd;
        m_t->finish(s_error, c, 0, 0);
    }
    if ((int)m_t->get(buf, max, token, c, in_len) < 0) {
        c = cmd;
        m_t->finish(s_error, c, 0, 0);
    }
    if (check_answer(token, c, buf, in_len) < 0) {
        c = cmd;
        m_t->finish(s_error, c, 0, 0);
    }
    return 0;
}

int session_a::put_key(unsigned int &token, unsigned int &cmd, unsigned char *out,
                       quad_uint &len)
{
    unsigned char plain[64] __attribute__((aligned(16)));
    unsigned char enc[64] __attribute__((aligned(16)));
    unsigned char msg[128] __attribute__((aligned(16)));

    *(unsigned int *)msg = 96;
    *(unsigned int *)(msg + 4) = 16;
    *(unsigned short *)(msg + 16) = 1;
    *(unsigned short *)(msg + 18) = 80;
    memset(plain, 0, 64);
    memcpy(plain, m_key, 24);
    put_type(plain + 24, m_type);
    memcpy(plain + 32, m_data, 24);
    memcpy(enc, plain, 64);
    if (aes_cbc_encrypt((vec_uchar16 *)enc, (const vec_uchar16 *)plain, 64, m_session_key, 192,
                        (const vec_uchar16 *)m_iv) < 0)
        return -1;
    memcpy(msg + 20, m_iv, 16);
    memcpy(msg + 36, enc, 64);
    return put_reply(token, cmd, out, len, msg);
}

unsigned int session_a::put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                                  quad_uint &len, const unsigned char *msg)
{
    len = GET_BE32(msg) + 16;
    memcpy(out, msg, len);
    token = 129;
    cmd = CMD_ANSWER;
    return 0;
}

unsigned int session_a::fill_random(unsigned char *p, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++) {
        spu_readchcnt(74);
        p[i] = spu_readch(74) >> 24;
    }
    return 0;
}

int session_a::check_answer(unsigned int token, unsigned int cmd, const unsigned char *msg,
                            unsigned int len)
{
    return check(token, cmd, msg, len, 2) != 0 ? -1 : 0;
}

int session_a::check(unsigned int token, unsigned int cmd, const unsigned char *msg,
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

int session_a::put_type(unsigned char *p, unsigned int type)
{
    if (type < 16) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 3;
        p[3] = type;
        return 0;
    }
    if (type - 0x100 < 4) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 2;
        p[3] = type & 15;
        p[7] = 1;
        return 0;
    }
    if (type - 0x110 < 4) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 2;
        p[3] = type & 15;
        p[7] = 0;
        return 0;
    }
    return -1;
}
