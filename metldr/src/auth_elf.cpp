#include "auth.h"

long authenticator::read_elf_header(Elf32_Ehdr *out)
{
    long rc = -3;

    if (m_loaded)
        rc = m_elf.get_header(out) != 0 ? -1 : 0;
    return rc;
}

long authenticator::read_program_header(unsigned int i, Elf32_Phdr *out)
{
    long rc;

    if (!m_loaded)
        return -3;
    rc = m_elf.get_program_header(i, out);
    if (rc)
        return rc;
    return 0;
}
