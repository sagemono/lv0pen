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
        if (in->offset != 0) {
            save_header_piece(in);
            if (g_aes128_ctr.decrypt(g_header.buf, in->length, g_header.buf))
                return 21;
            restore_header_piece(out);
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
