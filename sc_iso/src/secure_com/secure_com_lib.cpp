#include <spu_intrinsics.h>
#include "secure_com.h"
#include "aes.h"
#include "util.h"

extern "C" void prng_seed(void);
extern "C" int prng_random(unsigned char *out);

#define BE32(p) ((((p)[1] << 16) | ((p)[0] << 24)) | ((p)[2] << 8) | (p)[3])

#define CRYPTO_ERR      0xFFFF0001

union be32 {
    u32 v;
    u8 b[4];
};

union be64 {
    u64 v;
    u8 b[8];
};

u8 g_hdr[2] = { 0xAD, 0x1A };

vec_uchar16 g_key_out = { 0xA3, 0xAD, 0xB9, 0x9A, 0x21, 0xE4, 0x7A, 0xDF,
                          0xF3, 0xFD, 0x7F, 0xC3, 0x17, 0x39, 0x81, 0xCA };
vec_uchar16 g_key_in = { 0x69, 0x33, 0xCE, 0xE7, 0xA5, 0x18, 0xE5, 0xB8,
                         0xCB, 0xE1, 0xFC, 0x14, 0xB2, 0x61, 0xB7, 0x65 };

struct sc_auth_keys {
    vec_uchar16 key[8];
};
sc_auth_keys g_auth_keys = { {
    { 0x9F, 0x1D, 0xF8, 0x16, 0xBB, 0x4A, 0x4A, 0x01, 0x29, 0xD0, 0x31, 0xCF, 0xB0, 0xAD, 0x9B, 0x30 },
    { 0xD3, 0x02, 0xFD, 0xE1, 0x75, 0x78, 0xFB, 0xDB, 0xA1, 0x05, 0x84, 0x49, 0xBA, 0x5C, 0x1B, 0xEA },
    { 0x0E, 0x6B, 0x74, 0x80, 0xE5, 0xCE, 0xB2, 0x56, 0x2A, 0x33, 0x47, 0xBB, 0x41, 0x01, 0x24, 0x55 },
    { 0x79, 0x10, 0xAC, 0x5D, 0x2A, 0xD1, 0x60, 0x01, 0xF6, 0xA2, 0x78, 0x39, 0x79, 0x09, 0x61, 0x03 },
    { 0xE3, 0x05, 0x28, 0x04, 0xB7, 0xD2, 0x83, 0x6F, 0x28, 0x79, 0xA1, 0x75, 0x1B, 0xB4, 0x0D, 0x48 },
    { 0xEF, 0x58, 0x6F, 0x9D, 0x59, 0x91, 0x70, 0x67, 0x68, 0x50, 0x59, 0x0B, 0xA6, 0x7D, 0x4B, 0xC7 },
    { 0x5D, 0x95, 0x98, 0x63, 0x7A, 0xF2, 0x5F, 0x80, 0x23, 0x62, 0x3B, 0x12, 0x68, 0xB5, 0x13, 0x1A },
    { 0x0E, 0xAA, 0x32, 0x14, 0x0A, 0x28, 0x61, 0xD8, 0x65, 0x96, 0x26, 0xF6, 0xCE, 0x22, 0x86, 0xDB },
} };

vec_uchar16 g_09_keys[4] = {
    { 0xE3, 0xEF, 0xDE, 0x98, 0x7E, 0x4A, 0x2D, 0x3F, 0x8C, 0xF7, 0xB3, 0xB6, 0x0E, 0x84, 0x6B, 0x21 },
    { 0x4A, 0xB0, 0x26, 0x66, 0x4E, 0x9D, 0x02, 0xF5, 0x3E, 0xFF, 0x95, 0x44, 0x54, 0x9B, 0x1F, 0x97 },
    { 0x7E, 0xCA, 0x7F, 0x29, 0x98, 0x91, 0xF1, 0xB2, 0x43, 0x11, 0x9E, 0x35, 0xAE, 0x94, 0xC3, 0xDE },
    { 0xE0, 0xB7, 0xA0, 0x86, 0x7C, 0xF4, 0x49, 0x23, 0xBA, 0xE6, 0x5E, 0x33, 0x86, 0x46, 0x0C, 0x80 },
};

