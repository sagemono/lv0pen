#include "elf.h"
#include "sce.h"
#include "util.h"

long sce_image::get_app_info(u32 *out)
{
    if (this == 0 || out == 0)
        return -1;

    const sce_header *h = m_sce;

    *out = (u32)h + (u32)((const sce_ext_header *)((u32)h + 32))->app_offset;
    return 0;
}

int check_sce_header(const vec_uchar16 *p, const void *dev, u16 type, u32 limit)
{
    const sce_header *h = (const sce_header *)p;

    if (p == 0)
        return -1;
    if (h->magic != 0x53434500)
        return -2;
    if (h->version != 2)
        return -2;
    if ((h->key_revision & 0x8000) && dev == 0)
        return -2;
    if (h->type != type)
        return -2;
    if (h->metadata_offset & 15)
        return -2;
    if (h->metadata_offset >= limit)
        return -2;
    if (h->metadata_offset + 128 > limit)
        return -2;
    if (h->header_len & 15)
        return -2;
    if (h->header_len < h->metadata_offset + 128)
        return -2;
    if (h->header_len > limit)
        return -2;
    return 0;
}

extern const int sig_sizes[];
extern const u32 digest_slots[];
extern const u32 key_slots[];

int check_meta_header(const vec_uchar16 *p, const auth_meta *m, u32 sig_type)
{
    const sce_header *h = (const sce_header *)p;
    int sig;
    u32 len;

    if (m == 0)
        return -1;
    if (m->sig_type != sig_type)
        return -1;
    if (m->sig_type - 1 > 4)
        return -1;
    sig = sig_sizes[m->sig_type];
    if (m->signed_len == 0)
        return -2;
    if (m->signed_len & 15)
        return -2;
    if (m->opt_header_size & 15)
        return -2;
    if (m->reserved != 0)
        return -2;
    len = (h->metadata_offset + 128 + m->section_count * 48 + m->key_count * 16
           + m->opt_header_size + sig + 127) & ~127;
    if (len != h->header_len)
        return -2;
    if (m->signed_len + sig > len)
        return -2;
    return 0;
}

static int get_digest_slots(int type, u32 *n)
{
    if (type < 1 || type > 5)
        return -1;
    *n = digest_slots[type];
    return 0;
}

static int get_key_slots(int type, u32 *n)
{
    if (type < 1 || type > 4)
        return -1;
    *n = key_slots[type];
    return 0;
}

long check_sections(sce_image *a)
{
    const auth_section *s;
    u32 count, keys, i, n;
    long r;

    if (a == 0)
        return -2L;
    count = 0;
    keys = 0;
    s = a->get_sections();
    r = a->get_section_count(&count);
    if (r < 0)
        return r;
    r = a->get_key_count(&keys);
    if (r < 0)
        return r;
    for (i = 0; i != count; s++, i++) {
        if (s->hashed == 0 || s->hashed > 4)
            return -2L;
        if (get_digest_slots(s->hashed, &n))
            return -1L;
        if (s->digest + n > keys)
            return -2L;
        switch (s->encrypted) {
        case 1:
            continue;
        case 2:
        case 3:
        case 4:
        case 5:
            break;
        default:
            return -2L;
        }
        if (get_key_slots(s->encrypted, &n))
            return -1L;
        if (s->key + n > keys)
            return -2L;
        if (s->iv + n > keys)
            return -2L;
    }
    return 0;
}

extern const unsigned char control_digest[20];

int check_control_info(const auth_control *c, u64 size)
{
    const auth_control *e = c;
    u64 off = 0;

    while (off < size) {
        switch (e->type) {
        case 1:
            off += 0x30;
            break;
        case 2:
            if (memcmp(e->data, control_digest, 20))
                return -2;
            off += 0x40;
            break;
        case 3:
            off += 0x90;
            break;
        default:
            return -2;
        }
        if (off > size)
            return -2;
        if (e->next == 0)
            break;
        e = (const auth_control *)((const unsigned char *)c + (u32)off);
    }
    if (off != size)
        return -2;
    return 0;
}

int check_ext_header(sce_image *image)
{
    u32 ext, meta;

    if (image == 0)
        return -1;
    if (image->get_ext_header(&ext, &meta))
        return -1;

    const sce_ext_header *e = (const sce_ext_header *)ext;
    if (e->type != 3)
        return -2;
    if (e->reserved != 0)
        return -2;

    const unsigned char *base = (const unsigned char *)image->m_sce;
    const unsigned char *elf = base + (u32)e->elf_offset;
    const sce_version_info *v = (const sce_version_info *)(base + (u32)e->version_offset);
    if (elf[4] == 1) {
        bool ph = e->phdr_offset != 0;
        bool si = e->section_info_offset != 0;
        u32 room = ph ? ((const Elf32_Ehdr *)elf)->e_phnum * sizeof(Elf32_Phdr) + 176 : 176;
        u32 sinfo = si ? ((const Elf32_Ehdr *)elf)->e_phnum * 32 : 0;
        u32 total = room + sinfo + v->count * 32 + 16;
        if (meta != total + e->control_size)
            return -2;
    } else if (elf[4] == 2) {
        bool ph = e->phdr_offset != 0;
        bool si = e->section_info_offset != 0;
        u32 room = ph ? ((((const Elf64_Ehdr *)elf)->e_phnum * sizeof(Elf64_Phdr) + 15) & 0xfffffff0)
                        + 176 : 176;
        u32 sinfo = si ? ((const Elf64_Ehdr *)elf)->e_phnum * 32 : 0;
        u32 total = room + sinfo + v->count * 32 + 16;
        if (meta != total + e->control_size)
            return -2;
    } else
        return -2;

    u32 limit = meta + 32;

    if ((e->app_offset & 15) || e->app_offset + 32 > limit || e->app_offset <= 111)
        return -2;
    if (elf[4] == 1) {
        if ((e->elf_offset & 15) || e->elf_offset + 64 > limit || e->elf_offset <= 111)
            return -2;
        u64 room = ((const Elf32_Ehdr *)elf)->e_phnum * sizeof(Elf32_Phdr) + 15;

        room &= 0xfffffff0;
        if ((e->phdr_offset & 15) || e->phdr_offset + room > limit
            || e->phdr_offset <= 111)
            return -2;
    } else if (elf[4] == 2) {
        if ((e->elf_offset & 15) || e->elf_offset + 64 > limit || e->elf_offset <= 111)
            return -2;
        u64 room = ((const Elf64_Ehdr *)elf)->e_phnum * sizeof(Elf64_Phdr) + 15;

        room &= 0xfffffff0;
        if ((e->phdr_offset & 15) || e->phdr_offset + room > limit
            || e->phdr_offset <= 111)
            return -2;
    } else
        return -2;
    if (e->section_info_offset != 0) {
        if ((e->section_info_offset & 15) || e->section_info_offset + 32 > limit
            || e->section_info_offset <= 111)
            return -2;
    }
    if ((e->version_offset & 15) || e->version_offset + 16 > limit
        || e->version_offset <= 111)
        return -2;
    if (e->control_offset != 0) {
        if ((e->control_offset & 15) || e->control_offset + 16 > limit
            || e->control_offset <= 111)
            return -2;
    }
    return check_control_info((const auth_control *)(base + (u32)e->control_offset),
                              e->control_size);
}
