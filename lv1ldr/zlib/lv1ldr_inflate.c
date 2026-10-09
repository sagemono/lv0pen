/* Altered from zlib 1.2.3: comments removed, changes for the loader. */
#include "zlib.h"

typedef unsigned long long u64;

static z_stream g_strm;

int lv1ldr_inflate(Bytef *out, u64 out_len, u64 *out_done,
                   Bytef *in, u64 in_len, u64 *in_done)
{
    int rc;

    g_strm.next_in = in;
    g_strm.avail_in = in_len;
    g_strm.next_out = out;
    g_strm.avail_out = out_len;
    rc = inflate(&g_strm, Z_SYNC_FLUSH);
    *out_done = out_len - g_strm.avail_out;
    *in_done = in_len - g_strm.avail_in;
    switch (rc) {
    case Z_BUF_ERROR:
        return -1;
    case Z_STREAM_END:
        return 1;
    case Z_OK:
        return 0;
    case Z_DATA_ERROR:
        return -3;
    default:
        return -4;
    }
}

int lv1ldr_inflate_init(void)
{
    g_strm.zalloc = Z_NULL;
    g_strm.zfree = Z_NULL;
    g_strm.opaque = Z_NULL;
    return inflateInit(&g_strm) == Z_OK ? 0 : -4;
}

long lv1ldr_inflate_end(void)
{
    return inflateEnd(&g_strm) == Z_OK ? 0 : -4;
}
