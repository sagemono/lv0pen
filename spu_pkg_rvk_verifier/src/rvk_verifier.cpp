#include "pkg_rvk.h"

rvk_verifier::rvk_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                           const unsigned char *iv, const unsigned char *pub, unsigned int curve)
    : sce_verifier(reader, writer, key, iv, pub, curve)
{
}

rvk_verifier::~rvk_verifier()
{
}

int rvk_verifier::verify_header(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                                unsigned int in_size, u64 out_ea, unsigned int out_size,
                                unsigned char *copy)
{
    if (s == 0 || keys == 0)
        return VERIFY_EINVAL;
    if (in_ea == 0 || in_size == 0)
        return VERIFY_EINVAL;
    if (out_ea == 0 || out_size == 0)
        return VERIFY_EINVAL;
    if (copy == 0)
        return VERIFY_EINVAL;
    if (s->hashed != 2)
        return VERIFY_EBADF;
    if (s->encrypted != 1)
        return VERIFY_EBADF;
    return verify_section(s, keys, in_ea, in_size, out_ea, out_size, copy);
}

int rvk_verifier::check_list_header(const rvk_header *h, unsigned int magic, unsigned int type)
{
    if (h == 0)
        return VERIFY_EINVAL;
    return h->magic != magic || h->type != type ? VERIFY_EBADF : 0;
}

int rvk_verifier::verify_entries(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                                 unsigned int in_size, u64 out_ea, unsigned int out_size,
                                 unsigned char *copy)
{
    if (s == 0 || keys == 0)
        return VERIFY_EINVAL;
    if (in_ea == 0 || in_size == 0)
        return VERIFY_EINVAL;
    if (out_ea == 0 || out_size == 0)
        return VERIFY_EINVAL;
    if (in_size == 0 || out_size == 0)
        return VERIFY_EINVAL;
    if (s->hashed != 2)
        return VERIFY_EBADF;
    if (!(s->encrypted == 3 || s->encrypted == 1))
        return VERIFY_EBADF;
    return verify_section(s, keys, in_ea, in_size, out_ea, out_size, copy);
}
