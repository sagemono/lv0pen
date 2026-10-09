#ifndef MLDR_LOCAL_BUFFER_H
#define MLDR_LOCAL_BUFFER_H

#include "dma_stream.h"

class dma_mfc : public mfc_base {
};

class local_buffer : public dma_buffer {
public:
    local_buffer();
    long get(u64 ea, unsigned int size, unsigned int *done);
    long put(u64 ea, unsigned int size, unsigned int *done);
    bool get_done();
    bool put_done();

    dma_mfc m_mfc;
};

#endif
