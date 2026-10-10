#include <spu_intrinsics.h>
#include "pkg_rvk.h"
#include "aes.h"
#include "sha1.h"
#include "hmac_sha1.h"
#include "ec.h"
#include "util.h"

static const unsigned char g_rvk_key[32] __attribute__((aligned(16))) = {
    0x69, 0x59, 0x84, 0xB7, 0xEE, 0x2E, 0xC2, 0xF7, 0x7F, 0xCC, 0x31, 0x20, 0x15, 0x0D, 0xCE, 0x7E,
    0x44, 0x80, 0xD8, 0x44, 0x80, 0xDD, 0xD8, 0xC5, 0x94, 0x0A, 0xEB, 0x6F, 0x79, 0xE6, 0x3D, 0x17,
};
static const unsigned char g_rvk_pub[40] __attribute__((aligned(16))) = {
    0x7F, 0x19, 0x50, 0xC6, 0xE4, 0x97, 0xE9, 0x22, 0x40, 0x05, 0x86, 0xEE, 0x33, 0x8B, 0x41, 0xE0,
    0x1C, 0x90, 0x6C, 0x5A, 0x6D, 0xFD, 0x75, 0xFE, 0xB5, 0x24, 0x9C, 0xFA, 0x4B, 0xEC, 0x53, 0x4F,
    0x5C, 0xA6, 0x79, 0x67, 0x0A, 0x45, 0x2B, 0x2A,
};
static const unsigned char g_pkg_key[32] __attribute__((aligned(16))) = {
    0xF8, 0xF9, 0x90, 0x06, 0xF1, 0xC0, 0x07, 0xD5, 0xD0, 0xB1, 0x90, 0x9E, 0x95, 0x66, 0xE0, 0xE7,
    0x0B, 0x56, 0x93, 0x99, 0xFC, 0x33, 0x94, 0xA8, 0x11, 0x80, 0x9F, 0xDB, 0x5C, 0xAE, 0x92, 0xCD,
};
static const unsigned char g_pkg_pub[40] __attribute__((aligned(16))) = {
    0x54, 0x32, 0xBD, 0xDD, 0x1F, 0x97, 0x41, 0x81, 0x47, 0xAF, 0xF0, 0x16, 0xEA, 0xA6, 0x10, 0x08,
    0x34, 0xF2, 0xCA, 0xA8, 0xC4, 0x98, 0xB8, 0x89, 0x65, 0x68, 0x9E, 0xE4, 0x4D, 0xF3, 0x49, 0xB0,
    0x66, 0xCD, 0x43, 0xCB, 0xF4, 0xF2, 0xC5, 0xD0,
};

unsigned char g_pkg_iv[16] __attribute__((aligned(16))) = {
    0x59, 0xD2, 0x8D, 0xB4, 0xAD, 0xDF, 0xB4, 0x0B, 0x7D, 0x76, 0x8B, 0xC9, 0x66, 0x7C, 0x67, 0xB1,
};
unsigned int g_pkg_curve = 0x17;
unsigned char g_rvk_iv[16] __attribute__((aligned(16))) = {
    0xA8, 0xBA, 0x3E, 0x4E, 0x63, 0xB2, 0xBB, 0x06, 0xFC, 0x0C, 0xE5, 0x7E, 0x3B, 0xB8, 0xFC, 0x46,
};
unsigned int g_rvk_curve = 0x12;

#define LS_PARAMS       0x3E000
#define LS_READ         0x3E000
#define LS_WRITE        0x3F000
#define TAG_PARAMS      1
#define TAG_READ        1
#define TAG_WRITE       2

struct unused_args {
    u64 words[34];
} __attribute__((aligned(16)));

#define METADATA_HEADER         (sizeof(sce_header) + 64)
#define METADATA_SECTIONS       (METADATA_HEADER + sizeof(metadata_header))

