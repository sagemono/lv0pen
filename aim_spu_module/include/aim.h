#ifndef AIM_AIM_H
#define AIM_AIM_H

#include "types.h"
#include "dma_channel.h"
#include "eid0_reader.h"

#define AIM_DMA_SIZE    0x80

class dma_log {
public:
    dma_log();
    ~dma_log();
    unsigned int puts(const char *s);
    void put_digit(unsigned int d);
    void put_hex(s64 v);
    void reset();
    void set_buffer(const quad_u64 &ea, const quad_uint &size);
    void init(const quad_u64 &ea, const quad_uint &size);

    dma_channel m_dma;
    u64 m_ea __attribute__((aligned(16)));
    unsigned int m_size __attribute__((aligned(16)));
    unsigned int m_pos __attribute__((aligned(16)));
};

dma_log *get_log();

#endif
