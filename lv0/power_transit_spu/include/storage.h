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

#endif
