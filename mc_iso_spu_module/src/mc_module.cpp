#include <spu_intrinsics.h>
#include "mc.h"
#include "aes.h"
#include "util.h"

typedef unsigned int u128 __attribute__((mode(TI)));

#define GET_BE64(p) ({                              \
    union { u128 q; struct { u64 hi, lo; } s; } t_; \
    t_.q = (u128)(p)[0] << 56;                      \
    t_.q = (u128)(p)[1] << 48 | t_.q;               \
    t_.q = (u128)(p)[2] << 40 | t_.q;               \
    t_.q = (u128)(p)[3] << 32 | t_.q;               \
    t_.q = (u128)(p)[4] << 24 | t_.q;               \
    t_.q = (u128)(p)[5] << 16 | t_.q;               \
    t_.q = (u128)(p)[6] << 8 | t_.q;                \
    t_.q = (u128)(p)[7] | t_.q;                     \
    (u64)t_.q; })
#define PUT_BE64(p, v) do {  \
    u128 t_;                 \
    t_ = (v) >> 56;          \
    (p)[0] = t_;             \
    t_ = ((v) >> 48) & 0xff; \
    (p)[1] = t_;             \
    t_ = ((v) >> 40) & 0xff; \
    (p)[2] = t_;             \
    t_ = ((v) >> 32) & 0xff; \
    (p)[3] = t_;             \
    t_ = ((v) >> 24) & 0xff; \
    (p)[4] = t_;             \
    t_ = ((v) >> 16) & 0xff; \
    (p)[5] = t_;             \
    t_ = ((v) >> 8) & 0xff;  \
    (p)[6] = t_;             \
    t_ = (v) & 0xff;         \
    (p)[7] = t_; } while (0)

#define MC_TRACE(...)   do { } while (0)

#define MC_MAGIC        0x12345678ULL

static const unsigned char s_iv[8] = { 0 };
static const unsigned char s_mask0[8] = { 0x6c, 0x26, 0xd3, 0x7f, 0x46, 0xee, 0x9d, 0xa9 };
static const unsigned char s_key0[16] = {
    0xce, 0x62, 0xf6, 0x84, 0x20, 0xb6, 0x5a, 0x81, 0xe4, 0x59, 0xfa, 0x9a, 0x2b, 0xb3, 0x59, 0x8a
};
static const unsigned char s_mask1[8] = { 0x2c, 0xd1, 0x60, 0xfa, 0x8c, 0x2e, 0xd3, 0x62 };
static const unsigned char s_key1[16] = {
    0x70, 0x14, 0xa3, 0x2f, 0xcc, 0x5b, 0x12, 0x37, 0xac, 0x1f, 0xbf, 0x4e, 0xd2, 0x6d, 0x1c, 0xc1
};

static const quad_uint s_error = STOP_ERROR;
static const quad_uint s_ok = STOP_OK;
static const quad_uint s_mismatch = STOP_MISMATCH;
static const quad_uint s_unknown = STOP_UNKNOWN;

unsigned char g_chain_iv[8] = { 0x2c, 0x5b, 0xf4, 0x8d, 0x32, 0x74, 0x91, 0x27 };

void mc_module::init(key128 key, key128 iv)
{
    memcpy(m_key, &key, 16);
    memcpy(m_iv, &iv, 16);
}

