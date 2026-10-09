#include "auth.h"
#include "util.h"
#include "verifier.h"

unsigned char g_part_digest[20];

long authenticator::verify_section_digest(const auth_section *p)
{
    long rc = 19;

    if (p->hashed == 2) {
        g_hmac.final(g_part_digest);
        rc = memcmp(g_part_digest, m_keys + (p->digest << 4), 20) ? 14 : 0;
    }
    return rc;
}

const vec_uchar16 *auth_sink::append(const vec_uchar16 *src, const vec_uchar16 *end)
{
    while (end > src)
        *p++ = *src++;
    return src;
}

long authenticator::drain_read_queue()
{
    dma_buffer *b;
    bool last = false;

    do {
        unsigned long rc = m_queue.dequeue_read(&b);

        if (rc > 1)
            return 21;
        if (rc == 1)
            last = true;
        m_sink.append((const vec_uchar16 *)b->ls,
                      (const vec_uchar16 *)(b->ls + (((b->length >> 4) + b->offset) << 4)));
        if (m_queue.enqueue_read(b))
            return 21;
    } while (last != true);
    return 0;
}

void authenticator::save_header_piece(const dma_buffer *b)
{
    g_header.len = b->length;

    const unsigned char *s = (const unsigned char *)(b->ls + b->offset);
    const unsigned char *end = s + b->length;
    unsigned char *d = g_header.buf;

    while (end > s)
        *d++ = *s++;
}

void authenticator::restore_header_piece(const dma_buffer *b)
{
    unsigned char *d = (unsigned char *)b->ls + b->offset;
    unsigned char *end = d + g_header.len;
    const unsigned char *s = g_header.buf;

    while (end > d)
        *d++ = *s++;
}

auth_block::auth_block()
    : v()
{
}

void auth_block::load(const auth_entry *e)
{
    bool stop = false;

    v.a = 0;
    v.b = 0;
    v.c = 0;
    v.d = 0;
    do {
        switch (e->type) {
        case 1:
            v.a = e->v[0];
            v.b = e->v[1];
            v.c = e->v[2];
            v.d = e->v[3];
            if (e->next == 0)
                return;
            e++;
            break;
        default:
            stop = true;
            break;
        }
    } while (!stop);
}
