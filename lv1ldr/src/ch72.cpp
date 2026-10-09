#include <spu_intrinsics.h>
#include "aes.h"
#include "hmac_sha1.h"
#include "types.h"

int ch72_put(int skip, const u32 *buf, int n);
int ch72_get(int skip, u32 *buf, int n);

int ch72_get_all_ones(void)
{
    u32 msg[3];

    msg[0] = 0;
    msg[1] = 0;
    msg[2] = 0;
    ch72_get(0, msg, 3);
    if (msg[0] != 0xFFFFFFFF || msg[1] != 0xFFFFFFFF)
        return 0;
    return msg[2] == 0xFFFFFFFF;
}

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

extern const unsigned char ch72_mac_data_0[16];
extern const unsigned char ch72_mac_data_1[16];
extern const vec_uchar16 ch72_cbc_iv_0;
extern const vec_uchar16 ch72_cbc_iv_1;

static int ch72_unwrap(void *out, const void *in, int len,
                       const unsigned char *data, int datalen, const vec_uchar16 *iv)
{
    unsigned char reply[10];
    unsigned char mac[20];

    if (ch72_get_reply(reply))
        goto bad;
    if (hmac_sha1(mac, data, datalen, reply, 10))
        goto bad;
    if (aes_cbc_decrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, mac, 128, iv))
        goto bad;
    return 0;
bad:
    return -1;
}

int ch72_unwrap_key(void *out, const void *in, int len)
{
    return ch72_unwrap(out, in, len, ch72_mac_data_0, 16, &ch72_cbc_iv_0);
}

int ch72_unwrap_iv(void *out, const void *in, int len)
{
    return ch72_unwrap(out, in, len, ch72_mac_data_1, 16, &ch72_cbc_iv_1);
}
