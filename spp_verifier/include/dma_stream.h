#ifndef SPPV_DMA_STREAM_H
#define SPPV_DMA_STREAM_H

#include "types.h"
#include "dma_channel.h"

class ea_cursor {
public:
    virtual void set(u64 ea) = 0;
    virtual u64 start() = 0;
    virtual void set_size(unsigned int size) = 0;
    virtual void advance(unsigned int n) = 0;
    virtual unsigned int chunk(unsigned int n) = 0;
    virtual u64 current() = 0;
    virtual unsigned int line_offset() = 0;
};

class read_cursor : public ea_cursor {
public:
    read_cursor();
    void set(u64 ea);
    u64 start();
    void set_size(unsigned int size);
    void advance(unsigned int n);
    unsigned int chunk(unsigned int n)
    {
        unsigned int left = m_size - m_offset;

        n = n < left ? n : left;
        if (n >= 16)
            n &= ~15;
        else if (n >= 8 && n < 16)
            n &= ~7;
        else if (n >= 4 && n < 8)
            n &= ~3;
        else if (n >= 2 && n < 4)
            n &= ~1;
        return n;
    }
    u64 current();
    unsigned int line_offset();
    virtual ~read_cursor() { }

    u64 m_ea __attribute__((aligned(16)));
    unsigned int m_offset __attribute__((aligned(16)));
    unsigned int m_size __attribute__((aligned(16)));
};

class write_cursor : public ea_cursor {
public:
    write_cursor();
    void set(u64 ea);
    u64 start();
    void set_size(unsigned int size);
    void advance(unsigned int n);
    unsigned int chunk(unsigned int n)
    {
        unsigned int left = m_size - m_offset;

        n = n < left ? n : left;
        if (n >= 16)
            n &= ~15;
        else if (n >= 8 && n < 16)
            n &= ~7;
        else if (n >= 4 && n < 8)
            n &= ~3;
        else if (n >= 2 && n < 4)
            n &= ~1;
        return n;
    }
    u64 current();
    unsigned int line_offset();
    virtual ~write_cursor() { }

    u64 m_ea __attribute__((aligned(16)));
    unsigned int m_offset __attribute__((aligned(16)));
    unsigned int m_size __attribute__((aligned(16)));
};

class double_buffer {
public:
    double_buffer(unsigned int ls, unsigned int size);
    virtual ~double_buffer() { }
    void swap()
    {
        unsigned int t = m_cur;

        m_cur = m_next;
        m_next = t;
    }
    unsigned int cur() { return m_cur; }
    unsigned int next() { return m_next; }
    unsigned int half() { return m_half; }
    unsigned int end() { return m_cur + m_half; }

    unsigned int m_cur __attribute__((aligned(16)));
    unsigned int m_next __attribute__((aligned(16)));
    unsigned int m_half __attribute__((aligned(16)));
};

typedef unsigned int quad_uint __attribute__((aligned(16)));

struct ls_span {
    unsigned int begin;
    unsigned int end;
};

class dma_reader {
public:
    dma_reader(unsigned int ls, unsigned int size, unsigned int tag, ea_cursor *cursor);
    void wait();
    void seek(u64 ea);
    void set_size(unsigned int size);
    void fetch();
    void read(ls_span *s);

    dma_channel m_dma;
    double_buffer m_buf;
    unsigned int m_tag __attribute__((aligned(16)));
    unsigned int m_ls __attribute__((aligned(16)));
    unsigned int m_pending __attribute__((aligned(16)));
    ea_cursor *m_cursor;
};

class dma_writer {
public:
    dma_writer(unsigned int ls, unsigned int size, unsigned int tag, ea_cursor *cursor);
    virtual ~dma_writer() { }
    void wait();
    void seek(u64 ea);
    void set_size(unsigned int size);
    void get_buffer(ls_span *s, quad_uint *end);
    unsigned int flush(ls_span *s);
    void put(ls_span *s, quad_uint *end);
    void finish(ls_span *s);

    dma_channel m_dma;
    double_buffer m_buf;
    unsigned int m_tag __attribute__((aligned(16)));
    unsigned int m_ls __attribute__((aligned(16)));
    unsigned int m_pending __attribute__((aligned(16)));
    ea_cursor *m_cursor;
};

inline read_cursor::read_cursor()
{
    m_ea = 0;
    m_offset = 0;
    m_size = 0;
}

