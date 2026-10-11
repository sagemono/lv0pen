#include <spu_intrinsics.h>
#include "sb.h"
#include "aes.h"
#include "util.h"

static const quad_uint s_error = STOP_ERROR;

int bytes_differ(const unsigned char *a, const unsigned char *b, int n)
{
    int same = 1;
    int i;

    for (i = 0; i != n; i++)
        same = (a[i] == b[i]) && same;
    return !same;
}

unsigned int session_b::fill_random(unsigned char *p, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++)
        p[i] = 0;
    for (i = 0; i != n; i++) {
        spu_readchcnt(74);
        p[i] = spu_readch(74) >> 24;
    }
    return 0;
}

int session_b::run()
{
    dma_channel dma;
    quad_uint token = 0;
    quad_uint cmd = CMD_REPLY;
    quad_ptr buf = m_buf;
    quad_uint len = 0;
    quad_uint in_len = 0;
    quad_uint max = 128;

    put_challenge(token, cmd, buf, len);
    if ((int)m_t->put(token, cmd, len, buf) < 0) {
        cmd = m_cmd;
        m_t->finish(s_error, cmd, 0, 0);
    }
    if ((int)m_t->get(buf, max, token, cmd, in_len) < 0) {
        cmd = m_cmd;
        m_t->finish(s_error, cmd, 0, 0);
    }
    if (verify(token, cmd, buf, in_len) < 0) {
        cmd = m_cmd;
        m_t->finish(s_error, cmd, 0, 0);
    }
    if (m_cmd == CMD_START_C) {
        memcpy(m_out, m_peer, 24);
    } else {
        put_peer(token, cmd, buf, len);
        if ((int)m_t->put(token, cmd, len, buf) < 0) {
            cmd = m_cmd;
            m_t->finish(s_error, cmd, 0, 0);
        }
        if ((int)m_t->get(buf, max, token, cmd, in_len) < 0) {
            cmd = m_cmd;
            m_t->finish(s_error, cmd, 0, 0);
        }
        if (finish_xor(token, cmd, buf, in_len) < 0) {
            cmd = m_cmd;
            m_t->finish(s_error, cmd, 0, 0);
        }
        memcpy(m_out, m_xor, 24);
    }
    return 0;
}

int session_b::verify(unsigned int token, unsigned int cmd, const unsigned char *msg,
                      unsigned int len)
{
    unsigned char in[32] __attribute__((aligned(16)));
    unsigned char dec[32] __attribute__((aligned(16)));
    unsigned char in2[32] __attribute__((aligned(16)));

    if (check(token, cmd, msg, len, 2) != 0)
        return -1;
    memcpy(in, msg + 20, 32);
    if (aes_cbc_decrypt((vec_uchar16 *)dec, (const vec_uchar16 *)in, 32, m_dec_key, 192,
                        (const vec_uchar16 *)m_iv) < 0)
        goto fail;
    if (bytes_differ(m_rand, dec, 24))
        return -1;
    memcpy(in2, msg + 52, 32);
    if (aes_cbc_decrypt((vec_uchar16 *)m_peer, (const vec_uchar16 *)in2, 32, m_dec_key, 192,
                        (const vec_uchar16 *)m_iv) < 0)
        goto fail;
    return bytes_differ(m_rand, m_peer, 24) ? 0 : -1;
fail:
    return -11;
}

int session_b::put_challenge(unsigned int &token, unsigned int &cmd, unsigned char *out,
                             quad_uint &len)
{
    unsigned char enc[32] __attribute__((aligned(16)));
    unsigned char msg[128] __attribute__((aligned(16)));

    *(unsigned int *)msg = 64;
    *(unsigned int *)(msg + 4) = 80;
    *(unsigned short *)(msg + 16) = 1;
    *(unsigned short *)(msg + 18) = 48;
    if (aes_cbc_encrypt((vec_uchar16 *)enc, (const vec_uchar16 *)m_rand, 32, m_enc_key, 192,
                        (const vec_uchar16 *)m_iv) < 0)
        return -11;
    memcpy(msg + 20, m_iv, 16);
    memcpy(msg + 36, enc, 32);
    return put_reply(token, cmd, out, len, msg);
}

