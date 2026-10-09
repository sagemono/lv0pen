#include "loader.h"
#include "auth.h"
#include "cipher.h"
#include "params.h"
#include "mmio.h"
#include "util.h"

inline void *operator new(__SIZE_TYPE__, void *p) throw() { return p; }

extern unsigned char g_auth[];

int ch72_get_all_ones(void);
int ch72_put_request(u32 cmd, const unsigned char *payload);
int ch72_unwrap_key(void *out, const void *in, int len);
int ch72_unwrap_iv(void *out, const void *in, int len);
long ch64_request(void);
void wb_crypt(const u32 *rk, const unsigned char *in, u32 len,
              const unsigned char *data, unsigned char *out);

extern const u32 wb_aes_rk[44];
extern const unsigned char ch72_payload_seed[16];
extern const unsigned char ch72_payload_mask[16];

extern const u32 ch72_run_cmd;
extern const u64 params_max_size;

extern const unsigned char params_wrap_0[64];
extern const unsigned char params_wrap_1[64];

extern const unsigned char storage_key[32];
extern const unsigned char storage_iv[16];
extern const unsigned char storage_wrapped_0[32];
extern const unsigned char storage_wrapped_1[32];
extern const unsigned char storage_wrapped_2[32];
extern const unsigned char storage_const_0[32];
extern const unsigned char storage_const_1[32];
extern const unsigned char storage_const_2[32];
extern const unsigned char storage_const_3[32];

struct qa_flag_bit {
    u32 index;
    unsigned char mask;
} __attribute__((aligned(16)));

extern const qa_flag_bit qa_flag_bits[9];

struct body_view {
    u32 pad;
    unsigned short revision;
} __attribute__((aligned(16)));

loader::loader()
    : m_auth(0), m_req()
{
}

long loader::read_request(void)
{
    while (*(volatile unsigned int *)0x3E000 != 0xFF)
        ;
    m_req = *(const lv1ldr_request *)0x3E800;
    if (m_req.image & 15)
        return 23;
    return m_req.arg_70 < m_req.arg_78 ? 0 : 12;
}

