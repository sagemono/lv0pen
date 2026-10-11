#include "sc_pkt_hdr.h"
#include "dma_log.h"
#include "util.h"

#define BE16(p)         ((p)[1] | (p)[0] << 8)

sc_pkt_hdr::sc_pkt_hdr(const unsigned char &service_id)
    : m_service_id(service_id), m_version(1), m_transaction_id(0), m_4(0), m_6(0), m_8(0),
      m_len(0), m_len2(0)
{
}

sc_pkt_hdr::~sc_pkt_hdr()
{
}

unsigned int sc_pkt_hdr::make_hdr(unsigned char *addr, const quad_uint &size, const quad_uint &len)
{
    sc_pkt hdr;

    if (addr == 0) {
        get_log()->puts("In sc_pkt_hdr::make_hdr: ");
        get_log()->puts("addr must not be NULL\n");
        return 9;
    }
    if (size < 16) {
        get_log()->puts("In sc_pkt_hdr::make_hdr: ");
        get_log()->puts("size must be larger than ");
        get_log()->put((u64)16, 10);
        get_log()->puts("\n");
        return 9;
    }
    hdr.service_id = m_service_id;
    hdr.version = m_version;
    hdr.transaction_id = m_transaction_id;
    hdr.f4 = m_4;
    hdr.f6 = m_service_id + m_version + m_transaction_id + 0x8000;
    hdr.f8 = m_8;
    hdr.len = len;
    hdr.len2 = len;
    memcpy(addr, &hdr, sizeof(hdr));
    return 0;
}
unsigned int sc_pkt_hdr::verify_hdr(const unsigned char *addr, const quad_uint &size, quad_uint &len)
{
    u8 service_id, version;
    u16 tid;

    if (addr == 0) {
        get_log()->puts("In sc_pkt_hdr::verify_hdr: ");
        get_log()->puts("addr must not be NULL\n");
        return 9;
    }
    if (size < 16) {
        get_log()->puts("In sc_pkt_hdr::verify_hdr: ");
        get_log()->puts("size must be larger than ");
        get_log()->put((u64)size, 10);
        get_log()->puts("\n");
        return 9;
    }
    service_id = addr[0];
    if (service_id != m_service_id) {
        get_log()->puts("In sc_pkt_hdr::verify_hdr: ");
        get_log()->puts("service_id does not match: ");
        get_log()->put(addr[0], 16);
        get_log()->puts(" != ");
        get_log()->put(m_service_id, 16);
        get_log()->puts("\n");
        return 15;
    }
    version = addr[1];
    if (version != m_version) {
        get_log()->puts("In sc_pkt_hdr::verify_hdr: ");
        get_log()->puts("version does not match: ");
        get_log()->put(addr[1], 16);
        get_log()->puts(" != ");
        get_log()->put(m_version, 16);
        get_log()->puts("\n");
        return 15;
    }
    tid = addr[2] << 8 | addr[3];
    if (tid != m_transaction_id) {
        get_log()->puts("In sc_pkt_hdr::verify_hdr: ");
        get_log()->puts("transaction_id does not match: ");
        get_log()->put((u8)BE16(addr + 2), 16);
        get_log()->puts(" != ");
        get_log()->put((u8)m_transaction_id, 16);
        get_log()->puts("\n");
        return 15;
    }
    m_transaction_id = tid + 1;
    len = BE16(addr + 12);
    return 0;
}
