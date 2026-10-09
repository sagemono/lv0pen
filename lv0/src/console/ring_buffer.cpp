#include "console.h"

ring_buffer::ring_buffer() : capacity(0), count(0), write_index(0), read_index(0), data(0), overwrite(0)
{
}

void ring_buffer::attach(long size, ring_buffer_header *header)
{
    long *c = &header->capacity, *n = c + 1, *w = n + 1, *r = w + 1;

    *c = size - 32;
    *n = 0;
    *w = 0;
    overwrite = 1;
    *r = 0;
    *(long *)&capacity = (long)c;
    *(long *)&count = (long)n;
    *(long *)&write_index = (long)w;
    *(long *)&read_index = (long)r;
    data = (unsigned char *)(r + 1);
}

ring_buffer::~ring_buffer()
{
}

long ring_buffer::push(unsigned char byte)
{
    int ret;
    long *widx;

    if (*count == *capacity) {
        ret = -2;
        if (overwrite) {
            data[*write_index] = byte;
            widx = write_index;
            if (*widx == *capacity - 1)
                *widx = 0;
            else
                *widx += 1;
            ret = 0;
            *read_index = *widx;
        }
    } else {
        data[*write_index] = byte;
        widx = write_index;
        if (*widx == *capacity - 1)
            *widx = 0;
        else
            *widx += 1;
        ret = 0;
        ++*count;
    }
    return ret;
}

int ring_buffer::pop(unsigned char *out)
{
    long idx, cap;

    if (*count == 0)
        return -10;

    *out = data[*read_index];

    cap = *capacity;
    idx = *read_index;
    if (idx == cap - 1)
        *read_index = 0;
    else
        *read_index = idx + 1;
    --*count;
    return 0;
}

int ring_buffer::write(const unsigned char *src, unsigned long len, unsigned long *done)
{
    long *widx;
    int ret = 0;

    *done = 0;
    for (; !ret && len; len--) {
        if (*count == *capacity) {
            if (!overwrite) {
                ret = -2;
                continue;
            }
            data[*write_index] = *src;
            widx = write_index;
            if (*widx == *capacity - 1)
                *widx = 0;
            else
                *widx += 1;
            *read_index = *widx;
        } else {
            data[*write_index] = *src;
            widx = write_index;
            if (*widx == *capacity - 1)
                *widx = 0;
            else
                *widx += 1;
            ++*count;
        }
        src++;
        ++*done;
    }
    return ret;
}

int ring_buffer::read(unsigned char *dst, unsigned long len, unsigned long *done)
{
    long idx, cap;

    *done = 0;
    while (len--) {
        if (*count == 0)
            return -10;
        *dst = data[*read_index];
        cap = *capacity;
        idx = *read_index;
        if (idx == cap - 1)
            *read_index = 0;
        else
            *read_index = idx + 1;
        --*count;
        dst++;
        ++*done;
    }
    return 0;
}
