#include "auth_flags.h"

bool operator!=(const auth_flags &a, const auth_flags &b)
{
    if (a.v[0] != b.v[0] || a.v[1] != b.v[1] || a.v[2] != b.v[2] || a.v[3] != b.v[3])
        return true;
    return false;
}
