#ifndef LV0_STORAGE_H
#define LV0_STORAGE_H

#include "lv0.h"
#include "cxx.h"

#ifdef __cplusplus
class storage {
public:
    storage() {}
    int get_size(unsigned long dev_index, unsigned long *out_size);
    unsigned long get_device_count();
    int get_mapped_address(unsigned long dev_index, unsigned long offset, unsigned long *out_addr);
    int get_block_size(long dev_index, unsigned long *block_size);
    long reserve(long dev_index);
    int read(unsigned long dev_index, unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read);
    long write(unsigned long dev_index, unsigned long offset, unsigned long size, const char *src,
               unsigned long *bytes_written);
    int open(unsigned long dev_index);
    int close(unsigned long dev_index);
};

extern storage g_storage;
#endif

struct nv_entry_buf {
    unsigned char *buf;
    unsigned int size;
    unsigned char loaded;
};

struct nv_entry {
    unsigned int block;
    unsigned int offset;
    unsigned int last;
    unsigned int base;
    struct nv_entry_buf data;
};

extern struct nv_entry g_nv_entry_table[12];

struct storage_device;
struct storage_manager;

#ifdef __cplusplus
enum {
    DEVICE_CLOSED   = 0,
    DEVICE_OPEN     = 1,
    DEVICE_RESERVED = 2,
};

class storage_device {
public:
    storage_device() : state(DEVICE_CLOSED) {}
    virtual int open() = 0;
    virtual int close() = 0;
    virtual int read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read) = 0;
    virtual int write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written) = 0;
    virtual long get_size(unsigned long *out_size) = 0;
    virtual long get_block_size(unsigned long *block_size);
    virtual long get_mapped_address(unsigned long offset, unsigned long *out_addr);
    unsigned int state;
};
#endif

struct storage_manager_vtable {
    void *dtor;
    void *deleting_dtor;
    struct storage_device **(*get_device_table)(struct storage_manager *self);
    unsigned long (*get_device_count)(struct storage_manager *self);
};

CXX_VTABLE(storage_manager_ztv, struct storage_manager_vtable);

typedef const struct storage_manager_vtable *storage_manager_vptr __attribute__((may_alias));

#ifdef __cplusplus
class storage_manager {
public:
    virtual ~storage_manager();
    virtual struct storage_device **get_device_table() = 0;
    virtual unsigned long get_device_count() = 0;
};

extern storage_manager *g_storage_manager_ptr;
#else
struct storage_manager {
    storage_manager_vptr vtable;
};
#endif

struct region_table_header {
    unsigned int version;
    unsigned int count;
    char pad_8[8];
};

struct region_table_entry {
    unsigned long offset;
    unsigned long size;
    char name[32];
};

long find_flash_table_entry(unsigned long dev_index, unsigned long offset, const char *name,
                            struct region_table_entry *entry);
long locate_os_image(unsigned long *out_addr, unsigned long *out_size);
long get_os_image_size(void);
long get_os_image_address(void);
EXTERN_C long storage_device__get_mapped_address(void);
EXTERN_C long storage_device__get_block_size(void);

EXTERN_C long **storage_manager__base_dtor(long **self);

void init_nand_flash_storage(char *dma_buf, unsigned long dma_io_addr, unsigned long dma_buf_size);

#endif
