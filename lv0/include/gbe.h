#ifndef LV0_GBE_H
#define LV0_GBE_H

#include "lv0.h"

#ifdef __cplusplus
class gbe_work {
public:
    gbe_work();
    void initialize(unsigned long size);
    void finalize();
    unsigned long allocate(unsigned long size, unsigned long align);

    void *region;
    long io_addr;
    unsigned long size;
    unsigned long cursor;
    bool mapped;
};
#endif

struct gbe_work *get_gbe_work(void);

long read_cp_config(long bar0, long bar1, int *cfg_);

#endif
