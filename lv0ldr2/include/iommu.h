#ifndef LDR_IOMMU_H
#define LDR_IOMMU_H

#include "mmio.h"

class iommu_context {
public:
    iommu_context() : ioc_base(0), iost(0), iost_size(0), iopt(0), iopt_size(0) {}
    void clear_iopt_entries(u64 iopt, unsigned int count);
    void clear_iost_entry(u64 entry);
    u64 get_iost_entry_io_address(u64 iost_entry);
    long invalidate4_iopt_cache(u64 iopt, unsigned int count);
    long allocate_iopt_entry(u64 *out, unsigned int size);
    long allocate_iost_entry(u64 *out);
    void disable();
    long initialize(u64 ioc, u64 iost_addr, unsigned int iost_len,
                    u64 iopt_addr, unsigned int iopt_len);
    void invalidate_iost_cache(u64 iost_entry);
    long free_io_address(u64 io_addr);
    long allocate_io_address(unsigned int ioid, u64 ea_addr, unsigned int ea_size,
                             unsigned int pgsz_code, unsigned int prot,
                             unsigned int coherent, unsigned int ordering, unsigned int hint);

    u64 ioc_base;
    u64 iost;
    unsigned int iost_size;
    u64 iopt;
    unsigned int iopt_size;
};

iommu_context *get_iommu_context(void);

#endif
