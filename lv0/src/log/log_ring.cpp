#include "log.h"
#include "memory.h"

int log_ring::initialize(unsigned long base, unsigned long size, region_fn flush)
{
    this->base = base;
    this->size = size;
    this->flush = flush;
    cursor = base;
    return 0;
}

int log_ring::write_string(const char *str)
{
    if (!size)
        return -1;
    const char *src = str;
    unsigned long n, avail;
    do {
        n = lv0_strnlen(src, base + size - cursor);
        avail = base + size - cursor;
        lv0_memmove((char *)cursor, src, n);
        src += n;
        cursor += n;
        if (cursor >= base + size)
            cursor = base;
    } while (n >= avail);
    return 0;
}

int log_ring::dump(region_fn flush)
{
    if (!size)
        return -1;
    if (!flush)
        flush = this->flush;
    if (!flush)
        return -1;
    flush(cursor, base + size - cursor);
    flush(base, cursor - base);
    return 0;
}

log_ring *get_log_ring()
{
    static log_ring ring;
    return &ring;
}

int write_log_to_log_ring(const char *msg)
{
    return get_log_ring()->write_string(msg);
}
