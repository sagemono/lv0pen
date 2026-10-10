#ifndef SPPV_SPP_H
#define SPPV_SPP_H

#include "types.h"
#include "dma_stream.h"

struct sce_header {
    u32 magic;
    u32 version;
    u16 key_revision;
    u16 header_type;
    u32 metadata_offset;
    u64 header_len;
    u64 data_len;
};

#define SCE_MAGIC       0x53434500
#define SCE_VERSION     2
#define SCE_TYPE_SPP    4

struct metadata_header {
    u64 sig_input_length;
    u32 sig_type;
    u32 section_count;
    u32 key_count;
    u32 opt_header_size;
    u32 unknown[2];
};

#define SIG_ECDSA       1
#define SIG_SHA1        3

struct metadata_section {
    u64 offset;
    u64 size;
    u32 type;
    u32 index;
    u32 hashed;
    u32 sha1_index;
    u32 encrypted;
    u32 key_index;
    u32 iv_index;
    u32 compressed;
} __attribute__((aligned(16)));

struct metadata {
    unsigned char info[64];
    metadata_header header;
    metadata_section sections[1];
} __attribute__((aligned(16)));

#endif
