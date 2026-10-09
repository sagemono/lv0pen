#ifndef LV1LDR_AUTH_H
#define LV1LDR_AUTH_H

#include "elf.h"
#include "sce.h"
#include "dma_stream.h"
#include "types.h"
#include <spu_intrinsics.h>

struct auth_app_info {
    u64 auth_id;
    unsigned char vendor[4];
    unsigned int type;
    u64 version;
    u64 reserved;
} __attribute__((aligned(16)));

struct auth_entry {
    unsigned int type;
    unsigned int size;
    u64 next;
    union {
        u64 v[4];
        struct {
            unsigned char digest[20];
            unsigned char key[20];
            u64 w;
        } d;
    } u;
} __attribute__((aligned(16)));

extern vec_uchar16 g_part_area[512];
extern unsigned char g_part_digest[20];

struct header_data {
    void clear();
    long decrypt();
    long hash();
    long save(const dma_buffer *b);
    long restore(const dma_buffer *b);
    long prepend(const dma_buffer *b, unsigned int n);
    bool empty() const { return len == 0; }
    unsigned char buf[32];
    unsigned int len;
} __attribute__((aligned(16)));

extern header_data g_header;

struct auth_tail : header_data {
    auth_tail();
    void reset();
};

struct auth_sink {
    auth_sink() : base(g_part_area), p(g_part_area) { }

    const vec_uchar16 *append(const vec_uchar16 *src, const vec_uchar16 *end);

    vec_uchar16 *base;
    vec_uchar16 *p;
};

struct auth_block {
    auth_block();
    long load(const auth_entry *e, u64 size);
    long get(u64 *out);
    long get_digest(unsigned char *out);
    long get_w(u64 *out);
    u64 v[4];
    unsigned char digest[20];
    u64 w;
};

struct auth_state {
    auth_state();
    long load(const auth_entry *e, u64 size);
    long get(u64 *out);
    long get_data(unsigned char *out, unsigned int size);
    unsigned char buf[256];
    struct { u64 a, b, c, d; } t;
};

int check_ext_header(sce_image *image);

class authenticator {
public:
    authenticator();

    void clear_parts();

    void set_offset(u64 off);

    long load_header(unsigned int type, const unsigned char *key,
                     const unsigned char *iv, const unsigned char *pub,
                     const unsigned int *curve, dma_buffer **bufs,
                     unsigned int nbufs);

    long load_segment(unsigned int type, unsigned int index, u64 ea, u64 size,
                      dma_buffer **in, int nin, dma_buffer **out, int nout);
    long load_segment(unsigned int type, unsigned int index, u64 ea,
                      dma_buffer **in, int nin, dma_buffer **out, int nout);

    long read_elf_header(Elf64_Ehdr *out);
    long read_program_header(unsigned int i, Elf64_Phdr *out);
    long read_elf_header(Elf32_Ehdr *out);
    long read_program_header(unsigned int i, Elf32_Phdr *out);

    long check_app(sce_image *image, unsigned int type);
    long check_app_info(const auth_app_info *app, unsigned int type);

    long find_section(unsigned int type, unsigned int index, auth_section *out);

    long transfer_deflated(u64 ea);
    long transfer_stored();
    long inflate_piece(dma_buffer *in, u64 ea, u64 *written);

    long check_part_digest(const auth_section *p);

    long read_parts();

    long save_tail(const dma_buffer *b);
    long restore_tail(const dma_buffer *b);

    long prepend(const dma_buffer *b, unsigned int n);

    long get_app_info(auth_app_info *out);
    long get_state(u64 *out);
    long get_state_data(unsigned char *out, unsigned int size);

    bool has_required_flags();

    long check_auth_id_class(u64 a, u64 b);
    long check_issuer(u64 a, u64 b);

    void reset();

    long check_section_digest();

    long get_section_info(unsigned int i, void *out);

    long get_control_digest(unsigned char *out);
    long get_control_w(u64 *out);

    long get_control_values(u64 *out);

    bool is_elf64();
    bool is_elf32();

    unsigned char flag_1b9();

    long get_section_digest(unsigned int type, unsigned int index, unsigned char *out,
                            unsigned int size);

    long set_header(unsigned int type, const void *p);

    long select_section(unsigned int type, unsigned int index);

    long transfer_segment(u64 src, u64 size, u64 dest, u64 dest_size, dma_buffer **in,
                          unsigned int nin, dma_buffer **out, unsigned int nout,
                          u64 *written);

    long load_header(unsigned int type, const unsigned char *key,
                     const unsigned char *iv, const unsigned char *pub,
                     const unsigned int *curve, dma_buffer **bufs,
                     unsigned int nbufs, void *args, const unsigned char *args_iv,
                     bool flag);

    long build_meta(sce_image *image, auth_meta *meta);

    long transfer_deflated(u64 ea, bool decrypt, bool in_place, u64 *written);

    long transfer_stored(bool decrypt, bool raw, u64 *written);
    long requeue(dma_buffer *in, dma_buffer *out, const bool &decrypt, const bool &raw,
                 u64 *written);

    unsigned short key_revision() const { return m_key_revision; }

private:
    const sce_header *m_sce;
    const sce_ext_header *m_ext;
    unsigned char *m_info;
    const auth_app_info *m_app;
    const unsigned char *m_section_info;
    const unsigned char *m_version;
    const auth_entry *m_control;
    unsigned int m_1c;
    auth_block m_block;
    const auth_meta *m_meta;
    const auth_section *m_sections;
    const unsigned char *m_keys;
    const auth_entry *m_opt;
    auth_state m_state;
    elf_image m_elf;
    auth_sink m_sink;
    u64 m_offset;
    bool m_loaded;
    bool m_1b9;
    unsigned short m_key_revision;
    unsigned char m_1bc[4];
    auth_tail m_tail;
    u64 m_1f0;
    sce_image m_image;
    dma_queue m_queue;
    u64 m_258;
    auth_section m_section;
} __attribute__((aligned(16)));

#endif
