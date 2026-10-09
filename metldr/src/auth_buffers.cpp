#include "auth.h"
#include "util.h"
#include "verifier.h"

unsigned char g_part_digest[20];

long authenticator::verify_section_digest(const auth_section *p)
{
    long rc = 32;

    if (p->hashed == 2) {
        g_hmac.final(g_part_digest);
        rc = memcmp(g_part_digest, m_keys + (p->digest << 4), 20) ? 35 : 0;
    }
    return rc;
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

void authenticator::prepend(dma_buffer *b, unsigned int n)
{
    char *buf = (char *)g_header.buf;
    char *base = buf + n;
    char *dst = base + g_header.len;
    char *src = buf + g_header.len;
    while (dst > base)
        *--dst = *--src;
    char *end = (char *)(b->ls + b->length);
    char *p = end - n;
    while (p < end)
        *buf++ = *p++;
    g_header.len += n;
}
