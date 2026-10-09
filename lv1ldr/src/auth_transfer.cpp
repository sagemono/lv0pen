#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

extern "C" int lv1ldr_inflate_init(void);
extern "C" long lv1ldr_inflate_end(void);
extern "C" int lv1ldr_inflate(unsigned char *out, u64 out_len, u64 *out_done,
                              unsigned char *in, u64 in_len, u64 *in_done);

static dma_buffer *g_inf_out __attribute__((aligned(16)));
static u64 g_inf_offset __attribute__((aligned(16)));
static u64 g_inf_total __attribute__((aligned(16)));
static u64 g_inf_done __attribute__((aligned(16)));
static u64 g_inf_avail __attribute__((aligned(16)));
static unsigned char *g_inf_p __attribute__((aligned(16)));
static header_data g_inf_tail;
static unsigned char g_inf_end[13] __attribute__((aligned(16))) = { 1 };

#define INF_END { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 }

long authenticator::inflate_piece(dma_buffer *in, u64 ea, u64 *written)
{
    u64 in_done = 0;
    unsigned char *p = (unsigned char *)in->ls + in->offset;
    u64 n = in->length;

    if (g_inf_out == 0) {
        if (m_queue.dequeue_write(&g_inf_out))
            return 21;
        u64 off = ea & 15;

        g_inf_offset = off;
        g_inf_p = (unsigned char *)g_inf_out->ls + off;
        if (off != 0)
            g_inf_avail = 16 - off;
        else
            g_inf_avail = g_inf_out->size;
    }
    for (;;) {
        long rc = lv1ldr_inflate(g_inf_p, g_inf_avail, &g_inf_done, p, n, &in_done);

        g_inf_total = g_inf_done + g_inf_total;
        if ((unsigned long)rc > 1)
            return 19;
        if (rc == 1) {
            u64 part = g_inf_end[0] ? (g_inf_total - g_inf_offset) & 15 : g_inf_total & 15;
            u64 whole = g_inf_total > part ? g_inf_total - part : 0;

            if (g_inf_end[0] && g_inf_offset != 0) {
                g_inf_out->offset = g_inf_offset;
                g_inf_out->length = g_inf_total;
                if (m_queue.enqueue_write(g_inf_out))
                    return 21;
                *written += g_inf_out->length;
            } else {
                if (part != 0) {
                    memcpy(g_inf_tail.buf, (const unsigned char *)g_inf_out->ls + whole, part);
                    g_inf_tail.len = part;
                }
                if (whole != 0) {
                    g_inf_out->offset = g_inf_offset;
                    g_inf_out->length = whole;
                    if (m_queue.enqueue_write(g_inf_out))
                        return 21;
                    *written += g_inf_out->length;
                }
                if (part != 0) {
                    if (m_queue.dequeue_write(&g_inf_out))
                        return 21;
                    if (g_inf_tail.len > g_inf_out->size)
                        return 21;
                    memcpy((void *)g_inf_out->ls, g_inf_tail.buf, g_inf_tail.len);
                    g_inf_out->offset = g_inf_offset;
                    g_inf_out->length = g_inf_tail.len;
                    if (m_queue.enqueue_write(g_inf_out))
                        return 21;
                    *written += g_inf_out->length;
                }
            }
            g_inf_out = 0;
            g_inf_p = 0;
            g_inf_offset = 0;
            g_inf_tail.len = 0;
            g_inf_total = 0;
            g_inf_avail = 0;
            g_inf_done = 0;
            if (n != in_done)
                return 28;
            *(vec_uchar16 *)g_inf_end = (vec_uchar16)INF_END;
            return 0;
        }
        if (g_inf_avail == g_inf_done) {
            if (g_inf_end[0])
                g_inf_out->offset = g_inf_offset;
            else
                g_inf_out->offset = 0;
            g_inf_out->length = g_inf_total;
            if (m_queue.enqueue_write(g_inf_out))
                return 21;
            *written += g_inf_out->length;
            if (m_queue.dequeue_write(&g_inf_out))
                return 21;
            g_inf_avail = g_inf_out->size;
            g_inf_p = (unsigned char *)g_inf_out->ls;
            g_inf_total = 0;
            g_inf_offset = 0;
            *(vec_uchar16 *)g_inf_end = (vec_uchar16){ 0 };
            g_inf_tail.len = 0;
        } else {
            g_inf_p += g_inf_done;
            g_inf_avail = g_inf_avail - g_inf_done;
        }
        if (n == in_done)
            return 0;
        p += in_done;
        n -= in_done;
    }
}

