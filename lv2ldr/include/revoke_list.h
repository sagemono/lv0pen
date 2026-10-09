#ifndef LV2LDR_REVOKE_LIST_H
#define LV2LDR_REVOKE_LIST_H

#include "types.h"
#include "sce.h"

struct revoke_list_header {
    u32 type;
    u32 check;
    u64 version;
    u32 count;
    u32 count2;
} __attribute__((aligned(16)));

struct revoke_pair {
    u64 auth_id;
    u64 version;
} __attribute__((aligned(16)));

struct revoke_rule {
    u32 type;
    u32 check;
    u64 version;
    u64 value;
    u64 mask;
} __attribute__((aligned(16)));

typedef sce_header sce_header16 __attribute__((aligned(16)));

class revoke_list {
public:
    void init();

    long load(const void *src);

    long open(const unsigned char *key, const unsigned char *iv,
              const unsigned char *pub, const unsigned int *curve);

    long parse();

    long check_version(u64 version);

    long is_revoked(u32 type, u64 auth_id, u64 version);

    long scan_pairs(u64 auth_id, u64 version);
    long scan_pairs2(u64 auth_id, u64 version);
    long scan_rules(u32 type, u64 auth_id, u64 version);

    long check_header(const revoke_list_header *h);

    long check_plain(const auth_section *s);
    long check_encrypted(const auth_section *s);

    const sce_header16 *m_sce;
    const auth_meta *m_meta;
    const auth_section *m_section;
    const unsigned char *m_keys;
    const revoke_pair *m_pairs;
    const revoke_pair *m_pairs2;
    const revoke_rule *m_rules;
    const revoke_list_header *m_body;
    bool m_opened;
    bool m_loaded;
    sce_image m_img;
};

extern revoke_list g_revoke_list;

#endif