vec_uchar16 g_iv;

u8 g_buf[688];

#define BUF(i)  ((vec_uchar16 *)&g_buf[i])
#define KEY(k)  ((const unsigned char *)(k))

secure_com_lib::secure_com_lib()
    : m_seq(0), m_crypt(true)
{
    m_hdr3 = g_hdr[0];
    m_hdr4 = g_hdr[1];
    m_error = 0;
    m_key_out = g_key_out;
    m_key_in = g_key_in;
    prng_seed();
}

int secure_com_lib::create_auth1_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                     const u8 *data)
{
    if (aes_cbc_encrypt(BUF(16), (const vec_uchar16 *)data, 16, KEY(key), 128, &g_iv) != 0)
        return -255;
    return send(buf, 16, id, 2);
}

int secure_com_lib::create_auth2_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                     const u8 *data)
{
    if (aes_cbc_encrypt(BUF(16), (const vec_uchar16 *)data, 16, KEY(key), 128, &g_iv) != 0)
        return -255;
    return send(buf, 16, id, 3);
}

int secure_com_lib::create_get_state_cmd(const sc_buf *buf)
{
    return send(buf, 0, 0xFF, 0);
}

int secure_com_lib::create_04_cmd(const sc_buf *buf)
{
    return send(buf, 0, 0xFF, 6);
}

int secure_com_lib::create_09_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 mode,
                                  unsigned int neg, unsigned int value, unsigned int unused)
{
    u8 blk[32];
    vec_uchar16 keys[4] = { g_09_keys[0], g_09_keys[1], g_09_keys[2], g_09_keys[3] };

    be32 n, v;

    n.v = neg;
    v.v = value;
    blk[0] = n.b[0];
    blk[1] = n.b[1];
    blk[2] = n.b[2];
    blk[3] = n.b[3];
    blk[4] = v.b[0];
    blk[5] = v.b[1];
    blk[6] = v.b[2];
    blk[7] = v.b[3];
    if (mode > 3)
        return -4;
    if (aes_cbc_encrypt((vec_uchar16 *)blk, (vec_uchar16 *)blk, 16, KEY(&keys[mode]), 128,
                        &g_iv) == 0 &&
        aes_cmac((vec_uchar16 *)&blk[16], (vec_uchar16 *)blk, 16, KEY(&keys[mode]), 128) == 0)
        return send_09(buf, id, key, mode, blk);
    return CRYPTO_ERR;
}

int secure_com_lib::send_09(const sc_buf *buf, unsigned int id, const void *key, u8 mode,
                            const u8 *data)
{
    g_buf[32] = mode;
    memcpy(&g_buf[48], data, 32);
    if (mode > 3)
        return -4;
    return send_cmd(buf, 48, id, 2, key);
}

int secure_com_lib::create_07_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 mode)
{
    g_buf[32] = mode;
    return send_cmd(buf, 16, id, 3, key);
}

int secure_com_lib::create_08_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                  unsigned int offset, unsigned int blocks, unsigned int seq)
{
    be32 o, b;

    if (blocks > 8)
        return -4;
    o.v = offset;
    b.v = blocks;
    g_buf[32] = o.b[0];
    g_buf[33] = o.b[1];
    g_buf[34] = o.b[2];
    g_buf[35] = o.b[3];
    g_buf[36] = b.b[0];
    g_buf[37] = b.b[1];
    g_buf[38] = b.b[2];
    g_buf[39] = b.b[3];
    g_buf[40] = seq;
    return send_cmd(buf, 16, id, 4, key);
}

