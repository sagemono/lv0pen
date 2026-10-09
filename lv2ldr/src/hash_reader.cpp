#include "hash_reader.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n)
{
    int same = 1;
    int i;

    for (i = 0; i != n; i++)
        same = (a[i] == b[i]) && same;
    return !same;
}

long hash_reader::get_digest(unsigned char *out, unsigned int n)
{
    if (out == 0 || n < 20)
        return -1;
    memcpy(out, m_digest, n);
    return 0;
}

long hash_reader::open(u64 ea, u64 size, dma_buffer **bufs, unsigned int nbufs,
                       const unsigned char *key, unsigned int keylen)
{
    unsigned int i;

    if (ea == 0 || size == 0)
        return -1;
    if (bufs == 0 || nbufs == 0 || key == 0 || keylen == 0)
        return -1;
    if (m_queue.open(0, 0, ea, size))
        return -1;
    for (i = 0; i != nbufs; i++)
        if (m_queue.enqueue_read(bufs[i]))
            return -1;
    if (hmac_sha1_init(&m_ctx, key, keylen) < 0)
        return -1;
    return 0;
}

hash_reader::hash_reader()
{
    memset(m_digest, 0, sizeof m_digest);
    memset(&m_ctx, 0, sizeof m_ctx);
}

long hash_reader::read()
{
    unsigned char head[16];
    unsigned char tail[32];
    dma_buffer *b;
    unsigned int held = 0, len = 0;
    long r;
    bool last;

    memset(head, 0, sizeof head);
    memset(tail, 0, sizeof tail);
    for (;;) {
        r = m_queue.dequeue_read(&b);
        last = r == 1;
        if ((unsigned long)r > 1)
            return -1;
        if (b->offset) {
            if (b->length > 16)
                return -1;
            memcpy(head, (const unsigned char *)b->ls + b->offset * 16, b->length);
            held = b->length;
        } else {
            bool partial = false;

            if (!last || (b->length & 15) == 0) {
                if (hmac_sha1_update(&m_ctx, (const unsigned char *)b->ls, b->length) < 0)
                    return -1;
            } else {
                partial = true;
            }
            if (partial) {
                memcpy(tail, head, held);
                if (b->length + held > 32)
                    return -1;
                memcpy(tail + held, (const void *)b->ls, b->length);
                len = held + b->length;
            }
        }
        if (last)
            break;
        if (m_queue.enqueue_read(b))
            return -1;
    }
    if (len != 0) {
        if (hmac_sha1_update(&m_ctx, tail, len) < 0)
            return -1;
    }
    if (hmac_sha1_final(m_digest, &m_ctx) < 0)
        return -1;
    return 0;
}
