#include "auth.h"
#include "util.h"
#include "verifier.h"

vec_uchar16 g_part_area[512];
unsigned char g_part_digest[20];
header_data g_header;

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

auth_block::auth_block()
    : w(0)
{
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    memset(digest, 0, sizeof(digest));
}

auth_state::auth_state()
{
    memset(buf, 0, sizeof(buf));
    t.a = 0;
    t.b = 0;
    t.c = 0;
    t.d = 0;
}

auth_tail::auth_tail()
{
    len = 0;
    clear();
}

void header_data::clear()
{
    memset(buf, 0, sizeof(buf));
    len = 0;
}

const vec_uchar16 *auth_sink::append(const vec_uchar16 *src, const vec_uchar16 *end)
{
    while (end > src)
        *p++ = *src++;
    return src;
}

long authenticator::check_part_digest(const auth_section *p)
{
    long rc = 19;

    if (p->hashed == 2) {
        g_hmac.final(g_part_digest);
        rc = bytes_differ(g_part_digest, m_keys + (p->digest << 4), 20) ? 14 : 0;
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
    const auth_section *s;
    unsigned int i;

    if (!m_loaded)
        return -4;
    s = m_sections;
    for (i = 0; i < m_meta->section_count; i++, s++) {
        if (s->type == type && s->index == index) {
            *out = *s;
            return 0;
        }
    }
    return -3;
}

long auth_block::load(const auth_entry *e, u64 size)
{
    const auth_entry *base = e;
    u64 off = 0;

    if (e == 0)
        return -1;
    while (off < size) {
        switch (e->type) {
        case 1:
            v[0] = e->u.v[0];
            v[1] = e->u.v[1];
            v[2] = e->u.v[2];
            v[3] = e->u.v[3];
            off += 48;
            break;
        case 2:
            memcpy(digest, e->u.d.key, 20);
            w = e->u.d.w;
            off += 64;
            break;
        case 3:
            off += 144;
            break;
        }
        if (off > size)
            return -1;
        if (e->next == 0)
            return 0;
        e = (const auth_entry *)((const unsigned char *)base + off);
    }
    return 0;
}

long auth_state::load(const auth_entry *e, u64 size)
{
    const auth_entry *base = e;
    u64 off = 0;

    if (e == 0)
        return -3;
    while (off < size) {
        switch (e->type) {
        case 1:
            t.a = e->u.v[0];
            t.b = e->u.v[1];
            t.c = e->u.v[2];
            t.d = e->u.v[3];
            off += 48;
            break;
        case 2:
            memcpy(buf, e->u.v, 256);
            off += 272;
            break;
        }
        if (off > size)
            return -3;
        if (e->next == 0)
            return 0;
        e = (const auth_entry *)((const unsigned char *)base + off);
    }
    return 0;
}
