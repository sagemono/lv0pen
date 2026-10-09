#ifndef LV0_COMPONENT_H
#define LV0_COMPONENT_H

#include "lv0.h"
#include "cxx.h"

enum component_id {
    COMPONENT_METLDR     = 0,
    COMPONENT_LV1LDR     = 1,
    COMPONENT_LV2LDR     = 2,
    COMPONENT_APPLDR     = 3,
    COMPONENT_ISOLDR     = 4,
    COMPONENT_LV1_SELF   = 6,
    COMPONENT_PARM_TXT   = 7,
    COMPONENT_SYSCTL_TXT = 8,
    COMPONENT_EID0       = 12,
    COMPONENT_SDK_VERSION = 9,
    COMPONENT_METLDR_2   = 14,
    COMPONENT_LV1LDR_2   = 15,
    COMPONENT_LV2LDR_2   = 16,
    COMPONENT_APPLDR_2   = 17,
    COMPONENT_ISOLDR_2   = 18,
};

struct loaded_component;
struct memory_budget;

void select_component_loader(void);

struct component_record {
    unsigned int id;
    const char *name;
    unsigned long max_size;
    unsigned int region;
    unsigned long align;
    struct loaded_component *component;
};

struct component_alt_record {
    struct component_record rec;
    unsigned int pair_id;
    unsigned int flag;
};

#ifdef __cplusplus
class component {
public:
    virtual ~component();
    virtual int get(unsigned int id, unsigned long *out_addr, unsigned long *out_size) = 0;
};

class loaded_component : public component {
public:
    loaded_component();
    virtual ~loaded_component();
    virtual int get(unsigned int id, unsigned long *out_addr, unsigned long *out_size);
    int initialize(unsigned int id, unsigned long address);

    unsigned int id;
    unsigned long size;
    unsigned long address;
};

class component_loader {
public:
    component_loader();
    virtual ~component_loader();
    virtual int load_component(unsigned int id, const char *name, memory_budget *budget, unsigned int region,
                               unsigned long max_size, unsigned long align, unsigned long *out_addr,
                               unsigned long *out_size) = 0;
    virtual int load_component_to(unsigned int id, const char *name, void *dst, unsigned long size) = 0;
};

class boot_order_component_loader : public component_loader {
public:
    boot_order_component_loader(component_loader *primary, component_loader *secondary);
    virtual ~boot_order_component_loader();
    virtual int load_component(unsigned int id, const char *name, memory_budget *budget, unsigned int region,
                               unsigned long max_size, unsigned long align, unsigned long *out_addr,
                               unsigned long *out_size);
    virtual int load_component_to(unsigned int id, const char *name, void *dst, unsigned long size);

    void apply_boot_order();

    component_loader *primary;
    component_loader *secondary;
};

struct region_table_header;
struct region_table_entry;
struct composite_region_header;
struct composite_region_entry;

struct embedded_component {
    unsigned int id;
    unsigned long start;
    unsigned long end;
    unsigned long size;
};

