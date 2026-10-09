#include "auth.h"
#include "dma_stream.h"

header_data g_header;

void authenticator::prepend(dma_buffer *b, unsigned int n)
{
    char *buf = (char *)g_header.buf;
    char *base = buf + n;
    char *dst = base + g_header.len;
    char *src = buf + g_header.len;
    while (dst > base)
        *--dst = *--src;
    char *end = (char *)(b->ls + b->length);
    char *p = end - n;
    while (p < end)
        *buf++ = *p++;
    g_header.len += n;
}

long elf_image::get_header(Elf64_Ehdr *out)
{
    if (!ehdr)
        return -1;
    *out = *ehdr;
    return 0;
}

long elf_image::get_program_header(unsigned int i, Elf64_Phdr *out)
{
    if (!ehdr)
        return -4;
    if (i >= ehdr->e_phnum)
        return -2;
    *out = phdr[i];
    return 0;
}
