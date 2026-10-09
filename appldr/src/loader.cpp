#include "loader.h"
#include "key_store.h"
#include "channels.h"
#include "auth.h"
#include "dma_stream.h"
#include "util.h"
#include "auth_flags.h"
#include "cipher.h"
#include "params.h"
#include "qa_flag.h"
#include "revoke_list.h"
#include "hash_reader.h"
#include "segment_loader.h"
#include "mailbox.h"
#include "aes.h"
#include "data_decryptor.h"
#include <new>

struct block128 {
    vec_uchar16 q[8];
};

extern unsigned char g_auth_area[656];
extern unsigned char g_9b0[120];
extern qa_token g_qa_token;

extern unsigned char g_keyset_key[32];
extern unsigned char g_keyset_iv[16];
extern app_keyset g_keysets_a[21];
extern app_keyset g_keysets_b[21];
extern app_keyset g_keysets_7[1];
extern app_keyset g_keysets2_a[11];
extern app_keyset g_keysets2_b[11];
extern vec_uchar16 g_keys_a[2];
extern vec_uchar16 g_keys_b[2];

extern const auth_flags g_flags_none;
extern const auth_flags g_flags_1;
extern const auth_flags g_flags_2;
extern const auth_flags g_flags_3;
extern const auth_flags g_flags_4;
extern const auth_flags g_flags_5;

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

struct known_app {
    unsigned int f[3];
    unsigned int g[4];
    unsigned char digest[20];
};

extern const known_app g_known_apps[457];

extern const unsigned int g_table_a[31];
extern const unsigned int g_table_b[31];

struct block_ref {
    unsigned int offset;
    unsigned char mask;
};

extern unsigned char g_qa_flag[] __attribute__((aligned(16)));
extern const block_ref g_ref_2e840;
extern const block_ref g_ref_33f20;
extern const block_ref g_ref_33f30;

extern unsigned char g_key[32] __attribute__((aligned(16)));
extern unsigned char g_iv[16] __attribute__((aligned(16)));
extern const unsigned char params_wrap_0[64];
extern const unsigned char params_wrap_1[64];
extern revoke_list g_revoke_list;
extern const unsigned char g_list_pub[48];
extern const unsigned int g_list_curve;

extern const auth_flags g_flags_old;
extern const unsigned char g_args_key_a[16];
extern const unsigned char g_args_iv_a[16];
extern const unsigned char g_args_key_b[16];
extern const unsigned char g_args_iv_b[16];

extern unsigned char g_stream_area[48];
extern unsigned char g_direct_area[16];

extern const unsigned char g_hash_key[64];

extern const unsigned char g_revoked_digest[20];

static bool s_unwrapped_keysets;
static bool s_unwrapped_keys;
static long s_version;
static bool s_version_checked;

long key_store::request()
{
    if (ch64_request())
        return 48;
    return 0;
}

long key_store::unwrap_keys()
{
    int i;

    if (s_unwrapped_keys)
        return 0;
    for (i = 0; i < 2; i++)
        if (ch72_unwrap_key(&g_keys_a[i], &g_keys_a[i], 16))
            return 48;
    for (i = 0; i < 2; i++)
        if (ch72_unwrap_key(&g_keys_b[i], &g_keys_b[i], 16))
            return 48;
    s_unwrapped_keys = true;
    return 0;
}