int mc_module::serve(ss_iso_dma_data *t)
{
    dma_channel ch;
    quad_uint token = 0;
    quad_uint cmd = 0;
    quad_ptr buf = (unsigned char *)0x3E000;
    int r;

    spu_writech(SPU_WrDec, -1);
    quad_uint max = 128;
    quad_uint len = 0;
    quad_uint room = 0;

    do
        ch.out_intr_mbox_room(room);
    while (room == 0);
    ch.write_out_intr_mbox(0x80);
    if (t->get(buf, max, token, cmd, len) != 0) {
        t->finish(s_error, cmd, 0, 0);
        r = -1;
    } else if (cmd == CMD_ISSUE) {
        unsigned char *msg = buf;
        u64 x = GET_BE64(msg);
        u64 z = GET_BE64(msg + 16);
        u64 y = GET_BE64(msg + 8);
        u64 k0, o2, rnd, k1, o0, o1;
        unsigned char blob[48];

        for (int i = 0; i < 24; i++)
            MC_TRACE("%02x", msg[i]);
        if (derive(x, y, (unsigned char *)&k0, (unsigned char *)&k1) < 0) {
            t->finish(s_error, cmd, 0, 0);
            r = -1;
        } else if ((int)fill_random((unsigned char *)&rnd, 8) < 0) {
            t->finish(s_error, cmd, 0, 0);
            r = -1;
        } else if (chain(k0, k1, z, rnd, x, (unsigned char *)&o0, (unsigned char *)&o1,
                         (unsigned char *)&o2) < 0) {
            t->finish(s_error, cmd, 0, 0);
            r = -1;
        } else if (pack(blob, z, rnd, k0, k1) < 0) {
            t->finish(s_error, cmd, 0, 0);
            r = -1;
        } else {
            unsigned char *out = buf;

            out[0] = ((unsigned char *)&o0)[0];
            out[1] = ((unsigned char *)&o0)[1];
            out[2] = ((unsigned char *)&o0)[2];
            out[3] = ((unsigned char *)&o0)[3];
            out[4] = ((unsigned char *)&o0)[4];
            out[5] = ((unsigned char *)&o0)[5];
            out[6] = ((unsigned char *)&o0)[6];
            out[7] = ((unsigned char *)&o0)[7];
            out[8] = ((unsigned char *)&o1)[0];
            out[9] = ((unsigned char *)&o1)[1];
            out[10] = ((unsigned char *)&o1)[2];
            out[11] = ((unsigned char *)&o1)[3];
            out[12] = ((unsigned char *)&o1)[4];
            out[13] = ((unsigned char *)&o1)[5];
            out[14] = ((unsigned char *)&o1)[6];
            out[15] = ((unsigned char *)&o1)[7];
            out[16] = ((unsigned char *)&o2)[0];
            out[17] = ((unsigned char *)&o2)[1];
            out[18] = ((unsigned char *)&o2)[2];
            out[19] = ((unsigned char *)&o2)[3];
            out[20] = ((unsigned char *)&o2)[4];
            out[21] = ((unsigned char *)&o2)[5];
            out[22] = ((unsigned char *)&o2)[6];
            out[23] = ((unsigned char *)&o2)[7];
            memcpy(out + 32, blob, 48);
            t->finish(s_ok, cmd, 80, buf);
            spu_stop(STOP_ERROR);
            r = 0;
        }
    } else if (cmd == CMD_VERIFY) {
        unsigned char *msg = buf;
        u64 c, d, a, b, p2, p0, p1;
        unsigned char blob[48];

        memcpy(blob, msg + 32, 48);
        if (unpack(blob, &a, &b, &c, &d) < 0)
            spu_stop(0x213);
        r = check_chain(c, d, a, b, GET_BE64(msg), GET_BE64(msg + 8), GET_BE64(msg + 16), &p0,
                        &p1, (unsigned char *)&p2);
        if (r < 0) {
            if (r == -2)
                t->finish(s_mismatch, cmd, 0, 0);
            else
                t->finish(s_error, cmd, 0, 0);
            r = -1;
        } else {
            unsigned char *out = buf;

            out[0] = ((unsigned char *)&p0)[0];
            out[1] = ((unsigned char *)&p0)[1];
            out[2] = ((unsigned char *)&p0)[2];
            out[3] = ((unsigned char *)&p0)[3];
            out[4] = ((unsigned char *)&p0)[4];
            out[5] = ((unsigned char *)&p0)[5];
            out[6] = ((unsigned char *)&p0)[6];
            out[7] = ((unsigned char *)&p0)[7];
            out[8] = ((unsigned char *)&p1)[0];
            out[9] = ((unsigned char *)&p1)[1];
            out[10] = ((unsigned char *)&p1)[2];
            out[11] = ((unsigned char *)&p1)[3];
            out[12] = ((unsigned char *)&p1)[4];
            out[13] = ((unsigned char *)&p1)[5];
            out[14] = ((unsigned char *)&p1)[6];
            out[15] = ((unsigned char *)&p1)[7];
            out[16] = ((unsigned char *)&p2)[0];
            out[17] = ((unsigned char *)&p2)[1];
            out[18] = ((unsigned char *)&p2)[2];
            out[19] = ((unsigned char *)&p2)[3];
            out[20] = ((unsigned char *)&p2)[4];
            out[21] = ((unsigned char *)&p2)[5];
            out[22] = ((unsigned char *)&p2)[6];
            out[23] = ((unsigned char *)&p2)[7];
            t->finish(s_ok, cmd, 0, buf);
            spu_stop(STOP_ERROR);
            r = 0;
        }
    } else {
        t->finish(s_unknown, cmd, 0, 0);
        spu_stop(STOP_ERROR);
        r = -1;
    }
    return r;
}

int mc_module::derive(u64 a, u64 b, unsigned char *o0, unsigned char *o1)
{
    unsigned char x[8];
    unsigned char r0[8], r1[8];
    unsigned char t0[8], t1[8];
    u64 v = a ^ b;

    memcpy(x, &v, 8);
    xor_bytes(x, s_mask0, t0, 8);
    if (des3_cbc_encrypt(r0, t0, 8, s_key0, 128, s_iv) < 0)
        return -3;
    xor_bytes(x, s_mask1, t1, 8);
    if (des3_cbc_encrypt(r1, t1, 8, s_key1, 128, s_iv) < 0)
        return -3;
    memcpy(o0, r0, 8);
    memcpy(o1, r1, 8);
    return 0;
}

