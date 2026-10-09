#ifndef APPLDR_LOADER_H
#define APPLDR_LOADER_H

#include "types.h"
#include "auth_flags.h"

struct auth_app_info;

class authenticator;
class data_decryptor;

struct app_request {
    u64 args_ea;
    u64 arg;
    u64 r10[7];
    u64 version;
} __attribute__((aligned(16)));

struct app_args {
    u64 a0[5];
    u64 result_ea;
    u64 kind;
    u64 a7[9];
} __attribute__((aligned(16)));

struct qa_token {
    u64 none;
    u64 pad;
    unsigned char body[80];
    unsigned char sig[48];
} __attribute__((aligned(16)));

struct keyset_ref {
    const unsigned char *key;
    const unsigned char *iv;
    const unsigned char *pub;
    const unsigned int *curve;
    const unsigned char *args_key;
    const unsigned char *args_iv;
} __attribute__((aligned(16)));

class revoke_list;

class loader {
public:
    loader();

    long run();

    void reset();

    long read_request();

    void fetch(u64 ea, void *dst);

    void put_result();

    long load_a();

    bool keyset_pub_is_zero();

    bool check_flags_a(u64 auth_id, const auth_flags &f, const auth_flags &g);
    bool is_kind_b(const auth_app_info *app, const auth_flags &f, const auth_flags &g);
    bool is_kind_c(const auth_app_info *app, const auth_flags &f, const auth_flags &g);
    bool is_kind_d(const auth_app_info *app, const auth_flags &f, const auth_flags &g);

    bool has_flags(const auth_flags &f, const auth_flags &want);

    long unpack3(const unsigned int *w, auth_flags *out);
    long unpack4(const unsigned int *w, auth_flags *out);

    long check_known(const auth_flags &f, const auth_flags &g, const unsigned char *digest);

    long check_type_value(unsigned int type, unsigned int v);

    long lookup_a(unsigned short i, unsigned int *out);
    long lookup_b(unsigned short i, unsigned int *out);
    long lookup(unsigned int type, unsigned short i, unsigned int *out);

    bool is_129_130(unsigned int x, bool flag);

    long get_digest(unsigned char *out, unsigned int size);

    long check_version_limit(u64 version, authenticator *a);
    u64 version_number(u64 version);

    long read_mode();

    void override_state(auth_flags *state);

    bool flag_set();

    long check_control_digest(authenticator *a);

    long get_state(authenticator *a, auth_flags *out);

    long check_type_bit(unsigned int type, const auth_flags *state);

    long check_state(unsigned int type, authenticator *a, auth_flags *state);

    long open_params(unsigned int *revision, unsigned char *id);

    long read_qa_flag(unsigned int revision, const unsigned char *id,
                      const qa_token *token, unsigned char *flag, unsigned char *have);

    long check_revoke_list(revoke_list *list, const void *src);

    long prepare();

    long check_old_version(authenticator *a);

    long put_answer(authenticator *a, const app_args *args);

    long decrypt_args(const unsigned char *key);

    long pick_args_key(unsigned short index, bool flag, keyset_ref *ref);

    long find_keyset(unsigned int type, unsigned int value, unsigned short index,
                     unsigned int x, bool flag, keyset_ref *ref);

    long read_type(unsigned int *type, authenticator *a);

    long load_image(unsigned int type, authenticator *a, const keyset_ref *ref,
                    unsigned int revision);

    long open_image();

    long check_revoked(unsigned int revision, revoke_list *list, authenticator *a);

    long check_image();

    long check_kind();

    long check_hash();

    long section_extent(unsigned int index, unsigned int kind, u64 *room, u64 *size);

    long load_section();

    long load_b();

    u64 m_arg;
    app_args m_args;
    unsigned int m_buf;
    revoke_list *m_list;
    authenticator *m_auth;
    auth_flags m_result __attribute__((aligned(16)));
    keyset_ref m_keys;
    unsigned int m_revision;
    unsigned int m_type;
    unsigned char m_232[4];
    data_decryptor *m_236;
    struct {
        u64 ea;
        u64 size;
        unsigned char digest[20];
        unsigned char pad[12];
    } m_240 __attribute__((aligned(16)));
} __attribute__((aligned(16)));

extern loader g_loader;

#endif
