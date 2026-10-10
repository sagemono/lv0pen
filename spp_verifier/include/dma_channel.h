#ifndef SPPV_DMA_CHANNEL_H
#define SPPV_DMA_CHANNEL_H

#include "types.h"

#define MFC_PUT         0x20
#define MFC_GET         0x40

#define DMA_EINVAL      9

class dma_channel {
public:
    dma_channel();
    virtual ~dma_channel();

    unsigned int issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
                       unsigned int rid, unsigned int cmd);
    bool tag_done(unsigned int tag);
    void wait(unsigned int tag);
};

#endif
