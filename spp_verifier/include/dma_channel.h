#ifndef SPPV_DMA_CHANNEL_H
#define SPPV_DMA_CHANNEL_H

#include "types.h"

#define MFC_PUT         0x20
#define MFC_GET         0x40

#define DMA_EINVAL      9

#define MBOX_FULL       3
#define MBOX_EMPTY      4

class dma_channel {
public:
    dma_channel();
    virtual ~dma_channel();

    unsigned int in_mbox_count(unsigned int &n);
    unsigned int out_intr_mbox_room(unsigned int &n);

    unsigned int issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
                       unsigned int rid, unsigned int cmd);
    bool tag_done(unsigned int tag);
    void wait(unsigned int tag);

    unsigned int write_out_intr_mbox(unsigned int v);
    unsigned int read_in_mbox(unsigned int &v);
};

#endif