long key_store::unwrap_keysets()
{
    int i;

    if (s_unwrapped_keysets)
        return 0;
    if (ch72_unwrap_key((vec_uchar16 *)g_keyset_key, (const vec_uchar16 *)g_keyset_key, 32))
        return 48;
    if (ch72_unwrap_iv((vec_uchar16 *)g_keyset_iv, (const vec_uchar16 *)g_keyset_iv, 16))
        return 48;
    for (i = 0; i < 21; i++) {
        if (ch72_unwrap_key((vec_uchar16 *)g_keysets_a[i].key, (const vec_uchar16 *)g_keysets_a[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_keysets_a[i].iv, (const vec_uchar16 *)g_keysets_a[i].iv, 16))
            return 48;
        if (ch72_unwrap_key((vec_uchar16 *)g_keysets_b[i].key, (const vec_uchar16 *)g_keysets_b[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_keysets_b[i].iv, (const vec_uchar16 *)g_keysets_b[i].iv, 16))
            return 48;
    }
    if (ch72_unwrap_key((vec_uchar16 *)g_keysets_7[0].key, (const vec_uchar16 *)g_keysets_7[0].key, 32))
        return 48;
    if (ch72_unwrap_iv((vec_uchar16 *)g_keysets_7[0].iv, (const vec_uchar16 *)g_keysets_7[0].iv, 16))
        return 48;
    for (i = 0; i < 11; i++) {
        if (ch72_unwrap_key((vec_uchar16 *)g_keysets2_a[i].key, (const vec_uchar16 *)g_keysets2_a[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_keysets2_a[i].iv, (const vec_uchar16 *)g_keysets2_a[i].iv, 16))
            return 48;
        if (ch72_unwrap_key((vec_uchar16 *)g_keysets2_b[i].key, (const vec_uchar16 *)g_keysets2_b[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_keysets2_b[i].iv, (const vec_uchar16 *)g_keysets2_b[i].iv, 16))
            return 48;
    }
    s_unwrapped_keysets = true;
    return 0;
}

long key_store::check_version(u64 version)
{
    if (s_version_checked)
        return s_version;
    s_version = ch72_compare_version(version);
    s_version_checked = true;
    return s_version;
}

void loader::fetch(u64 ea, void *dst)
{
    mfc_issue(0x3E000, ea, 128, 1, 1, 64);
    while (!mfc_tag_done(1))
        ;
    *(block128 *)dst = *(const block128 *)0x3E000;
}

long loader::read_request()
{
    while (*(volatile unsigned int *)0x3E000 != 0xFF)
        ;
    u64 version = *(const u64 *)0x3E848;

    if (__builtin_expect(version != 5, 0))
        return 39;
    m_arg = *(const u64 *)0x3E808;
    fetch(*(const u64 *)0x3E800, &m_args);
    m_buf = 0x3F000;
    memcpy(&g_qa_token, (const void *)0x3EC00, sizeof g_qa_token);
    memcpy(&m_240, (const void *)0x3EE00, sizeof m_240);
    u64 f = m_args.kind;
    long r = 0;

    if ((f & 0xFFFF) > version || (f & 0x00FF0000) >> 16 > 3 || (f & 0xFF000000) >> 24 > 2)
        r = 39;
    return r;
}

void loader::put_result()
{
    memcpy((void *)0x3E000, &m_result, sizeof m_result);
    mfc_issue(0x3E000, m_args.result_ea, 32, 5, 0, 32);
    while (!mfc_tag_done(5))
        ;
}

void loader::reset()
{
    m_keys.key = 0;
    m_keys.pub = 0;
    m_keys.curve = 0;
    m_keys.args_key = 0;
    m_keys.args_iv = 0;
    if (m_auth) {
        m_auth->reset();
        m_auth = 0;
    }
    memset(g_auth_area, 0, sizeof g_auth_area);
    memset(&m_result, 0, sizeof m_result);
    memset(g_9b0, 0, sizeof g_9b0);
}

loader::loader()
{
    m_arg = 0;
    m_buf = 0;
    m_list = 0;
    m_auth = 0;
    m_revision = 0xFFFF;
    m_type = 0;
    m_232[0] = 0;
    m_232[1] = 0;
    m_232[2] = 0;
    m_232[3] = 0;
    m_236 = 0;
    memset(&m_240, 0, sizeof m_240);
    reset();
}

long loader::run()
{
    {
        key_store keys;

        if (keys.check_version(0x0004009300000000ULL) || keys.unwrap_keysets() || keys.unwrap_keys()
            || keys.request())
            return 48;
    }
    long r = read_request();
    if (r)
        return r;
    u64 kind = m_args.kind & 0xFFFF;
    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return load_a();
    case 5:
        return load_b();
    default:
        return 39;
    }
}

bool loader::keyset_pub_is_zero()
{
    unsigned char zero[40];

    memset(zero, 0, sizeof zero);
    return bytes_differ(zero, g_keysets_a[1].pub, sizeof zero) == 0;
}

bool loader::check_flags_a(u64 auth_id, const auth_flags &f, const auth_flags &g)
{
    if (f != g_flags_none)
        return true;
    if (!(g != g_flags_4) || !(g != g_flags_5) || !(g != g_flags_1) || !(g != g_flags_2)
        || !(g != g_flags_3))
        return (auth_id & 0xFFFFFF) != 3;
    return true;
}

bool loader::is_kind_b(const auth_app_info *app, const auth_flags &f, const auth_flags &g)
{
    if (app->type != 4 || (app->auth_id & 0xFFFFFF) != 3)
        return false;
    if (!(f == g_flags_none))
        return false;
    if (g == g_flags_1)
        return true;
    return g == g_flags_2;
}

bool loader::is_kind_c(const auth_app_info *app, const auth_flags &f, const auth_flags &g)
{
    if (app->type != 4 || (app->auth_id & 0xFFFFFF) != 3)
        return false;
    if (!(f == g_flags_none))
        return false;
    if (g == g_flags_4)
        return true;
    return g == g_flags_5;
}

bool loader::is_kind_d(const auth_app_info *app, const auth_flags &f, const auth_flags &g)
{
    if (f == g_flags_none && (g == g_flags_1 || g == g_flags_2 || g == g_flags_3)
        && (app->auth_id & 0xFFFFFF) == 3)
        return app->type == 8;
    return false;
}

bool loader::has_flags(const auth_flags &f, const auth_flags &want)
{
    auth_flags x = f;

    return (x & want) == want;
}

long loader::unpack3(const unsigned int *w, auth_flags *out)
{
    out->v[0] = (u64)w[0] << 32;
    out->v[1] = 0;
    out->v[2] = 0;
    out->v[3] = (u64)w[1] << 32 | w[2];
    return 0;
}

long loader::unpack4(const unsigned int *w, auth_flags *out)
{
    out->v[0] = 0;
    out->v[1] = w[0];
    out->v[2] = w[1];
    out->v[3] = (u64)w[2] << 32 | w[3];
    return 0;
}

long loader::check_known(const auth_flags &f, const auth_flags &g, const unsigned char *digest)
{
    int i;

    for (i = 0; i < 457; i++) {
        auth_flags a, b;

        if (unpack3(g_known_apps[i].f, &a))
            return 21;
        if (unpack4(g_known_apps[i].g, &b))
            return 21;
        if (f == a && g == b && !bytes_differ(digest, g_known_apps[i].digest, 20))
            return 0;
    }
    return 28;
}

long loader::check_type_value(unsigned int type, unsigned int v)
{
    switch (type) {
    case 7:
        if (v != 3)
            return 39;
        return 0;
    case 4:
        if (v > 2)
            return 39;
        return 0;
    case 8:
        if (v != 4)
            return 39;
        return 0;
    default:
        return 28;
    }
}

long loader::lookup_a(unsigned short i, unsigned int *out)
{
    if (i > 30 || g_table_a[i] == ~0U)
        return 19;
    *out = g_table_a[i];
    return 0;
}

long loader::lookup_b(unsigned short i, unsigned int *out)
{
    if (i > 30)
        return 19;
    if (g_table_b[i] != ~0U)
        *out = g_table_b[i];
    return 0;
}

long loader::lookup(unsigned int type, unsigned short i, unsigned int *out)
{
    switch (type) {
    case 7:
        if (i != 0)
            return 19;
        *out = 0;
        return 0;
    case 4:
        return lookup_a(i, out);
    case 8:
        return lookup_b(i, out);
    default:
        return 28;
    }
}

bool loader::is_129_130(unsigned int x, bool flag)
{
    return flag && (x == 129 || x == 130);
}

long loader::get_digest(unsigned char *out, unsigned int size)
{
    return m_auth->get_section_digest(2, (unsigned int)m_args.a0[3], out, size);
}

long loader::check_version_limit(u64 version, authenticator *a)
{
    u64 w;
    long r = 21;

    if (a) {
        r = a->get_control_w(&w);
        if (r == 0 && w != 0 && w > version_number(version))
            r = 13;
    }
    return r;
}

long loader::check_control_digest(authenticator *a)
{
    unsigned char d[20];

    if (!a)
        return 21;
    if (a->get_control_digest(d))
        return 21;
    if (!bytes_differ(d, g_revoked_digest, 20))
        return 13;
    return 0;
}

long loader::get_state(authenticator *a, auth_flags *out)
{
    if (!a)
        return 21;
    if (a->flag_1b9()) {
        out->v[0] = 0x8000000000000000ULL;
        out->v[1] = 0;
        out->v[2] = 0;
        out->v[3] = 0;
        return 0;
    }
    if (a->get_state(out->v)) {
        out->v[0] = 0;
        out->v[1] = 0;
        out->v[2] = 0;
        out->v[3] = 0;
    }
    return 0;
}

long loader::check_type_bit(unsigned int type, const auth_flags *state)
{
    unsigned int bits = state->v[2];

    switch (type) {
    case 129:
        if (bits & 8)
            return 0;
        return 43;
    case 130:
        if (bits & 16)
            return 0;
        return 43;
    case 131: case 132: case 133: case 134: case 135: case 136: case 137:
    case 138: case 139: case 140: case 141: case 142: case 143:
        if (bits & 32)
            return 0;
        return 43;
    case 160:
        if (bits & 64)
            return 0;
        return 43;
    default:
        return 40;
    }
}

u64 loader::version_number(u64 version)
{
    return ((version & 0x00F0000000000000ULL) >> 52) * 100000
        + ((version & 0x000F000000000000ULL) >> 48) * 10000
        + ((version & 0x000000F000000000ULL) >> 36) * 1000
        + ((version & 0x0000000F00000000ULL) >> 32) * 100;
}

long loader::read_mode()
{
    switch ((m_args.kind & 0xFF000000) >> 24) {
    case 0:
        m_232[0] = 0;
        m_232[1] = 0;
        m_232[3] = 0;
        return 0;
    case 1:
        m_232[3] = 0;
        break;
    case 2:
        m_232[3] = 1;
        break;
    default:
        return 39;
    }
    switch ((m_args.kind & 0xFF0000) >> 16) {
    case 1:
        m_232[0] = 0;
        break;
    case 2:
        break;
    default:
        return 39;
    }
    return 0;
}

void loader::override_state(auth_flags *state)
{
    unsigned int flags = state->v[1];
    unsigned short v = state->v[2];

    if (flags & g_ref_33f30.mask)
        v = *(const unsigned short *)&g_qa_flag[g_ref_33f30.offset];
    if (flags & g_ref_33f20.mask)
        v = *(const unsigned short *)&g_qa_flag[g_ref_33f20.offset];
    state->v[2] = (state->v[2] & ~0xFFFFULL) | v;
}

bool loader::flag_set()
{
    return m_232[2] && (g_qa_flag[g_ref_2e840.offset] & g_ref_2e840.mask);
}

long loader::check_state(unsigned int type, authenticator *a, auth_flags *state)
{
    if (!a)
        return 21;
    if (get_state(a, state))
        return 21;
    if (m_232[2] && (type == 129 || type == 130))
        override_state(state);
    if (a->flag_1b9())
        return 0;
    return check_type_bit(type, state);
}

long loader::open_params(unsigned int *revision, unsigned char *id)
{
    aes256_cbc_encryptor enc;
    unsigned char wrap1[64];
    unsigned char wrap0[64];
    param_block p;

    enc.set_key(g_key, (const cipher_iv *)g_iv);
    if (enc.encrypt(params_wrap_0, 64, wrap0)) {
        *revision = 0xFFFF;
        return 21;
    }
    if (enc.encrypt(params_wrap_1, 64, wrap1)) {
        *revision = 0xFFFF;
        return 21;
    }
    p.init();
    if (p.open((const unsigned char *)0x3E400, 1024, wrap0 + 32, wrap0 + 16, wrap1 + 32,
               wrap1 + 16, revision, id, 0)) {
        *revision = 0xFFFF;
        return 40;
    }
    return 0;
}

long loader::read_qa_flag(unsigned int revision, const unsigned char *id,
                          const qa_token *token, unsigned char *flag, unsigned char *have)
{
    qa_flag_reader q;

    *have = 0;
    memset(flag, 0, 4);
    if ((revision == 129 || revision == 130) && token->none == 0) {
        q.init();
        if (q.check(revision, token->body, flag, id, g_qa_token.sig))
            return 42;
        *have = 1;
    }
    return 0;
}

long loader::check_revoke_list(revoke_list *list, const void *src)
{
    long r;

    if (!list || !src)
        return 21;
    r = list->load(src);
    if (r)
        return r;
    r = list->open(g_keyset_key, g_keyset_iv, g_list_pub, &g_list_curve);
    if (r)
        return r;
    r = list->parse();
    if (r)
        return r;
    r = list->check_version(0x0004009300000000ULL);
    if (r)
        r = 22;
    return r;
}

long loader::prepare()
{
    unsigned char id[16];

    memcpy(g_key, (const void *)0, 32);
    memcpy(g_iv, (const void *)32, 16);
    open_params(&m_revision, id);
    read_qa_flag(m_revision, id, &g_qa_token, g_qa_flag, &m_232[2]);
    g_revoke_list.init();
    m_list = &g_revoke_list;
    return check_revoke_list(m_list, (const void *)m_buf);
}

long loader::check_old_version(authenticator *a)
{
    auth_app_info app;
    auth_flags f;

    if (!a)
        return 21;
    if (a->get_app_info(&app))
        return 21;
    if (a->get_control_values(f.v))
        return 21;
    if (app.auth_id == 0x10700003FD000001ULL && has_flags(f, g_flags_old)
        && app.version <= 0x00030054FFFFFFFFULL)
        return 13;
    return 0;
}

long loader::put_answer(authenticator *a, const app_args *args)
{
    auth_app_info app;
    auth_flags f;
    auth_flags state;
    u64 answer;
    long r;

    if (!a || !args)
        return 21;
    if (args->a7[7] & 0xF)
        return 21;
    memset(&app, 0, sizeof app);
    memset(&f, 0, sizeof f);
    memset(&state, 0, sizeof state);
    r = a->get_app_info(&app);
    if (r)
        return r;
    r = a->get_control_values(f.v);
    if (r)
        return r;
    r = get_state(a, &state);
    if (r)
        return r;
    answer = 0;
    bool deny = !is_kind_d(&app, f, state) || (unsigned short)(a->key_revision() - 13) <= 17;
    if (!deny)
        answer = 1;
    memcpy((void *)0x3E000, &answer, 8);
    mfc_issue(0x3E000, args->a7[7], 8, 5, 0, MFC_PUT_CMD);
    while (!mfc_tag_done(5))
        ;
    return r;
}

long loader::decrypt_args(const unsigned char *key)
{
    vec_uchar16 rk[11];

    if (aes_set_decrypt_key(rk, key, 128) != 10)
        return 21;
    aes_decrypt_block((vec_uchar16 *)&m_args.a7[5], (const vec_uchar16 *)&m_args.a7[5], rk, 10);
    return 0;
}

long loader::pick_args_key(unsigned short index, bool flag, keyset_ref *ref)
{
    if (!ref)
        return 46;
    if (index == 0 || !flag) {
        ref->args_key = g_args_key_a;
        ref->args_iv = g_args_iv_a;
    } else {
        ref->args_key = g_args_key_b;
        ref->args_iv = g_args_iv_b;
    }
    return decrypt_args(ref->args_key);
}

long loader::find_keyset(unsigned int type, unsigned int value, unsigned short index,
                         unsigned int x, bool flag, keyset_ref *ref)
{
    const app_keyset *table;
    const app_keyset *ks;
    unsigned int i;
    long r;

    if (!ref)
        return 21;
    r = check_type_value(type, value);
    if (r)
        return r;
    i = ~0U;
    r = lookup(type, index, &i);
    if (r)
        return r;
    switch (type) {
    case 4:
        table = g_keysets_a;
        if (is_129_130(x, flag))
            table = g_keysets_b;
        break;
    case 8:
        table = g_keysets2_a;
        if (is_129_130(x, flag))
            table = g_keysets2_b;
        break;
    case 7:
        table = g_keysets_7;
        break;
    default:
        return 28;
    }
    ks = &table[i];
    ref->key = ks->key;
    ref->iv = ks->iv;
    ref->pub = ks->pub;
    ref->curve = &ks->curve;
    if (type == 8)
        return pick_args_key(index, flag, ref);
    ref->args_key = 0;
    ref->args_iv = 0;
    return 0;
}

long loader::read_type(unsigned int *type, authenticator *a)
{
    long r;

    if (!a)
        return 21;
    if (m_args.a0[1] & 0xF)
        return 23;
    mfc_issue(0x3E000, m_args.a0[1], 144, 1, 1, MFC_GET_CMD);
    while (!mfc_tag_done(1))
        ;
    r = a->set_header(1, (const void *)0x3E000);
    if (r)
        return r;
    *type = ((const auth_app_info *)0x3E070)->type;
    return 0;
}

long loader::load_image(unsigned int type, authenticator *a, const keyset_ref *ref,
                        unsigned int revision)
{
    long r;

    if (!a)
        return 21;
    if (m_args.a0[1] & 0xF)
        return 23;
    a->set_offset(m_args.a0[1]);
    tagged_dma_buffer buf(0);
    dma_buffer *bufs[1];
    buf.set_buffer(0x3E000, 8192);
    bufs[0] = &buf;
    r = a->load_header(type, ref->key, ref->iv, ref->pub, ref->curve, bufs, 1, &m_args.a7[5],
                       ref->args_iv, revision == 129 || revision == 130 || revision == 160);
    return r;
}

long loader::open_image()
{
    long r;

    reset();
    m_auth = new (g_auth_area) authenticator;
    r = read_type(&m_type, m_auth);
    if (r)
        return r;
    r = find_keyset(m_type, m_args.kind & 0xFFFF, m_auth->key_revision(), m_revision,
                    flag_set(), &m_keys);
    if (r)
        return r;
    return load_image(m_type, m_auth, &m_keys, m_revision);
}

long loader::check_revoked(unsigned int revision, revoke_list *list, authenticator *a)
{
    auth_app_info app;
    long r;

    if (!a || !list)
        return 21;
    if (a->flag_1b9() && revision != 129 && revision != 130 && revision != 160)
        return 41;
    if (a->get_app_info(&app))
        return 21;
    u64 req_class = m_args.a0[0] >> 60;
    u64 app_class = app.auth_id >> 60;
    u64 arg_class = m_arg >> 60;
    if (app_class != 1 || req_class != 1)
        return 37;
    if (arg_class != 1)
        return 37;
    r = a->check_auth_id_class(m_args.a0[0], m_arg);
    if (r)
        return r;
    if (list->is_revoked(app.type, app.auth_id, app.version) != 1)
        return 13;
    if (!(m_args.a7[4] & 2))
        r = a->check_issuer(m_args.a0[0], m_arg);
    return r;
}

long loader::check_image()
{
    long r;

    r = check_revoked(m_revision, m_list, m_auth);
    if (r)
        return r;
    r = check_state(m_revision, m_auth, &m_result);
    if (r)
        return r;
    r = check_control_digest(m_auth);
    if (r)
        return r;
    r = check_version_limit(0x0004009300000000ULL, m_auth);
    if (r)
        return r;
    return check_old_version(m_auth);
}

static inline bool new_key_revision(unsigned short rev)
{
    if (rev >= 13 && rev <= 30)
        return true;
    return false;
}

long loader::check_kind()
{
    auth_app_info app;
    unsigned char digest[20];
    auth_flags f;
    auth_flags state;
    u64 info[4];
    long r;

    memset(&app, 0, sizeof app);
    memset(digest, 0, sizeof digest);
    memset(&f, 0, sizeof f);
    memset(&state, 0, sizeof state);
    memset(info, 0, sizeof info);
    if (!m_auth)
        return 21;
    r = m_auth->get_app_info(&app);
    if (r)
        return r;
    r = m_auth->get_control_values(f.v);
    if (r)
        return r;
    r = get_state(m_auth, &state);
    if (r)
        return r;
    unsigned short rev = m_auth->key_revision();
    if (new_key_revision(rev))
        return r;
    if (m_auth->flag_1b9()) {
        if (m_revision == 129 || m_revision == 130 || m_revision == 160)
            return r;
        return 41;
    }
    if (rev == 0 && keyset_pub_is_zero())
        return r;
    r = m_auth->get_section_info((unsigned int)m_args.a0[3], info);
    if (r)
        return 21;
    if (check_flags_a(app.auth_id, f, state)) {
        if (info[1] == 0)
            return 28;
        r = get_digest(digest, 20);
        if (r)
            return r;
        r = check_known(f, state, digest);
        if (r)
            return 52;
        return r;
    }
    if (is_kind_b(&app, f, state)) {
        if (info[1] == 0)
            return 28;
        r = get_digest(digest, 20);
        if (r)
            return r;
        r = check_known(f, state, digest);
        if (r)
            return 53;
        return r;
    }
    if (is_kind_c(&app, f, state))
        return r;
    if (is_kind_d(&app, f, state))
        return r;
    return 28;
}

long loader::check_hash()
{
    long r = 46;
    hash_reader h;
    tagged_dma_buffer b0(0);
    tagged_dma_buffer b1(1);

    b0.set_buffer(0x3E000, 4096);
    b1.set_buffer(0x3F000, 4096);
    dma_buffer *bufs[2] = { &b0, &b1 };
    if (m_240.ea == 0 || m_240.size == 0)
        return r;
    if (h.open(m_240.ea, m_240.size, bufs, 2, g_hash_key, 64))
        return r;
    long e = h.read();
    if (e)
        return r;
    if (h.verify(m_240.digest, 20))
        return 51;
    return e;
}

long loader::section_extent(unsigned int index, unsigned int kind, u64 *room, u64 *size)
{
    u64 info[4];
    long r = 21;

    if (!m_auth)
        return r;
    if (m_auth->is_elf64()) {
        Elf64_Ehdr eh;
        Elf64_Phdr ph;

        r = m_auth->read_elf_header(&eh);
        if (r)
            return 29;
        if (!(eh.e_type == 2 || eh.e_type == 3) && eh.e_type != 0xFFA4)
            return 29;
        switch (kind) {
        case 1:
            *room = eh.e_shentsize * eh.e_shnum;
            *size = eh.e_shentsize * eh.e_shnum;
            return r;
        case 0:
            r = m_auth->read_program_header(index, &ph);
            if (r)
                return 29;
            if (ph.p_filesz == 0) {
                *room = ph.p_filesz;
                *size = ph.p_filesz;
                return r;
            }
            if (ph.p_type == 1 && ph.p_align > 1
                && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
                return 29;
            *room = ph.p_filesz;
            break;
        default:
            return 28;
        }
    } else if (m_auth->is_elf32()) {
        Elf32_Ehdr eh;
        Elf32_Phdr ph;

        r = m_auth->read_elf_header(&eh);
        if (r)
            return 29;
        if (!(eh.e_type == 2 || eh.e_type == 3) && eh.e_type != 0xFFA4)
            return 29;
        switch (kind) {
        case 1:
            *room = eh.e_shentsize * eh.e_shnum;
            *size = eh.e_shentsize * eh.e_shnum;
            return r;
        case 0:
            r = m_auth->read_program_header(index, &ph);
            if (r)
                return 29;
            if (ph.p_filesz == 0) {
                *room = 0;
                *size = 0;
                return r;
            }
            if (ph.p_type == 1 && ph.p_align > 1
                && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
                return 29;
            *room = ph.p_filesz;
            break;
        default:
            return 28;
        }
    } else
        return 29;
    memset(info, 0, sizeof info);
    r = m_auth->get_section_info(index, info);
    if (r)
        return r;
    *size = info[1];
    return r;
}

long loader::load_section()
{
    u64 room, size;
    segment_loader *sl;
    long r = 21;

    if (!m_auth)
        return r;
    u64 sec = m_args.a0[3];
    room = 0;
    size = 0;
    switch ((unsigned int)(sec >> 32)) {
    case 1:
        r = m_auth->select_section(1, 3);
        break;
    case 0:
        r = m_auth->select_section(2, (unsigned int)sec);
        break;
    default:
        return 28;
    }
    if (r)
        return r;
    r = section_extent((unsigned int)m_args.a0[3], sec >> 32, &room, &size);
    if (r)
        return r;
    if (room == 0 || size == 0)
        return r;
    tagged_dma_buffer b0(0);
    tagged_dma_buffer b1(1);
    tagged_dma_buffer b2(2);
    tagged_dma_buffer b3(3);
    b0.set_buffer(0x3E000, 2048);
    b1.set_buffer(0x3E800, 2048);
    b2.set_buffer(0x3F000, 2048);
    b3.set_buffer(0x3F800, 2048);
    dma_buffer *in[2] = { &b0, &b1 };
    dma_buffer *out[2] = { &b2, &b3 };
    if (m_232[3])
        sl = new (g_stream_area) stream_loader;
    else
        sl = new (g_direct_area) direct_loader;
    r = sl->open(m_auth, 0);
    if (r == 0) {
        r = sl->load(m_args.a0[2], size, m_args.a0[4], room, in, 2, out, 2);
        sl->close();
    }
    return r;
}

struct decrypt_request {
    u64 pad;
    unsigned int mac_flags;
    unsigned int cipher_flags;
    u64 src;
    u64 len;
    u64 mac_ea;
    u64 state_ea;
    u64 kind;
    unsigned int index;
    unsigned int pad2;
    unsigned char mac_key[16];
    unsigned char key[16];
    unsigned char iv[16];
    unsigned char pad3[16];
} __attribute__((aligned(16)));

long loader::load_b()
{
    const decrypt_request *req = (const decrypt_request *)&m_args;
    unsigned char id[16];
    long r;

    memcpy(g_key, (const void *)0, 32);
    memcpy(g_iv, (const void *)32, 16);
    open_params(&m_revision, id);
    tagged_dma_buffer b0(0);
    tagged_dma_buffer b1(1);
    tagged_dma_buffer b2(2);
    tagged_dma_buffer b3(3);
    b0.set_buffer(0x3E000, 2048);
    b1.set_buffer(0x3E800, 2048);
    b2.set_buffer(0x3F000, 2048);
    b3.set_buffer(0x3F800, 2048);
    dma_buffer *in[2] = { &b0, &b1 };
    dma_buffer *out[2] = { &b2, &b3 };
    m_236 = new (g_9b0) data_decryptor;
    u64 mode = (req->kind & 0xFF000000) >> 24;
    if (mode == 2) {
        if (req->state_ea == 0) {
            r = 46;
            goto fail;
        }
        switch ((req->kind & 0x00FF0000) >> 16) {
        case 1:
            r = m_236->open(req->mac_key, req->key, req->iv, req->mac_flags, req->cipher_flags,
                            req->state_ea, req->index);
            if (r)
                goto fail;
            r = m_236->check(m_revision, req->mac_flags, req->cipher_flags);
            break;
        case 2:
            r = m_236->process(req->src, req->len, req->state_ea, in, 2, out, 2);
            break;
        case 3:
            r = m_236->finish(req->mac_ea, req->state_ea);
            break;
        default:
            r = 46;
        }
    } else {
        if (mode != 0 || req->state_ea != 0) {
            r = 46;
            goto fail;
        }
        switch ((req->kind & 0x00FF0000) >> 16) {
        case 1:
            r = m_236->open(req->mac_key, req->key, req->iv, req->mac_flags, req->cipher_flags,
                            0, req->index);
            if (r)
                goto fail;
            r = m_236->check(m_revision, req->mac_flags, req->cipher_flags);
            if (r)
                goto fail;
            r = m_236->process(req->src, req->len, req->state_ea, in, 2, out, 2);
            if (r)
                goto fail;
            r = m_236->finish(req->mac_ea, req->state_ea);
            break;
        default:
            r = 46;
        }
    }
    m_236->reset();
    m_236 = 0;
    if (r)
        return r;
    return 60;
fail:
    m_236->reset();
    m_236 = 0;
    return r;
}

long loader::load_a()
{
    auth_app_info app;
    Elf32_Ehdr eh32;
    Elf64_Ehdr eh64;
    unsigned short type;
    long r;

    r = read_mode();
    if (r)
        return r;
    if (!m_232[1]) {
        r = prepare();
        if (r)
            return r;
        m_232[1] = 1;
    }
    if (!m_232[0]) {
        r = open_image();
        if (r) {
            long e = check_hash();
            if (e)
                return e;
            return r;
        }
        if (m_auth->get_app_info(&app))
            return 21;
        if (m_auth->is_elf64()) {
            if (m_auth->read_elf_header(&eh64))
                return 29;
            type = eh64.e_type;
        } else {
            if (m_auth->read_elf_header(&eh32))
                return 29;
            type = eh32.e_type;
        }
        if ((app.type == 4 || app.type == 8) && type == 2) {
            r = check_hash();
            if (r)
                return r;
        }
        r = check_image();
        if (r)
            return r;
        m_232[0] = 1;
    }
    put_result();
    if (put_answer(m_auth, &m_args))
        return 21;
    if (m_232[3] && m_232[0]) {
        if (mbox_out_intr_write(7))
            return 21;
    }
    r = load_section();
    if (r)
        return r;
    r = check_kind();
    if (r)
        return r;
    u64 mode = (m_args.kind & 0xFF000000) >> 24;
    if (mode == 0)
        return 10;
    if (mode > 2)
        return 39;
    return 44;
}