long loader::run()
{
    unsigned char payload[16];
    long rc;

    if (!ch72_get_all_ones())
        return 48;
    rc = read_request();
    if (rc)
        return rc;
    rc = check_request_token();
    if (rc)
        return rc;
    wb_crypt(wb_aes_rk, ch72_payload_seed, 16, ch72_payload_mask, payload);
    if (ch72_put_request(ch72_run_cmd, payload))
        return 48;
    init_log(m_req.arg_08[2] >> 32);
    if (m_req.arg_08[2] & 2)
        return 10;

    unsigned char key[32];
    unsigned char iv[16];

    memcpy(key, (const void *)0, 32);
    memcpy(iv, (const void *)32, 16);

    aes256_cbc_encryptor enc;
    unsigned char wrap1[64];
    unsigned char wrap0[64];
    u32 revision;
    unsigned char iv2[16];
    unsigned char key1[32];
    unsigned char body[192];
    unsigned char s0[32];
    unsigned char id[16];
    unsigned char s2[32];
    unsigned char iv1[16];
    unsigned char key2[32];
    unsigned char staged[2144];
    param_block p;
    unsigned char flag[32];
    unsigned char s1[32];
    unsigned char token[128];
    qa_flag_reader q;
    u64 lo, hi;
    u32 i;

    enc.set_key(key, (const cipher_iv *)iv);
    if (enc.encrypt(params_wrap_0, 64, wrap0))
        return 21;
    if (enc.encrypt(params_wrap_1, 64, wrap1))
        return 21;
    revision = 0xFFFF;
    memset(id, 255, 16);
    const body_view *bv = (const body_view *)body;
    memset((void *)bv, 0, 192);
    if (m_req.arg_08[3] != 0 && m_req.arg_08[4] <= params_max_size) {
        memcpy(key1, wrap0 + 32, 32);
        memcpy(iv1, wrap0 + 16, 16);
        memcpy(key2, wrap1 + 32, 32);
        memcpy(iv2, wrap1 + 16, 16);
        copy_qwords_from_mmio(m_req.arg_08[3], (char *)staged,
                              (u32)(m_req.arg_08[4] + 15) & ~15U);
        p.init();
        if (p.open(staged, m_req.arg_08[4], key1, iv1, key2, iv2, &revision, id, body)) {
            revision = 0xFFFF;
            memset(id, -1, 16);
        }
        if (p.verify(body, m_req.arg_08[4]) == 0)
            goto have_params;
    }
    if ((u32)(bv->revision - 131) > 28)
        return 40;
have_params:
    enc.set_key(storage_key, (const cipher_iv *)storage_iv);
    if (enc.encrypt(storage_wrapped_0, 32, s0))
        return 21;
    if (enc.encrypt(storage_wrapped_1, 32, s1))
        return 21;
    if (enc.encrypt(storage_wrapped_2, 32, s2))
        return 21;
    rc = init_device_1(m_req.arg_08[0], m_req.arg_08[1], m_req.arg_08[2],
                       (const device_id *)id, key, (const vec_uchar16 *)iv,
                       (const vec_uchar16 *)storage_const_0, (const vec_uchar16 *)storage_const_1,
                       (const vec_uchar16 *)storage_const_2, (const vec_uchar16 *)storage_const_3,
                       s0, s1, s2);
    if (rc)
        return rc;

    if (revision != 0xFFFF
        && m_req.arg_08[5] != 0 && m_req.arg_08[6] != 0 && m_req.arg_08[6] <= 128) {
        copy_qwords_from_mmio(m_req.arg_08[5], (char *)token,
                              (u32)(m_req.arg_08[6] + 15) & ~15U);
        q.init();
        if (q.check(revision, token, flag, id, token + 80))
            memset(flag, 0, 32);
        else
            for (i = 0; i != 32; i++) {
                if (i == qa_flag_bits[0].index && (revision == 129 || revision == 130))
                    flag[i] &= qa_flag_bits[0].mask;
                else if (i == qa_flag_bits[1].index)
                    flag[i] &= qa_flag_bits[1].mask | qa_flag_bits[2].mask
                             | qa_flag_bits[3].mask | qa_flag_bits[4].mask
                             | qa_flag_bits[5].mask | qa_flag_bits[6].mask
                             | qa_flag_bits[7].mask | qa_flag_bits[8].mask;
                else
                    flag[i] = 0;
            }
        copy_qwords_to_mmio(m_req.arg_08[7], (char *)flag, 32);
    }

    if (revision == 129)
        lo = 0;
    else
        lo = 1;
    switch (revision) {
    case 129:
        if (*(const unsigned short *)(id + 4) == 129 && *(const unsigned short *)(id + 6) > 8)
            hi = 0x0400000000000000ULL;
        else
            hi = 0x0100000000000000ULL;
        break;
    case 130:
        hi = 0x0200000000000000ULL;
        break;
    case 160:
        hi = 0x0300000000000000ULL;
        break;
    case 131 ... 143:
        hi = 0x1000000000000000ULL;
        break;
    default:
        hi = 0;
        break;
    }
    write64(m_req.arg_08[8], lo | hi);
    write64(m_req.arg_08[9], (u64)*(const unsigned short *)(id + 4) << 16
                             | *(const unsigned short *)(id + 6));

    if ((m_req.arg_08[2] & 1) == 0)
        return 10;
    if (ch72_unwrap_key((void *)0x37A80, (const void *)0x37A80, 32))
        return 48;
    if (ch72_unwrap_iv((void *)0x37AA0, (const void *)0x37AA0, 16))
        return 48;
    if (ch64_request())
        return 48;
    m_auth = new (g_auth) authenticator;
    m_auth->set_offset(m_req.image);
    rc = auth_request();
    if (rc)
        return rc;
    if (m_auth->key_revision() != 0)
        return 19;
    rc = load_segments();
    if (rc)
        return rc;
    return 10;
}
