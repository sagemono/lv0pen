#ifndef ISO_DMA_STREAM_H
#define ISO_DMA_STREAM_H
#define LDR_DMA_STREAM_H

#include "types.h"
#include <spu_mfcio.h>

long mfc_issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
               unsigned int rid, unsigned int cmd);
bool mfc_tag_done(unsigned int tag);

class mfc_base {
public:
    mfc_base();
};

struct list_node {
    void unlink()
    {
        list_node *next = this->next;
        this->prev->next = next;
        next->prev = this->prev;
        this->next = this;
        this->prev = this;
    }

    list_node *next;
    list_node *prev;
};

class list_entry : public list_node {
public:
    list_entry()
    {
        next = this;
        prev = this;
    }
};

class dma_buffer : public list_entry {
public:
    dma_buffer();
    virtual ~dma_buffer() { }
    virtual long get(u64 ea, unsigned int size, unsigned int *done) = 0;
    virtual long put(u64 ea, unsigned int size, unsigned int *done) = 0;
    virtual bool get_done() = 0;
    virtual bool put_done() = 0;
    void set_buffer(unsigned int ls, unsigned int size);

    unsigned int ls;
    unsigned int size;
    unsigned int offset;
    unsigned int length;
    bool idle;
};

class tagged_dma_buffer : public dma_buffer {
public:
    tagged_dma_buffer(unsigned int tag);
    long get(u64 ea, unsigned int size, unsigned int *done);
    long put(u64 ea, unsigned int size, unsigned int *done);
    bool get_done();
    bool put_done();
    unsigned int max_chunk(unsigned int n);
    long transfer(u64 ls, u64 ea, unsigned int size, unsigned int cmd);

    unsigned int tag;
};

class dma_queue {
public:
    dma_queue();
    long open(u64 put_ea, unsigned int put_size, u64 get_ea, unsigned int get_size);
    long add_written(dma_buffer *b);
    long enqueue_read(dma_buffer *b);
    long enqueue_write(dma_buffer *b);
    long dequeue(dma_buffer **out, list_node *list, bool (dma_buffer::*done)());
    long drain(list_node *list, bool (dma_buffer::*done)());
    void close();
    long dequeue_write(dma_buffer **out);
    long dequeue_read(dma_buffer **out);

    u64 put_ea;
    unsigned int put_size;
    u64 get_ea;
    unsigned int get_size;
    u64 put_next;
    unsigned int put_left;
    u64 get_next;
    unsigned int get_left;
    list_node reading;
    list_node writing;
    bool is_open;
    dma_buffer *last_read;
};

#endif
