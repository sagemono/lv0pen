#include "secure_com.h"

secure_com_lib g_secure_com;

int sc_make_auth1(const sc_buf *buf, unsigned int id, const void *key, const u8 *data)
{
    return g_secure_com.create_auth1_cmd(buf, id, key, data);
}

int sc_check_auth1(const sc_buf *buf, unsigned int id, const void *key, const u8 *rnd, u8 *data)
{
    return g_secure_com.validate_auth1_cmd(buf, id, key, rnd, data);
}

int sc_make_auth2(const sc_buf *buf, unsigned int id, const void *key, const u8 *data)
{
    return g_secure_com.create_auth2_cmd(buf, id, key, data);
}

int sc_check_auth2(const sc_buf *buf, unsigned int id, unsigned int unused, const u8 *rnd,
                   const void *key, u8 *session_key)
{
    return g_secure_com.validate_auth2_cmd(buf, id, unused, rnd, key, session_key);
}

int sc_make_09(const sc_buf *buf, unsigned int id, const void *key, u8 mode, unsigned int neg,
               unsigned int value, unsigned int unused)
{
    return g_secure_com.create_09_cmd(buf, id, key, mode, neg, value, unused);
}

int sc_check_09(const sc_buf *buf, unsigned int id, const void *key, u8 *mode)
{
    return g_secure_com.validate_09_cmd(buf, id, key, mode);
}

int sc_make_07(const sc_buf *buf, unsigned int id, const void *key, u8 mode)
{
    return g_secure_com.create_07_cmd(buf, id, key, mode);
}

int sc_check_07(const sc_buf *buf, unsigned int id, const void *key, u8 *mode,
                unsigned int *value)
{
    return g_secure_com.validate_07_cmd(buf, id, key, mode, value);
}

int sc_make_08(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
               unsigned int blocks, unsigned int seq)
{
    return g_secure_com.create_08_cmd(buf, id, key, offset, blocks, seq);
}

int sc_check_08(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                unsigned int *blocks, unsigned int *seq, u8 *data, unsigned int max)
{
    return g_secure_com.validate_08_cmd(buf, id, key, offset, blocks, seq, data, max);
}

int sc_make_0d(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
               unsigned int blocks, const u8 *data, unsigned int len, unsigned int seq)
{
    return g_secure_com.create_0d_cmd(buf, id, key, offset, blocks, data, len, seq);
}

int sc_check_0d(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                unsigned int *blocks, unsigned int *seq)
{
    return g_secure_com.validate_0d_cmd(buf, id, key, offset, blocks, seq);
}

int sc_make_05(const sc_buf *buf, unsigned int id, const void *key)
{
    return g_secure_com.create_05_cmd(buf, id, key);
}

int sc_check_05(const sc_buf *buf, unsigned int id, const void *key, u8 *a, u8 *b)
{
    return g_secure_com.validate_05_cmd(buf, id, key, a, b);
}

int sc_make_0c(const sc_buf *buf, unsigned int id, const void *key, u8 v)
{
    return g_secure_com.create_0c_cmd(buf, id, key, v);
}

int sc_check_0c(const sc_buf *buf, unsigned int id, const void *key)
{
    return g_secure_com.validate_0c_cmd(buf, id, key);
}

int sc_make_0b(const sc_buf *buf, unsigned int id, const void *key, const u8 *data)
{
    return g_secure_com.create_0b_cmd(buf, id, key, data);
}

int sc_check_0b(const sc_buf *buf, unsigned int id, const void *key)
{
    return g_secure_com.validate_0b_cmd(buf, id, key);
}

int sc_create_get_state_cmd(const sc_buf *buf)
{
    return g_secure_com.create_get_state_cmd(buf);
}

int sc_validate_get_state_cmd(const sc_buf *buf, u8 *state)
{
    return g_secure_com.validate_get_state_cmd(buf, state);
}

int sc_make_04(const sc_buf *buf)
{
    return g_secure_com.create_04_cmd(buf);
}

int sc_check_04(const sc_buf *buf, u8 *data)
{
    return g_secure_com.validate_04_cmd(buf, data);
}

unsigned int sc_lib_error()
{
    return g_secure_com.get_error();
}
