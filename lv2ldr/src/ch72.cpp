#include <spu_intrinsics.h>
#include "types.h"
#include "aes.h"
#include "hmac_sha1.h"

int ch72_put(int skip, const u32 *buf, int n);
int ch72_get(int skip, u32 *buf, int n);

int ch72_get_reply(unsigned char *payload)
{
    u32 msg[3];
    const unsigned char *b = (const unsigned char *)msg;
    int r, i;

    r = ch72_get(0, msg, 3);
    if (r == 0) {
        payload[0] = b[0];
        payload[1] = b[2];
        for (i = 2; i < 10; i++)
            payload[i] = b[i + 2];
    }
    return r;
}

int ch72_put_request(u32 cmd, const unsigned char *payload)
{
    u32 msg[3];
    unsigned char *b = (unsigned char *)msg;
    int i;

    b[0] = payload[0];
    b[1] = cmd >> 16;
    b[2] = payload[1];
    b[3] = cmd;
    for (i = 2; i < 10; i++)
        b[i + 2] = payload[i];
    return ch72_put(0, msg, 3);
}

int ch72_get(int skip, u32 *buf, int n)
{
    int i;

    spu_writech(64, 0x10000);
    for (i = 0; i != skip; i++)
        spu_readch(73);
    for (i = 0; i != n; i++) {
        *buf = spu_readch(73);
        buf++;
    }
    return 0;
}

int ch72_put(int skip, const u32 *buf, int n)
{
    int i;

    spu_writech(64, 0x10000);
    for (i = 0; i != skip; i++)
        spu_writech(72, spu_readch(73));
    for (i = 0; i != n; i++) {
        spu_writech(72, *buf);
        buf++;
    }
    return 0;
}

int ch72_get_version(u64 *version)
{
    u32 msg[2];
    int r;

    r = ch72_get(0, msg, 2);
    *version = ((u64)msg[0] << 32 | msg[1]) & 0x00FF00FF00000000ULL;
    return r;
}

int ch72_compare_version(u64 version)
{
    u64 v;

    ch72_get_version(&v);
    if (version < v)
        return -1;
    return version > v;
}

static int ch72_unwrap(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *data,
                       int data_len, const vec_uchar16 *iv)
{
    unsigned char reply[10] __attribute__((aligned(16)));
    unsigned char key[20] __attribute__((aligned(16)));

    if (ch72_get_reply(reply) == 0 && hmac_sha1(key, data, data_len, reply, 10) == 0
        && aes_cbc_decrypt(out, in, len, key, 128, iv) == 0)
        return 0;
    return -1;
}

extern const unsigned char ch72_key_data[16], ch72_iv_data[16];
extern const vec_uchar16 ch72_key_iv, ch72_iv_iv;

int ch72_unwrap_iv(vec_uchar16 *out, const vec_uchar16 *in, int len)
{
    return ch72_unwrap(out, in, len, ch72_iv_data, 16, &ch72_iv_iv);
}

int ch72_unwrap_key(vec_uchar16 *out, const vec_uchar16 *in, int len)
{
    return ch72_unwrap(out, in, len, ch72_key_data, 16, &ch72_key_iv);
}
