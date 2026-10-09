#include "dma_stream.h"
#include "util.h"

unsigned int mfc_atomic_status(void);
void ll_getllar(unsigned int ls, u64 ea);
void ll_putllc(unsigned int ls, u64 ea);

long dma_put(u64 ea, void *ls, const void *src, unsigned int size)
{
    if (ea == 0 || src == 0)
        return -3;
    memcpy(ls, src, size);
    mfc_issue((unsigned int)ls, ea, size, 1, 0, 32);
    while (!mfc_tag_done(1))
        ;
    return 0;
}

long dma_get(u64 ea, void *ls, void *dst, unsigned int size)
{
    if (ea == 0 || dst == 0)
        return -3;
    mfc_issue((unsigned int)ls, ea, size, 1, 0, 64);
    while (!mfc_tag_done(1))
        ;
    memcpy(dst, ls, size);
    return 0;
}

long atomic_get(u64 ea, void *ls)
{
    if (!(((ea & 127) == 0) & (((u64)(unsigned int)ls & 127) == 0)))
        return -3;
    do {
        ll_getllar((unsigned int)ls, ea);
        __asm__ __volatile__ ("dsync");
    } while (mfc_atomic_status() != 4);
    return 0;
}

long atomic_put(u64 ea, void *ls, const void *src, unsigned int off, unsigned int n)
{
    long r;
    bool ok = ((ea & 127) == 0) & (((u64)(unsigned int)ls & 127) == 0);

    if (!ok || !src || off + n > 128)
        return -3;
    do {
        r = atomic_get(ea, ls);
        if (r)
            return r;
        memcpy((unsigned char *)ls + off, src, n);
        __asm__ __volatile__ ("dsync");
        ll_putllc((unsigned int)ls, ea);
    } while (mfc_atomic_status() != 0);
    return r;
}