int mc_module::chain(u64 k0, u64 k1, u64 a, u64 b, u64 c, unsigned char *o0,
                     unsigned char *o1, unsigned char *o2)
{
    unsigned char r0[8], r1[8], r2[8];
    unsigned char t0[8], t1[8], t2[8];
    unsigned char pa[8], pb[8], pc[8];
    unsigned char key[16];

    memcpy(pa, &a, 8);
    memcpy(pb, &b, 8);
    memcpy(pc, &c, 8);
    memcpy(key, &k0, 8);
    memcpy(key + 8, &k1, 8);
    xor_bytes(pb, g_chain_iv, t0, 8);
    if (des3_cbc_encrypt(r0, t0, 8, key, 128, s_iv) < 0)
        return -3;
    xor_bytes(pa, r0, t1, 8);
    if (des3_cbc_encrypt(r1, t1, 8, key, 128, s_iv) < 0)
        return -3;
    xor_bytes(pc, r1, t2, 8);
    if (des3_cbc_encrypt(r2, t2, 8, key, 128, s_iv) < 0)
        return -3;
    memcpy(o0, r0, 8);
    memcpy(o1, r1, 8);
    memcpy(o2, r2, 8);
    return 0;
}

int mc_module::check_chain(u64 k0, u64 k1, u64 a, u64 b, u64 c0, u64 c1, u64 c2, u64 *p0,
                           u64 *p1, unsigned char *p2)
{
    unsigned char t0[8], t1[8], t2[8];
    unsigned char in0[8], in1[8], in2[8];
    unsigned char r0[8], r1[8], r2[8];
    unsigned char key[16];
    unsigned char pa[8];

    memcpy(pa, &a, 8);
    memcpy(key, &k0, 8);
    memcpy(key + 8, &k1, 8);
    memcpy(in0, &c0, 8);
    if (des3_cbc_decrypt(t0, in0, 8, key, 128, s_iv) < 0)
        return -3;
    xor_bytes(t0, g_chain_iv, r0, 8);
    memcpy(in1, &c1, 8);
    if (des3_cbc_decrypt(t1, in1, 8, key, 128, s_iv) < 0)
        return -3;
    xor_bytes(t1, in0, r1, 8);
    memcpy(in2, &c2, 8);
    if (des3_cbc_decrypt(t2, in2, 8, key, 128, s_iv) < 0)
        return -3;
    xor_bytes(t2, in1, r2, 8);
    memcpy(p0, r0, 8);
    memcpy(p1, r1, 8);
    if (*p0 != a)
        return -2;
    if (*p1 != b)
        return -2;
    return 0;
}

int mc_module::pack(unsigned char *out, u64 a, u64 b, u64 c, u64 d)
{
    unsigned char buf[48];

    PUT_BE64(buf, a);
    PUT_BE64(buf + 8, b);
    PUT_BE64(buf + 16, c);
    PUT_BE64(buf + 24, d);
    PUT_BE64(buf + 32, MC_MAGIC);
    if (aes_cbc_encrypt((vec_uchar16 *)out, (const vec_uchar16 *)buf, 48, m_key, 128,
                        (const vec_uchar16 *)m_iv) < 0)
        return -3;
    return 0;
}

int mc_module::unpack(const unsigned char *in, u64 *a, u64 *b, u64 *c, u64 *d)
{
    unsigned char buf[48];
    int r = -1;

    if (aes_cbc_decrypt((vec_uchar16 *)buf, (const vec_uchar16 *)in, 48, m_key, 128,
                        (const vec_uchar16 *)m_iv) < 0)
        spu_stop(0x211);
    if (GET_BE64(buf + 32) == MC_MAGIC) {
        *a = GET_BE64(buf);
        *b = GET_BE64(buf + 8);
        *c = GET_BE64(buf + 16);
        *d = GET_BE64(buf + 24);
        r = 0;
    }
    return r;
}

unsigned int mc_module::fill_random(unsigned char *p, unsigned int n)
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

unsigned int mc_module::xor_bytes(const unsigned char *a, const unsigned char *b,
                                  unsigned char *out, unsigned int n)
{
    unsigned int i;

    for (i = 0; i != n; i++)
        out[i] = a[i] ^ b[i];
    return 0;
}

int main(u64 ea, u64 size, u64 arg2, u64 arg3, key128 k0, key128 k1, key128 k2, key128 k3)
{
    quad_uint ls = 0x3FFF0;
    quad_uint msg_size;
    quad_uint hdr_size;
    quad_u64 msg_ea;
    mc_module m;

    if ((unsigned int)size < 80) {
        spu_stop(0x101);
        return -1;
    }
    msg_ea = ea;
    msg_size = size;
    hdr_size = 16;
    ss_iso_dma_data t(msg_ea, msg_size, ls, hdr_size);

    m.init(k2, k1);
    m.serve(&t);
    return 0;
}