int secure_com_lib::create_0d_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                  unsigned int offset, unsigned int blocks, const u8 *data,
                                  unsigned int len, unsigned int seq)
{
    be32 o, b;

    if (seq == 0xFFFFFFFF)
        seq = id;
    if (blocks > 8)
        return -4;
    o.v = offset;
    b.v = blocks;
    g_buf[32] = o.b[0];
    g_buf[33] = o.b[1];
    g_buf[34] = o.b[2];
    g_buf[35] = o.b[3];
    g_buf[36] = b.b[0];
    g_buf[37] = b.b[1];
    g_buf[38] = b.b[2];
    g_buf[39] = b.b[3];
    g_buf[40] = seq;
    memcpy(&g_buf[48], data, BE32(&g_buf[36]) << 4);
    return send_cmd(buf, (BE32(&g_buf[36]) << 4) + 16, id, 5, key);
}

int secure_com_lib::create_05_cmd(const sc_buf *buf, unsigned int id, const void *key)
{
    return send_cmd(buf, 0, id, 6, key);
}

int secure_com_lib::create_0c_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 v)
{
    g_buf[32] = v;
    return send_cmd(buf, 16, id, 7, key);
}

int secure_com_lib::create_0b_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                  const u8 *data)
{
    memcpy(&g_buf[32], data, 32);
    return send_cmd(buf, 32, id, 10, key);
}

int secure_com_lib::send_cmd(const sc_buf *buf, unsigned int len, unsigned int id,
                             unsigned int cmd, const void *key)
{
    int r;

    r = wrap_cmd(len, id, cmd, key);
    if (r != 0)
        return r;
    return send(buf, len + 32, id, 4);
}

int secure_com_lib::send(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int type)
{
    int r;
    unsigned int size;

    r = wrap(len, id, type);
    if (r != 0)
        return r;
    size = len + 32;
    if (size > buf->size)
        return -1;
    memcpy(buf->addr, g_buf, size);
    *buf->len = size;
    return 0;
}

int secure_com_lib::wrap(unsigned int len, unsigned int id, unsigned int type)
{
    u8 rnd[20];

    if ((len & 15) != 0)
        return -1;
    g_buf[0] = id;
    g_buf[1] = type;
    g_buf[2] = 0xFF;
    g_buf[3] = m_hdr3;
    g_buf[4] = m_hdr4;
    prng_random(rnd);
    memcpy(&g_buf[5], rnd, 11);
    if (m_crypt) {
        if (aes_cbc_encrypt(BUF(0), BUF(0), len + 16, KEY(&m_key_out), 128, &g_iv) != 0 ||
            aes_cmac(BUF((len + 16) & ~15), BUF(0), len + 16, KEY(&m_key_out), 128) != 0)
            return CRYPTO_ERR;
    }
    return 0;
}

int secure_com_lib::validate_auth1_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                       const u8 *rnd, u8 *data)
{
    int r = -1;

    if (buf->size >= 32) {
        r = receive(buf, buf->size - 32, id, 2);
        if (r == 0) {
            if (aes_cbc_decrypt(BUF(16), BUF(16), 16, KEY(key), 128, &g_iv) != 0)
                r = -255;
            else if (aes_cbc_decrypt(BUF(32), BUF(32), 16, KEY(key), 128, &g_iv) != 0)
                r = -255;
            else if (memcmp(BUF(16), rnd, 16) != 0)
                r = -3;
            else
                memcpy(data, BUF(32), 16);
        }
    }
    return r;
}

int secure_com_lib::validate_08_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                    unsigned int *offset, unsigned int *blocks,
                                    unsigned int *seq, u8 *data, unsigned int max)
{
    int r = -1;
    unsigned int n;

    if (buf->size >= 64) {
        r = receive_cmd(buf, buf->size - 64, id, 4, key);
        if (r == 0) {
            *offset = BE32(&g_buf[32]);
            *blocks = BE32(&g_buf[36]);
            *seq = g_buf[40];
            n = BE32(&g_buf[36]) << 4;
            memcpy(data, &g_buf[48], n > max ? max : n);
        }
    }
    return r;
}

