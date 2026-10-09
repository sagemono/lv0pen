#include "dma_stream.h"

dma_buffer::dma_buffer()
    : ls(0), size(0), offset(0), length(0), idle(false)
{
}

void dma_buffer::set_buffer(unsigned int ls, unsigned int size)
{
    this->ls = ls;
    this->size = size;
    offset = 0;
    length = 0;
}

tagged_dma_buffer::tagged_dma_buffer(unsigned int tag)
{
    this->tag = tag;
}

bool tagged_dma_buffer::get_done()
{
    return mfc_tag_done(tag);
}

bool tagged_dma_buffer::put_done()
{
    return mfc_tag_done(tag);
}

unsigned int tagged_dma_buffer::max_chunk(unsigned int n)
{
    if (n >= 16)
        return 16;
    if (n >= 8)
        return 8;
    if (n >= 4)
        return 4;
    if (n >= 2)
        return 2;
    if (n >= 1)
        return 1;
    return 0;
}

long tagged_dma_buffer::transfer(u64 ls, u64 ea, unsigned int size, unsigned int cmd)
{
    unsigned int i = 0;
    unsigned int n;
    if (size > 15)
        return -241;
    do {
        u64 a = ls + i;
        if (a & 1)
            n = max_chunk(size < 1 ? size : 1);
        else if (a & 3)
            n = max_chunk(size < 2 ? size : 2);
        else if (a & 7)
            n = max_chunk(size < 4 ? size : 4);
        else if (a & 15)
            n = max_chunk(size < 8 ? size : 8);
        else {
            n = max_chunk(size);
            if (n == 0)
                break;
        }
        mfc_issue((unsigned int)ls + i, ea + i, n, tag, 0, cmd);
        size -= n;
        i += n;
    } while (size);
    return 0;
}

class dma_mfc : public mfc_base {
};

static dma_mfc g_dma_mfc;
