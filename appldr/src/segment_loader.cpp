#include "segment_loader.h"
#include "mfc_atomic.h"
#include "auth.h"
#include "dma_stream.h"
#include "mailbox.h"
#include "util.h"

direct_loader::direct_loader()
{
}

long direct_loader::open(authenticator *a, unsigned int flags)
{
    if (!a)
        return 21;
    m_auth = a;
    return 0;
}

void direct_loader::close()
{
    m_auth = 0;
}

long direct_loader::load(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
                         unsigned int nin, dma_buffer **out, unsigned int nout)
{
    u64 written;
    long r;

    if (!m_auth)
        return -1;
    written = 0;
    r = m_auth->transfer_segment(src, size, dest, dest_size, in, nin, out, nout, &written);
    if (r)
        return r;
    return m_auth->check_section_digest();
}

stream_loader::stream_loader()
    : m_line(0), m_next(0), m_ready(0), m_in(0), m_nin(0)
{
}

long stream_loader::open(authenticator *a, unsigned int flags)
{
    if (!a)
        return 21;
    m_auth = a;
    return 0;
}

void stream_loader::close()
{
    m_line = 0;
    m_next = 0;
    m_ready = 0;
    m_in = 0;
    m_nin = 0;
    m_auth = 0;
}

long stream_loader::notify(unsigned int code)
{
    unsigned int v = code;
    u64 ea = m_line;
    unsigned int off = 20;

    if (ea & 0x7F) {
        off = ((unsigned int)ea & 0x7F) + 20;
        ea &= ~0x7FULL;
    }
    long r = atomic_put(ea, (void *)0x3E000, &v, off, 4);

    if (r)
        r = 21;
    return r;
}

long stream_loader::wait_ready()
{
    unsigned int ev;
    long r = 45;

    if (m_ready & 0x7F)
        return r;
    for (;;) {
        event_mask(1024);
        r = atomic_get(m_ready, (void *)0x3E000);
        if (r)
            return r;
        if (*(const u64 *)0x3E000 != 0)
            break;
        event_status(&ev);
        event_mask(0);
        event_ack(ev);
    }
    u64 line = *(const u64 *)0x3E000;
    m_line = line;
    m_next = line;
    return r;
}

long stream_loader::next_block(stream_block *blk)
{
    const stream_block *b = (const stream_block *)0x3E000;
    unsigned int ev;
    long r;

    if (m_line & 0xF)
        return 45;
    while (m_next == 0) {
        event_mask(1024);
        u64 ea = m_line;
        u64 off = ea & 0x7F;
        if (off)
            ea &= ~0x7FULL;
        r = atomic_get(ea, (void *)0x3E000);
        if (r)
            return r;
        m_next = ((const stream_block *)((const unsigned char *)0x3E000 + off))->next;
        if (m_next != 0)
            break;
        event_status(&ev);
        event_mask(0);
        event_ack(ev);
    }
    mfc_issue(0x3E000, m_next, 64, 1, 0, MFC_GET_CMD);
    while (!mfc_tag_done(1))
        ;
    if (b->magic != 1 || b->kind != 1 || b->page != 4096 || b->version != 1)
        return 45;
    m_line = m_next;
    m_next = b->next;
    memcpy(blk, b, 64);
    return notify(4097);
}

long stream_loader::load(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
                         unsigned int nin, dma_buffer **out, unsigned int nout)
{
    stream_block blk;
    u64 written;
    u64 left, cur, used;
    long r;

    if (!m_auth)
        return -1;
    if (src == 0 || dest == 0)
        return -2;
    m_ready = src;
    m_line = 0;
    written = 0;
    m_in = in;
    m_next = 0;
    m_nin = nin;
    memset(&blk, 0, sizeof blk);
    r = wait_ready();
    if (r)
        return r;
    cur = dest;
    left = dest_size;
    used = 0;
    for (;;) {
        r = next_block(&blk);
        if (r)
            return r;
        if (used + blk.len > size)
            return 21;
        written = 0;
        r = m_auth->transfer_segment(blk.ea, blk.len, cur, left, in, nin, out, nout, &written);
        if (r)
            return r;
        cur += written;
        left -= written;
        used += blk.len;
        if (cur - dest > dest_size)
            return 21;
        if (blk.last == 0xFF)
            break;
        notify(4099);
        mbox_out_intr_write(6);
    }
    r = m_auth->check_section_digest();
    if (r)
        return r;
    notify(4098);
    return r;
}
