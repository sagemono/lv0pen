#include "xdr.h"

xdr::~xdr()
{
}

long xdr::set_config(memory_config *c)
{
    m_mcp = c;
    m_mcp->f_98 = 490;
    m_mcp->f_9c = m_mcp->xio_ref_clk * 8;
    m_mcp->f_a0 = m_mcp->f_9c >> 1;
    m_mcp->f_a4 = m_mcp->f_9c >> 3;
    m_mcp->f_a8 = m_mcp->f_a4 / 1000000000 * m_mcp->f_98 - 1;
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    switch (ratio) {
    case 1:
        m_mcp->f_ac = 0;
        break;
    case 2:
        m_mcp->f_ac = 1;
        break;
    case 4:
        m_mcp->f_ac = 2;
        break;
    case 8:
        m_mcp->f_ac = 3;
        break;
    case 16:
        m_mcp->f_ac = 4;
        break;
    default:
        m_mcp->f_ac = 4;
        break;
    }
    m_mcp->f_b0 = (m_mcp->basic[3] << 2) / 2;
    m_mcp->f_b4 = m_mcp->basic[2] / ratio;
    m_mcp->f_b8 = m_mcp->f_b0 / m_mcp->f_b4;
    if (m_mcp->basic[5] == 1) {
        m_mcp->ecc_off_ch0 = false;
        m_mcp->ecc_off_ch1 = false;
        m_mcp->ecc_on = true;
    } else {
        m_mcp->ecc_off_ch0 = true;
        m_mcp->ecc_off_ch1 = true;
        m_mcp->ecc_on = false;
    }
    m_mcp->xio_ch0_on = false;
    m_mcp->xio_ch1_on = false;
    return 20;
}
