#ifndef SCISO_SC_PROXY_HDR_H
#define SCISO_SC_PROXY_HDR_H

#include "types.h"
#include "ss_iso_dma_data.h"

struct sc_proxy {
    u64 a;
    u64 b;
};

class sc_proxy_hdr {
public:
    sc_proxy_hdr();
    ~sc_proxy_hdr();
    unsigned int make_hdr(const quad_uint &a, const quad_uint &b, const quad_ptr &addr,
                          const quad_uint &size);
    unsigned int verify_hdr(const quad_ptr &addr, const quad_uint &size, quad_uint &a,
                            quad_uint &b);
};

#endif
