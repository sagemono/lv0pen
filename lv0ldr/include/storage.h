#ifndef PT_STORAGE_H
#define PT_STORAGE_H

#include "types.h"

struct nv_entry_buf {
    unsigned char *buf;
    unsigned int size;
    bool loaded;
};

struct nv_entry {
    unsigned int block;
    unsigned int offset;
    unsigned int last;
    unsigned int base;
    struct nv_entry_buf data;
};

extern struct nv_entry g_nv_entry_table[7];

long get_eeprom_select_net_device(unsigned char *out);
long get_eeprom_update_flag(unsigned char *out);
long get_eeprom_boot_fir_config(unsigned char *out);
long get_eeprom_bootrom_diag(unsigned char *out);
long get_eeprom_bootrom_trace_level(unsigned char *out);
long get_eeprom_48c0d(unsigned char *out);
long get_eeprom_48c0e(unsigned char *out);
long set_eeprom_bootrom_diag(unsigned char val);
long set_eeprom_48c0d(unsigned char val);
long set_eeprom_48c0e(unsigned char val);

#endif
