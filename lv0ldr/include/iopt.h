#ifndef LDR_IOPT_H
#define LDR_IOPT_H

#include "mmio.h"

union iopt_desc {
    u64 addr;
    unsigned int w[12];
};

void iopt_unpack_entry_v2(u64 dev, unsigned int id, iopt_desc *out);
void iopt_pack_entry_v2(u64 dev, unsigned int id, iopt_desc *desc);
void iopt_pack_entry_v1(u64 dev, unsigned int id, iopt_desc *desc);
void iopt_unpack_entry_v1(u64 dev, unsigned int id, iopt_desc *out);

class iopt_codec {
public:
    void pack_entry(u64 dev, unsigned int id, void *desc);
    void unpack_entry(u64 dev, unsigned int id, void *out);
};

#endif
