#include "revoke_list.h"

long revoke_list::is_revoked(u32 type, u64 auth_id, u64 version)
{
    if (!m_opened)
        return -2;
    switch (type) {
    case 4:
    case 5:
    case 7:
    case 8:
        if (m_body->type == 3)
            return scan_pairs(auth_id, version);
        return scan_rules(type, auth_id, version);
    case 3:
        if (m_body->type == 3)
            return scan_pairs2(auth_id, version);
        return scan_rules(3, auth_id, version);
    default:
        return -3;
    }
}
