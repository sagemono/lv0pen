#ifndef AIM_DMA_LOG_H
#define AIM_DMA_LOG_H

#include "types.h"
#include "dma_channel.h"

typedef u64 quad_u64 __attribute__((aligned(16)));
typedef unsigned int quad_uint __attribute__((aligned(16)));

class dma_log {
public:
    dma_log();
    ~dma_log();
    unsigned int puts(const char *s);
    u64 top_power(u64 v);
    void put_digit(unsigned int d);
    int put_number(const void *p, int size, unsigned int base, int neg);
    int put(u8 v, unsigned int base);
    int put(int v, unsigned int base);
    int put(unsigned int v, unsigned int base);
    int put(u64 v, unsigned int base);
    int put(s64 v, unsigned int base);
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