int pkg_verifier::read_headers(const verify_params *p, sce_header *h, metadata_header *m,
                               metadata_section *s0, metadata_section *s1,
                               metadata_section *s2, unsigned char *keys)
{
    u64 ea = p->in_ea, size = p->in_size;
    unsigned int off;
    int r;

    if (size < sizeof *h)
        return VERIFY_EINVAL;
    r = read_header(ea, sizeof *h, h);
    if (r)
        return r;
    if (h->sizes.header_len > size)
        return VERIFY_EINVAL;
    if (h->sizes.header_len > 0x1000)
        return VERIFY_EBADF;
    r = check_header(h, SCE_VERSION, SCE_TYPE_PKG, 0);
    if (r)
        return r;
    unsigned char buf[(unsigned int)h->sizes.header_len] __attribute__((aligned(16)));
    r = read_metadata(ea, h, buf);
    if (r)
        return r;
    r = check_section_count((const metadata_header *)(buf + METADATA_HEADER), 3);
    if (r)
        return r;
    memcpy(m, buf + METADATA_HEADER, sizeof *m);
    r = check_section((const metadata_section *)(buf + METADATA_SECTIONS), 1, 1);
    if (r)
        return r;
    r = check_section((const metadata_section *)(buf + METADATA_SECTIONS + 48), 2, 2);
    if (r)
        return r;
    r = check_section((const metadata_section *)(buf + METADATA_SECTIONS + 96), 3, 3);
    if (r)
        return r;
    memcpy(s0, buf + METADATA_SECTIONS, sizeof *s0);
    memcpy(s1, buf + METADATA_SECTIONS + 48, sizeof *s1);
    memcpy(s2, buf + METADATA_SECTIONS + 96, sizeof *s2);
    off = h->metadata_offset + METADATA_SECTIONS + m->section_count * sizeof *s0;
    if ((u64)m->key_count * 16 > 0x1000)
        return VERIFY_ERANGE;
    memcpy(keys, buf + off, (u64)m->key_count * 16);
    return r;
}

int read_params(u64 ea, u64 size, verify_params *p)
{
    dma_channel dma;
    unsigned int ls;

    if (size != sizeof *p)
        return VERIFY_EINVAL;
    ls = LS_PARAMS + (ea & 0x7F);
    if (dma.issue(ls, ea, sizeof *p, TAG_PARAMS, 0, MFC_GET))
        return VERIFY_EDMA;
    dma.wait(TAG_PARAMS);
    memcpy(p, (const void *)ls, sizeof *p);
    return 0;
}

int rvk_verifier::read_headers(const verify_params *p, sce_header *h, metadata_header *m,
                               metadata_section *s0, metadata_section *s1, unsigned char *keys)
{
    u64 ea = p->in_ea, size = p->in_size;
    unsigned int off;
    int r;

    if (size < sizeof *h)
        return VERIFY_EINVAL;
    r = read_header(ea, sizeof *h, h);
    if (r)
        return r;
    if (h->sizes.header_len > size)
        return VERIFY_EINVAL;
    if (h->sizes.header_len > 0x1000)
        return VERIFY_EBADF;
    r = check_header(h, SCE_VERSION, SCE_TYPE_RVK, 0);
    if (r)
        return r;
    if (h->sizes.header_len + h->sizes.data_len > 0x1000)
        return VERIFY_EBADF;
    unsigned char buf[(unsigned int)h->sizes.header_len] __attribute__((aligned(16)));
    r = read_metadata(ea, h, buf);
    if (r)
        return r;
    r = check_section_count((const metadata_header *)(buf + METADATA_HEADER), 2);
    if (r)
        return r;
    memcpy(m, buf + METADATA_HEADER, sizeof *m);
    r = check_section((const metadata_section *)(buf + METADATA_SECTIONS), 1, 1);
    if (r)
        return r;
    r = check_section((const metadata_section *)(buf + METADATA_SECTIONS + 48), 2, 2);
    if (r)
        return r;
    memcpy(s0, buf + METADATA_SECTIONS, sizeof *s0);
    memcpy(s1, buf + METADATA_SECTIONS + 48, sizeof *s1);
    off = h->metadata_offset + METADATA_SECTIONS + m->section_count * sizeof *s0;
    if ((u64)m->key_count * 16 > 0x1000)
        return VERIFY_ERANGE;
    memcpy(keys, buf + off, (u64)m->key_count * 16);
    return r;
}

