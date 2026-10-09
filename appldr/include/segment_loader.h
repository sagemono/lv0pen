#ifndef APPLDR_SEGMENT_LOADER_H
#define APPLDR_SEGMENT_LOADER_H

#include "types.h"

class authenticator;
class dma_buffer;

class segment_loader {
public:
    segment_loader() : m_auth(0) { }
    virtual ~segment_loader() { }

    virtual long load(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
                      unsigned int nin, dma_buffer **out, unsigned int nout) = 0;

    virtual long open(authenticator *a, unsigned int flags) = 0;
    virtual void close() = 0;

protected:
    authenticator *m_auth;
};

class direct_loader : public segment_loader {
public:
    direct_loader();

    long load(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
              unsigned int nin, dma_buffer **out, unsigned int nout);
    long open(authenticator *a, unsigned int flags);
    void close();
};

struct stream_block {
    u32 magic;
    u32 len;
    u64 ea;
    u16 version;
    u8 kind;
    u8 last;
    u32 page;
    u8 pad[32];
    u64 next;
} __attribute__((aligned(16)));

class stream_loader : public segment_loader {
public:
    stream_loader();

    long load(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
              unsigned int nin, dma_buffer **out, unsigned int nout);
    long open(authenticator *a, unsigned int flags);
    void close();

private:
    long notify(unsigned int code);
    long wait_ready();
    long next_block(stream_block *blk);

    u64 m_line;
    u64 m_next;
    u64 m_ready;
    dma_buffer **m_in;
    unsigned int m_nin;
};

#endif
