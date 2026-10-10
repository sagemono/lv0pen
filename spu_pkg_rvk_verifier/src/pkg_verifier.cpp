#include "pkg_rvk.h"

pkg_verifier::pkg_verifier(dma_reader *reader, dma_writer *writer, const unsigned char *key,
                           const unsigned char *iv, const unsigned char *pub, unsigned int curve)
    : sce_verifier(reader, writer, key, iv, pub, curve)
{
}

pkg_verifier::~pkg_verifier()
{
}

int pkg_verifier::verify_info(const metadata_section *s, const unsigned char *keys, u64 in_ea,
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

int pkg_verifier::check_info(const pkg_info *info, unsigned int magic, unsigned int type)
{
    if (info == 0)
        return VERIFY_EINVAL;
    return info->magic != magic || info->type != type ? VERIFY_EBADF : 0;
}

int pkg_verifier::verify_block(const metadata_section *s, const unsigned char *keys, u64 in_ea,
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

int pkg_verifier::verify_body(const metadata_section *s, const unsigned char *keys, u64 in_ea,
                              unsigned int in_size, u64 out_ea, unsigned int out_size)
{
    if (s == 0 || keys == 0)
        return VERIFY_EINVAL;
    if (in_ea == 0 || in_size == 0)
        return VERIFY_EINVAL;
    if (out_ea == 0 || out_size == 0)
        return VERIFY_EINVAL;
    if (s->hashed != 2)
        return VERIFY_EBADF;
    if (!(s->encrypted == 3 || s->encrypted == 1))
        return VERIFY_EBADF;
    return verify_section(s, keys, in_ea, in_size, out_ea, out_size, 0);
}