int pkg_verifier::verify(const verify_params *p, const sce_header *h, const metadata_header *m,
                         const metadata_section *s0, const metadata_section *s1,
                         const metadata_section *s2, const unsigned char *keys, pkg_info *info)
{
    unsigned char block[64] __attribute__((aligned(16)));
    u64 in_ea = p->in_ea, in_size = p->in_size;
    u64 out_ea = p->out_ea, out_size = p->out_size;
    u64 header_len = h->sizes.header_len;
    unsigned int type, body;
    int r;

    switch ((unsigned int)p->type) {
    case 1:
        type = 1;
        break;
    case 4:
        type = 5;
        break;
    case 5:
        type = 6;
        break;
    case 6:
        type = 7;
        break;
    case 7:
        type = 3;
        break;
    case 8:
        type = 4;
        break;
    case 9:
        type = 8;
        break;
    default:
        return VERIFY_EINVAL;
    }
    if (in_size < 64)
        return VERIFY_EINVAL;
    if (out_size < 64)
        return VERIFY_EINVAL;
    in_ea += header_len;
    r = verify_info(s0, keys, in_ea, 64, out_ea, 64, (unsigned char *)info);
    if (r)
        return r;
    r = check_info(info, 3, type);
    if (r)
        return r;
    in_size -= 64;
    if (in_size < 64)
        return VERIFY_EINVAL;
    out_size -= 64;
    if (out_size < 64)
        return VERIFY_EINVAL;
    in_ea += 64;
    out_ea += 64;
    r = verify_block(s1, keys, in_ea, 64, out_ea, 64, block);
    if (r)
        return r;
    body = info->body_size;
    if (body > in_size - 64)
        return VERIFY_EINVAL;
    if (info->body_size > out_size - 64)
        return VERIFY_EINVAL;
    return verify_body(s2, keys, in_ea + 64, body, out_ea + 64, body);
}

int rvk_verifier::verify(const verify_params *p, const sce_header *h, const metadata_header *m,
                         const metadata_section *s0, const metadata_section *s1,
                         const unsigned char *keys, unsigned char *list)
{
    const rvk_header *lh = (const rvk_header *)list;
    u64 in_ea = p->in_ea, in_size = p->in_size;
    u64 out_ea = p->out_ea, out_size = p->out_size;
    u64 header_len = h->sizes.header_len;
    unsigned int type;
    u64 len;
    int r;

    switch ((unsigned int)p->type) {
    case 2:
        type = 1;
        break;
    case 3:
        type = 2;
        break;
    default:
        return VERIFY_EINVAL;
    }
    if (list == 0)
        return VERIFY_EINVAL;
    if (out_size & 15)
        return VERIFY_EBADF;
    if (in_size < 32)
        return VERIFY_EINVAL;
    if (out_size < 32)
        return VERIFY_EINVAL;
    in_ea += header_len;
    r = verify_header(s0, keys, in_ea, 32, out_ea, 32, list);
    if (r)
        return r;
    if (type == 2) {
        r = check_list_header(lh, 3, type);
    } else {
        r = check_list_header(lh, 4, type);
        if (r)
            r = check_list_header(lh, 3, type);
    }
    if (r)
        return r;
    len = lh->type == 2 ? (u64)lh->entry_count * 32
        : lh->magic == 3 ? ((u64)lh->count2 + lh->entry_count) * 16
        : (u64)lh->entry_count * 32;
    if (len > in_size - 32)
        return VERIFY_EINVAL;
    if ((unsigned int)len > out_size - 32)
        return VERIFY_EINVAL;
    if (len == 0)
        return r;
    return verify_entries(s1, keys, in_ea + 32, len, out_ea + 32, len, list + 32);
}

