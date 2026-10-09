#ifndef LDR_SYSTEMIO_DMAC_H
#define LDR_SYSTEMIO_DMAC_H

#include "sb.h"

struct systemio_dmac_dx_desc {
    u64 next;
    u64 src;
    u64 dst;
    unsigned int flags;
    unsigned int len;
};

class systemio_dmac_dx {
public:
    systemio_dmac_dx() : base(0), ioid(0), dev_addr(0), desc(0), desc_io(0) {}
    int configure(u64 base, u64 desc, u64 desc_io, u64 ioid, u64 dev_addr);
    int get_dma_status();
    long start_transfer(u64 dst, u64 src, unsigned int len);
    long kick_dma(u64 buf, unsigned int len);

    u64 base;
    u64 ioid;
    u64 dev_addr;
    u64 desc;
    u64 desc_io;
};

class systemio_dmac_px {
public:
    systemio_dmac_px() : base(0), dev_addr(0), desc(0), desc_io(0) {}
    long configure(u64 base, u64 dev_addr, u64 desc, unsigned int desc_io);
    int get_dma_status();
    bool is_ebus_interrupt_asserted();
    long kick_dma(u64 dst, unsigned int len);

    u64 base;
    u64 dev_addr;
    u64 desc;
    unsigned int desc_io;
};

systemio_dmac_dx *get_systemio_dmac_dx(void);
systemio_dmac_px *get_systemio_dmac_px(void);
bool poll_ebus_interrupt(void);

#endif
