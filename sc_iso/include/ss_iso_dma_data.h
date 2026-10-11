#ifndef SCISO_SS_ISO_DMA_DATA_H
#define SCISO_SS_ISO_DMA_DATA_H

#include "types.h"
#include "dma_channel.h"

typedef u64 quad_u64 __attribute__((aligned(16)));
typedef unsigned int quad_uint __attribute__((aligned(16)));
typedef unsigned char *quad_ptr __attribute__((aligned(16)));

struct ss_iso_msg_header {
    unsigned int seq;
    unsigned int token;
    unsigned int cmd;
    unsigned int len;
} __attribute__((aligned(16)));

#define SS_ISO_MSG_STATUS       130

class ss_iso_dma_data {
public:
    ss_iso_dma_data(const quad_u64 &ea, const quad_uint &size, const quad_ptr &ls,
                    const quad_uint &hdr_size);
    ~ss_iso_dma_data();

    bool aligned();
    void chunk(const quad_uint &left, quad_uint &n);
    unsigned int notify(const unsigned int &v);
    unsigned int get(const quad_ptr &buf, const quad_uint &max, unsigned int &token,
                     unsigned int &cmd, quad_uint &len);
    void finish(const unsigned int &status, const unsigned int &cmd, const quad_uint &len,
                const quad_ptr &buf);
    unsigned int put(const unsigned int &token, const unsigned int &cmd, const quad_uint &len,
                     const quad_ptr &buf);

    dma_channel m_dma;
    unsigned int m_seq;
    u64 m_ea __attribute__((aligned(16)));
    unsigned int m_size __attribute__((aligned(16)));
    unsigned char *m_ls __attribute__((aligned(16)));
    unsigned int m_hdr_size __attribute__((aligned(16)));
};

#endif
