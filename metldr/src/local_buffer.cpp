#include "local_buffer.h"

local_buffer::local_buffer()
{
}

long local_buffer::get(u64 ea, unsigned int size, unsigned int *done)
{
    return 0;
}

bool local_buffer::get_done()
{
    return true;
}

bool local_buffer::put_done()
{
    return true;
}

long local_buffer::put(u64 ea, unsigned int size, unsigned int *done)
{
    unsigned int off = offset & 15;
    unsigned int n;

    if (off) {
        n = length;
        if (n + off > 16)
            return -2;
        *done = n;
        ls += n;
        this->size -= n;
        return 0;
    } else {
        n = length;
        if (n & 15) {
            if (n != (n & 15))
                return -2;
            *done = n;
            ls += n;
            this->size -= n;
            return 0;
        } else {
            n = this->size > n ? n : this->size;
            *done = n;
            ls += n & ~15;
            this->size -= n;
            return 0;
        }
    }
}
