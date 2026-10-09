#ifndef LV1LDR_SCE_H
#define LV1LDR_SCE_H

#include <spu_intrinsics.h>
#include "types.h"

struct sce_header {
    u32 magic;
    u32 version;
    u16 key_revision;
    u16 type;
    u32 metadata_offset;
    u64 header_len;
    u64 data_len;
};

struct sce_ext_header {
    u64 type;
    u64 app_offset;
    u64 elf_offset;
    u64 phdr_offset;
    u64 shdr_offset;
    u64 section_info_offset;
    u64 version_offset;
    u64 control_offset;
    u64 control_size;
    u64 reserved;
} __attribute__((aligned(16)));

struct sce_version_info {
    u32 type;
    u32 count;
    u32 reserved[2];
} __attribute__((aligned(16)));

struct auth_meta {
    u64 signed_len;
    u32 sig_type;
    u32 section_count;
    u32 key_count;
    u32 opt_header_size;
    u64 reserved;
} __attribute__((aligned(16)));

struct auth_section {
    u64 offset;
    u64 size;
    u32 type;
    u32 index;
    u32 hashed;
    u32 digest;
    u32 encrypted;
    u32 key;
    u32 iv;
    u32 compressed;
} __attribute__((aligned(16)));

struct auth_control {
    u32 type;
    u32 size;
    u64 next;
    unsigned char data[20];
} __attribute__((aligned(16)));

class sce_image {
public:
    const auth_section *get_sections(void);
    long get_section_count(u32 *out);
    long get_key_count(u32 *out);
    long get_ext_header(u32 *ext, u32 *meta_offset);
    long get_app_info(u32 *out);

    const sce_header *m_sce;
    u32 m_size;
};

int check_sce_header(const vec_uchar16 *p, const void *dev, u16 type, u32 limit);
int check_meta_header(const vec_uchar16 *p, const auth_meta *m, u32 sig_type);
long check_sections(sce_image *a);
int check_control_info(const auth_control *c, u64 size);
int check_ext_header(sce_image *image);

#endif