int check_revoked(const pkg_info *info, const unsigned char *list)
{
    dma_channel dma;
    const rvk_header *h = (const rvk_header *)list;
    const rvk_entry *entries;
    int hits = 0;
    unsigned int i, type;
    u64 id, version;

    if (info == 0 || list == 0)
        return VERIFY_EINVAL;
    entries = (const rvk_entry *)(list + sizeof *h);
    type = info->type;
    id = info->id;
    version = info->version;
    for (i = 0; i != h->entry_count; i++) {
        const rvk_entry *e = &entries[i];

        if (type == e->type && id == e->id) {
            switch (e->op) {
            case RVK_EQ:
                if (version == e->version)
                    hits++;
                break;
            case RVK_NE:
                if (version != e->version)
                    hits++;
                break;
            case RVK_LT:
                if (version < e->version)
                    hits++;
                break;
            case RVK_LE:
                if (version <= e->version)
                    hits++;
                break;
            case RVK_GT:
                if (version > e->version)
                    hits++;
                break;
            case RVK_GE:
                if (version >= e->version)
                    hits++;
                break;
            default:
                return VERIFY_ERANGE;
            }
        }
        if (hits > 0)
            return VERIFY_REVOKED;
    }
    return 0;
}

int verify_list(const verify_params *p, unsigned char *list)
{
    sce_header h;
    metadata_header m;
    metadata_section s0, s1;
    unsigned char keys[0x1000] __attribute__((aligned(16)));
    int r;
    read_cursor rc;
    write_cursor wc;
    dma_reader reader(LS_READ, 0x1000, TAG_READ, &rc);
    dma_writer writer(LS_WRITE, 0x1000, TAG_WRITE, &wc);
    rvk_verifier v(&reader, &writer, g_rvk_key, g_rvk_iv, g_rvk_pub, g_rvk_curve);

    r = v.read_headers(p, &h, &m, &s0, &s1, keys);
    if (r == 0)
        r = v.verify(p, &h, &m, &s0, &s1, keys, list);
    return r;
}

static inline bool version_ok(u64 version, u64 min_version)
{
    return min_version == 0 || min_version <= version;
}

int verify_package(const verify_params *p, pkg_info *info)
{
    sce_header h;
    metadata_header m;
    metadata_section s0, s1, s2;
    unsigned char keys[0x1000] __attribute__((aligned(16)));
    int r;
    read_cursor rc;
    write_cursor wc;
    dma_reader reader(LS_READ, 0x1000, TAG_READ, &rc);
    dma_writer writer(LS_WRITE, 0x1000, TAG_WRITE, &wc);
    pkg_verifier v(&reader, &writer, g_pkg_key, g_pkg_iv, g_pkg_pub, g_pkg_curve);

    r = v.read_headers(p, &h, &m, &s0, &s1, &s2, keys);
    if (r == 0)
        r = v.verify(p, &h, &m, &s0, &s1, &s2, keys, info);
    return r;
}

int main(u64 ea, u64 size, u64 arg3, u64 arg4, unused_args args, u64 min_version)
{
    pkg_info info;
    verify_params list_params;
    verify_params p;
    unsigned char list[0x1000] __attribute__((aligned(16)));

    if (read_params(ea, size, &p))
        spu_stop(STOP_BAD_PARAMS);
    memcpy(&list_params, &p, sizeof p);
    memset(list, 0, sizeof list);
    switch (p.type) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (verify_package(&p, &info))
            spu_stop(STOP_BAD_FILE);
        if (p.type == 1 && !version_ok(info.version, min_version))
            goto too_old;
    version_checked:
        if (p.flags & 1) {
            list_params.type = 3;
            list_params.in_ea = list_params.list_in_ea;
            list_params.in_size = list_params.list_in_size;
            list_params.out_ea = list_params.list_out_ea;
            list_params.out_size = list_params.list_out_size;
            if (verify_list(&list_params, list))
                spu_stop(STOP_BAD_LIST);
            if (check_revoked(&info, list))
                spu_stop(STOP_REVOKED);
        }
        break;
    case 2:
    case 3:
        if (verify_list(&p, list))
            spu_stop(STOP_BAD_FILE);
        break;
    default:
        spu_stop(STOP_BAD_PARAMS);
        break;
    too_old:
        spu_stop(STOP_REVOKED);
        goto version_checked;
    }
    spu_stop(STOP_DONE);
    return STOP_DONE;
}