long authenticator::transfer_deflated(u64 ea)
{
    dma_buffer *in = 0;
    u64 written = 0;
    bool last = false;
    long rc;

    if (lv1ldr_inflate_init())
        return 21;
    do {
        unsigned long r = m_queue.dequeue_read(&in);

        if (r > 1)
            return 21;
        if (r == 1)
            last = true;
        if (in->offset != 0) {
            if (save_tail(in))
                return 21;
            if (g_aes128_ctr.decrypt(g_header.buf, in->length, g_header.buf))
                return 21;
            if (restore_tail(in))
                return 21;
            if (last) {
                if (g_hmac.update(g_header.buf, g_header.len))
                    return 21;
                g_header.len = 0;
            }
        } else {
            if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, (void *)in->ls))
                return 21;
            if (last) {
                if (g_header.len == 0) {
                    if (g_hmac.update((const unsigned char *)in->ls, in->length))
                        return 21;
                } else {
                    unsigned int rem = in->length & 15;

                    if (g_hmac.update((const unsigned char *)in->ls, in->length - rem))
                        return 21;
                    if (rem)
                        if (prepend(in, rem))
                            return 21;
                    if (g_hmac.update(g_header.buf, g_header.len))
                        return 21;
                }
                g_header.len = 0;
            } else {
                if (g_hmac.update((const unsigned char *)in->ls, in->length))
                    return 21;
            }
        }
        rc = inflate_piece(in, ea, &written);
        if (rc)
            return rc;
        if (m_queue.enqueue_read(in))
            return 21;
    } while (last != true);
    rc = lv1ldr_inflate_end();
    if (rc)
        return 21;
    return rc;
}

long authenticator::transfer_stored()
{
    dma_buffer *in, *out;
    bool last = false;

    do {
        unsigned long rc = m_queue.dequeue_read(&in);

        if (rc > 1)
            return 21;
        if (rc == 1)
            last = true;
        if (m_queue.dequeue_write(&out))
            return 21;
        out->length = in->length;
        out->offset = in->offset;
        if (in->offset != 0) {
            if (save_tail(in))
                return 21;
            if (g_aes128_ctr.decrypt(g_header.buf, in->length, g_header.buf))
                return 21;
            if (restore_tail(out))
                return 21;
            if (last) {
                if (g_hmac.update(g_header.buf, g_header.len))
                    return 21;
                g_header.len = 0;
            }
        } else {
            if (in->length > out->size)
                return 21;
            if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, (void *)out->ls))
                return 21;
            if (last) {
                if (g_header.len == 0) {
                    if (g_hmac.update((const unsigned char *)out->ls, in->length))
                        return 21;
                } else {
                    unsigned int rem = in->length & 15;

                    if (g_hmac.update((const unsigned char *)out->ls, in->length - rem))
                        return 21;
                    if (rem)
                        if (prepend(out, rem))
                            return 21;
                    if (g_hmac.update(g_header.buf, g_header.len))
                        return 21;
                }
                g_header.len = 0;
            } else {
                if (g_hmac.update((const unsigned char *)out->ls, in->length))
                    return 21;
            }
        }
        if (m_queue.enqueue_write(out))
            return 21;
        if (m_queue.enqueue_read(in))
            return 21;
    } while (last != true);
    return 0;
}
