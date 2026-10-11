#include <spu_intrinsics.h>
#include "ss_iso_dma_data.h"
#include "ss_iso_log.h"
#include "util.h"

#define TAG             7

#define LINK_EIO        15
#define LINK_EALIGN     22

#define BE32(p)         ((p)[0] << 24 | (p)[1] << 16 | (p)[2] << 8 | (p)[3])

bool ss_iso_dma_data::aligned()
{
    if ((m_ea & 15) != 0)
        return false;
    return ((unsigned int)m_ls & 15) == 0;
}

void ss_iso_dma_data::chunk(const quad_uint &left, quad_uint &n)
{
    if (left > 0x4000)
        n = 0x4000;
    else if (left > 15)
        n = left & ~15;
    else if (left > 7)
        n = left & ~7;
    else if (left > 3)
        n = left & ~3;
    else if (left > 1)
        n = left & ~1;
    else
        n = left;
}

unsigned int ss_iso_dma_data::put(const unsigned int &token, const unsigned int &cmd,
                                  const quad_uint &len, const quad_ptr &buf)
{
    unsigned int err = 0x80010300;
    unsigned int r;
    quad_uint left, step;
    u64 ea;
    unsigned char *p;

    if (!aligned()) {
        r = LINK_EALIGN;
        SS_ISO_ERR(err, 0x80);
        return r;
    }
    if (((unsigned int)buf & 15) == 0)
        goto header;
    r = DMA_EINVAL;
    SS_ISO_ERR(err, 0x81);
    return r;
header_failed:
    r = LINK_EIO;
    SS_ISO_ERR(err, 0x82);
    return r;
data:
    left = len;
    step = 0;
    ea = m_ea + m_hdr_size;
    p = buf;
    while (left != 0) {
        chunk(left, step);
        if (m_dma.issue((unsigned int)p, ea, step, TAG, 0, MFC_PUT) != 0)
            goto data_failed;
        p += step;
        left -= step;
        ea += step;
    }
    m_dma.wait(TAG);
    r = notify(token);
    if (r != 0) {
        SS_ISO_ERR(err, 0x84);
        return r;
    }
    m_seq++;
    return r;
header:
    {
        ss_iso_msg_header hdr;

        hdr.seq = m_seq;
        hdr.token = token;
        hdr.cmd = cmd;
        hdr.len = len;
        memcpy(m_ls, &hdr, sizeof(hdr));
        if (m_dma.issue((unsigned int)m_ls, m_ea, sizeof(hdr), TAG, 0, MFC_PUT) != 0)
            goto header_failed;
        goto data;
    }
data_failed:
    r = LINK_EIO;
    SS_ISO_ERR(err, 0x83);
    return r;
}

void ss_iso_dma_data::finish(const unsigned int &status, const unsigned int &cmd,
                             const quad_uint &len, const quad_ptr &buf)
{
    unsigned int err = 0x80010400;
    ss_iso_msg_header hdr;
    unsigned int n = len;
    quad_uint left, step;
    u64 ea;
    unsigned char *p;

    if (((unsigned int)buf & 15) == 0)
        goto header;
    goto misaligned;
data_failed:
    SS_ISO_ERR(err, 0x82);
    goto done;
misaligned:
    SS_ISO_ERR(err, 0x80);
    n = 0;
header:
    hdr.seq = m_seq;
    hdr.token = SS_ISO_MSG_STATUS;
    hdr.cmd = cmd;
    hdr.len = n;
    memcpy(m_ls, &hdr, sizeof(hdr));
    if (m_dma.issue((unsigned int)m_ls, m_ea, sizeof(hdr), TAG, 0, MFC_PUT) != 0) {
        SS_ISO_ERR(err, 0x81);
        n = 0;
    }
    left = n;
    step = 0;
    ea = m_ea + m_hdr_size;
    p = buf;
    while (left != 0) {
        chunk(left, step);
        if (m_dma.issue((unsigned int)p, ea, step, TAG, 0, MFC_PUT) != 0)
            goto data_failed;
        p += step;
        left -= step;
        ea += step;
    }
done:
    m_dma.wait(TAG);
    switch (status) {
    case 0x100:
        asm volatile("stop %0" : : "i"(0x100));
        break;
    case 0x101:
        asm volatile("stop %0" : : "i"(0x101));
        break;
    case 0x102:
        asm volatile("stop %0" : : "i"(0x102));
        break;
    case 0x104:
        asm volatile("stop %0" : : "i"(0x104));
        break;
    case 0x10A:
        asm volatile("stop %0" : : "i"(0x10A));
        break;
    case 0x10B:
        asm volatile("stop %0" : : "i"(0x10B));
        break;
    default:
        asm volatile("stop %0" : : "i"(0x103));
        break;
    }
}

