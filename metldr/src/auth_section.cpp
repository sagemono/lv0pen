#include "auth.h"

long authenticator::find_section(unsigned int type, unsigned int index, auth_section *out)
{
    const auth_section *s;
    unsigned int i;

    if (!m_loaded)
        return -3;
    s = m_sections;
    for (i = 0; i < m_meta->section_count; i++, s++) {
        if (s->type == type && s->index == index) {
            *out = *s;
            return 0;
        }
    }
    return -2;
}
