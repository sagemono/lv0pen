#include "elf.h"

header_data g_header;

long elf_image::get_header(Elf32_Ehdr *out)
{
    if (!ehdr)
        return -1;
    *out = *ehdr;
    return 0;
}

long elf_image::get_program_header(unsigned int i, Elf32_Phdr *out)
{
    if (!ehdr)
        return -3;
    if (i > ehdr->e_phnum)
        return -2;
    *out = phdr[i];
    return 0;
}