int session_b::put_peer(unsigned int &token, unsigned int &cmd, unsigned char *out,
                        quad_uint &len)
{
    unsigned char enc[32] __attribute__((aligned(16)));
    unsigned char msg[128] __attribute__((aligned(16)));

    *(unsigned int *)msg = 48;
    *(unsigned int *)(msg + 4) = 16;
    *(unsigned short *)(msg + 16) = 3;
    *(unsigned short *)(msg + 18) = 32;
    if (aes_cbc_encrypt((vec_uchar16 *)enc, (const vec_uchar16 *)m_peer, 32, m_enc_key, 192,
                        (const vec_uchar16 *)m_iv) < 0)
        return -11;
    memcpy(msg + 20, enc, 32);
    return put_reply(token, cmd, out, len, msg);
}

int session_b::finish_xor(unsigned int token, unsigned int cmd, const unsigned char *msg,
                          unsigned int len)
{
    if (check(token, cmd, msg, len, 4) != 0)
        return -1;
    xor_bytes(m_rand, m_peer, m_xor, 24);
    return 0;
}

int session_b::check(unsigned int token, unsigned int cmd, const unsigned char *msg,
                     unsigned int len, unsigned int size)
{
    if (token == 3) {
        cmd = m_cmd;
        m_t->finish(s_error, cmd, 0, 0);
    } else if (token != 2)
        return -1;
    if (cmd - CMD_PEER_OK > 1)
        return -1;
    return (msg[17] | msg[16] << 8) == size ? 0 : -1;
}

unsigned int session_b::put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                                  quad_uint &len, const unsigned char *msg)
{
    len = GET_BE32(msg) + 16;
    memcpy(out, msg, len);
    token = 129;
    cmd = CMD_ANSWER;
    return 0;
}

unsigned int session_b::xor_bytes(const unsigned char *a, const unsigned char *b,
                                  unsigned char *out, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++)
        out[i] = a[i] ^ b[i];
    return 0;
}

void session_b::init(ss_iso_dma_data *t, unsigned char *buf, unsigned int cmd, unsigned int version,
                     unsigned char *out)
{
    m_buf = buf;
    m_t = t;
    m_out = out;
    m_cmd = cmd;
    fill_random(m_rand, 32);
    fill_random(m_iv, 16);
    if (version == 0x01000201 || version == 0x01000301 || version == 0x01000302 ||
        version == 0x01000303) {
        m_enc_key = g_b_enc_key0;
        m_dec_key = g_b_dec_key0;
    } else if (version == 0x02000101 || version == 0x02000102) {
        m_enc_key = g_b_enc_key1;
        m_dec_key = g_b_dec_key1;
    } else if (version == 0x03000101) {
        m_enc_key = g_b_enc_key2;
        m_dec_key = g_b_dec_key2;
    } else if (version == 0x04000101) {
        m_enc_key = g_b_enc_key3;
        m_dec_key = g_b_dec_key3;
    } else if ((version & 0xFF000000) == 0x01000000) {
        m_enc_key = g_b_enc_key0;
        m_dec_key = g_b_dec_key0;
    } else if ((version & 0xFF000000) == 0x02000000) {
        m_enc_key = g_b_enc_key1;
        m_dec_key = g_b_dec_key1;
    } else if ((version & 0xFF000000) == 0x03000000) {
        m_enc_key = g_b_enc_key2;
        m_dec_key = g_b_dec_key2;
    } else if ((version & 0xFF000000) == 0x04000000) {
        m_enc_key = g_b_enc_key3;
        m_dec_key = g_b_dec_key3;
    } else
        spu_stop(0x219);
}
