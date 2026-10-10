#include <spu_intrinsics.h>
#include "utoken.h"
#include "aes.h"
#include "hmac_sha1.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

read_cursor g_read_cursor;
write_cursor g_write_cursor;
dma_reader g_reader(LS_READ, 0x1000, TAG_READ, &g_read_cursor);
dma_writer g_writer(LS_WRITE, 0x1000, TAG_WRITE, &g_write_cursor);

utoken::~utoken()
{
}

bool utoken::check_header(const unsigned char *buf, u64 size)
{
    if (buf == 0)
        return false;
    if (size != UTOKEN_SIZE)
        return false;
    if ((u32)(buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]) != UTOKEN_MAGIC)
        return false;
    if ((u32)(buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7]) != UTOKEN_VERSION)
        return false;
    return (u64)((u128)buf[8] << 56 | (u128)buf[9] << 48 | (u128)buf[10] << 40 | (u128)buf[11] << 32 |
                 (u128)buf[12] << 24 | (u128)buf[13] << 16 | (u128)buf[14] << 8 | (u128)buf[15]) == size;
}

int utoken::check_idps(const unsigned char *buf)
{
    int i;

    if (m_idps.product >= 0x83 && m_idps.product <= 0x9F)
        return 8;
    for (i = 0; i < sizeof m_idps; i++)
        if (buf[UTOKEN_IDPS + i] != 0)
            break;
    if (i == sizeof m_idps)
        return 0;
    if (bytes_differ((const unsigned char *)&m_idps, buf + UTOKEN_IDPS, sizeof m_idps))
        return 8;
    return 0;
}

utoken::utoken(const unsigned char *key, unsigned char *iv, const unsigned char *hmac_key,
               const unsigned char *idps)
{
    m_key = key;
    m_iv = iv;
    m_hmac_key = hmac_key;
    memcpy(&m_idps, idps, sizeof m_idps);
}

int utoken::put(const utoken_params *p, const unsigned char *buf)
{
    dma_channel dma;
    int r = DMA_EINVAL;

    if (p == 0 || buf == 0)
        return r;
    if ((p->out_size & 15) == 0 && p->out_size == UTOKEN_SIZE) {
        u64 off = p->out_ea & 0x7F;

        if (off + p->out_size <= 0x1000) {
            u64 ls = off + LS_WRITE;

            memcpy((void *)(unsigned int)ls, buf, p->token_size);
            r = dma.issue(ls, p->token_ea, p->token_size, TAG_WRITE, 0, MFC_PUT);
            if (r == 0)
                dma.wait(TAG_WRITE);
        }
    }
    return r;
}

int utoken::get(const utoken_params *p, unsigned char *buf)
{
    dma_channel dma;
    int r = DMA_EINVAL;

    if (p == 0 || buf == 0)
        return r;
    if ((p->token_size & 15) == 0 && p->token_size == UTOKEN_SIZE) {
        u64 off = p->token_ea & 0x7F;

        if (off + p->token_size <= 0x1000) {
            u64 ls = off + LS_READ;

            r = dma.issue(ls, p->token_ea, p->token_size, TAG_READ, 0, MFC_GET);
            if (r == 0) {
                dma.wait(TAG_READ);
                memcpy(buf, (const void *)(unsigned int)ls, p->token_size);
            }
        }
    }
    return r;
}

int utoken::seal(const utoken_params *p, unsigned char *buf)
{
    int r;

    memset(buf, 0, UTOKEN_SIZE);
    r = get(p, buf);
    if (r != 0)
        return r;
    if (!check_header(buf, UTOKEN_SIZE))
        return 27;
    if (hmac_sha1(buf + UTOKEN_HMAC, buf, UTOKEN_HMAC, m_hmac_key, 64))
        return 20;
    if (aes_cbc_encrypt((vec_uchar16 *)(buf + 16), (const vec_uchar16 *)(buf + 16), UTOKEN_SIZE - 16,
                        m_key, 256, (const vec_uchar16 *)m_iv))
        return 20;
    return put(p, buf);
}

int utoken::open(const utoken_params *p, unsigned char *buf)
{
    unsigned char digest[20];
    int r;

    memset(buf, 0, UTOKEN_SIZE);
    memset(digest, 0, sizeof digest);
    r = get(p, buf);
    if (r != 0)
        return r;
    if (!check_header(buf, UTOKEN_SIZE))
        return 27;
    if (aes_cbc_decrypt((vec_uchar16 *)(buf + 16), (const vec_uchar16 *)(buf + 16), UTOKEN_SIZE - 16,
                        m_key, 256, (const vec_uchar16 *)m_iv))
        return 20;
    if (hmac_sha1(digest, buf, UTOKEN_HMAC, m_hmac_key, 64))
        return 20;
    if (bytes_differ(digest, buf + UTOKEN_HMAC, 20))
        return 20;
    r = check_idps(buf);
    if (r != 0)
        return r;
    return put(p, buf);
}
