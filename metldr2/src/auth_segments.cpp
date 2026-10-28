#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

long authenticator::load_segment(unsigned int type, unsigned int index, u64 ea, u64 size,
                                 dma_buffer **in, int nin, dma_buffer **out, int nout)
{
    long rc;
    int i;

    if (!m_loaded)
        return -3;
    rc = find_section(type, index, &m_section);
    if (rc)
        return 32;
    if (m_section.size == 0)
        return rc;
    if (m_section.size > size)
        return 32;
    m_queue.open(ea, m_section.size, m_offset + m_section.offset, m_section.size);
    for (i = 0; i < nin; i++)
        if (m_queue.enqueue_read(in[i]))
            return 21;
    for (i = 0; i < nout; i++)
        if (m_queue.add_written(out[i]))
            return 21;
    g_hmac.init(m_keys + (m_section.digest << 4) + 32, 64);
    switch (m_section.encrypted) {
    case 3:
        g_aes128_ctr.set_key(m_keys + (m_section.key << 4), m_keys + (m_section.iv << 4));
        break;
    default:
        return 32;
    }
    if (m_section.compressed != 1)
        return 32;
    rc = transfer_stored();
    if (rc)
        return rc;
    if (check_part_digest(&m_section))
        return 35;
    m_queue.close();
    return 0;
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
        if (in->offset != 0) {
            if (save_tail(in))
                return 21;
            if (g_aes128_ctr.decrypt(g_header.buf, in->length, g_header.buf))
                return 21;
            if (restore_tail(out))
                return 21;
            out->offset = in->offset;
            if (__builtin_expect(last, 0)) {
                if (g_hmac.update(g_header.buf, g_header.len))
                    return 21;
                g_header.len = 0;
            }
        } else {
            out->offset = in->offset;
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
