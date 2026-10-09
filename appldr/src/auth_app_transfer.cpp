#include "auth.h"
#include "cipher.h"
#include "verifier.h"

extern "C" int lv1ldr_inflate_init(void);
extern "C" long lv1ldr_inflate_end(void);

extern unsigned char g_3e40[];

long authenticator::transfer_segment(u64 src, u64 size, u64 dest, u64 dest_size,
                                     dma_buffer **in, unsigned int nin, dma_buffer **out,
                                     unsigned int nout, u64 *written)
{
    unsigned int i;
    long rc;
    u64 done;

    if (!m_loaded)
        return -4;
    if (m_section.size == 0) {
        *written = 0;
        return 0;
    }
    if (m_queue.open(dest, dest_size, src, size))
        return 21;
    switch (m_section.compressed) {
    case 1:
        for (i = 0; i != nout; i++)
            if (m_queue.add_written(out[i]))
                return 21;
        break;
    case 2:
        for (i = 0; i != nout; i++)
            if (!out[i]->idle)
                if (m_queue.add_written(out[i]))
                    return 21;
        break;
    default:
        return 19;
    }
    for (i = 0; i != nin; i++)
        if (m_queue.enqueue_read(in[i]))
            return 21;
    done = 0;
    switch (m_section.encrypted) {
    case 3:
        if (m_section.compressed == 1)
            rc = transfer_stored(true, m_1b9, &done);
        else if (m_section.compressed == 2)
            rc = transfer_deflated(dest, true, m_1b9, &done);
        else
            rc = 19;
        break;
    case 1:
        if (m_section.compressed == 1)
            rc = transfer_stored(false, m_1b9, &done);
        else if (m_section.compressed == 2)
            rc = transfer_deflated(dest, false, m_1b9, &done);
        else
            rc = 19;
        break;
    default:
        rc = 19;
    }
    m_queue.close();
    if (rc)
        return rc;
    *written = done;
    return 0;
}

long authenticator::transfer_deflated(u64 ea, bool decrypt, bool in_place, u64 *written)
{
    dma_buffer *in = 0;
    u64 done = 0;
    bool last = false;
    long rc;

    if (m_1f0 == 0)
        if (lv1ldr_inflate_init())
            { rc = 21; goto out; }
    do {
        unsigned long r = m_queue.dequeue_read(&in);

        if (r > 1)
            { rc = 21; goto out; }
        if (r == 1)
            last = true;
        m_1f0 += in->length;
        if (in->offset != 0) {
            m_tail.save(in);
            if (decrypt) {
                if (in_place) {
                    if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, (void *)in->ls))
                        { rc = 21; goto out; }
                } else if (m_tail.decrypt())
                    { rc = 21; goto out; }
                if (m_tail.restore(in))
                    { rc = 21; goto out; }
                if (m_1f0 != m_section.size)
                    goto piece;
            } else if (m_1f0 != m_section.size)
                goto piece;
            if (m_tail.hash())
                { rc = 21; goto out; }
        piece:
            rc = inflate_piece(in, ea, &done);
            if (rc)
                goto out;
        } else {
            if (decrypt) {
                if (in_place) {
                    if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, g_3e40))
                        { rc = 21; goto out; }
                } else {
                    if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, (void *)in->ls))
                        { rc = 21; goto out; }
                }
            }
            if (!last) {
                if (g_hmac.update((const unsigned char *)in->ls, in->length))
                    { rc = 21; goto out; }
            } else {
                if (m_tail.empty()) {
                    if (g_hmac.update((const unsigned char *)in->ls, in->length))
                        { rc = 21; goto out; }
                } else {
                    unsigned int rem = in->length & 15;

                    if (g_hmac.update((const unsigned char *)in->ls, in->length - rem))
                        { rc = 21; goto out; }
                    if (m_1f0 == m_section.size) {
                        if (rem) {
                            if (m_tail.prepend(in, rem))
                                { rc = 21; goto out; }
                            if (m_tail.hash())
                                { rc = 21; goto out; }
                        } else if (m_tail.hash())
                            { rc = 21; goto out; }
                    }
                }
            }
            rc = inflate_piece(in, ea, &done);
            if (rc)
                goto out;
        }
        if (m_queue.enqueue_read(in))
            { rc = 21; goto out; }
    } while (last != true);
    *written = done;
    rc = 0;
    if (m_1f0 == m_section.size) {
        rc = lv1ldr_inflate_end();
        if (rc)
            rc = 21;
    }
out:
    return rc;
}

long authenticator::transfer_stored(bool decrypt, bool raw, u64 *written)
{
    dma_buffer *in = 0, *out = 0;
    u64 n = 0;
    bool last = false;

    do {
        unsigned long r = m_queue.dequeue_read(&in);

        if (r > 1)
            return 21;
        if (r == 1)
            last = true;
        m_1f0 += in->length;
        if (m_queue.dequeue_write(&out))
            return 21;
        out->length = in->length;
        out->offset = in->offset;
        switch (in->offset) {
        default:
            m_tail.save(in);
            if (decrypt) {
                if (raw) {
                    if (g_aes128_ctr.decrypt((const void *)out->ls, in->length, (void *)out->ls))
                        return 21;
                } else if (m_tail.decrypt())
                    return 21;
                if (m_tail.restore(out))
                    return 21;
                if (m_1f0 != m_section.size)
                    break;
            } else if (m_1f0 != m_section.size)
                break;
            if (m_tail.hash())
                return 21;
            break;
        case 0:
            if (decrypt) {
                if (in->length > out->size)
                    return 21;
                if (g_aes128_ctr.decrypt((const void *)in->ls, in->length, (void *)out->ls))
                    return 21;
            }
            if (!last) {
                if (decrypt) {
                    if (g_hmac.update((const unsigned char *)out->ls, in->length))
                        return 21;
                } else {
                    if (g_hmac.update((const unsigned char *)in->ls, in->length))
                        return 21;
                }
            } else {
                if (m_tail.empty()) {
                    if (decrypt) {
                        if (g_hmac.update((const unsigned char *)out->ls, in->length))
                            return 21;
                    } else {
                        if (g_hmac.update((const unsigned char *)in->ls, in->length))
                            return 21;
                    }
                } else {
                    unsigned int rem = in->length & 15;

                    if (decrypt) {
                        if (g_hmac.update((const unsigned char *)out->ls, in->length - rem))
                            return 21;
                        if (m_1f0 == m_section.size) {
                            if (rem)
                                if (m_tail.prepend(out, rem))
                                    return 21;
                            if (m_tail.hash())
                                return 21;
                        }
                    } else {
                        if (g_hmac.update((const unsigned char *)in->ls, in->length - rem))
                            return 21;
                        if (m_1f0 == m_section.size) {
                            if (rem)
                                if (m_tail.prepend(in, rem))
                                    return 21;
                            if (m_tail.hash())
                                return 21;
                        }
                    }
                }
            }
            break;
        }
        if (requeue(in, out, decrypt, raw, &n))
            return 21;
        *written += n;
    } while (last != true);
    return 0;
}

long authenticator::requeue(dma_buffer *in, dma_buffer *out, const bool &decrypt,
                            const bool &raw, u64 *written)
{
    if (!decrypt || raw) {
        if (m_queue.enqueue_write(in))
            return 21;
        *written = in->length;
        if (m_queue.enqueue_read(out))
            return 21;
    } else {
        if (m_queue.enqueue_write(out))
            return 21;
        *written = out->length;
        if (m_queue.enqueue_read(in))
            return 21;
    }
    return 0;
}
