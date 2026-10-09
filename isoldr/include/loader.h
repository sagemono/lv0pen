#ifndef ISOLDR_LOADER_H
#define ISOLDR_LOADER_H

#include "types.h"

class revoke_list;
class authenticator;

struct spu_reg {
    unsigned char b[16];
};

struct isoldr_request {
    u64 auth_id_a;
    u64 auth_id;
    u64 image;
    u64 arg[4];
    u64 arg_38[3];
} __attribute__((aligned(16)));

struct isoldr_qa_request {
    u64 skip;
    u64 reserved;
    unsigned char token[80];
    unsigned char sig[48];
} __attribute__((aligned(16)));

struct isoldr_keyset {
    unsigned char key[32];
    unsigned char iv[16];
    unsigned char pub[40];
    unsigned int curve;
} __attribute__((aligned(16)));

class loader {
public:
    loader();

    long run(u64 *a0, u64 *a1, u64 *a2, u64 *a3,
             spu_reg *r0, spu_reg *r1, spu_reg *r2, spu_reg *r3,
             spu_reg *r4, spu_reg *r5, spu_reg *r6, spu_reg *r7,
             spu_reg *r8, spu_reg *r9, spu_reg *r10, spu_reg *r11,
             spu_reg *r12, spu_reg *r13, spu_reg *r14, spu_reg *r15,
             void (**entry)(void), u64 *a4);

    long read_request();

    long open_params();

    long check_qa_flag();

    long select_keyset(u16 revision);

    long check_revoke_list();

    long load_header();

    long load_segments(void (**entry)(void));

    long load(void (**entry)(void));

    void load_state();

    void apply_qa_flag();

    long check_revision();

    bool takes_keys();

    long export_keys(spu_reg *k0, spu_reg *k1, spu_reg *k2);

    long export_state(spu_reg *r0, spu_reg *r1, spu_reg *r2, spu_reg *r3,
                      spu_reg *r4, spu_reg *r5, spu_reg *r6, spu_reg *r7,
                      spu_reg *r8, spu_reg *r9, spu_reg *r10, spu_reg *r11,
                      spu_reg *r12, spu_reg *r13, spu_reg *r14, spu_reg *r15);

    isoldr_request m_req;
    unsigned int m_buf;
    authenticator *m_auth;
    revoke_list *m_list;
    u32 m_revision;
    unsigned char m_id[16];
    bool m_qa;
    u64 m_state[4];
    const unsigned char *m_key;
    const unsigned char *m_iv;
    const void *m_160;
    const unsigned char *m_pub;
    const unsigned int *m_curve;
} __attribute__((aligned(16)));

extern loader g_loader;

#endif
