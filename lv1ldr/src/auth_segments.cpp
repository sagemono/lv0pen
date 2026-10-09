#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

long authenticator::load_segment(unsigned int type, unsigned int index, u64 ea, u64 size,
                                 dma_buffer **in, int nin, dma_buffer **out, int nout)
{
    long rc;
    int err = 19;
    int i;

    if (!m_loaded)
        return -4;
    rc = find_section(type, index, &m_section);
    if (rc)
        return 19;
    if (m_section.size == 0)
        return rc;
    if (m_queue.open(ea, size, m_offset + m_section.offset, m_section.size))
        return 21;
    for (i = 0; i < nin; i++)
        if (m_queue.enqueue_read(in[i]))
            return 21;
    for (i = 0; i < nout; i++)
        if (m_queue.add_written(out[i]))
            return 21;
    switch (m_section.hashed) {
    case 2:
        g_hmac.init(m_keys + (m_section.digest << 4) + 32, 64);
        break;
    default:
        return 19;
    }
    if (m_section.encrypted != 3)
        return err;
    g_aes128_ctr.set_key(m_keys + (m_section.key << 4), m_keys + (m_section.iv << 4));
    if (m_section.compressed == 1)
        rc = transfer_stored();
    else if (m_section.compressed == 2)
        rc = transfer_deflated(ea);
    else
        return 19;
    if (rc)
        return rc;
    if (check_part_digest(&m_section))
        return 14;
    m_queue.close();
    return 0;
}
