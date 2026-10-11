#include "sc_iso_module.h"
#include "sc_log.h"
#include "util.h"

struct key128 {
    u64 hi;
    u64 lo;
};

static const unsigned int s_bad_status = 0x101;
static const unsigned int s_no_cmd = 0;

int main(u64 dma_ea, u64 dma_size, u64 area, u64 word, key128 arg4, key128 key2, key128 key0,
         key128 key1)
{
    unsigned int err = 0x81000100;
    bool bad;
    u8 buf[0x1800] __attribute__((aligned(128)));
    quad_u64 ea;
    quad_uint hdr_size;
    key128 key[2] __attribute__((aligned(16)));
    quad_uint no_len;
    quad_uint log_size;
    quad_uint reply_len;
    quad_ptr ls;
    quad_uint msg_size;
    quad_ptr reply;
    quad_uint id;
    quad_ptr data;
    quad_uint data_size;
    quad_uint status;
    quad_uint cmd;
    quad_uint size;
    quad_ptr msg;
    quad_ptr no_buf;
    int r;

    log_size = dma_size;
    get_log()->init(dma_ea, log_size);
    get_log()->puts("SC Iso SPU module: Hello!\n");

    ea = 0;
    size = 0;
    key[0] = key0;
    key[1] = key1;
    data = buf;
    data_size = sizeof(buf);
    msg = (unsigned char *)0x3E000;
    msg_size = 0x1800;
    ls = (unsigned char *)0x3FFF0;
    hdr_size = 16;
    if (dma_ea == 0 || dma_size < 0x230) {
        err |= 0x80;
        get_log()->put(err, 16);
    } else {
        ea = dma_ea;
        if (dma_size > 0xFFFFFFFF)
            size = 0xFFFFFFFF;
        else
            size = dma_size;
        if ((ea & 15) == 0) {
            bad = false;
            if ((size & 15) == 0)
                goto check_buffers;
        }
        err |= 0x81;
        get_log()->put(err, 16);
    }
    get_log()->puts(", ");
    get_log()->put((unsigned int)dma_ea, 16);
    get_log()->puts(", ");
    get_log()->put((unsigned int)dma_size, 16);
    get_log()->puts("\n");
    if (((unsigned int)ls & 15) != 0 || ((unsigned int)msg & 15) != 0)
        goto bad_buffers;
    bad = true;
check_buffers:
    if (((unsigned int)data & 15) != 0)
        goto bad_buffers;
    if ((msg_size & 15) != 0 || (data_size & 15) != 0 || (hdr_size & 15) != 0)
        goto bad_buffers;
make_link:
    {
        ss_iso_dma_data dma(ea, size, ls, hdr_size);

        if (!bad) {
            id = (unsigned int)word;
            sc_iso_module module(&dma, msg, msg_size, data, data_size, (const u8 *)key,
                                 (const u8 *)&key2, area, id);

            status = 259;
            cmd = 0;
            reply = 0;
            reply_len = 0;
            status = module.run(cmd, reply, reply_len);
            get_log()->puts("SC Iso SPU module: Bye!\n");
            dma.finish(status, cmd, reply_len, reply);
            r = 0;
        } else {
            no_len = 0;
            no_buf = 0;
            dma.finish(s_bad_status, s_no_cmd, no_len, no_buf);
            r = -1;
        }
    }
    return r;
bad_buffers:
    err |= 0x82;
    get_log()->put(err, 16);
    get_log()->puts(", ");
    bad = true;
    goto make_link;
}
