#include "auth.h"

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