unsigned int ss_iso_dma_data::get(const quad_ptr &buf, const quad_uint &max, unsigned int &token,
                                  unsigned int &cmd, quad_uint &len)
{
    unsigned int err = 0x80010200;
    quad_uint n = 0;
    quad_uint word = 0;
    unsigned char *h;
    quad_uint left, step;
    u64 ea;
    unsigned char *p;

    do {
        if (m_dma.in_mbox_count(n) != 0)
            goto mbox_failed;
    } while (n == 0);
    SS_ISO_LOG_RESET();
    if (m_dma.read_in_mbox(word) != 0) {
        SS_ISO_ERR(err, 0x81);
        return LINK_EIO;
    }
    if (m_dma.issue((unsigned int)m_ls, m_ea, m_hdr_size, TAG, 0, MFC_GET) != 0) {
        SS_ISO_ERR(err, 0x82);
        return LINK_EIO;
    }
    m_dma.wait(TAG);
    h = m_ls;
    if (BE32(h) != m_seq) {
        SS_ISO_ERR(err, 0x83);
        return LINK_EIO;
    }
    m_seq = BE32(h) + 1;
    if (BE32(h + 4) != word) {
        SS_ISO_ERR(err, 0x84);
        return LINK_EIO;
    }
    token = BE32(h + 4);
    cmd = BE32(h + 8);
    len = BE32(h + 12);
    if (!aligned()) {
        SS_ISO_ERR(err, 0x85);
        return LINK_EALIGN;
    }
    if (((unsigned int)buf & 15) == 0)
        goto check_len;
    SS_ISO_ERR(err, 0x86);
    return DMA_EINVAL;
too_long:
    SS_ISO_ERR2(err, 0x87, max, len);
    return DMA_EINVAL;
data:
    left = len;
    step = 0;
    ea = m_ea + m_hdr_size;
    p = buf;
    while (left != 0) {
        chunk(left, step);
        if (m_dma.issue((unsigned int)p, ea, step, TAG, 0, MFC_GET) != 0)
            goto data_failed;
        p += step;
        left -= step;
        ea += step;
    }
    if (len != 0)
        m_dma.wait(TAG);
    return 0;
check_len:
    if (len > max)
        goto too_long;
    goto data;
data_failed:
    SS_ISO_ERR(err, 0x88);
    return LINK_EIO;
mbox_failed:
    SS_ISO_ERR(err, 0x80);
    return LINK_EIO;
}

ss_iso_dma_data::ss_iso_dma_data(const quad_u64 &ea, const quad_uint &size, const quad_ptr &ls,
                                 const quad_uint &hdr_size)
    : m_seq(1), m_ea(ea), m_size(size), m_ls(ls), m_hdr_size(hdr_size)
{
}

ss_iso_dma_data::~ss_iso_dma_data()
{
}

unsigned int ss_iso_dma_data::notify(const unsigned int &v)
{
    unsigned int err = 0x80010100;
    for (;;) {
        unsigned int r = m_dma.write_out_intr_mbox(v);

        if (r == 0)
            return 0;
        if (r != MBOX_FULL) {
            SS_ISO_ERR(err, 0x80);
            return LINK_EIO;
        }
    }
}
