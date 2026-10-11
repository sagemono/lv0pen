#include <spu_intrinsics.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include "sc_iso_module.h"
#include "secure_com.h"
#include "sc_log.h"
#include "util.h"
#include "aes.h"
#include "aes.h"
#include "ec.h"
#include "hmac_sha1.h"
#include "sc_iso_module.h"
#include "sc_log.h"
#include "secure_com.h"
#include "util.h"

#define SC_STATUS_BAD_CMD       257

#define SC_STATUS_ERROR         259

static const unsigned char s_service = 0x1F;

static const unsigned int s_ready = 0x80;

extern u8 g_116352[16], g_116368[16], g_116384[16], g_116400[16], g_116416[16], g_116432[16];

extern u8 g_116752[16], g_116768[16], g_116784[16], g_116800[16], g_116816[16], g_116832[16];

extern const u8 op32_key[16];

extern const u8 op32_iv[16];

#define GSS_LOG(what) \
    do { \
        get_log()->puts("In sc_iso_module::get_syscon_state: "); \
        get_log()->puts(what); \
    } while (0)

extern u8 g_116752[16], g_116768[16], g_116784[16], g_116800[16], g_116816[16], g_116832[16];

extern const u8 g_116144[16], g_116160[16], g_116176[16], g_116192[16], g_116208[16],
    g_116224[16];

extern const u8 g_state_116448[16];

static const u64 g_mode_ff = 0xFF;

struct cmd_11_req {
    u64 mode;
    u64 time;
};

struct cmd_0c_req {
    u64 mode;
} __attribute__((aligned(16)));

static const u8 g_op33_key[16] = {
    0x3B, 0x05, 0x0A, 0x7C, 0xA2, 0x96, 0x15, 0x82,
    0x22, 0x8E, 0xE8, 0x02, 0x9E, 0x41, 0x59, 0xD5
};

static const u8 g_op33_iv[16] __attribute__((aligned(16))) = {
    0xC3, 0x67, 0xA9, 0xAA, 0x6A, 0x16, 0x61, 0x28,
    0xB7, 0x94, 0x88, 0x3E, 0x61, 0x58, 0x8B, 0x48
};

static const u8 g_op33_pub[40] = {
    0x97, 0xA6, 0x60, 0x90, 0xC2, 0xB6, 0x1A, 0xC1,
    0x66, 0x16, 0x2F, 0xBD, 0x85, 0x65, 0x2E, 0xF3,
    0xD3, 0xC0, 0x40, 0xA8, 0x1A, 0x9F, 0x7B, 0xD5,
    0x10, 0x06, 0xDD, 0x1D, 0x3F, 0x3E, 0x2E, 0x2B,
    0x72, 0x6F, 0x98, 0x50, 0x14, 0x57, 0x7D, 0x4B
};

static const u8 key_2[16] = {
    0xD4, 0x13, 0xB8, 0x96, 0x63, 0xE1, 0xFE, 0x9F,
    0x75, 0x14, 0x3D, 0x3B, 0xB4, 0x56, 0x52, 0x74
};

static const u8 key_3[16] = {
    0xFA, 0x72, 0xCE, 0xEF, 0x59, 0xB4, 0xD2, 0x98,
    0x9F, 0x11, 0x19, 0x13, 0x28, 0x7F, 0x51, 0xC7
};

static const u8 key_4[16] = {
    0x29, 0xC1, 0x94, 0xFF, 0xEC, 0x1F, 0xD1, 0x4D,
    0x4A, 0xAE, 0x00, 0x6C, 0x32, 0xB3, 0x59, 0x90
};

static const u8 seed_5[16] = {
    'k', 'e', 'y', 's', 'e', 'e', 'd', '_', 'f', 'o', 'r', '_', 's', 'r', 'k', '2'
};

static const u8 seed_6[16] = {
    's', 'e', 'e', 'd', '_', 'f', 'o', 'r', '_', 'b', 'a', 'c', 'k', 'u', 'p', 0
};

static const u8 seed_7[16] = {
    's', 'h', 'e', 'r', 'w', 'o', 'o', 'd', '_', 's', 's', '_', 's', 'e', 'e', 'd'
};

static const u8 seed_8[16] = {
    's', 's', '_', 's', 'e', 'e', 'd', '_', 'o', 'n', 'e', '_', 'm', 'o', 'r', 'e'
};

static const quad_u64 key_id_07 = 7;

static const quad_u64 key_id_08 = 8;

struct sc_request_07 {
    u64 id;
} __attribute__((aligned(16)));

struct sc_reply_07 {
    u64 id;
    u8 data[48];
    u8 reserved[8];
};

static const quad_uint mngblk_offset = 32;

struct sc_reply_04 {
    u8 data[40];
    u8 reserved[8];
};

static const quad_uint mngblk_zero = 0;

struct sc_reply_10 {
    u8 data[8];
    u8 reserved[8];
};

static const u64 s_id_6 = 6;

static const quad_uint s_16 = 16;

static const u64 s_id_1 = 1;

static const u8 s_keyseed[16] = {
    'k', 'e', 'y', 's', 'e', 'e', 'd', '_', 'f', 'o', 'r', '_', 's', 'r', 'k', '2'
};

struct sc_blob96 {
    u64 version;
    u64 size;
    u8 data[64];
    u8 mac[16];
};

struct sc_state16 {
    unsigned int a;
    unsigned int flags;
    unsigned int c;
    unsigned int d;
};

struct sc_crypt_hdr {
    u64 type;
    u64 size;
    u8 a[16];
    u8 iv[16];
};

