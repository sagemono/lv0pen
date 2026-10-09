#include "revoke_list.h"
#include "cipher.h"
#include "verifier.h"
#include "util.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

extern unsigned char g_revoke_buf[4096];
extern unsigned char g_revoke_digest[20];

void revoke_list::init()
{
    m_sce = 0;
    m_meta = 0;
    m_section = 0;
    m_keys = 0;
    m_pairs = 0;
    m_pairs2 = 0;
    m_rules = 0;
    m_body = 0;
    m_opened = false;
    m_loaded = false;
    m_img.m_sce = (const sce_header *)g_revoke_buf;
    m_img.m_size = sizeof g_revoke_buf;
}

long revoke_list::check_header(const revoke_list_header *h)
{
    if (h == 0)
        return 21;
    if (h->check != 1)
        return 22;
    if (h->type != 3 && h->type != 4)
        return 22;
    return 0;
}

long revoke_list::check_version(u64 version)
{
    if (!m_opened)
        return -2;
    if (m_body == 0)
        return -2;
    return m_body->version != version ? 13 : 0;
}

long revoke_list::check_encrypted(const auth_section *s)
{
    if (s->encrypted != 3 || s->hashed != 2)
        return -1;
    g_aes128_ctr.set_key(m_keys + s->key * 16, m_keys + s->iv * 16);
    if (g_aes128_ctr.decrypt((const void *)((u32)m_img.m_sce + (u32)s->offset), (u32)s->size,
                             (void *)((u32)m_img.m_sce + (u32)s->offset)))
        return -1;
    if (g_hmac.digest((const unsigned char *)m_img.m_sce + (u32)s->offset, (u32)s->size,
                      g_revoke_digest, m_keys + s->digest * 16 + 32, 64))
        return -1;
    if (bytes_differ(g_revoke_digest, m_keys + s->digest * 16, 20))
        return -1;
    return 0;
}

long revoke_list::check_plain(const auth_section *s)
{
    if (s->encrypted != 1 || s->hashed != 2)
        return -1;
    if (g_hmac.digest((const unsigned char *)m_img.m_sce + (u32)s->offset, (u32)s->size,
                      g_revoke_digest, m_keys + s->digest * 16 + 32, 64))
        return -1;
    if (bytes_differ(g_revoke_digest, m_keys + s->digest * 16, 20))
        return -1;
    return 0;
}

long revoke_list::parse()
{
    long r;
    u32 i;

    if (!m_opened)
        return -2;
    for (i = 0; i < m_meta->section_count; i++) {
        switch (m_section->type) {
        case 1:
            if (check_plain(m_section))
                return 15;
            m_body = (const revoke_list_header *)((const unsigned char *)m_img.m_sce
                                                  + (u32)m_sce->header_len);
            if (check_header(m_body))
                return 22;
            break;
        case 2:
            r = check_encrypted(m_section);
            if (r)
                return 15;
            if (m_body->type == 3) {
                m_pairs = (const revoke_pair *)((const unsigned char *)m_img.m_sce
                                                + (u32)m_sce->header_len + 32);
                m_pairs2 = (const revoke_pair *)((const unsigned char *)m_img.m_sce
                                                 + (u32)m_sce->header_len
                                                 + m_body->count * 16 + 32);
                m_rules = 0;
            } else {
                m_rules = (const revoke_rule *)((const unsigned char *)m_img.m_sce
                                                + (u32)m_sce->header_len + 32);
                m_pairs = 0;
                m_pairs2 = 0;
            }
            break;
        default:
            return 22;
        }
        m_section++;
    }
    return 0;
}

long revoke_list::open(const unsigned char *key, const unsigned char *iv,
                       const unsigned char *pub, const unsigned int *curve)
{
    long r = -4;

    if (!m_loaded)
        return r;
    m_sce = m_img.m_sce;
    if (check_sce_header((const vec_uchar16 *)m_img.m_sce, 0, 2, m_img.m_size))
        return 22;
    g_aes256_cbc.set_key(key, iv);
    if (g_aes256_cbc.decrypt((const void *)((u32)m_img.m_sce + 32), 64, (void *)((u32)m_img.m_sce + 32)))
        return 15;
    g_aes128_ctr.set_key((const unsigned char *)((u32)m_img.m_sce + 32),
                         (const unsigned char *)((u32)m_img.m_sce + 64));
    if (g_aes128_ctr.decrypt((const void *)((u32)m_img.m_sce + 96), m_sce->header_len - 96,
                             (void *)((u32)m_img.m_sce + 96)))
        return 15;
    m_meta = (const auth_meta *)((u32)m_img.m_sce + 96);
    if (check_meta_header((const vec_uchar16 *)m_sce, m_meta, 1))
        return 22;
    if (check_sections(&m_img))
        return 22;
    r = 15;
    ecdsa_verifier v(pub, *curve);
    if (v.verify((const unsigned char *)m_img.m_sce, m_meta->signed_len,
                 (const unsigned char *)((u32)m_img.m_sce + (u32)m_meta->signed_len)) == 0) {
        m_section = (const auth_section *)((u32)m_img.m_sce + 128);
        m_keys = (const unsigned char *)m_img.m_sce + 128 + m_meta->section_count * 48;
        m_opened = true;
        r = 0;
    }
    return r;
}

long revoke_list::load(const void *src)
{
    memcpy(g_revoke_buf, src, sizeof g_revoke_buf);
    m_loaded = true;
    return 0;
}
