#include "../../lv1ldr/include/auth.h"

long authenticator::read_program_header(unsigned int i, Elf32_Phdr *out)
{
    if (!m_loaded)
        return -4;
    return m_elf.get_program_header(i, out);
}

long authenticator::read_elf_header(Elf32_Ehdr *out)
{
    long rc = -4;

    if (m_loaded)
        rc = m_elf.get_header(out) != 0 ? -1 : 0;
    return rc;
}

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
