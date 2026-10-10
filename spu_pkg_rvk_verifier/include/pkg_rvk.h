#ifndef PKGRVK_PKG_RVK_H
#define PKGRVK_PKG_RVK_H

#include "types.h"
#include "dma_stream.h"
#include "hmac_sha1.h"

struct verify_params {
    u64 type;
    u64 flags;
    u64 in_ea;
    u64 in_size;
    u64 out_ea;
    u64 out_size;
    u64 list_in_ea;
    u64 list_in_size;
    u64 list_out_ea;
    u64 list_out_size;
} __attribute__((aligned(16)));

#define STOP_DONE               0x100
#define STOP_BAD_PARAMS         0x101
#define STOP_BAD_FILE           0x104
#define STOP_REVOKED            0x105
#define STOP_BAD_LIST           0x106

#define VERIFY_EINVAL           9
#define VERIFY_ERANGE           15
#define VERIFY_EDMA             16
#define VERIFY_REVOKED          17
#define VERIFY_EBADF            20
#define VERIFY_ECRYPTO          21

struct sce_sizes {
    u64 header_len;
    u64 data_len;
};

struct sce_header {
    u32 magic;
    u32 version;
    u16 key_revision;
    u16 header_type;
    u32 metadata_offset;
    sce_sizes sizes;
} __attribute__((aligned(16)));

#define SCE_MAGIC               0x53434500
#define SCE_VERSION             2
#define SCE_TYPE_RVK            2
#define SCE_TYPE_PKG            3

struct metadata_header {
    u64 sig_input_length;
    u32 sig_type;
    u32 section_count;
    u32 key_count;
    u32 opt_header_size;
    u32 unknown[2];
} __attribute__((aligned(16)));

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

struct pkg_info {
    u32 magic;
    u32 type;
    u64 id;
    u64 version;
    u64 unknown0;
    u64 body_size;
    u64 unknown1[3];
} __attribute__((aligned(16)));

struct rvk_header {
    u32 magic;
    u32 type;
    u32 unknown0[2];
    u32 entry_count;
    u32 count2;
    u32 unknown1[2];
} __attribute__((aligned(16)));

struct rvk_entry {
    u32 type;
    u64 id __attribute__((packed));
    u32 op;
    u64 version;
    u8 unknown[8];
} __attribute__((aligned(16)));

enum { RVK_EQ, RVK_NE, RVK_LT, RVK_LE, RVK_GT, RVK_GE };

void span_set(ls_span *s, unsigned char *p, unsigned int size);
const unsigned char *span_fill(ls_span *s, const unsigned char *src, const unsigned char *src_end);

class sce_verifier {
public:
    sce_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                 const unsigned char *iv, const unsigned char *pub, unsigned int curve);
    virtual ~sce_verifier();
    virtual int read_header(u64 ea, unsigned int size, sce_header *h);
    virtual int read_metadata(u64 ea, const sce_header *h, unsigned char *out);
    virtual int verify_section(const metadata_section *s, const unsigned char *keys,
                               u64 in_ea, unsigned int in_size, u64 out_ea,
                               unsigned int out_size, unsigned char *copy);
    virtual int check_header(const sce_header *h, unsigned int version,
                             unsigned int header_type, unsigned int metadata_offset);
    virtual int check_section_count(const metadata_header *m, unsigned int count);
    virtual int check_section(const metadata_section *s, unsigned int type,
                              unsigned int index);

    int verify_signature(const unsigned char *headers);
    int read_metadata_info(u64 ea, unsigned int size, unsigned char *buf,
                           unsigned char *info);
    int decrypt_metadata(const unsigned char *in, unsigned int size,
                         const unsigned char *info, unsigned char *out);
    int transfer_blocks(u64 in_ea, unsigned int in_size, u64 out_ea, unsigned int out_size,
                        const unsigned char *key, unsigned char *iv, hmac_sha1_ctx *ctx,
                        unsigned char *copy);
    int transfer_tail(u64 in_ea, unsigned int in_size, u64 out_ea, unsigned int out_size,
                      const unsigned char *key, unsigned char *iv, hmac_sha1_ctx *ctx,
                      unsigned char *copy);

    dma_reader *m_reader;
    dma_writer *m_writer;
    sce_sizes m_sizes __attribute__((aligned(16)));
    u64 m_data_len;
    const unsigned char *m_key;
    const unsigned char *m_iv;
    const unsigned char *m_pub;
    unsigned int m_curve;
};

class pkg_verifier : public sce_verifier {
public:
    pkg_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                 const unsigned char *iv, const unsigned char *pub, unsigned int curve);
    virtual ~pkg_verifier();
    int read_headers(const verify_params *p, sce_header *h, metadata_header *m,
                     metadata_section *s0, metadata_section *s1, metadata_section *s2,
                     unsigned char *keys);
    int verify(const verify_params *p, const sce_header *h, const metadata_header *m,
               const metadata_section *s0, const metadata_section *s1,
               const metadata_section *s2, const unsigned char *keys, pkg_info *info);
    int verify_info(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                    unsigned int in_size, u64 out_ea, unsigned int out_size,
                    unsigned char *copy);
    int check_info(const pkg_info *info, unsigned int magic, unsigned int type);
    int verify_block(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                     unsigned int in_size, u64 out_ea, unsigned int out_size,
                     unsigned char *copy);
    int verify_body(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                    unsigned int in_size, u64 out_ea, unsigned int out_size);
};

class rvk_verifier : public sce_verifier {
public:
    rvk_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                 const unsigned char *iv, const unsigned char *pub, unsigned int curve);
    virtual ~rvk_verifier();
    int read_headers(const verify_params *p, sce_header *h, metadata_header *m,
                     metadata_section *s0, metadata_section *s1, unsigned char *keys);
    int verify(const verify_params *p, const sce_header *h, const metadata_header *m,
               const metadata_section *s0, const metadata_section *s1,
               const unsigned char *keys, unsigned char *list);
    int verify_header(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                      unsigned int in_size, u64 out_ea, unsigned int out_size,
                      unsigned char *copy);
    int check_list_header(const rvk_header *h, unsigned int magic, unsigned int type);
    int verify_entries(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                       unsigned int in_size, u64 out_ea, unsigned int out_size,
                       unsigned char *copy);
};

#endif
