#ifndef LV0_SPANSION_STORAGE_H
#define LV0_SPANSION_STORAGE_H

#include "storage.h"
#include "iommu.h"

#define FLASH_ROTATE 0x400000UL

struct nor_region {
    unsigned int start;
    unsigned int sector_size;
    unsigned int sector_count;
};

struct nor_chip {
    unsigned int id;
    unsigned int chip_size;
    unsigned char chip_count;
    unsigned char region_count;
    unsigned long iopt_field;
    unsigned long write_buffer_words;
    struct nor_region regions[5];
};

extern struct nor_chip g_nor_chips[9];

#ifdef __cplusplus
class mapped_storage : public storage_device {
public:
    mapped_storage();
    virtual int open();
    virtual int close();
    virtual int read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read);
    virtual long get_mapped_address(unsigned long offset, unsigned long *out_addr);
    virtual void set_base(unsigned long base);

    unsigned long base;
};

class spansion_storage : public mapped_storage {
public:
    spansion_storage();
    virtual int open();
    virtual int write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written);
    virtual long get_size(unsigned long *out_size);
    virtual long get_block_size(unsigned long *block_size);
    virtual unsigned long translate(unsigned long offset);

    int init(unsigned long base, unsigned long iopt_index, int type, unsigned int iopt_w0);
    int identify();
    int swap16(unsigned int value);
    unsigned long sector_start(unsigned long offset);
    unsigned long sector_size(unsigned long addr);
    int for_each_sector(unsigned long offset, unsigned long size, int (spansion_storage::*op)(unsigned long));
    int protect_range(unsigned long addr, unsigned long size);
    int unlock_bypass_enter(unsigned long addr);
    int unlock_bypass_exit(unsigned long addr);
    int dyb_enter(unsigned long addr);
    int cmdset_exit(unsigned long addr);
    bool dyb_is_protected(unsigned long addr);
    int dyb_protect(unsigned long addr);
    int dyb_unprotect(unsigned long addr);
    unsigned char is_protected_type8(unsigned long addr);
    int protect_type8(unsigned long addr);
    int unprotect_type8(unsigned long addr);
    unsigned long sector_group(unsigned long addr);
    bool is_protected_type7(unsigned long addr);
    int unprotect_type7(unsigned long addr);
    int protect_type7(unsigned long addr);
    void progress_tick();
    void halt_blink();
    int program_write_buffer(unsigned long addr, const unsigned short *src, unsigned int count);
    int unlock_bypass_program(unsigned long addr, unsigned short value);
    int program(unsigned long addr, const char *src, unsigned long size);
    int erase_sector(unsigned long addr);
    int write_sector(unsigned long addr, unsigned long at, const char *src, unsigned long len);

    int type;
    char *sector_work;
    bool swap;
};

class spansion_storage_plain : public spansion_storage {
public:
    spansion_storage_plain();
    virtual int read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read);
    virtual long get_mapped_address(unsigned long offset, unsigned long *out_addr);
    virtual unsigned long translate(unsigned long offset);
    virtual ~spansion_storage_plain();
};
#endif

#endif