unsigned int sc_iso_module::run(unsigned int &cmd_out, quad_ptr &buf, quad_uint &len)
{
    unsigned int err = 0x81010100;
    quad_uint token = 0;
    quad_uint cmd = 0;
    quad_uint in_len = 0;
    quad_uint status = SC_STATUS_ERROR;
    sc_pkt_hdr hdr(s_service);
    unsigned int r;

    m_pkt = &hdr;
    cmd = 0;
    buf = 0;
    len = 0;
    if (derive_auth_keys() != 0) {
        SC_ERR(err, 0x01);
        return SC_STATUS_ERROR;
    }
    if (m_dma->notify(s_ready) != 0) {
        SC_ERR(err, 0x02);
        return SC_STATUS_ERROR;
    }
    if (m_dma->get(m_buf, m_size, token, cmd, in_len) != 0) {
        SC_ERR(err, 0x03);
        return SC_STATUS_ERROR;
    }
    if (token != 1) {
        SC_ERR(err, 0x05);
        return SC_STATUS_BAD_CMD;
    }
    get_log()->puts("command = ");
    get_log()->put(cmd, 16);
    get_log()->puts("\n");
    if ((int)cmd < 0) {
        m_176 = 1;
        cmd &= 0x7FFFFFFF;
    }
    switch (cmd) {
    case 255:
        r = cmd_get_property(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 1:
        r = cmd_encrypt(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 2:
        r = cmd_decrypt(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 4:
        r = cmd_get_srh(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 5:
        r = cmd_set_srh(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 6:
        r = cmd_init_srk(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 14:
        r = cmd_reset_records(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 16:
        r = cmd_get_sc_status(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 22:
        r = cmd_set_sc_status(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 7:
        r = cmd_get_region_data(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 8:
        r = cmd_set_region_data(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 12:
        r = cmd_get_time(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 17:
        r = cmd_set_time(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 18:
        r = cmd_rekey(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 19:
        r = cmd_sc_binary_patch(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 23:
        r = cmd_backup_root_info(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 24:
        r = cmd_restore_root_info(m_buf, in_len, m_buf, m_size, len, status);
        break;
    case 25:
        r = cmd_pmode_srv_auth(m_buf, in_len, m_buf, m_size, len, status);
        break;
    default:
        err |= 0x06;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(cmd, 16);
        get_log()->puts("\n");
        return SC_STATUS_BAD_CMD;
    }
    if (r != 0)
        SC_ERR(err, 0x01);
    cmd_out = cmd;
    buf = m_buf;
    return status;
}

unsigned int sc_iso_module::cmd_sc_binary_patch(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81011100;
    unsigned int n = 4112;
    u8 tmp[n];
    quad_ptr p = tmp;
    quad_uint state;
    unsigned int r;

    if (len > n) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    __builtin_memcpy(tmp, buf, len);
    state = 0;
    r = get_sc_version((u8 *)&state);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (state <= 3)
        r = patch_areas((const quad_hdr_ptr &)p, len, out, size, out_len, status);
    else
        r = patch_4k((const quad_hdr_ptr &)p, len, out, size, out_len, status);
    return r;
}

unsigned int sc_iso_module::cmd_encrypt(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    int mode = 1;
    u8 encrypt = 1;

    return crypt_data(mode, encrypt, buf, len, out, size, out_len, status);
}

unsigned int sc_iso_module::cmd_decrypt(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    int mode = 1;
    u8 encrypt = 0;

    return crypt_data(mode, encrypt, buf, len, out, size, out_len, status);
}

unsigned int sc_iso_module::crypt_data(const int &mode, const u8 &encrypt, const quad_ptr &buf,
                                  const quad_uint &len, const quad_ptr &out,
                                  const quad_uint &size, quad_uint &out_len,
                                  unsigned int &status)
{
    unsigned int err = 0x81011e00;
    unsigned int r;
    const sc_crypt_hdr *in;
    u8 *o;
    quad_u64 type;
    u64 hsize;
    unsigned int pad;
    unsigned int total;
    unsigned int i;
    quad_uint klen;
    int d_key_bits;
    int d_len;
    int d_in_len;
    int e_iv_bits;
    int e_key_bits;
    quad_ptr kp;
    int d_iv_bits;
    int e_in_len;
    int e_len;
    u8 iv[16];
    u8 a[16];
    u8 key[16];
    sc_crypt_hdr hdr;
    u8 obuf[512];
    u8 ibuf[512];

    out_len = 0;
    status = 259;
#define BAD \
    do { \
        SC_ERR(err, 0x07); \
        status = 257; \
        return 9; \
    } while (0)
    if (len < 48)
        BAD;
    if (size < 48)
        BAD;
    in = (const sc_crypt_hdr *)(u8 *)buf;
    if (in == 0)
        BAD;
    if (out == 0)
        BAD;
    type = in->type;
    hsize = in->size;
    if (hsize > 512)
        BAD;
    pad = (16 - hsize % 16) % 16;
    total = hsize + 48 + pad;
    if (total > len)
        BAD;
    if (total > size)
        BAD;
    if (type < 1 || type > 5)
        BAD;
#undef BAD
    memcpy(a, in->a, 16);
    memcpy(iv, in->iv, 16);
    memcpy(ibuf, in + 1, hsize);
    klen = 16;
    kp = key;
    r = get_key(type, kp, klen);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return 9;
    }
    if (type == 3 || type == 4) {
        for (i = 0; i < 16; i++)
            key[i] ^= a[i];
    }
    sc_crypto c(mode);
    if (encrypt) {
        e_in_len = hsize;
        e_len = hsize;
        e_key_bits = (u64)klen * 8;
        e_iv_bits = 128;
        r = cbc_encrypt(&c, ibuf, e_in_len, obuf, e_len, key, e_key_bits, iv, e_iv_bits);
    } else {
        d_in_len = hsize;
        d_len = hsize;
        d_key_bits = (u64)klen * 8;
        d_iv_bits = 128;
        r = cbc_decrypt(&c, ibuf, d_in_len, obuf, d_len, key, d_key_bits, iv, d_iv_bits);
    }
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    hdr.type = type;
    hdr.size = hsize;
    memcpy(hdr.iv, iv, 16);
    o = out;
    memcpy(o, &hdr, 48);
    o += 48;
    memcpy(o, obuf, hsize);
    if (pad != 0)
        memset(o + hsize, 0, pad);
    out_len = total;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::cmd_init_srk(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010800;
    unsigned int r;
    unsigned int i;
    quad_uint klen;
    quad_ptr kp;
    quad_uint n40;
    quad_ptr p40;
    u8 key[16];
    sc_state16 st;
    u8 data[40];

    out_len = 0;
    status = 259;
    if (len != 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    klen = 16;
    kp = key;
    r = derive_srk2_key(kp, klen);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    r = write_srk(s_id_1, kp, klen);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    for (i = 0; i != klen; i++)
        key[i] = 0;
    n40 = 40;
    p40 = data;
    memset(data, 0, 40);
    r = write_srh(p40, n40);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    r = read_mngblk((u8 *)&st);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    st.flags |= 0x40000000;
    r = write_mngblk((u8 *)&st);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    out_len = 0;
    status = 256;
    return r;
}

unsigned int sc_iso_module::derive_srk2_key(const quad_ptr &buf, const quad_uint &len)
{
    return encrypt_seed(buf, len, s_keyseed);
}

unsigned int sc_iso_module::cmd_backup_root_info(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81011400;
    unsigned int r;
    int ret;
    sc_blob96 blob;
    u8 key[16];
    quad_uint klen1;
    quad_ptr kp1;
    quad_uint klen2;
    quad_ptr p2;
    quad_ptr kp2;
    quad_uint n2;

    out_len = 0;
    status = 259;
    if (len != 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (size < 96 || out == 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    klen1 = 16;
    n2 = 40;
    kp1 = blob.data;
    p2 = blob.data + 16;
    memset(&blob, 0, 96);
    blob.version = 1;
    blob.size = 96;
    r = get_key(s_id_1, kp1, klen1);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    r = read_srh(p2, n2);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    kp2 = key;
    klen2 = 16;
    r = get_key(s_id_6, kp2, klen2);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    ret = aes_cbc_encrypt((vec_uchar16 *)blob.data, (const vec_uchar16 *)blob.data, 64, key,
                          (u64)klen2 * 8, (const vec_uchar16 *)m_key2);
    if (ret != 0) {
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)ret, 10);
        get_log()->puts("\n");
        status = 259;
        return 15;
    }
    ret = aes_cmac((vec_uchar16 *)blob.mac, (const vec_uchar16 *)&blob, 80, key, (u64)klen2 * 8);
    if (ret != 0) {
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)ret, 10);
        get_log()->puts("\n");
        status = 259;
        return 15;
    }
    memcpy(out, &blob, 96);
    out_len = 96;
    status = 256;
    return r;
}

unsigned int sc_iso_module::cmd_reset_records(const quad_ptr &in, const quad_uint &in_len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010b00;
    unsigned int ret;
    unsigned int ver;
    struct {
        unsigned int w0;
        unsigned int w1;
        u64 w2;
    } data;

    out_len = 0;
    status = 259;
    if (in_len != 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    ret = read_mngblk(&data);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    ver = 0;
    ret = get_sc_version((u8 *)&ver);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    if (ver <= 3)
        goto old;
    if (data.w0 == 3)
        goto mark;
    data.w0 = 3;
    data.w1 = 0;
    data.w2 = 0;
    goto mark;
set2:
    data.w0 = 2;
    data.w1 = 0;
    data.w2 = 0;
mark:
    data.w1 |= 0x80000000;
    ret = erase_records(status);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    ret = write_mngblk(&data);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    out_len = 0;
    status = 256;
    return 0;
old:
    if (data.w0 != 2)
        goto set2;
    goto mark;
}

unsigned int sc_iso_module::erase_records(unsigned int &status)
{
    unsigned int err = 0x81012300;
    unsigned int ret;
    unsigned int i;
    quad_uint len;
    quad_ptr p;
    quad_uint offset;
    quad_uint got;
    u64 pattern;
    u8 buf[128];

    p = buf;
    len = 128;
    got = 0;
    pattern = 0xDEADBEAFFACEBABEULL;
    offset = 128;
    for (i = 0; i < 16; i++)
        memcpy(buf + i * 8, &pattern, 8);
    len = 128;
    for (i = 0; i < 6; i++) {
        ret = write_data(p, offset, len, got);
        if (ret != 0) {
            SC_ERR(err, 0x01);
            status = 259;
            return ret;
        }
        if (got != len) {
            SC_ERR(err, 0x80);
            status = 259;
            return 15;
        }
        offset += 128;
    }
    return 0;
}

unsigned int sc_iso_module::cmd_set_sc_status(const quad_ptr &in, const quad_uint &in_len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81011000;
    unsigned int ret;
    unsigned int a, b;
    unsigned int data[4];

    out_len = 0;
    status = 259;
    if (in_len != 16) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (in == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    a = *(const unsigned int *)(unsigned char *)in;
    b = *(const unsigned int *)(in + 4);
    if (m_176 != 0) {
        if (a != 3) {
            SC_ERR(err, 0x07);
            status = 257;
            return 9;
        }
    } else if (a > 2) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    ret = read_mngblk(data);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    data[0] = a;
    data[1] = (data[1] & ~0xFF) | (b & 0xFF);
    ret = write_mngblk(data);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    out_len = 0;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::write_mngblk(void *data)
{
    static const quad_uint s_0 = 0;
    unsigned int ret;
    quad_ptr p;
    quad_uint len;
    quad_uint got;

    len = 16;
    got = 0;
    p = (u8 *)data;

    ret = write_data(p, s_0, len, got);
    if (ret == 0)
        ret = got != len ? 15 : 0;
    return ret;
}

unsigned int sc_iso_module::write_srk(const u64 &id, const quad_ptr &buf, const quad_uint &len)
{
    unsigned int err = 0x81011700;
    unsigned int ret;

    if (buf == 0 || len != 16)
        return 9;
    quad_uint offset = 0;
    quad_uint got;
    if (id == 1) {
        offset = 16;
        got = 0;
        ret = write_data(buf, offset, len, got);
        if (ret != 0)
            return ret;
    } else {
        err |= 0x81;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(id, 16);
        get_log()->puts("\n");
        return 9;
    }
    if (got != len)
        return 15;
    return 0;
}

unsigned int sc_iso_module::cmd_set_srh(const quad_ptr &in, const quad_uint &in_len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010700;
    unsigned int ret;
    quad_uint n;
    quad_ptr p;
    u8 data[40];

    out_len = 0;
    status = 259;
    if (in_len != 48) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (in == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    n = sizeof(data);
    p = data;
    memcpy(data, in, sizeof(data));
    ret = write_srh(p, n);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    out_len = 0;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::write_srh(const quad_ptr &buf, const quad_uint &len)
{
    static const quad_uint s_32 = 32;
    unsigned int ret;
    quad_ptr p;
    quad_uint n;
    quad_uint got;
    u8 data[48];

    if (buf == 0 || len != 40)
        return 9;
    p = data;
    n = sizeof(data);
    memset(data, 0, sizeof(data));
    memcpy(data, buf, len);
    got = 0;
    ret = write_data(p, s_32, n, got);
    if (ret != 0)
        return ret;
    if (got != n)
        return 15;
    return 0;
}

unsigned int sc_iso_module::cmd_set_region_data(const quad_ptr &in, const quad_uint &in_len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010a00;
    unsigned int ret;
    quad_uint len;
    quad_ptr p;
    u64 id;
    struct {
        u64 id;
        u8 rsv[8];
    } reply;
    u8 data[48];

    out_len = 0;
    status = 259;
    if (in_len != 64) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (size < 16) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (in == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (out == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    id = *(const u64 *)(unsigned char *)in;
    if (id > 15)
        goto bad_id;
    len = sizeof(data);
    p = data;
    memcpy(data, in + 8, sizeof(data));
    ret = write_record(id, p, len);
    if (ret != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return ret;
    }
    reply.id = id;
    memset(reply.rsv, 0, sizeof(reply.rsv));
    memcpy(out, &reply, sizeof(reply));
    out_len = sizeof(reply);
    status = 256;
    return 0;
bad_id:
    SC_ERR(err, 0x81);
    status = 257;
    return 9;
}

unsigned int sc_iso_module::write_record(const u64 &id, const quad_ptr &buf, const quad_uint &len)
{
    unsigned int err = 0x81011900;
    unsigned int ret;
    quad_uint offset;
    quad_uint got;

    if (buf == 0 || len != 48)
        return 9;
    if (id > 15)
        goto bad;
    offset = id * 48 + 128;
    got = 0;
    ret = write_data(buf, offset, len, got);
    if (ret != 0)
        return ret;
    if (got != len)
        return 15;
    return 0;
bad:
    err |= 0x81;
    get_log()->put(err, 16);
    get_log()->puts(", ");
    get_log()->put(id, 16);
    get_log()->puts("\n");
    return 9;
}

unsigned int sc_iso_module::patch_areas(const quad_hdr_ptr &buf, const quad_uint &len, const quad_ptr &buf2,
                                  const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    static const quad_uint s_1024 = 1024;
    static const quad_uint s_3072 = 3072;
    static const quad_uint s_130 = 130;
    static const quad_uint s_129 = 129;
    static const quad_uint s_0 = 0;
    unsigned int err = 0x81012b00;
    const sc_data_hdr *hdr;
    unsigned int n;
    const u8 *data;

    out_len = 0;
    status = 259;
    if (len <= 16) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    hdr = buf;
    if (hdr == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    n = (unsigned int)hdr->size;
    if (n != 1024 && n != 4096) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    m_id = 6;
    data = (const u8 *)(hdr + 1);
    if (write_area(data, s_0, s_1024, s_129) != 0)
        goto fail;
    if (n == 4096) {
        if (write_area(data + 1024, s_0, s_3072, s_130) != 0) {
fail:
            SC_ERR(err, 0x01);
            status = 259;
            return 15;
        }
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::patch_4k(const quad_hdr_ptr &buf, const quad_uint &len, const quad_ptr &buf2,
                                  const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    static const quad_uint s_4096 = 4096;
    static const quad_uint s_130 = 130;
    static const quad_uint s_0 = 0;
    unsigned int err = 0x81012c00;
    u8 saved = m_176;
    const sc_data_hdr *hdr;

    m_176 = 0;
    out_len = 0;
    status = 259;
    if (len <= 16) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    hdr = buf;
    if (hdr == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if ((unsigned int)hdr->size != 4096) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    m_id = 6;
    if (write_area((const u8 *)(hdr + 1), s_0, s_4096, s_130) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    m_176 = saved;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::write_area(const u8 *data, const quad_uint &offset, const quad_uint &size,
                                   const quad_uint &x)
{
    unsigned int ret = 15;
    unsigned int done;
    quad_ptr p;
    quad_uint len = 128;
    quad_uint got = 0;
    quad_uint off = offset;

    for (done = 0; done < size; done += len) {
        if (size - done <= 128)
            len = size - done;
        p = (quad_ptr)(data + done);
        ret = write_slot_data(p, off, len, x, got);
        if (ret != 0)
            break;
        if (len != got)
            ret = 15;
        else
            off += len;
    }
    return ret;
}

unsigned int sc_iso_module::cbc_encrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                                   const int &len, const u8 *key, const int &key_bits, u8 *iv,
                                   const int &iv_bits)
{
    u8 encrypt = 1;
    return c->crypt(in, in_len, out, len, key, key_bits, iv, iv_bits, encrypt);
}

unsigned int sc_iso_module::cbc_decrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                                   const int &len, const u8 *key, const int &key_bits, u8 *iv,
                                   const int &iv_bits)
{
    u8 encrypt = 0;
    return c->crypt(in, in_len, out, len, key, key_bits, iv, iv_bits, encrypt);
}

unsigned int sc_iso_module::cmd_restore_root_info(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len, unsigned int &status)
{
    unsigned int r = 9;
    int ret;
    unsigned int err = 0x81011500;
    quad_ptr p;
    quad_uint n;
    quad_ptr kp;
    quad_uint klen;
    quad_uint got;
    u8 key[16];
    u8 mac[16];
    sc_blob96 blob;

    out_len = 0;
    status = 259;
    if (len != 96 || buf == 0) {
        SC_ERR(err, 0x01);
        status = 257;
        return r;
    }
    memcpy(&blob, buf, 96);
    if (blob.version != 1) {
        SC_ERR(err, 0x08);
        r = 27;
        status = 266;
        return r;
    }
    if (blob.size != 96) {
        SC_ERR(err, 0x08);
        r = 27;
        status = 266;
        return r;
    }
    kp = key;
    klen = 16;
    r = get_key(s_id_6, kp, klen);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    ret = aes_cmac((vec_uchar16 *)mac, (const vec_uchar16 *)&blob, 80, key, (u64)klen * 8);
    if (ret != 0) {
        r = 15;
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)ret, 10);
        get_log()->puts("\n");
        status = 259;
        return r;
    }
    if (bytes_differ(blob.mac, mac, 16)) {
        SC_ERR(err, 0x81);
        r = 20;
        status = 260;
        return r;
    }
    ret = aes_cbc_decrypt((vec_uchar16 *)blob.data, (const vec_uchar16 *)blob.data, 64, key,
                        (u64)klen * 8, (const vec_uchar16 *)m_key2);
    if (ret != 0) {
        r = 15;
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)ret, 10);
        get_log()->puts("\n");
        status = 259;
        return r;
    }
    got = 0;
    n = 64;
    u8 tmp[n];
    fill_bytes(tmp, 0, 64);
    copy_bytes(tmp, blob.data, 56);
    p = tmp;
    r = write_data(p, s_16, n, got);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    if (got != n) {
        SC_ERR(err, 0x82);
        status = 259;
        return r;
    }
    out_len = 0;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::write_data(const quad_ptr &data, const quad_uint &offset, const quad_uint &len,
                                  quad_uint &out_len)
{
    unsigned int err = 0x81011b00;
    sc_cmd_0d cmd;

    out_len = 0;
    if (data == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    if (len == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    if (offset + len > 1024) {
        SC_ERR(err, 0x07);
        return 9;
    }
    u8 saved[len];
    memcpy(saved, data, len);
    if (m_176 != 0) {
        if (encrypt_sectors(data, offset, len) != 0) {
            SC_ERR(err, 0x01);
            return 15;
        }
    }
    if (cmd.set(data, offset, len) != 0) {
        SC_ERR(err, 0x07);
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    quad_ptr p;
    quad_uint got;
    got = 0;
    if (cmd.get(got) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    memcpy(data, saved, len);
    u8 check[len];
    quad_uint n;
    n = 0;
    p = check;
    if (read_data(p, offset, len, n) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    unsigned int diff = 0;
    for (unsigned int i = 0; i < len; i++)
        if (check[i] != saved[i])
            diff++;
    if (diff != 0) {
        err |= 0x81;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(diff, 10);
        get_log()->puts(" / ");
        get_log()->put((unsigned int)len, 10);
        get_log()->puts("\n");
        return 16;
    }
    out_len = got;
    return 0;
}

unsigned int sc_iso_module::write_slot_data(const quad_ptr &buf, const quad_uint &offset,
                                  const quad_uint &len, const unsigned int &seq,
                                  quad_uint &out_len)
{
    unsigned int err = 0x81011d00;
    sc_cmd_0d cmd(seq);

    out_len = 0;
    if (buf == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    if (len == 0) {
        SC_ERR(err, 0x07);
        return 9;
    }
    u8 data[len];
    memcpy(data, buf, len);
    if (m_176 != 0) {
        if (encrypt_sectors(buf, offset, len) != 0) {
            SC_ERR(err, 0x01);
            return 15;
        }
    }
    if (cmd.set(buf, offset, len) != 0) {
        SC_ERR(err, 0x07);
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    quad_ptr p;
    quad_uint n;
    n = 0;
    if (cmd.get(n) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    memcpy(buf, data, len);
    u8 back[len];
    quad_uint n2;
    n2 = 0;
    p = back;
    if (read_slot_data(p, offset, len, seq, n2) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    unsigned int diff = 0;
    for (unsigned int i = 0; i < len; i++)
        if (back[i] != data[i])
            diff++;
    if (diff != 0) {
        err |= 0x81;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(diff, 10);
        get_log()->puts(" / ");
        get_log()->put((unsigned int)len, 10);
        get_log()->puts("\n");
        return 16;
    }
    out_len = n;
    return 0;
}

unsigned int sc_iso_module::encrypt_sectors(const quad_ptr &buf, const quad_uint &offset,
                                  const quad_uint &len)
{
    unsigned int err = 0x81012d00;
    unsigned int ret;

    if (offset % 16 != 0 || len % 16 != 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else {
        unsigned int sector = offset / 16 + m_id * 64;
        unsigned int n = len / 16;
        u8 in[16] __attribute__((aligned(16)));
        u8 out[16] __attribute__((aligned(16)));
        u8 tweak[16] __attribute__((aligned(16)));
        u8 key1[16] __attribute__((aligned(16)));
        u8 key2[16] __attribute__((aligned(16)));
        quad_uint key_len = 16;
        sc_crypto crypto(SC_CRYPTO_XTS);

        if (get_key(key_id_07, key1, key_len) != 0 || (ret = get_key(key_id_08, key2, key_len)) != 0) {
            ret = 9;
            SC_ERR(err, 0x01);
        } else {
            for (unsigned int i = 0; i < n; i++) {
                make_tweak(tweak, 16, sector + i);
                memcpy(in, buf + i * 16, 16);
                get_log()->puts("encrypting sector #");
                get_log()->put((u64)(sector + i), 10);
                get_log()->puts("\n");
                ret = xts_encrypt(&crypto, in, 16, out, 16, key1, 128, key2, 128, tweak, 128);
                if (ret != 0) {
                    SC_ERR(err, 0x01);
                    break;
                }
                memcpy(buf + i * 16, out, 16);
            }
        }
    }
    return ret;
}

unsigned int sc_iso_module::cmd_get_sc_status(const quad_ptr &in, const quad_uint &in_len,
                                   const quad_ptr &out, const quad_uint &size,
                                   quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010f00;
    unsigned int ret;

    out_len = 0;
    status = 259;
    if (in_len != 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (size < 16) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (out == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else {
        unsigned int mngblk[4] __attribute__((aligned(16)));
        ret = read_mngblk((u8 *)mngblk);
        if (ret != 0) {
            SC_ERR(err, 0x01);
            status = 259;
        } else {
            sc_reply_10 reply __attribute__((aligned(16)));
            memcpy(&reply, mngblk, 16);
            memset(reply.reserved, 0, sizeof(reply.reserved));
            memcpy(out, &reply, sizeof(reply));
            out_len = sizeof(reply);
            status = 256;
            get_log()->puts("mngblk = (");
            get_log()->put(mngblk[0], 16);
            get_log()->puts(", ");
            get_log()->put(mngblk[1], 16);
            get_log()->puts(")\n");
            if (m_176 != 0 && mngblk[0] != 3 && mngblk[0] != 0) {
                ret = 27;
                SC_ERR(err, 0x89);
                status = 266;
            }
        }
    }
    return ret;
}

unsigned int sc_iso_module::read_mngblk(void *buf)
{
    quad_ptr p;
    quad_uint size;
    quad_uint n;

    size = 16;
    n = 0;
    p = (u8 *)buf;
    unsigned int ret = read_data(p, mngblk_zero, size, n);
    if (ret == 0 && n != size)
        ret = 15;
    return ret;
}

unsigned int sc_iso_module::cmd_get_srh(const quad_ptr &in, const quad_uint &in_len,
                                   const quad_ptr &out, const quad_uint &size,
                                   quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010600;
    unsigned int ret;

    out_len = 0;
    status = 259;
    if (in_len != 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (size < 48) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (out == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else {
        quad_uint n;
        quad_ptr p;
        u8 data[40] __attribute__((aligned(16)));
        n = 40;
        p = data;
        ret = read_srh(p, n);
        if (ret != 0) {
            SC_ERR(err, 0x01);
            status = 259;
        } else {
            sc_reply_04 reply __attribute__((aligned(16)));
            memcpy(reply.data, data, n);
            memset(reply.reserved, 0, sizeof(reply.reserved));
            memcpy(out, &reply, sizeof(reply));
            out_len = sizeof(reply);
            status = 256;
        }
    }
    return ret;
}

unsigned int sc_iso_module::read_srh(const quad_ptr &buf, const quad_uint &len)
{
    unsigned int ret = 9;

    if (buf != 0 && len == 40) {
        u8 data[48] __attribute__((aligned(16)));
        quad_ptr p = data;
        quad_uint size = 48;
        quad_uint n = 0;
        ret = read_data(p, mngblk_offset, size, n);
        if (ret == 0) {
            if (n != size)
                ret = 15;
            else
                memcpy(buf, data, len);
        }
    }
    return ret;
}

unsigned int sc_iso_module::cmd_get_region_data(const quad_ptr &in, const quad_uint &in_len,
                                   const quad_ptr &out, const quad_uint &size,
                                   quad_uint &out_len, unsigned int &status)
{
    unsigned int err = 0x81010900;
    unsigned int ret;

    out_len = 0;
    status = 259;
    if (in_len != 16) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (size < 64) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (in == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else if (out == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
        status = 257;
    } else {
        quad_uint n;
        quad_ptr p;
        quad_u64 id;
        u8 data[48] __attribute__((aligned(16)));
        sc_reply_07 reply __attribute__((aligned(16)));

        id = ((const sc_request_07 *)in)->id;
        if (id <= 15) {
            n = 48;
            p = data;
            ret = read_record(id, p, n);
            if (ret != 0) {
                SC_ERR(err, 0x01);
                status = 259;
            } else {
                reply.id = id;
                memcpy(reply.data, data, sizeof(reply.data));
                memset(reply.reserved, 0, sizeof(reply.reserved));
                memcpy(out, &reply, sizeof(reply));
                out_len = sizeof(reply);
                status = 256;
            }
        } else {
            ret = 9;
            SC_ERR(err, 0x81);
            status = 257;
        }
    }
    return ret;
}

unsigned int sc_iso_module::read_record(const u64 &id, const quad_ptr &buf, const quad_uint &len)
{
    unsigned int err = 0x81011800;
    unsigned int ret = 9;

    if (buf != 0 && len == 48) {
        if (id <= 15) {
            quad_uint offset = id * 48 + 128;
            quad_uint n = 0;
            ret = read_data(buf, offset, len, n);
            if (ret == 0 && n != len)
                ret = 15;
        } else {
            err |= 0x81;
            get_log()->put(err, 16);
            get_log()->puts(", ");
            get_log()->put(id, 16);
            get_log()->puts("\n");
            ret = 9;
        }
    }
    return ret;
}

unsigned int sc_iso_module::read_data(const quad_ptr &buf, const quad_uint &offset,
                                  const quad_uint &len, quad_uint &out_len)
{
    unsigned int err = 0x81011a00;
    unsigned int ret;
    sc_cmd_08 cmd;

    out_len = 0;
    if (buf == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else if (len == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else if (offset + len > 1024) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else if (cmd.set(offset, len) != 0) {
        ret = 15;
        SC_ERR(err, 0x07);
    } else if (exec(&cmd) != 0) {
        ret = 15;
        SC_ERR(err, 0x01);
    } else {
        quad_uint n = 0;
        if (cmd.get(buf, len, n) != 0
            || (m_176 != 0 && decrypt_sectors(buf, offset, len) != 0)) {
            ret = 15;
            SC_ERR(err, 0x01);
        } else {
            out_len = n;
            ret = 0;
        }
    }
    return ret;
}

unsigned int sc_iso_module::read_slot_data(const quad_ptr &buf, const quad_uint &offset,
                                  const quad_uint &len, const unsigned int &seq,
                                  quad_uint &out_len)
{
    unsigned int err = 0x81011c00;
    unsigned int ret;
    sc_cmd_08 cmd(seq);

    out_len = 0;
    if (buf == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else if (len == 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else if (cmd.set(offset, len) != 0) {
        ret = 15;
        SC_ERR(err, 0x07);
    } else if (exec(&cmd) != 0) {
        ret = 15;
        SC_ERR(err, 0x01);
    } else {
        quad_uint n = 0;
        if (cmd.get(buf, len, n) != 0
            || (m_176 != 0 && decrypt_sectors(buf, offset, len) != 0)) {
            ret = 15;
            SC_ERR(err, 0x01);
        } else {
            out_len = n;
            ret = 0;
        }
    }
    return ret;
}

unsigned int sc_iso_module::decrypt_sectors(const quad_ptr &buf, const quad_uint &offset,
                                  const quad_uint &len)
{
    unsigned int err = 0x81012e00;
    unsigned int ret;

    if (offset % 16 != 0 || len % 16 != 0) {
        ret = 9;
        SC_ERR(err, 0x07);
    } else {
        unsigned int sector = offset / 16 + m_id * 64;
        unsigned int n = len / 16;
        u8 in[16] __attribute__((aligned(16)));
        u8 out[16] __attribute__((aligned(16)));
        u8 tweak[16] __attribute__((aligned(16)));
        u8 key1[16] __attribute__((aligned(16)));
        u8 key2[16] __attribute__((aligned(16)));
        quad_uint key_len = 16;
        sc_crypto crypto(SC_CRYPTO_XTS);

        if (get_key(key_id_07, key1, key_len) != 0 || (ret = get_key(key_id_08, key2, key_len)) != 0) {
            ret = 9;
            SC_ERR(err, 0x01);
        } else {
            for (unsigned int i = 0; i < n; i++) {
                make_tweak(tweak, 16, sector + i);
                memcpy(in, buf + i * 16, 16);
                get_log()->puts("decrypting sector #");
                get_log()->put((u64)(sector + i), 10);
                get_log()->puts("\n");
                ret = xts_decrypt(&crypto, in, 16, out, 16, key1, 128, key2, 128, tweak, 128);
                if (ret != 0) {
                    SC_ERR(err, 0x01);
                    break;
                }
                memcpy(buf + i * 16, out, 16);
            }
        }
    }
    return ret;
}

unsigned int sc_iso_module::get_key(const u64 &id, const quad_ptr &buf, quad_uint &len)
{
    unsigned int err = 0x81011600;
    unsigned int r;
    unsigned int offset;
    unsigned int out;

    if (buf == 0 || len != 16)
        return 9;
    offset = 0;
    switch (id) {
    case 1:
        offset = 16;
        out = 0;
        r = read_data(buf, offset, len, out);
        if (r == 0)
            goto check;
        return r;
    case 2:
        memcpy(buf, key_2, len);
        return 0;
    case 3:
        memcpy(buf, key_3, len);
        return 0;
    case 4:
        memcpy(buf, key_4, len);
        return 0;
    case 5:
        return encrypt_seed(buf, len, seed_5);
    case 6:
        return encrypt_seed(buf, len, seed_6);
    case 7:
        return encrypt_seed(buf, len, seed_7);
    case 8:
        return encrypt_seed(buf, len, seed_8);
    default:
        err |= 0x81;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(id, 16);
        get_log()->puts("\n");
        return 9;
    }
check:
    if (out != len)
        return 15;
    return 0;
}

unsigned int sc_iso_module::encrypt_seed(const quad_ptr &buf, const quad_uint &len, const u8 *data)
{
    unsigned int err = 0x81011f00;
    unsigned int r = 9;
    sc_crypto c(SC_CRYPTO_CBC);
    unsigned int n;
    u8 in[16];
    u8 key[16];
    u8 iv[16];
    u8 out[16];

    if (buf == 0 || (n = len) != 16) {
        SC_ERR(err, 0x07);
    } else {
        memset(out, 0, 16);
        memcpy(in, data, 16);
        memcpy(key, m_keys, 16);
        memcpy(iv, m_key2, 16);
        r = cbc_encrypt(&c, in, n, out, n, key, 128, iv, 128);
        memcpy(buf, out, len);
    }
    return r;
}

unsigned int sc_iso_module::cmd_rekey(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len,
                                   unsigned int &status)
{
    unsigned int err = 0x81010e00;
    unsigned int r;
    u8 mode;
    unsigned int ver;
    u8 data[32];
    u8 v;
    unsigned int n;

    out_len = 0;
    status = 259;
    if ((n = len) != 32) {
        SC_ERR(err, 0x07);
        r = 9;
        status = 257;
        return r;
    }
    if (buf == 0) {
        SC_ERR(err, 0x07);
        r = 9;
        status = 257;
        return r;
    }
    memcpy(data, buf, 32);
    m_id = 6;
    ver = 0;
    if (get_sc_version((u8 *)&ver) != 0) {
        r = 15;
        get_log()->puts("[ERROR]: cannot get system version\n");
        status = 259;
        return r;
    }
    v = ver > 2 ? 0x31 : 0x21;
    mode = v;
    if ((r = set_state_byte(mode, status)) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    if ((r = install_key_material((unsigned int)data, (unsigned int)n, status)) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    mode = 0xff;
    if ((r = get_state_byte(mode, status)) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    if (mode != v) {
        r = 15;
        err |= 0x81;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(mode, 10);
        get_log()->puts("\n");
        status = 259;
        return r;
    }
    out_len = 0;
    status = 256;
    return r;
}

unsigned int sc_iso_module::cmd_pmode_srv_auth(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len,
                                   unsigned int &status)
{
    unsigned int err = 0x81013100;
    unsigned int r;
    unsigned int n;
    quad_uint rlen;
    unsigned int cmd;
    unsigned int token;
    u8 rnd[20];
    op32_msg resp;
    op32_msg req;
    op33_msg reply;
    op33_msg result;

    out_len = 0;
    status = 259;
    if ((n = len) != 80) {
        r = 9;
        SC_ERR(err, 0x81);
        status = 257;
        return r;
    }
    if (buf == 0) {
        r = 9;
        SC_ERR(err, 0x82);
        status = 257;
        return r;
    }
    if (size < 80) {
        r = 9;
        SC_ERR(err, 0x83);
        status = 257;
        return r;
    }
    if (out == 0) {
        r = 9;
        SC_ERR(err, 0x84);
        status = 257;
        return r;
    }
    memcpy(&req, buf, sizeof(req));
    r = pmode_make_challenge(&req, &resp, rnd, sizeof(rnd), status);
    if (r != 0) {
        SC_ERR(err, 0x85);
        return r;
    }
    token = 129;
    cmd = 256;
    memcpy(out, &resp, sizeof(resp));
    if (m_dma->put(token, cmd, (unsigned int)n, out) != 0) {
        r = 15;
        SC_ERR(err, 0x86);
        status = 259;
        return r;
    }
    if (m_dma->get(out, size, token, cmd, rlen) != 0) {
        r = 15;
        SC_ERR(err, 0x87);
        status = 259;
        return r;
    }
    memcpy(&reply, out, sizeof(reply));
    r = pmode_check_response(&reply, &result, rnd, sizeof(rnd), status);
    if (r != 0) {
        SC_ERR(err, 0x88);
        return r;
    }
    memcpy(out, &result, sizeof(result));
    out_len = n;
    status = 256;
    return r;
}

unsigned int sc_iso_module::pmode_check_response(const op33_msg *in, op33_msg *out, u8 *rnd, unsigned int rnd_len,
                                  unsigned int &status)
{
    unsigned int err = 0x81013300;
    bool ok = false;
    unsigned int r;
    int ret;
    u8 digest[20];
    u8 data[20];
    u8 sig[64] __attribute__((aligned(16)));
    op33_body body;

    if (rnd == 0) {
        SC_ERR(err, 0x81);
        r = 27;
        status = 266;
        goto done;
    }
    if (rnd_len != 20) {
        SC_ERR(err, 0x82);
        r = 27;
        status = 266;
        goto done;
    }
    if (in->magic != 0xD1) {
        SC_ERR(err, 0x83);
        r = 27;
        status = 266;
        goto done;
    }
    if (in->type != 2) {
        SC_ERR(err, 0x84);
        r = 27;
        status = 266;
        goto done;
    }
    if (in->ver == 0) {
        SC_ERR(err, 0x85);
        r = 0;
        status = 256;
        goto done;
    }
    if (in->ver != 2) {
        SC_ERR(err, 0x86);
        r = 27;
        status = 266;
        goto done;
    }
    if (in->b4 != 0) {
        SC_ERR(err, 0x87);
        r = 27;
        status = 266;
        goto done;
    }
    if (in->b5 != 0) {
        SC_ERR(err, 0x88);
        r = 27;
        status = 266;
        goto done;
    }
    memcpy(&body, in->body, sizeof(body));
    if (body.b0 != 32) {
        SC_ERR(err, 0x89);
        r = 27;
        status = 266;
        goto done;
    }
    if (body.b1 != 1) {
        SC_ERR(err, 0x8a);
        r = 27;
        status = 266;
        goto done;
    }
    if (body.b2 != 0) {
        SC_ERR(err, 0x8b);
        r = 27;
        status = 266;
        goto done;
    }
    memcpy(sig, body.sig, 48);
    ret = aes_cbc_decrypt((vec_uchar16 *)sig, (const vec_uchar16 *)sig, 48, g_op33_key,
                          128, (const vec_uchar16 *)g_op33_iv);
    if (ret != 0) {
        SC_ERR_RET(err, 0x8c, ret);
        r = 15;
        status = 259;
        goto done;
    }
    memcpy(body.sig, sig, 48);
    memcpy(data, rnd, sizeof(data));
    ret = sha1_digest(digest, data, sizeof(data));
    if (ret != 0) {
        SC_ERR_RET(err, 0x8d, ret);
        r = 15;
        status = 259;
        goto done;
    }
    ok = ecdsa_verify_id(body.sig, digest, g_op33_pub, 29) == 0;
    status = 256;
    r = 0;
done:
    memset(out, 0, sizeof(*out));
    out->b0 = ok;
    get_log()->puts("is_ok: ");
    get_log()->put((int)ok, 10);
    get_log()->puts("\n");
    return r;
}

unsigned int sc_iso_module::cmd_get_time(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len,
                                   unsigned int &status)
{
    unsigned int err = 0x81010c00;
    unsigned int r;
    unsigned int e;
    u8 v;
    unsigned int ver;
    u64 flags;
    u64 value;
    u64 mode;
    u64 res[4];
    u8 want;

    out_len = 0;
    status = 259;
    if (len != 16) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (size < 32) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (buf == 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (out == 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    mode = ((const cmd_0c_req *)buf)->mode;
    if (mode > 1)
        goto bad_mode;
    v = 0xff;
    if (get_state_byte(v, status) != 0) {
        r = 15;
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    value = 0;
    ver = 0;
    if (get_sc_version((u8 *)&ver) != 0) {
        r = 15;
        get_log()->puts("[ERROR]: cannot get system version\n");
        status = 259;
        return r;
    }
    want = ver > 2 ? 0x31 : 0x21;
    flags = 0;
    if ((v & 0x2f) != (want & 0x2f)) {
        if (v & 0x80)
            flags = 8;
        if (v & 0x40)
            flags |= 1;
        r = 15;
        err |= 0x05;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)v, 16);
        get_log()->puts(", ");
        get_log()->puts("status: ");
        get_log()->put((unsigned int)flags, 16);
        get_log()->puts("\n");
        res[0] = mode;
        res[1] = value;
        res[2] = flags;
        memcpy(out, res, sizeof(res));
        out_len = 32;
        status = 259;
        return r;
    }
    r = read_time(mode, value, flags, status);
    if (r != 0) {
        e = sc_lib_error();
        if (e == 0x80000414)
            flags |= 2;
        else if (e == 0x8000041c)
            flags |= 4;
        err |= 0x01;
        get_log()->put(err, 16);
        get_log()->puts(", ");
        get_log()->put(r, 10);
        r = 15;
        get_log()->puts(", ");
        get_log()->put(e, 16);
        get_log()->puts(", ");
        get_log()->put((unsigned int)flags, 16);
        get_log()->puts("\n");
        status = 259;
    } else {
        status = 256;
    }
    res[0] = mode;
    res[1] = value;
    res[2] = flags;
    memcpy(out, res, sizeof(res));
    out_len = 32;
    return r;
bad_mode:
    r = 9;
    SC_ERR(err, 0x81);
    status = 257;
    return r;
}

unsigned int sc_iso_module::cmd_set_time(const quad_ptr &buf, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len,
                                   unsigned int &status)
{
    unsigned int err = 0x81010d00;
    unsigned int r;
    u64 mode;
    u64 time;
    u64 value;
    u64 value2;
    s64 diff;
    u64 res[2];

    out_len = 0;
    status = 259;
    if (len != 16) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (size < 16) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (buf == 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    if (out == 0) {
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    {
        const cmd_11_req req = *(const cmd_11_req *)buf;
        mode = req.mode;
        time = req.time;
    }
    switch (mode) {
    case 0:
        m_id = 0;
        break;
    case 1:
        m_id = 1;
        break;
    default:
        r = 9;
        SC_ERR(err, 0x07);
        status = 257;
        return r;
    }
    value = 0;
    value2 = 0;
    r = read_time(g_mode_ff, value, value2, status);
    if (r != 0)
        goto fail;
    if (value2 != 0) {
        SC_ERR(err, 0x81);
        status = 259;
        return r;
    }
    diff = 0;
    r = time_diff(time, value, (u64 &)diff);
    if (r != 0) {
        SC_ERR(err, 0x01);
        status = 257;
        return r;
    }
    r = set_time_offset(mode, diff, status);
    if (r != 0) {
fail:
        SC_ERR(err, 0x01);
        status = 259;
        return r;
    }
    res[0] = mode;
    memset(&res[1], 0, sizeof(res[1]));
    memcpy(out, res, sizeof(res));
    out_len = 16;
    status = 256;
    return r;
}

unsigned int sc_iso_module::read_time(const u64 &mode, u64 &value, u64 &value2,
                                  unsigned int &status)
{
    unsigned int err = 0x81012000;
    sc_cmd_07 cmd;

    status = 259;
    if (cmd.set(mode) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get(value, value2) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::set_time_offset(const u64 &mode, const s64 &value, unsigned int &status)
{
    unsigned int err = 0x81012100;
    sc_cmd_09 cmd;

    status = 259;
    if (cmd.set(mode, value) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get() != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::install_key_material(const quad_uint &data, const quad_uint &len,
                                  unsigned int &status)
{
    unsigned int err = 0x81012200;
    sc_cmd_0b cmd;

    status = 259;
    if (cmd.set(data, len) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get() != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::get_state_byte(u8 &a, unsigned int &status)
{
    unsigned int err = 0x81012500;
    u8 b;
    sc_cmd_05 cmd;

    b = 0;
    status = 259;
    if (cmd.set() != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get(a, b) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::set_state_byte(const u8 &v, unsigned int &status)
{
    unsigned int err = 0x81012700;
    sc_cmd_0c cmd;

    status = 259;
    if (cmd.set(v) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get() != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::derive_auth_keys()
{
    unsigned int err = 0x81010400;
    unsigned int r;

    r = derive_auth_key(g_116752, g_116144);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    r = derive_auth_key(g_116768, g_116160);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    r = derive_auth_key(g_116784, g_116176);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    r = derive_auth_key(g_116800, g_116192);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    r = derive_auth_key(g_116816, g_116208);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    r = derive_auth_key(g_116832, g_116224);
    if (r != 0) {
        SC_ERR(err, 0x01);
        return r;
    }
    return r;
}

unsigned int sc_iso_module::derive_auth_key(u8 *out, const u8 *in)
{
    unsigned int err = 0x81010500;
    u8 iv[16] __attribute__((aligned(16))) = {0};
    int r;

    r = aes_cbc_encrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, 16, m_keys, 256,
                        (const vec_uchar16 *)iv);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    return 0;
}

unsigned int sc_iso_module::exec(sc_cmd *cmd)
{
    return exec_cmd(cmd, m_152);
}

unsigned int sc_iso_module::cmd_get_property(const quad_ptr &in, const quad_uint &len, const quad_ptr &out,
                                   const quad_uint &size, quad_uint &out_len,
                                   unsigned int &status)
{
    unsigned int err = 0x81013000;
    unsigned int ver;
    u8 state[16];
    u64 info[2];
    u64 flags;

    out_len = 0;
    status = 259;
    if (len != 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (size < 16) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    if (out == 0) {
        SC_ERR(err, 0x07);
        status = 257;
        return 9;
    }
    ver = 0;
    if (get_sc_version((u8 *)&ver) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    flags = ver > 3;
    if (get_syscon_state(state) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (bytes_differ(state, g_state_116448, 16) == 0)
        flags |= 2;
    info[0] = flags;
    memset(&info[1], 0, sizeof(info[1]));
    memcpy(out, info, sizeof(info));
    out_len = 16;
    status = 256;
    return 0;
}

unsigned int sc_iso_module::get_sc_version(u8 *out)
{
    unsigned int status = 259;
    u8 data[128];

    if (read_sc_info(data, status) != 0)
        return 15;
    memcpy(out, data, 4);
    return 0;
}

unsigned int sc_iso_module::exec_plain(sc_cmd *cmd)
{
    return exec_cmd(cmd, true);
}

unsigned int sc_iso_module::exec_cmd(sc_cmd *cmd, u8 authed)
{
    unsigned int err = 0x81012400;
    quad_ptr buf = m_buf;
    quad_uint max = m_size;
    quad_uint psize = 16;
    quad_uint len = 0;
    quad_uint reply_len = 0;
    quad_uint rlen = 0;
    quad_uint token = 129;
    quad_uint cmd_no = 131;
    u8 *pkt = buf + 16;
    unsigned int room = max - 16;
    sc_proxy_hdr proxy;
    quad_uint hdr;
    quad_uint plen;
    unsigned int r;

    if (!authed) {
        if (open_session() != 0) {
            SC_ERR(err, 0x01);
            get_log()->puts("Trying again with old key...\n");
            if (use_old_auth_keys() != 0) {
                SC_ERR(err, 0x01);
                return 15;
            }
            if (open_session() != 0) {
                SC_ERR(err, 0x01);
                return 15;
            }
        }
    }
    hdr = 16;
    do {
        if (cmd->make(pkt + hdr, room - hdr, len, reply_len, m_id, &m_ctx) != 0) {
            SC_ERR(err, 0x80);
            return 15;
        }
        if (m_pkt->make_hdr(pkt, hdr, len) != 0) {
            SC_ERR(err, 0x81);
            return 15;
        }
        if (proxy.make_hdr(len + hdr, reply_len, buf, psize) != 0) {
            SC_ERR(err, 0x82);
            return 15;
        }
        if (m_dma->put(token, cmd_no, psize + hdr + len, buf) != 0) {
            SC_ERR(err, 0x83);
            return 15;
        }
        if (m_dma->get(buf, max, token, cmd_no, rlen) != 0) {
            SC_ERR(err, 0x84);
            return 15;
        }
        if (token != 2) {
            SC_ERR(err, 0x85);
            return 15;
        }
        if (cmd_no != 131) {
            SC_ERR(err, 0x86);
            return 15;
        }
        if (proxy.verify_hdr(buf, psize, len, reply_len) != 0) {
            SC_ERR(err, 0x87);
            return 15;
        }
        plen = 0;
        if (m_pkt->verify_hdr(pkt, hdr, plen) != 0) {
            SC_ERR(err, 0x88);
            return 15;
        }
        r = cmd->check(pkt + hdr, reply_len - hdr, m_id, &m_ctx);
        if (r != 0 && r != 23) {
            SC_ERR(err, 0x89);
            return 15;
        }
    } while (r == 23);
    return 0;
}

unsigned int sc_iso_module::open_session()
{
    unsigned int err = 0x81010200;
    const u8 *key1;
    const u8 *key2;

    switch ((int)m_id) {
    case 1:
        key1 = g_116784;
        key2 = g_116800;
        break;
    case 6:
        key1 = g_116816;
        key2 = g_116832;
        break;
    case 0:
        key1 = g_116752;
        key2 = g_116768;
        break;
    default:
        SC_ERR(err, 0x05);
        return 15;
    }
    return authenticate(m_id, key1, key2, &m_ctx);
}

unsigned int sc_iso_module::pmode_make_challenge(const op32_msg *in, op32_msg *out, u8 *rnd,
                                  unsigned int rnd_len, unsigned int &status)
{
    unsigned int err = 0x81013200;
    unsigned int r;
    op32_body body;
    op32_body resp;
    u8 enc[64] __attribute__((aligned(16)));
    int ret;

    memset(out, 0, sizeof(*out));
    if (rnd == 0) {
        r = 27;
        SC_ERR(err, 0x81);
        status = 266;
        return r;
    }
    if (rnd_len != 20) {
        r = 27;
        SC_ERR(err, 0x82);
        status = 266;
        return r;
    }
    if (in->magic != 0xD1) {
        r = 27;
        SC_ERR(err, 0x83);
        status = 266;
        return r;
    }
    if (in->b1 != 2) {
        r = 27;
        SC_ERR(err, 0x84);
        status = 266;
        return r;
    }
    if (in->b2 != 1) {
        r = 27;
        SC_ERR(err, 0x85);
        status = 266;
        return r;
    }
    memcpy(&body, in->body, sizeof(body));
    if (body.b0 != 16) {
        r = 27;
        SC_ERR(err, 0x86);
        status = 266;
        return r;
    }
    if (body.b1 != 0) {
        r = 27;
        SC_ERR(err, 0x87);
        status = 266;
        return r;
    }
    if (body.b2 != 0) {
        r = 27;
        SC_ERR(err, 0x88);
        status = 266;
        return r;
    }
    resp.b0 = 32;
    resp.b1 = 16;
    resp.b2 = 0;
    resp.b3 = 0;
    r = get_random(resp.data, 20, status);
    if (r != 0) {
        SC_ERR(err, 0x89);
        return r;
    }
    memset(resp.data + 20, 0, 28);
    memcpy(rnd, resp.data, 20);
    r = get_random(resp.data + 48, 12, status);
    if (r != 0) {
        SC_ERR(err, 0x8A);
        return r;
    }
    memcpy(enc, resp.data, 48);
    int (*encrypt)(vec_uchar16 *, const vec_uchar16 *, int, const unsigned char *, int,
                   const vec_uchar16 *) = aes_cbc_encrypt;
    ret = encrypt((vec_uchar16 *)enc, (const vec_uchar16 *)enc, 48, op32_key, 128,
                  (const vec_uchar16 *)op32_iv);
    if (ret != 0) {
        SC_ERR_RET(err, 0x8B, ret);
        r = 15;
        status = 259;
        return r;
    }
    memcpy(resp.data, enc, 48);
    out->magic = 0xD1;
    out->b1 = 2;
    out->b2 = 1;
    memcpy(out->body, &resp, sizeof(resp));
    status = 256;
    return r;
}

unsigned int sc_iso_module::authenticate(unsigned int id, const u8 *key1, const u8 *key2, void *ctx)
{
    unsigned int err = 0x81010300;
    quad_ptr buf = m_buf;
    quad_uint max = m_size;
    quad_uint psize = 16;
    quad_uint hdr = 16;
    quad_uint a = 0;
    quad_uint b = 0;
    quad_uint rlen = 0;
    quad_uint token = 129;
    quad_uint cmd = 131;
    u8 *base = m_buf;
    unsigned int room = m_size - 32;
    sc_proxy_hdr proxy;
    u8 rnd[16] __attribute__((aligned(16)));
    vec_uchar16 sess;
    quad_uint len;
    quad_uint reply_len;
    sc_buf reply;
    sc_buf req;
    int r;
    int i;

    for (i = 0; i < 16; i++)
        rnd[i] = spu_readch(74) >> 24;
    len = 0;
    reply_len = 0;
    req.addr = base + 32;
    req.size = room;
    req.len = &len;
    reply.addr = base + 32;
    reply.size = room;
    sess = spu_splats((u8)0xff);

    get_log()->puts("AUTH1\n");
    r = sc_make_auth1(&req, id, key1, rnd);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    if (m_pkt->make_hdr(buf + psize, hdr, len) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    reply_len = hdr + REPLY_LEN;
    if (proxy.make_hdr(len + hdr, reply_len, buf, psize) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    if (m_dma->put(token, cmd, len + hdr + psize, buf) != 0) {
        SC_ERR(err, 0x04);
        return 15;
    }
    if (m_dma->get(buf, max, token, cmd, rlen) != 0) {
        SC_ERR(err, 0x03);
        return 15;
    }
    if (token != 2) {
        SC_ERR(err, 0x05);
        return 15;
    }
    if (cmd != 131) {
        SC_ERR(err, 0x06);
        return 15;
    }
    if (proxy.verify_hdr(buf, psize, a, b) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    quad_uint plen = 0;
    if (m_pkt->verify_hdr(buf + psize, hdr, plen) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    reply.size = b - hdr;
    r = sc_check_auth1(&reply, id, key2, rnd, (u8 *)&sess);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }

    get_log()->puts("AUTH2\n");
    r = sc_make_auth2(&req, id, key1, (u8 *)&sess);
    if (r != 0) {
        SC_ERR_RET(err, 0x01, r);
        return 15;
    }
    if (m_pkt->make_hdr(buf + psize, hdr, len) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    reply_len = hdr + REPLY_LEN;
    if (proxy.make_hdr(len + hdr, reply_len, buf, psize) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    if (m_dma->put(token, cmd, len + hdr + psize, buf) != 0) {
        SC_ERR(err, 0x04);
        return 15;
    }
    if (m_dma->get(buf, max, token, cmd, rlen) != 0) {
        SC_ERR(err, 0x03);
        return 15;
    }
    if (token != 2) {
        SC_ERR(err, 0x05);
        return 15;
    }
    if (cmd != 131) {
        SC_ERR(err, 0x06);
        return 15;
    }
    if (proxy.verify_hdr(buf, psize, a, b) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    if (m_pkt->verify_hdr(buf + psize, hdr, plen) != 0) {
        SC_ERR(err, 0x01);
        return 15;
    }
    reply.size = b - hdr;
    r = sc_check_auth2(&reply, id, 0, rnd, &sess, (u8 *)ctx);
    if (r != 0) {
        SC_ERR_LIB(err, 0x01, r);
        return 15;
    }
    get_log()->puts("succeeded\n");
    m_152 = 1;
    return 0;
}

unsigned int sc_iso_module::read_sc_info(u8 *data, unsigned int &status)
{
    unsigned int err = 0x81012a00;
    sc_cmd_04 cmd;

    status = 259;
    if (cmd.set() != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (exec_plain(&cmd) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    if (cmd.get(data) != 0) {
        SC_ERR(err, 0x01);
        status = 259;
        return 15;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::get_syscon_state(u8 *state)
{
    quad_uint token;
    quad_uint pkt_a;
    quad_ptr addr;
    quad_uint max;
    quad_uint proxy_size;
    quad_uint pkt_size;
    quad_uint pkt_len;
    quad_uint proxy_a;
    quad_uint proxy_b;
    quad_uint cmd;
    quad_uint msg_len;
    quad_uint len;
    quad_uint pkt_b;
    quad_uint reply_len;
    unsigned int ret;
    int r;
    unsigned int u;

    addr = (unsigned char *)0x3E000;
    max = 4096;
    proxy_size = 16;
    pkt_size = 16;
    proxy_a = 0;
    proxy_b = 0;
    reply_len = 0;
    token = 129;
    cmd = 131;
    sc_proxy_hdr proxy;
    sc_buf reply;
    sc_buf req;

    req.addr = (unsigned char *)0x3E020;
    req.size = 4128;
    req.len = &len;
    reply.addr = (unsigned char *)0x3E020;
    reply.size = 4128;
    len = 0;
    pkt_b = 0;
    r = sc_create_get_state_cmd(&req);
    if (r != 0) {
        ret = 15;
        GSS_LOG("secure_com_lib::create_get_state_cmd failed: ");
        get_log()->puts("ret = ");
        get_log()->put(r, 10);
        get_log()->puts("\n");
        return ret;
    }
    u = m_pkt->make_hdr(addr + proxy_size, pkt_size, len);
    if (u != 0) {
        ret = 15;
        GSS_LOG("sc_pkt_hdr::make_hdr failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    pkt_b = pkt_size + 688;
    pkt_a = len + pkt_size;
    u = proxy.make_hdr(pkt_a, pkt_b, addr, proxy_size);
    if (u != 0) {
        ret = 15;
        GSS_LOG("sc_proxy_hdr::make_hdr failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    msg_len = len + pkt_size + proxy_size;
    u = m_dma->put(token, cmd, msg_len, addr);
    if (u != 0) {
        ret = 15;
        GSS_LOG("ss_iso_dma_data::put failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    u = m_dma->get(addr, max, token, cmd, reply_len);
    if (u != 0) {
        ret = 15;
        GSS_LOG("ss_iso_dma_data::get failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    if (token != 2) {
        ret = 15;
        GSS_LOG("ss_iso_dma_data::get ");
        get_log()->puts("returns invalid mbmsg: ");
        get_log()->put(token, 16);
        get_log()->puts("\n");
        return ret;
    }
    if (cmd != 131) {
        ret = 15;
        GSS_LOG("ss_iso_dma_data::get ");
        get_log()->puts("returns invalid cmd: ");
        get_log()->put(cmd, 16);
        get_log()->puts("\n");
        return ret;
    }
    u = proxy.verify_hdr(addr, proxy_size, proxy_a, proxy_b);
    if (u != 0) {
        ret = 15;
        GSS_LOG("sc_proxy_hdr::verify_hdr failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    pkt_len = 0;
    u = m_pkt->verify_hdr(addr + proxy_size, pkt_size, pkt_len);
    if (u != 0) {
        ret = 15;
        GSS_LOG("sc_pkt_hdr::verify_hdr failed: ");
        get_log()->puts("ret = ");
        get_log()->put(u, 10);
        get_log()->puts("\n");
        return ret;
    }
    reply.size = proxy_b - pkt_size;
    reply.addr = addr + proxy_size + pkt_size;
    ret = 0;
    r = sc_validate_get_state_cmd(&reply, state);
    if (r != 0) {
        ret = 15;
        GSS_LOG("secure_com_lib::validate_get_state_cmd failed: ");
        get_log()->puts("ret = ");
        get_log()->put(r, 10);
        get_log()->puts(", ");
        get_log()->puts("err = ");
        get_log()->put(sc_lib_error(), 16);
        get_log()->puts("\n");
    }
    return ret;
}

void sc_iso_module::make_tweak(u8 *buf, unsigned int n, unsigned int v)
{
    memset(buf, 0, n);
    buf[0] = v;
    buf[1] = v >> 8;
    buf[2] = v >> 16;
    buf[3] = v >> 24;
}

unsigned int sc_iso_module::xts_encrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                                   const int &len, const u8 *key1, const int &key1_bits,
                                   const u8 *key2, const int &key2_bits, const u8 *tweak,
                                   const int &tweak_bits)
{
    u8 encrypt = 1;

    return c->crypt(in, in_len, out, len, key1, key1_bits, key2, key2_bits, tweak, tweak_bits,
                    encrypt);
}

unsigned int sc_iso_module::xts_decrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                                   const int &len, const u8 *key1, const int &key1_bits,
                                   const u8 *key2, const int &key2_bits, const u8 *tweak,
                                   const int &tweak_bits)
{
    u8 encrypt = 0;

    return c->crypt(in, in_len, out, len, key1, key1_bits, key2, key2_bits, tweak, tweak_bits,
                    encrypt);
}

sc_iso_module::sc_iso_module(ss_iso_dma_data *dma, const quad_ptr &buf, const quad_uint &size,
                             const quad_ptr &data, const quad_uint &data_size, const u8 *keys,
                             const u8 *key2, const quad_u64 &area, const quad_uint &id)
    : m_dma(dma), m_buf(buf), m_size(size), m_data(data), m_data_size(data_size), m_keys(),
      m_key2(), m_area(area), m_144(id), m_pkt(0), m_152(0), m_id(1), m_ctx(0), m_176(0)
{
    memcpy(m_keys, keys, sizeof(m_keys));
    memcpy(m_key2, key2, sizeof(m_key2));
}

unsigned int sc_iso_module::use_old_auth_keys()
{
    memcpy(g_116752, g_116352, 16);
    memcpy(g_116768, g_116368, 16);
    memcpy(g_116784, g_116384, 16);
    memcpy(g_116800, g_116400, 16);
    memcpy(g_116816, g_116416, 16);
    memcpy(g_116832, g_116432, 16);
    return 0;
}

u8 *sc_iso_module::copy_bytes(u8 *dst, const u8 *src, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++)
        dst[i] = src[i];
    return dst;
}

int sc_iso_module::bytes_differ(const unsigned char *a, const unsigned char *b, int n)
{
    int same = 1;
    int i;

    for (i = 0; i != n; i++)
        same = (a[i] == b[i]) && same;
    return !same;
}

void sc_iso_module::fill_bytes(u8 *dst, u8 c, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++)
        dst[i] = c;
}

sc_iso_module::~sc_iso_module()
{
}

unsigned int sc_iso_module::get_random(u8 *buf, unsigned int n, unsigned int &status)
{
    unsigned int i;

    for (i = 0; i != n; i++) {
        unsigned int v;

        asm volatile("rdch %0,$ch74" : "=r"(v));
        buf[i] = v >> 24;
    }
    status = 256;
    return 0;
}

unsigned int sc_iso_module::time_diff(const u64 &a, const u64 &b, u64 &diff)
{
    u64 d;

    diff = 0;
    d = a > b ? a - b : b - a;
    if (d > 0xFFFFFFFFULL)
        return 9;
    diff = a - b;
    return 0;
}
