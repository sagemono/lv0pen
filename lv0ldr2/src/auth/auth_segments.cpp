#include "auth.h"
#include "cipher.h"
#include "util.h"
#include "verifier.h"

auth_state::auth_state()
    : buf(), t()
{
    memset(buf, 0, sizeof(buf));
    t.a = 0;
    t.b = 0;
    t.c = 0;
    t.d = 0;
}

long authenticator::load_segment(unsigned int type, unsigned int index, u64 ea,
                                 dma_buffer **in, int nin, dma_buffer **out, int nout)
{
    long rc;
    int i;

    if (!m_loaded)
        return -4;
    if (find_section(type, index, &m_section))
        return 19;
    if (m_section.size == 0)
        return 0;
    m_queue.open(ea, m_section.size, m_offset + m_section.offset, m_section.size);
    for (i = 0; i < nin; i++)
        if (m_queue.enqueue_read(in[i]))
            return 21;
    for (i = 0; i < nout; i++)
        if (m_queue.add_written(out[i]))
            return 21;
    if (m_section.hashed != 2)
        return 19;
    g_hmac.init(m_keys + (m_section.digest << 4) + 32, 64);
    if (m_section.encrypted != 1) {
        if (m_section.encrypted != 3)
            return 19;
        g_aes128_ctr.set_key(m_keys + (m_section.key << 4),
                             (const cipher_iv *)(m_keys + (m_section.iv << 4)));
        if (m_section.compressed == 1)
            rc = transfer_encrypted();
        else if (m_section.compressed == 2)
            rc = 0;
        else
            return 19;
    } else {
        if (m_app->type == 1 || m_app->type == 2 || m_app->type == 5)
            return 19;
        if (m_section.compressed == 1)
            rc = transfer_plain();
        else if (m_section.compressed == 2)
            rc = 0;
        else
            return 19;
    }
    if (rc)
        return rc;
    if (verify_section_digest(&m_section))
        return 14;
    m_queue.close();
    return 0;
}