class flash_format_1_component_loader : public component_loader {
public:
    flash_format_1_component_loader(long dev_index, const char *ros_name, const char *asecure_loader_name,
                                    const embedded_component *embedded, unsigned int embedded_count,
                                    unsigned long flash_base);
    virtual ~flash_format_1_component_loader();
    int read_component_data(long dev_index, long offset, unsigned long len, void *buf, unsigned long cap);
    int read_region_table_header(long dev_index, long offset, region_table_header *hdr);
    int read_composite_region_header(long dev_index, long offset, composite_region_header *hdr);
    int find_composite_region_entry(long dev_index, long addr, unsigned int count, long key,
                                    composite_region_entry *entry);
    int find_region_table_entry(long dev_index, long addr, unsigned int count, const char *name,
                                 region_table_entry *entry);
    int load_component_from_rom_region(const char *region_name, long id, memory_budget *budget, unsigned long max_size,
                            unsigned long align, unsigned long *out_addr, unsigned long *out_size);
    int find_embedded_component(unsigned int id, const embedded_component **out);
    int load_2nd_component(unsigned int id, void *dst, unsigned long size);
    int load_component_from_memory(unsigned int id, memory_budget *budget, unsigned long max_size,
                                   unsigned long align, unsigned long *out_addr, unsigned long *out_size);
    int load_component_from_region(const char *region_name, const char *name, memory_budget *budget,
                                       unsigned long max_size, unsigned long align, unsigned long *out_addr,
                                       unsigned long *out_size);
    virtual int load_component(unsigned int id, const char *name, memory_budget *budget, unsigned int region,
                               unsigned long max_size, unsigned long align, unsigned long *out_addr,
                               unsigned long *out_size);
    virtual int load_component_to(unsigned int id, const char *name, void *dst, unsigned long size);
    virtual int load_component_from_bank(const char *region_name, const char *name, memory_budget *budget,
                                         unsigned int region, unsigned long max_size, unsigned long align,
                                         unsigned long *out_addr, unsigned long *out_size);

    long dev_index;
    const char *ros_name;
    const char *asecure_loader_name;
    const embedded_component *embedded;
    unsigned int embedded_count;
    unsigned long flash_base;
};

class flash_bank_component_loader : public flash_format_1_component_loader {
public:
    flash_bank_component_loader(long dev_index, const char *ros_name, const char *asecure_loader_name,
                                const embedded_component *embedded, unsigned int embedded_count,
                                const char *bank0_name, const char *bank1_name);
    virtual int load_component_from_bank(const char *region_name, const char *name, memory_budget *budget,
                                         unsigned int region, unsigned long max_size, unsigned long align,
                                         unsigned long *out_addr, unsigned long *out_size);

    long bank_dev_index;
    const char *bank0_name;
    const char *bank1_name;
    const char *bank_asecure_loader_name;
};

class host_file_loader : public component_loader {
public:
    host_file_loader();
    virtual ~host_file_loader();
    virtual int load_component(unsigned int id, const char *name, memory_budget *budget, unsigned int region,
                               unsigned long max_size, unsigned long align, unsigned long *out_addr,
                               unsigned long *out_size);
    virtual int load_component_to(unsigned int id, const char *name, void *dst, unsigned long size);
};

class componnet_manager {
public:
    componnet_manager(component_record *records, int count, component_alt_record *alt_records, int alt_count);
    virtual ~componnet_manager();
    int find_record(unsigned int id, component_record **out);
    int load_record(component_record *rec);
    int load_components();
    int overwrite_2nd_component();
    int load_2nd_component();
    int initialize(component_loader *loader);
    int get_component_name(unsigned int id, unsigned long *out_addr, const char **out_name);
    int get_component(unsigned int id, unsigned long *out_addr, unsigned long *out_size);

    component_record *records;
    unsigned int count;
    component_alt_record *alt_records;
    unsigned int alt_count;
    component_loader *loader;
    bool initialized;
    loaded_component components[19];
};

struct component_info_entry {
    unsigned long addr;
    unsigned long size;
};

class component_info_table {
public:
    int set(unsigned int id, unsigned long addr, unsigned long size);
    int clear();
    int copy_entry(unsigned long *out_addr, unsigned long *out_size, unsigned long addr, unsigned long size);
    int get(unsigned int id, unsigned long *out_addr, unsigned long *out_size);

    component_info_entry lv1ldr;
    component_info_entry metldr;
    component_info_entry lv2ldr;
    component_info_entry isoldr;
    component_info_entry appldr;
    component_info_entry eid0;
    component_info_entry lv1ldr_2;
    component_info_entry lv2ldr_2;
    component_info_entry appldr_2;
};

component_info_table *get_component_info_table(void);

void decrypt_lv1ldr(void *data, unsigned int size);

extern component_loader *g_component_loader_ptr;
#endif

#endif
