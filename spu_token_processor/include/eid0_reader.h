#ifndef SPTP_EID0_READER_H
#define SPTP_EID0_READER_H

#include "types.h"
#include "dma_channel.h"

typedef u64 quad_u64 __attribute__((aligned(16)));
typedef unsigned int quad_uint __attribute__((aligned(16)));
typedef unsigned char *quad_ptr __attribute__((aligned(16)));

struct key128 {
    u64 hi;
    u64 lo;
};

class eid0_reader {
public:
    eid0_reader(dma_channel *dma, key128 arg, key128 iv, key128 key_hi, key128 key_lo);
    ~eid0_reader();
    unsigned int read(const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                      const quad_uint &unused, const quad_ptr &buf, const quad_uint &buf_size);

    unsigned int m_section;
    dma_channel *m_dma;
    key128 m_arg __attribute__((aligned(16)));
    key128 m_iv __attribute__((aligned(16)));
    key128 m_key[2] __attribute__((aligned(16)));
};

#endif
