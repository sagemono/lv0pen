#include "revoke_list.h"

long revoke_list::scan_pairs(u64 auth_id, u64 version)
{
    const revoke_pair *p = m_pairs;
    u32 n = m_body->count;
    u32 i;

    for (i = 0; i != n; i++, p++)
        if (p->auth_id == auth_id && p->version == version)
            return 13;
    return 1;
}

long revoke_list::scan_pairs2(u64 auth_id, u64 version)
{
    const revoke_pair *p = m_pairs2;
    u32 n = m_body->count2;
    u32 i;

    for (i = 0; i != n; i++, p++)
        if (p->auth_id == auth_id && p->version == version)
            return 13;
    return 1;
}

long revoke_list::scan_rules(u32 type, u64 auth_id, u64 version)
{
    const revoke_rule *r = m_rules;
    u32 n = m_body->type == 4 ? m_body->count : 0;
    long rc = 1;
    u32 i;

    for (i = 0; i != n; r++, i++) {
        if (r->type != type || (auth_id & r->mask) != r->value)
            continue;
        switch (r->check) {
        case 0:
            if (version == r->version)
                return 13;
            break;
        case 1:
            if (version != r->version)
                return 13;
            break;
        case 2:
            if (version < r->version)
                return 13;
            break;
        case 3:
            if (version <= r->version)
                return 13;
            break;
        case 4:
            if (version > r->version)
                return 13;
            break;
        case 5:
            if (version >= r->version)
                return 13;
            break;
        default:
            rc = 13;
            break;
        }
    }
    return rc;
}