int secure_com_lib::validate_auth2_cmd(const sc_buf *buf, unsigned int id, unsigned int unused,
                                       const u8 *rnd, const void *key, u8 *session_key)
{
    int r = -1;
    vec_uchar16 k;

    if (buf->size >= 32) {
        r = receive(buf, buf->size - 32, id, 3);
        if (r == 0) {
            sc_auth_keys keys;

            memset(&keys, 0, sizeof(keys));
            keys = g_auth_keys;
            if (aes_cbc_encrypt(&k, (const vec_uchar16 *)rnd, 16, KEY(key), 128, &g_iv) != 0)
                r = -255;
            else if (aes_cbc_encrypt(&k, &k, 16, KEY(&keys.key[id]), 128, &g_iv) != 0)
                r = -255;
            else {
                *(vec_uchar16 *)session_key = k;
                if (aes_cbc_decrypt(BUF(16), BUF(16), 16, session_key, 128, &g_iv) != 0)
                    r = -255;
                else {
                    u8 zero[16] = { 0 };

                    if (memcmp(BUF(16), zero, 16) != 0)
                        r = -3;
                }
            }
        }
    }
    return r;
}

int secure_com_lib::validate_get_state_cmd(const sc_buf *buf, u8 *state)
{
    int r = -1;

    if (buf->size >= 32) {
        r = receive(buf, buf->size - 32, 0xFF, 0);
        if (r == 0)
            memcpy(state, &g_buf[16], 16);
    }
    return r;
}

int secure_com_lib::validate_04_cmd(const sc_buf *buf, u8 *data)
{
    int r = -1;

    if (buf->size >= 32) {
        r = receive(buf, buf->size - 32, 0xFF, 6);
        if (r == 0)
            memcpy(data, &g_buf[16], 128);
    }
    return r;
}

int secure_com_lib::validate_09_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *mode)
{
    int r;

    if (buf->size < 64)
        return -1;
    r = receive_cmd(buf, buf->size - 64, id, 2, key);
    if (r != 0)
        return r;
    *mode = g_buf[32];
    return 0;
}

int secure_com_lib::validate_07_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *mode,
                                    unsigned int *value)
{
    int r;

    if (buf->size < 64)
        return -1;
    r = receive_cmd(buf, buf->size - 64, id, 3, key);
    if (r != 0)
        return r;
    *mode = g_buf[32];
    *value = BE32(&g_buf[44]);
    return 0;
}

int secure_com_lib::validate_0d_cmd(const sc_buf *buf, unsigned int id, const void *key,
                                    unsigned int *offset, unsigned int *blocks,
                                    unsigned int *seq)
{
    int r;

    if (buf->size < 64)
        return -1;
    r = receive_cmd(buf, buf->size - 64, id, 5, key);
    if (r != 0)
        return r;
    *offset = BE32(&g_buf[32]);
    *blocks = BE32(&g_buf[36]);
    if (seq != 0)
        *seq = g_buf[40];
    return 0;
}

int secure_com_lib::validate_05_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *a,
                                    u8 *b)
{
    int r;

    if (buf->size < 64)
        return -1;
    r = receive_cmd(buf, buf->size - 64, id, 6, key);
    if (r != 0)
        return r;
    *a = g_buf[32];
    *b = g_buf[33];
    return 0;
}

int secure_com_lib::validate_0c_cmd(const sc_buf *buf, unsigned int id, const void *key)
{
    if (buf->size < 64)
        return -1;
    return receive_cmd(buf, buf->size - 64, id, 7, key);
}

int secure_com_lib::validate_0b_cmd(const sc_buf *buf, unsigned int id, const void *key)
{
    if (buf->size < 64)
        return -1;
    return receive_cmd(buf, buf->size - 64, id, 10, key);
}

