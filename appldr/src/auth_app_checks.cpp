#include "auth.h"

long authenticator::check_section_digest()
{
    if (check_part_digest(&m_section) && !m_1b9)
        return 14;
    return 0;
}

struct section_info {
    u64 a[4];
} __attribute__((aligned(16)));

long authenticator::get_section_info(unsigned int i, void *out)
{
    if (!m_loaded)
        return -4;
    if (i >= m_meta->section_count)
        return -3;
    *(section_info *)out = ((const section_info *)m_section_info)[i];
    return 0;
}

long authenticator::get_control_digest(unsigned char *out)
{
    if (!m_loaded)
        return -4;
    return m_block.get_digest(out) ? 19 : 0;
}

bool authenticator::is_elf64()
{
    if (!m_loaded)
        return true;

    bool none = m_elf.ehdr == 0;

    return !none;
}

bool authenticator::is_elf32()
{
    if (!m_loaded)
        return true;

    bool none = m_elf.ehdr32 == 0;

    return !none;
}

unsigned char authenticator::flag_1b9()
{
    return m_1b9;
}
