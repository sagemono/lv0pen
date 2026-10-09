#ifndef MLDR_AUTH_H
#define MLDR_AUTH_H

#include "elf.h"
#include "dma_stream.h"
#include "types.h"
#include <spu_intrinsics.h>

extern vec_uchar16 g_part_area[];

struct auth_sink {
    auth_sink() : base(g_part_area), p(g_part_area) { }

    const vec_uchar16 *append(const vec_uchar16 *src, const vec_uchar16 *end);

    vec_uchar16 *base;
    vec_uchar16 *p;
};

struct sce_header {
    unsigned int magic;
    unsigned int version;
    unsigned short key_revision;
    unsigned short type;
    unsigned int metadata_offset;
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
};

struct auth_app_info {
    u64 auth_id;
    unsigned char vendor0, vendor1, vendor2, vendor3;
    unsigned int type;
    u64 version;
    u64 reserved;
};

struct auth_meta {
    u64 signed_len;
    unsigned int sig_type;
    unsigned int section_count;
    unsigned int key_count;
};

struct auth_section {
    u64 offset;
    u64 size;
    unsigned int type;
    unsigned int index;
    unsigned int hashed;
    unsigned int digest;
    unsigned int encrypted;
    unsigned int key;
    unsigned int iv;
    unsigned int compressed;
};

class authenticator {
public:
    authenticator();

    void set_offset(u64 off);

    long load_header(unsigned int type, const unsigned char *key,
                     const unsigned char *iv, const unsigned char *pub,
                     const unsigned int *curve, dma_buffer **bufs,
                     unsigned int nbufs);

    long load_segment(unsigned int type, unsigned int index, u64 ea,
                      dma_buffer **in, int nin, dma_buffer **out, int nout);

    long read_elf_header(Elf32_Ehdr *out);
    long read_program_header(unsigned int i, Elf32_Phdr *out);

    long check_sce_header(unsigned int type);

    long check_type(unsigned int type);

    long get_app_info(auth_app_info *out);

    long find_section(unsigned int type, unsigned int index, auth_section *out);

    long transfer_encrypted();

    long verify_section_digest(const auth_section *p);

    long drain_read_queue();

    void save_header_piece(const dma_buffer *b);
    void restore_header_piece(const dma_buffer *b);

    void prepend(dma_buffer *b, unsigned int n);

private:
    const sce_header *m_sce;
    const sce_ext_header *m_ext;
    unsigned char *m_info;
    const auth_app_info *m_app;
    const unsigned char *m_section_info;
    const auth_meta *m_meta;
    const auth_section *m_sections;
    const unsigned char *m_keys;
    elf_image m_elf;
    auth_sink m_sink;
    u64 m_offset;
    bool m_loaded;
    unsigned char m_57;
    unsigned short m_58;
    unsigned char m_60[4];
    dma_queue m_queue;
    auth_section m_section;
};

extern unsigned char g_auth_area[];

extern unsigned char g_part_digest[20];

#endif
