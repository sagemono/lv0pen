#include "mac.h"
#include "util.h"

mac_type1 g_mac_type1;

mac_type1::mac_type1() : mac(1, 40, 42)
{
    memset(&m_sha, 0, sizeof m_sha);
    m_a = 0;
    m_b = -1;
}

void mac_type1::init(const unsigned char *key, unsigned int flags)
{
}

int mac_type1::verify(const unsigned char *expected)
{
    return 1;
}

int mac_type1::update(const void *data, unsigned int len)
{
    return 1;
}
