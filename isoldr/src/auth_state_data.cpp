#include "../../lv1ldr/include/auth.h"
#include "util.h"

long authenticator::get_state_data(unsigned char *out, unsigned int size)
{
    if (!m_loaded)
        return -4;
    return m_state.get_data(out, size) ? -2 : 0;
}

unsigned char authenticator::flag_1b9()
{
    return m_1b9;
}

long auth_state::get_data(unsigned char *out, unsigned int size)
{
    if (size > 256)
        memcpy(out, buf, 256);
    else
        memcpy(out, buf, size);
    return 0;
}
