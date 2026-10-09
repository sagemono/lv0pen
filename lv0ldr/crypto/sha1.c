#include "sha1.h"

int sha1_init(sha1_ctx *ctx)
{
    ctx->h[0] = spu_splats(0x67452301u);
    ctx->h[1] = spu_splats(0xEFCDAB89u);
    ctx->h[2] = spu_splats(0x98BADCFEu);
    ctx->h[3] = spu_splats(0x10325476u);
    ctx->h[4] = spu_splats(0xC3D2E1F0u);
    ctx->count = 0;
    ctx->num = 0;
    return 0;
}

int sha1_update(sha1_ctx *ctx, const unsigned char *data, int len)
{
    unsigned char *buf;
    const unsigned char *s;
    unsigned char *d;
    int i, n;

    if (len < 0)
        return -2;
    if ((unsigned int)ctx->num > 63)
        return 0;
    if (ctx->num + len > 63) {
        for (i = ctx->num / 16; i < 4; i++) {
            ((vec_uchar16 *)ctx->buf)[i] = *(const vec_uchar16 *)data;
            data += 16;
        }
        sha1_transform(ctx, ctx->buf, 1);
        len = ctx->num + len - 64;
        ctx->count++;
        ctx->num = 0;
    }
    if (len > 63)
        ctx->num = 0;
    n = len / 64;
    ctx->count += n;
    len -= n * 64;
    sha1_transform(ctx, data, n);
    data += n * 64;
    buf = ctx->buf;
    for (i = 0; i < len; i++) {
        s = data + i;
        d = buf + ctx->num + i;
        *d = *s;
    }
    ctx->num += i;
    return 0;
}

int sha1_final(unsigned char *md, sha1_ctx *ctx)
{
    unsigned char *buf, *p;
    int i;

    if ((unsigned int)ctx->num > 63)
        return -1;
    i = ctx->num;
    buf = ctx->buf;
    buf[i] = 0x80;
    if (ctx->num > 55) {
        for (i = ctx->num + 1; i < 64; i++) {
            p = buf + i;
            *p = 0;
        }
        sha1_transform(ctx, buf, 1);
        i = -1;
    }
    for (i = i + 1; i < 56; i++) {
        p = buf + i;
        *p = 0;
    }
    ((vec_ullong2 *)ctx->buf)[3] = spu_insert((ctx->count << 9) + (ctx->num << 3),
                                              ((vec_ullong2 *)ctx->buf)[3], 1);
    sha1_transform(ctx, buf, 1);
    md[0] = spu_extract((vec_uchar16)ctx->h[0], 0);
    md[1] = spu_extract((vec_uchar16)ctx->h[0], 1);
    md[2] = spu_extract((vec_uchar16)ctx->h[0], 2);
    md[3] = spu_extract((vec_uchar16)ctx->h[0], 3);
    md[4] = spu_extract((vec_uchar16)ctx->h[1], 0);
    md[5] = spu_extract((vec_uchar16)ctx->h[1], 1);
    md[6] = spu_extract((vec_uchar16)ctx->h[1], 2);
    md[7] = spu_extract((vec_uchar16)ctx->h[1], 3);
    md[8] = spu_extract((vec_uchar16)ctx->h[2], 0);
    md[9] = spu_extract((vec_uchar16)ctx->h[2], 1);
    md[10] = spu_extract((vec_uchar16)ctx->h[2], 2);
    md[11] = spu_extract((vec_uchar16)ctx->h[2], 3);
    md[12] = spu_extract((vec_uchar16)ctx->h[3], 0);
    md[13] = spu_extract((vec_uchar16)ctx->h[3], 1);
    md[14] = spu_extract((vec_uchar16)ctx->h[3], 2);
    md[15] = spu_extract((vec_uchar16)ctx->h[3], 3);
    md[16] = spu_extract((vec_uchar16)ctx->h[4], 0);
    md[17] = spu_extract((vec_uchar16)ctx->h[4], 1);
    md[18] = spu_extract((vec_uchar16)ctx->h[4], 2);
    md[19] = spu_extract((vec_uchar16)ctx->h[4], 3);
    return 0;
}
