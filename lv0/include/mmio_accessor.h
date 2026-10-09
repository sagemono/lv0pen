#ifndef LV0_MMIO_ACCESSOR_H
#define LV0_MMIO_ACCESSOR_H

#include "lv0.h"
#include "cxx.h"

#ifdef __cplusplus
class mmio_accessor {
public:
    mmio_accessor(char *base);
    virtual ~mmio_accessor();
    virtual void write64(long off, long val);
    virtual void write32(long off, unsigned int val);
    virtual long read64(long off);
    virtual long read32(long off);
    virtual void on_write(long off, long size, long val);
    virtual void on_read(long off, long size, long val);
    unsigned char is_trace_enabled();

    char *base;
    unsigned char trace;
};
#endif

struct mmio_accessor *get_mmio_accessor(void);

#endif
