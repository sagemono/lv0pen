#ifndef AIM_AIM_H
#define AIM_AIM_H

#include "types.h"
#include "dma_channel.h"
#include "eid0_reader.h"

#define AIM_DMA_SIZE    0x80

#define AIM_DEVICE_TYPE 1
#define AIM_DEVICE_ID   2
#define AIM_PS_CODE     3
#define AIM_OPEN_PS_ID  4

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

unsigned int read_id_type(eid0_reader *eid, const quad_u64 &ea, const quad_uint &size,
                          const quad_uint &ls, const quad_uint &unused);
unsigned int put_id(eid0_reader *eid, const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                    const quad_uint &unused, const quad_ptr &buf, const quad_uint &buf_size);

#endif
