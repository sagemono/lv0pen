#include "auth.h"

long elf_image::get_header(Elf32_Ehdr *out)
{
    if (!ehdr32)
        return -1;
    *out = *ehdr32;
    return 0;
}

long elf_image::get_program_header(unsigned int i, Elf32_Phdr *out)
{
    if (!ehdr32)
        return -3;
    if (i >= ehdr32->e_phnum)
        return -2;
    *out = phdr32[i];
    return 0;
}
