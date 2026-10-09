#include "sce.h"

const auth_section *sce_image::get_sections(void)
{
    const sce_header *h = m_sce;

    return (const auth_section *)((const unsigned char *)h + h->metadata_offset + 128);
}

long sce_image::get_section_count(u32 *out)
{
    if (this == 0 || out == 0)
        return -1;

    const sce_header *h = m_sce;

    const auth_meta *m = (const auth_meta *)((const unsigned char *)h + h->metadata_offset + 96);

    *out = m->section_count;
    return 0;
}

long sce_image::get_key_count(u32 *out)
{
    if (this == 0 || out == 0)
        return -1;

    const sce_header *h = m_sce;

    const auth_meta *m = (const auth_meta *)((const unsigned char *)h + h->metadata_offset + 96);

    *out = m->key_count;
    return 0;
}

long sce_image::get_ext_header(u32 *ext, u32 *meta_offset)
{
    if (this == 0 || ext == 0 || meta_offset == 0)
        return -1;

    const sce_header *h = m_sce;

    *ext = (u32)h + 32;
    *meta_offset = h->metadata_offset;
    return 0;
}
