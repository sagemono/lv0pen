#include <spu_intrinsics.h>
#include "sb.h"
#include "aes.h"
#include "util.h"

static const quad_uint s_error = STOP_ERROR;
static const quad_uint s_ok = STOP_OK;

unsigned char g_key_a[32] __attribute__((aligned(16)));
unsigned char g_key_b[32] __attribute__((aligned(16)));
unsigned char g_type_key[16] __attribute__((aligned(16)));

int sb_module::serve(ss_iso_dma_data *t)
{
    dma_channel ch;
    quad_uint token = 0;
    quad_uint cmd = 0;
    quad_ptr buf = (unsigned char *)0x3E000;
    quad_uint max = 128;
    quad_uint len = 0;
    quad_uint room = 0;
    session_c c;
    session_a a;
    int r;

    spu_writech(SPU_WrDec, -1);
    do
        ch.out_intr_mbox_room(room);
    while (room == 0);
    ch.write_out_intr_mbox(0x80);
    if (t->get(buf, max, token, cmd, len) != 0) {
        t->finish(s_error, cmd, 0, 0);
        r = -1;
    } else if (cmd == CMD_START_A) {
        const unsigned char *msg = buf;
        unsigned int type = GET_BE32(msg);
        const unsigned char *key;

        if (type == 0x15) {
            key = g_key_15;
            goto enc4;
        } else if (type == 0x16) {
            key = g_key_16;
            goto enc5;
        } else if (type == 0x18) {
        enc4:
            if (aes_cbc_encrypt((vec_uchar16 *)g_type_key, (const vec_uchar16 *)(msg + 20), 16,
                                g_type_enc_key4, 128, (const vec_uchar16 *)g_type_iv) != 0)
                t->finish(s_error, cmd, (unsigned int)key, (unsigned char *)key);
            key = g_type_key;
            type = 4;
        } else if (type == 0x19) {
        enc5:
            if (aes_cbc_encrypt((vec_uchar16 *)g_type_key, (const vec_uchar16 *)(msg + 20), 16,
                                g_type_enc_key5, 128, (const vec_uchar16 *)g_type_iv) != 0)
                t->finish(s_error, cmd, (unsigned int)key, (unsigned char *)key);
            key = g_type_key;
            type = 5;
        } else if (type == 0x17 || type == 0x1A) {
            key = g_key_17;
            type = 6;
        } else if (type < 0x10)
            key = msg + 20;
        else if ((type & 0xF) == 0)
            key = g_key_b;
        else if ((type & 0xF) == 1)
            key = g_key_a;
        a.init(t, type, GET_BE32(msg + 4), len, key);
        if (a.run(cmd) < 0)
            t->finish(s_error, cmd, 0, 0);
        t->finish(s_ok, cmd, 0, 0);
        spu_stop(0x103);
        r = 0;
    } else if (cmd == CMD_START_C) {
        const unsigned char *msg = buf;

        c.init(t, GET_BE32(msg));
        c.run();
        spu_stop(0x103);
        r = -1;
    } else {
        spu_stop(0x101);
        r = -1;
    }
    return r;
}

int main(u64 ea, u64 size, u64 arg2, u64 arg3, key128 a0, key128 a1, key128 a2, key128 a3,
         key128 b0, key128 b1, key128 b2, key128 b3)
{
    quad_uint ls = 0x3FFF0;
    quad_uint msg_size;
    quad_uint hdr_size;
    quad_u64 msg_ea;
    sb_module m;

    if ((unsigned int)size < 80) {
        spu_stop(0x101);
        return -1;
    }
    msg_ea = ea;
    msg_size = size;
    hdr_size = 16;
    ss_iso_dma_data t(msg_ea, msg_size, ls, hdr_size);

    memset(g_key_a, 0, sizeof(g_key_a));
    memcpy(g_key_a, &a0, 16);
    memcpy(g_key_a + 16, &a1, 16);
    memset(g_key_b, 0, sizeof(g_key_b));
    memcpy(g_key_b, &b0, 16);
    memcpy(g_key_b + 16, &b1, 16);
    m.serve(&t);
    return 0;
}