int secure_com_lib::receive_cmd(const sc_buf *buf, unsigned int len, unsigned int id,
                                unsigned int cmd, const void *key)
{
    int r;

    r = receive(buf, len + 32, id, 4);
    if (r != 0)
        return r;
    return unwrap_cmd(len, id, cmd, key);
}

int secure_com_lib::receive(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int type)
{
    unsigned int size = buf->size;

    if (size > sizeof(g_buf) || buf->size < 32)
        return -1;
    memcpy(g_buf, buf->addr, buf->size);
    return unwrap(len, id, type);
}

int secure_com_lib::wrap_cmd(unsigned int len, unsigned int id, unsigned int cmd,
                             const void *key)
{
    be64 seq;

    if ((len & 15) != 0)
        return -1;
    seq.v = ++m_seq;
    g_buf[16] = seq.b[0];
    g_buf[17] = seq.b[1];
    g_buf[18] = seq.b[2];
    g_buf[19] = seq.b[3];
    g_buf[20] = seq.b[4];
    g_buf[21] = seq.b[5];
    g_buf[22] = seq.b[6];
    g_buf[23] = seq.b[7];
    g_buf[24] = cmd;
    g_buf[25] = 0;
    if (aes_cbc_encrypt(BUF(16), BUF(16), len + 16, KEY(key), 128, &g_iv) != 0)
        return CRYPTO_ERR;
    if (aes_cmac(BUF((len + 32) & ~15), BUF(16), len + 16, KEY(key), 128) != 0)
        return CRYPTO_ERR;
    return 0;
}

int secure_com_lib::unwrap(unsigned int len, unsigned int id, unsigned int type)
{
    if (m_crypt) {
        vec_uchar16 mac = spu_splats((u8)0);

        if (aes_cmac(&mac, BUF(0), len + 16, KEY(&m_key_in), 128) != 0)
            goto crypto_err;
        if (memcmp(&mac, &g_buf[(len + 16) & ~15], 16) != 0)
            goto bad;
        if (aes_cbc_decrypt(BUF(0), BUF(0), len + 16, KEY(&m_key_in), 128, &g_iv) != 0)
            goto crypto_err;
    }
    switch (g_buf[2]) {
    case 1:
        if (len != 48)
            return -5;
        m_error = BE32(&g_buf[16]);
        return -2;
    case 0:
        if (g_buf[0] != id)
            goto bad;
        if (g_buf[1] != type)
            goto bad;
        return 0;
    default:
        goto bad;
    }
crypto_err:
    return CRYPTO_ERR;
bad:
    return -3;
}

int secure_com_lib::unwrap_cmd(unsigned int len, unsigned int id, unsigned int cmd,
                               const void *key)
{
    vec_uchar16 mac;
    be64 seq;

    if (aes_cmac(&mac, BUF(16), len + 16, KEY(key), 128) != 0)
        goto crypto_err;
    if (aes_cbc_decrypt(BUF(16), BUF(16), len + 16, KEY(key), 128, &g_iv) != 0)
        goto crypto_err;
    if (memcmp(&mac, &g_buf[(len + 32) & ~15], 16) != 0)
        goto bad;
    seq.v = 0;
    seq.b[0] = g_buf[16];
    seq.b[1] = g_buf[17];
    seq.b[2] = g_buf[18];
    seq.b[3] = g_buf[19];
    seq.b[4] = g_buf[20];
    seq.b[5] = g_buf[21];
    seq.b[6] = g_buf[22];
    seq.b[7] = g_buf[23];
    if (seq.v != m_seq || g_buf[24] != cmd || g_buf[25] != 1)
        goto bad;
    return 0;
crypto_err:
    return CRYPTO_ERR;
bad:
    return -3;
}

unsigned int secure_com_lib::get_error()
{
    return m_error;
}
