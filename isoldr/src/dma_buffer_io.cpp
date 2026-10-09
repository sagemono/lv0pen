#include "dma_stream.h"

long tagged_dma_buffer::put(u64 ea, unsigned int size, unsigned int *done)
{
    if (size == 0) {
        *done = size;
        return 0;
    }
    unsigned int off = offset & 15;
    if (off != (ea & 15))
        return -1;
    unsigned int n;
    if (off) {
        n = length;
        if (n + off > 16)
            return -2;
        transfer((u64)ls + off, ea, n, MFC_PUT_CMD);
        *done = n;
        return 0;
    } else {
        n = length;
        if (n & 15) {
            if (n != (n & 15))
                return -2;
            transfer(ls, ea, n, MFC_PUT_CMD);
            *done = n;
            return 0;
        } else {
            n = this->size > n ? n : this->size;
            mfc_issue(ls, ea, n, tag, 0, MFC_PUT_CMD);
            *done = n;
            return 0;
        }
    }
}

long tagged_dma_buffer::get(u64 ea, unsigned int size, unsigned int *done)
{
    if (size == 0) {
        *done = size;
        return 0;
    }
    unsigned int off = (unsigned int)ea & 15;
    unsigned int n;
    if (off) {
        unsigned int m = 16 - off;
        n = size < m ? size : m;
        transfer((u64)ls + off, ea, n, MFC_GET_CMD);
        offset = off;
        length = n;
        *done = n;
        return 0;
    } else {
        n = size & 15;
        if (n && size < 16) {
            transfer(ls, ea, n, MFC_GET_CMD);
            offset = off;
            length = n;
            *done = n;
            return 0;
        } else {
            unsigned int m = size - n;
            n = this->size > m ? m : this->size;
            mfc_issue(ls, ea, n, tag, 0, MFC_GET_CMD);
            offset = off;
            length = n;
            *done = n;
            return 0;
        }
    }
}
