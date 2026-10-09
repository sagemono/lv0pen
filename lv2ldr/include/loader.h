#ifndef LV2LDR_LOADER_H
#define LV2LDR_LOADER_H

#include "types.h"

class revoke_list;
class authenticator;

struct lv2ldr_request {
    u64 auth_id;
    u64 image;
    u64 arg_10;
    u64 arg_18;
    u64 arg_20[6];
} __attribute__((aligned(16)));

class loader {
public:
    loader();

    long run();

    long read_request();

    long read_file();

    long check_revoke_list();

    long load_header();

    long load_segments();

    long hash_segment();

    lv2ldr_request m_req;
    lv2ldr_request m_file;
    unsigned int m_buf;
    revoke_list *m_list;
    authenticator *m_auth;
};

extern loader g_loader;

#endif
