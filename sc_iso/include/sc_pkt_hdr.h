#ifndef SCISO_SC_PKT_HDR_H
#define SCISO_SC_PKT_HDR_H

#include "types.h"
#include "ss_iso_dma_data.h"

struct sc_pkt {
    u8 service_id;
    u8 version;
    u16 transaction_id;
    u16 f4;
    u16 f6;
    u32 f8;
    u16 len;
    u16 len2;
};

class sc_pkt_hdr {
public:
    sc_pkt_hdr(const unsigned char &service_id);
    ~sc_pkt_hdr();
    unsigned int verify_hdr(const unsigned char *addr, const quad_uint &size, quad_uint &len);
    unsigned int make_hdr(unsigned char *addr, const quad_uint &size, const quad_uint &len);

    u8 m_service_id;
    u8 m_version;
    u16 m_transaction_id;
    u16 m_4;
    u16 m_6;
    u32 m_8;
    u16 m_len;
    u16 m_len2;
};

#endif
