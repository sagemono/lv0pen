#include "auth.h"
#include "cipher.h"
#include "verifier.h"

long authenticator::transfer_encrypted()
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
            save_header_piece(in);
            if (g_aes128_ctr.decrypt(g_header.buf, in->length, g_header.buf))
                return 21;
            restore_header_piece(out);
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
                        prepend(out, rem);
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

long authenticator::transfer_plain()
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
        if (__builtin_expect(in->offset != 0, 1)) {
            save_header_piece(in);
            if (last) {
                if (g_hmac.update(g_header.buf, g_header.len))
                    return 21;
                g_header.len = 0;
            }
        } else if (last) {
            if (g_header.len == 0) {
                if (g_hmac.update((const unsigned char *)in->ls, in->length))
                    return 21;
            } else {
                unsigned int rem = in->length & 15;

                if (g_hmac.update((const unsigned char *)in->ls, in->length - rem))
                    return 21;
                if (rem)
                    prepend(in, rem);
                if (g_hmac.update(g_header.buf, g_header.len))
                    return 21;
            }
            g_header.len = 0;
        } else {
            if (g_hmac.update((const unsigned char *)in->ls, in->length))
                return 21;
        }
        if (m_queue.enqueue_write(in))
            return 21;
        if (m_queue.enqueue_read(out))
            return 21;
    } while (last != true);
    return 0;
}

long authenticator::check_type(unsigned int type)
{
    return type == m_app->type ? 0 : 28;
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

long authenticator::check_sce_header(unsigned int type, const sce_header *h)
{
    m_sce = h;
    if (h->magic != 0x53434500)
        return 19;
    if (h->version != 2)
        return 19;
    if (h->type != type)
        return 19;
    if (h->metadata_offset & 15)
        return 19;
    if (h->header_len & 127)
        return 19;
    if (h->header_len < 128)
        return 19;
    m_410 = h->key_revision & 15;
    if (m_410 > 1)
        return 19;
    if (h->key_revision & 0x8000)
        m_409 = 1;
    return 0;
}
