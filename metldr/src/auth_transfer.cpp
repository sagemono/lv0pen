#include "auth.h"
#include "cipher.h"
#include "verifier.h"

long authenticator::check_type(unsigned int type)
{
    return type == m_app->type ? 0 : 32;
}

long authenticator::check_sce_header(unsigned int type)
{
    const sce_header *h = (const sce_header *)m_sink.base;

    m_sce = h;
    if (h->magic != 0x53434500)
        return 32;
    if (h->version != 2)
        return 32;
    if (h->type != type)
        return 32;
    if (h->metadata_offset & 15)
        return 19;
    if (h->header_len & 127)
        return 19;
    if (h->header_len < 128)
        return 19;
    m_58 = h->key_revision & 15;
    if (m_58 != 0)
        return 32;
    if (h->key_revision & 0x8000)
        return 32;
    return 0;
}

long authenticator::get_app_info(auth_app_info *out)
{
    if (!m_loaded)
        return -3;
    *out = *m_app;
    return 0;
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

long authenticator::load_segment(unsigned int type, unsigned int index, u64 ea,
                                 dma_buffer **in, int nin, dma_buffer **out, int nout)
{
    long rc;
    int i;

    if (!m_loaded)
        return -3;
    if (find_section(type, index, &m_section))
        return 32;
    if (m_section.size == 0)
        return 0;
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
        g_aes128_ctr.set_key(m_keys + (m_section.key << 4),
                             (const cipher_iv *)(m_keys + (m_section.iv << 4)));
        break;
    default:
        return 32;
    }
    if (m_section.compressed != 1)
        return 32;
    rc = transfer_encrypted();
    if (rc)
        return rc;
    if (verify_section_digest(&m_section))
        return 35;
    m_queue.close();
    return 0;
}
