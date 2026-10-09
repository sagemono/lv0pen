#ifndef APPLDR_MFC_ATOMIC_H
#define APPLDR_MFC_ATOMIC_H

#include "types.h"

long dma_put(u64 ea, void *ls, const void *src, unsigned int size);
long dma_get(u64 ea, void *ls, void *dst, unsigned int size);
long atomic_get(u64 ea, void *ls);
long atomic_put(u64 ea, void *ls, const void *src, unsigned int off, unsigned int n);

long event_status(unsigned int *out);
long event_mask(unsigned int m);
long event_ack(unsigned int m);

#endif
