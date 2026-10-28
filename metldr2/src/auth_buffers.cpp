#include "auth.h"
#include "util.h"
#include "verifier.h"

vec_uchar16 g_part_area[128];
unsigned char g_part_digest[20];
header_data g_header;

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

long authenticator::get_app_info(auth_app_info *out)
{
    long rc = -3;

    if (m_loaded) {
        *out = *m_app;
        rc = 0;
    }
    return rc;
}

const vec_uchar16 *auth_sink::append(const vec_uchar16 *src, const vec_uchar16 *end)
{
    while (end > src)
        *p++ = *src++;
    return src;
}

long authenticator::check_part_digest(const auth_section *p)
{
    long rc = 32;

    if (p->hashed == 2) {
        g_hmac.final(g_part_digest);
        rc = bytes_differ(g_part_digest, m_keys + (p->digest << 4), 20) ? 35 : 0;
    }
    return rc;
}

long authenticator::read_parts()
{
    dma_buffer *b;
    bool last = false;

    do {
        unsigned long rc = m_queue.dequeue_read(&b);

        if (rc > 1)
            return 21;
        if (rc == 1)
            last = true;
        if (b->offset != 0)
            return 21;
        m_sink.append((const vec_uchar16 *)b->ls,
                      (const vec_uchar16 *)(b->ls + (b->length & ~15)));
        if (m_queue.enqueue_read(b))
            return 21;
    } while (last != true);
    return 0;
}

long authenticator::save_tail(const dma_buffer *b)
{
    if (b == 0)
        return 21;
    if (b->length > 32)
        return 21;
    g_header.len = b->length;

    const unsigned char *s = (const unsigned char *)b->ls + b->offset;
    const unsigned char *end = s + b->length;
    unsigned char *d = g_header.buf;

    while (end > s)
        *d++ = *s++;
    return 0;
}

long authenticator::restore_tail(const dma_buffer *b)
{
    if (b == 0)
        return 21;
    if (g_header.len > b->size - b->offset)
        return 21;

    unsigned char *d = (unsigned char *)b->ls + b->offset;
    unsigned char *end = d + g_header.len;
    const unsigned char *s = g_header.buf;

    while (end > d)
        *d++ = *s++;
    return 0;
}

long authenticator::prepend(const dma_buffer *b, unsigned int n)
{
    if (b == 0)
        return 21;

    char *buf = (char *)g_header.buf;
    char *base = buf + n;

    if (g_header.len + n > 32)
        return 21;

    char *dst = base + g_header.len;
    char *src = buf + g_header.len;

    while (dst > base)
        *--dst = *--src;

    const char *end = (const char *)b->ls + b->length;
    const char *p = end - n;

    while (p < end)
        *buf++ = *p++;
    g_header.len += n;
    return 0;
}

long authenticator::find_section(unsigned int type, unsigned int index, auth_section *out)
{
    long rc = -3;

    if (m_loaded) {
        const auth_section *s = m_sections;
        unsigned int i;

        rc = -2;
        for (i = 0; i < m_meta->section_count; i++, s++) {
            if (s->type == type && s->index == index) {
                *out = *s;
                rc = 0;
                break;
            }
        }
    }
    return rc;
}
