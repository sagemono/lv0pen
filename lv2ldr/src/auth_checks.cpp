#include "auth.h"
#include "util.h"

struct auth_flags {
    u64 v[4];
};

bool operator==(const auth_flags &a, const auth_flags &b)
{
    if (a.v[0] == b.v[0] && a.v[1] == b.v[1] && a.v[2] == b.v[2] && a.v[3] == b.v[3])
        return true;
    return false;
}

auth_flags operator&(const auth_flags &a, const auth_flags &b)
{
    auth_flags r;

    r.v[0] = a.v[0] & b.v[0];
    r.v[1] = a.v[1] & b.v[1];
    r.v[2] = a.v[2] & b.v[2];
    r.v[3] = a.v[3] & b.v[3];
    return r;
}

extern const auth_flags g_required_flags;

long authenticator::check_auth_id_class(u64 a, u64 b)
{
    u64 id;

    if (!m_loaded)
        return -4;
    id = m_app->auth_id & 0x0FF0000000000000ULL;
    if ((b & id) == 0)
        return 12;
    if (m_app->type == 4 || m_app->type == 5 || m_app->type == 7 || m_app->type == 8)
        if ((a & id) == 0)
            return 12;
    return 0;
}

bool authenticator::has_required_flags()
{
    auth_flags f;

    if (m_block.get(f.v))
        return false;
    f = f & g_required_flags;
    return f == g_required_flags;
}

long authenticator::check_issuer(u64 a, u64 b)
{
    if (has_required_flags())
        return 0;
    switch (m_app->type) {
    case 4:
    case 5:
    case 7:
    case 8:
        if ((a & 0xFFFFFF) != (m_app->auth_id & 0xFFFFFF))
            return 12;
        return 0;
    case 3:
        if ((b & 0xFF000000) >> 24 != m_app->vendor[3])
            return 12;
        return 0;
    default:
        return 12;
    }
}

long authenticator::get_state(u64 *out)
{
    if (!m_loaded)
        return -4;
    return m_state.get(out) ? -1 : 0;
}

long authenticator::get_app_info(auth_app_info *out)
{
    if (!m_loaded)
        return -4;
    *out = *m_app;
    return 0;
}

long auth_state::get(u64 *out)
{
    out[0] = t.a;
    out[1] = t.b;
    out[2] = t.c;
    out[3] = t.d;
    if (t.a == 0 && t.b == 0 && t.c == 0 && t.d == 0)
        return -1;
    return 0;
}