inline void read_cursor::set(u64 ea)
{
    m_ea = ea;
    m_offset = 0;
    m_size = 0;
}

inline u64 read_cursor::start()
{
    return m_ea;
}

inline void read_cursor::set_size(unsigned int size)
{
    m_size = size;
}

inline void read_cursor::advance(unsigned int n)
{
    m_offset += n;
}

inline u64 read_cursor::current()
{
    return m_ea + m_offset;
}

inline unsigned int read_cursor::line_offset()
{
    return current() & 0x7F;
}

inline write_cursor::write_cursor()
{
    m_ea = 0;
    m_offset = 0;
    m_size = 0;
}

inline void write_cursor::set(u64 ea)
{
    m_ea = ea;
    m_offset = 0;
}

inline u64 write_cursor::start()
{
    return m_ea;
}

inline void write_cursor::set_size(unsigned int size)
{
    m_size = size;
}

inline void write_cursor::advance(unsigned int n)
{
    m_offset += n;
}

inline u64 write_cursor::current()
{
    return m_ea + m_offset;
}

inline unsigned int write_cursor::line_offset()
{
    return current() & 0x7F;
}

inline double_buffer::double_buffer(unsigned int ls, unsigned int size)
{
    m_half = size / 2;
    m_cur = ls;
    m_next = ls + m_half;
}

inline void dma_reader::set_size(unsigned int size)
{
    m_cursor->set_size(size);
}

inline void dma_writer::seek(u64 ea)
{
    m_cursor->set(ea);
}

inline void dma_writer::set_size(unsigned int size)
{
    m_cursor->set_size(size);
}

inline void dma_writer::get_buffer(ls_span *s, quad_uint *end)
{
    unsigned int e;

    s->begin = m_buf.cur() + m_cursor->line_offset();
    e = m_buf.end();
    *end = e;
    s->end = e;
}

inline void dma_writer::wait()
{
    m_dma.wait(m_tag);
}

inline void dma_reader::wait()
{
    m_dma.wait(m_tag);
}

inline void dma_reader::seek(u64 ea)
{
    m_cursor->set(ea);
    if (m_pending)
        wait();
    m_pending = 0;
}

inline unsigned int dma_writer::flush(ls_span *s)
{
    if (__builtin_expect(m_pending != 0, 1))
        wait();
    m_pending = m_cursor->chunk(s->end - s->begin);
    if (m_pending) {
        m_dma.issue(s->begin, m_cursor->current(), m_pending, m_tag, 0, MFC_PUT);
        m_cursor->advance(m_pending);
        s->begin += m_pending;
        m_buf.swap();
    }
    return m_pending;
}

inline void dma_writer::put(ls_span *s, quad_uint *end)
{
    unsigned int n;

    do {
        n = flush(s);
        if (s->end == s->begin)
            break;
    } while (n != 0);
    get_buffer(s, end);
}

inline void dma_writer::finish(ls_span *s)
{
    quad_uint end;

    put(s, &end);
    if (m_pending)
        wait();
    m_pending = 0;
}

inline void dma_reader::fetch()
{
    unsigned int off = m_cursor->line_offset();

    m_ls = m_buf.next() + off;
    m_pending = m_cursor->chunk(m_buf.half() - off);
    m_dma.issue(m_ls, m_cursor->current(), m_pending, m_tag, 0, MFC_GET);
    m_cursor->advance(m_pending);
}

inline void dma_reader::read(ls_span *s)
{
    if (__builtin_expect(m_pending == 0 && m_cursor->chunk(m_buf.m_half) != 0, 1))
        fetch();
    wait();
    s->begin = m_ls;
    s->end = s->begin + m_pending;
    m_buf.swap();
    if (__builtin_expect(m_cursor->chunk(m_buf.half()) != 0, 1))
        fetch();
    else
        m_pending = 0;
}

inline dma_writer::dma_writer(unsigned int ls, unsigned int size, unsigned int tag, ea_cursor *cursor)
    : m_buf(ls, size)
{
    m_tag = tag;
    m_pending = 0;
    m_cursor = cursor;
}

inline dma_reader::dma_reader(unsigned int ls, unsigned int size, unsigned int tag, ea_cursor *cursor)
    : m_buf(ls, size)
{
    m_tag = tag;
    m_ls = 0;
    m_pending = 0;
    m_cursor = cursor;
}

#endif
