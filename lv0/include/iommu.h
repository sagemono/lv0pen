#ifndef LV0_IOMMU_H
#define LV0_IOMMU_H

#include "lv0.h"

#ifdef __cplusplus
class iommu_context {
public:
    iommu_context();
    unsigned long get_iost_entry_io_address(long iost_entry);
    void clear_iopt_entries(unsigned long iopt, unsigned int count);
    int allocate_iost_entry(long **out);
    int allocate_iopt_entry(unsigned long *out, unsigned int size);
    void disable();
    int invalidate4_iopt_cache(unsigned long iopt, unsigned int count);
    void invalidate_iost_cache(unsigned long iost_entry);
    int initialize(unsigned long ioc, unsigned long iost_addr, unsigned int iost_len,
                    unsigned long iopt_addr, unsigned int iopt_len);
    int free_io_address(unsigned long io_addr);
    int allocate_io_address(unsigned int ioid, long ea_addr, unsigned int ea_size, int pgsz_code, unsigned int prot,
                             unsigned int coherent, unsigned int ordering, unsigned int hint);

    unsigned long ioc_base;
    unsigned long iost;
    unsigned int iost_size;
    volatile unsigned long iopt;
    unsigned int iopt_size;
};
#endif

struct iommu_context *get_iommu_context(void);

struct chan_status;
void iopt_pack_entry_v1(long dev, unsigned int id, const char *desc);
void iopt_unpack_entry_v1(long dev, unsigned int id, unsigned int *out);
long iopt_unpack_entry_v2(long dev, unsigned int id, volatile struct chan_status *out);

struct iopt_window {
    unsigned long addr;
    unsigned int w[10];
};

#ifdef __cplusplus
class iopt_codec {
public:
    void unpack_entry(long dev, unsigned int id, void *out);
    void pack_entry(long dev, unsigned int id, void *desc);
    void set_address(long dev, unsigned int id, unsigned int addr);
};
#endif

long iopt_pack_entry_v2(long dev, unsigned int id, unsigned int *desc);

#endif
