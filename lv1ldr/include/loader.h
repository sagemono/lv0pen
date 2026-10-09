#ifndef LV1LDR_LOADER_H
#define LV1LDR_LOADER_H

#include <spu_intrinsics.h>
#include "types.h"

class authenticator;

struct lv1ldr_request {
    u64 image;
    u64 arg_08[10];
    u64 token_len;
    unsigned char token[16];
    u64 arg_70;
    u64 arg_78;
    u64 arg_80;
    u64 arg_88;
} __attribute__((aligned(16)));

class qa_flag_reader {
public:
    void init();

    long check(u32 revision, const unsigned char *token, unsigned char *out,
               const unsigned char *idps, const unsigned char *sig);
};

struct device_id {
    u32 w0;
    u16 device;
    u16 model;
    u32 w2;
    u32 w3;
} __attribute__((aligned(16)));

class loader {
public:
    loader();

    long run();

    long read_request();

    long init_device_1(u64 ea, u64 io, u64 flags, const device_id *id,
                       const unsigned char *key, const vec_uchar16 *iv,
                       const vec_uchar16 *k0, const vec_uchar16 *k1,
                       const vec_uchar16 *k2, const vec_uchar16 *k3,
                       const unsigned char *x0, const unsigned char *x1,
                       const unsigned char *x2);

    void init_log(unsigned int flags);

    long load_segments();

    long auth_request(void);

    long check_request_token(void);

    authenticator *m_auth;
    lv1ldr_request m_req;
};

extern loader g_loader;

#endif
