#include "loader.h"
#include "cipher.h"
#include "qa_flag.h"
#include "local_buffer.h"
#include "mailbox.h"
#include "util.h"
#include "../../lv1ldr/include/auth.h"
#include "../../lv1ldr/include/params.h"
#include "../../lv2ldr/include/revoke_list.h"
#include "../../lv2ldr/include/channels.h"
#include <spu_mfcio.h>
#include <new>

extern unsigned char g_ls_key[32];
extern cipher_iv g_ls_iv;

extern isoldr_keyset g_keysets[2];
extern isoldr_keyset g_debug_keysets[2];
extern isoldr_keyset g_keyset_100;
extern unsigned char g_list_key[32] __attribute__((aligned(16)));
extern unsigned char g_list_iv[16] __attribute__((aligned(16)));

extern const unsigned char g_list_pub[40];
extern const unsigned int g_list_curve;

extern const unsigned char g_key_seed[64];
extern const unsigned char g_params_seed[64];

extern unsigned char g_qa_flag[32] __attribute__((aligned(16)));
struct qa_flag_bit {
    unsigned int byte;
    unsigned char mask;
};
extern const qa_flag_bit g_qa_debug_bit;
extern isoldr_qa_request g_qa_request;

extern unsigned char g_auth_area[];

void loader::apply_qa_flag()
{
    unsigned int i;
    u16 v = 0;

    for (i = 0; i < 4; i++) {
        if ((u32)m_state[1] & (1 << i)) {
            if (i == 0) {
                v = ((const u16 *)g_qa_flag)[0];
                break;
            }
            if (i == 1) {
                v = ((const u16 *)g_qa_flag)[1];
                break;
            }
        }
    }
    if (v)
        m_state[2] = (m_state[2] & ~0xFFFFULL) | v;
}

long loader::check_revision()
{
    u32 w = m_state[2];

    switch (m_revision) {
    case 129:
        if (w & 8)
            return 0;
        return 43;
    case 130:
        if (w & 16)
            return 0;
        return 43;
    case 131 ... 143:
        if (w & 32)
            return 0;
        return 43;
    case 160:
        if (w & 64)
            return 0;
        return 43;
    default:
        return 40;
    }
}

long loader::select_keyset(u16 revision)
{
    u16 kind = (revision & 0xF00) >> 8;
    unsigned int i = revision & 0xFF;
    u16 index = i;

    switch (kind) {
    case 0:
        if (index > 1)
            return 19;
        if (m_revision - 129 <= 1 && m_qa
            && (g_qa_flag[g_qa_debug_bit.byte] & g_qa_debug_bit.mask)) {
            m_key = g_debug_keysets[i].key;
            m_iv = g_debug_keysets[i].iv;
            m_pub = g_debug_keysets[i].pub;
            m_curve = &g_debug_keysets[i].curve;
        } else {
            m_key = g_keysets[index].key;
            m_iv = g_keysets[index].iv;
            m_pub = g_keysets[index].pub;
            m_curve = &g_keysets[index].curve;
        }
        break;
    case 1:
        if (index != 0)
            return 19;
        m_key = g_keyset_100.key;
        m_iv = g_keyset_100.iv;
        m_pub = g_keyset_100.pub;
        m_curve = &g_keyset_100.curve;
        break;
    default:
        return 19;
    }
    return 0;
}

long loader::export_keys(spu_reg *k0, spu_reg *k1, spu_reg *k2)
{
    long r = 21;
    aes256_cbc_encryptor enc;
    unsigned char out[64];

    enc.set_key(g_ls_key, &g_ls_iv);
    if (enc.encrypt(g_key_seed, 64, out) == 0) {
        memcpy(k0, out + 32, 16);
        memcpy(k1, out + 48, 16);
        r = 0;
        memcpy(k2, out + 16, 16);
    }
    return r;
}

long loader::read_request()
{
    while (*(volatile unsigned int *)0x3E000 != 0xFF)
        ;
    memcpy(&m_req, (const void *)0x3E800, sizeof m_req);
    if ((m_req.image & 15) != 0 || m_req.image == 0)
        return 23;
    m_buf = 0x3F000;
    memcpy(&g_qa_request, (const void *)0x3EC00, sizeof g_qa_request);
    return 0;
}

bool loader::takes_keys()
{
    auth_app_info app;

    if (!m_auth)
        return true;
    if (m_auth->get_app_info(&app))
        return true;
    return app.auth_id == 0x1070000024000001ULL;
}

void loader::load_state()
{
    if (m_auth->flag_1b9()) {
        m_state[0] = 0x8000000000000000ULL;
        m_state[1] = 0;
        m_state[2] = 0;
        m_state[3] = 0;
    } else if (m_auth->get_state(m_state)) {
        m_state[0] = 0;
        m_state[1] = 0;
        m_state[2] = 0;
        m_state[3] = 0;
    }
}

loader::loader()
    : m_buf(0), m_auth(0), m_list(0), m_revision(0xFFFF), m_id(), m_qa(false), m_state(),
      m_key(0), m_iv(0), m_160(0), m_pub(0), m_curve(0)
{
    memset(&m_req, 0, sizeof m_req);
}

long loader::check_qa_flag()
{
    qa_flag_reader r;

    memset(g_qa_flag, 0, sizeof g_qa_flag);
    if (g_qa_request.skip != 0)
        return 0;
    r.init();
    if (r.check(m_revision, g_qa_request.token, g_qa_flag, m_id, g_qa_request.token + 80))
        return 42;
    m_qa = true;
    return 0;
}

long loader::open_params()
{
    param_block pb;
    aes256_cbc_encryptor enc;
    unsigned char k2[64];
    unsigned char k1[64];
    long r;

    enc.set_key(g_ls_key, &g_ls_iv);
    if (enc.encrypt(g_params_seed, 64, k1))
        return 21;
    r = enc.encrypt(g_key_seed, 64, k2);
    if (r)
        return 21;
    pb.init();
    if (pb.open((const unsigned char *)0x3E400, 1024, k1 + 32, k1 + 16, k2 + 32, k2 + 16,
                &m_revision, m_id, 0))
        return 40;
    return 0;
}

long loader::export_state(spu_reg *r0, spu_reg *r1, spu_reg *r2, spu_reg *r3,
                          spu_reg *r4, spu_reg *r5, spu_reg *r6, spu_reg *r7,
                          spu_reg *r8, spu_reg *r9, spu_reg *r10, spu_reg *r11,
                          spu_reg *r12, spu_reg *r13, spu_reg *r14, spu_reg *r15)
{
    unsigned char buf[256];
    int i;

    if (m_auth->get_state_data(buf, 256)) {
        memset(buf, 0, 256);
        return -1;
    }
    g_aes256_cbc_encryptor.set_key(g_ls_key, &g_ls_iv);
    for (i = 0; i < 4; i++) {
        if (g_aes256_cbc_encryptor.encrypt(buf + 64 * i, 64, buf + 64 * i)) {
            memset(buf, 0, 256);
            return 21;
        }
    }
    memcpy(r0, buf, 16);
    memcpy(r1, buf + 16, 16);
    memcpy(r2, buf + 32, 16);
    memcpy(r3, buf + 48, 16);
    memcpy(r4, buf + 64, 16);
    memcpy(r5, buf + 80, 16);
    memcpy(r6, buf + 96, 16);
    memcpy(r7, buf + 112, 16);
    memcpy(r8, buf + 128, 16);
    memcpy(r9, buf + 144, 16);
    memcpy(r10, buf + 160, 16);
    memcpy(r11, buf + 176, 16);
    memcpy(r12, buf + 192, 16);
    memcpy(r13, buf + 208, 16);
    memcpy(r14, buf + 224, 16);
    memcpy(r15, buf + 240, 16);
    return 0;
}

long loader::load_header()
{
    tagged_dma_buffer buf(0);
    unsigned int i;
    long r;

    buf.set_buffer(0x3E000, 8192);
    dma_buffer *bufs[1] = { &buf };
    r = m_auth->load_header(5, m_key, m_iv, m_pub, m_curve, bufs, 1);
    for (i = 0; i < 2; i++) {
        memset(g_keysets[i].key, 0, 32);
        memset(g_keysets[i].iv, 0, 16);
        memset(g_debug_keysets[i].key, 0, 32);
        memset(g_debug_keysets[i].iv, 0, 16);
    }
    return r;
}

long loader::check_revoke_list()
{
    long r;

    r = m_list->load((const void *)m_buf);
    if (r)
        return r;
    r = m_list->open(g_list_key, g_list_iv, g_list_pub, &g_list_curve);
    memset(g_list_key, 0, 32);
    memset(g_list_iv, 0, 16);
    if (r)
        return r;
    r = m_list->parse();
    if (r)
        return r;
    r = m_list->check_version(0x0004009300000000ULL);
    if (r)
        r = 22;
    return r;
}

long loader::load_segments(void (**entry)(void))
{
    tagged_dma_buffer b0(0), b1(1);
    local_buffer lb;
    Elf32_Phdr ph;
    Elf32_Ehdr eh;
    u16 i;
    long r;

    r = m_auth->read_elf_header(&eh);
    if (r)
        return r;
    if (eh.e_type != 2)
        return 29;
    if (eh.e_machine != 23)
        return 19;
    if (eh.e_entry > 0x257FF)
        return 18;
    *entry = (void (*)(void))eh.e_entry;
    b0.set_buffer(0x3E000, 4096);
    b1.set_buffer(0x3F000, 4096);
    dma_buffer *in[2] = { &b0, &b1 };
    for (i = 0; i < eh.e_phnum; i++) {
        if (m_auth->read_program_header(i, &ph))
            return 19;
        if (ph.p_filesz == 0 || ph.p_type != 1)
            continue;
        if (ph.p_align > 1 && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
            return 29;
        if ((u64)ph.p_vaddr + ph.p_filesz > 0x257FF)
            return 18;
        lb.set_buffer(ph.p_vaddr & ~15, ph.p_filesz + (ph.p_vaddr & 15));
        dma_buffer *out[1] = { &lb };
        r = m_auth->load_segment(2, i, ph.p_vaddr, in, 2, out, 1);
        if (r)
            return r;
    }
    return 0;
}

long loader::load(void (**entry)(void))
{
    sce_header h;
    auth_app_info app;
    u16 kind;
    long r;

    mfc_issue(0x3E000, m_req.image, 32, 4, 1, MFC_GET_CMD);
    while (!mfc_tag_done(4))
        ;
    memcpy(&h, (const void *)0x3E000, 32);
    kind = (h.key_revision & 0xF00) >> 8;
    if (kind != 1) {
        r = check_revoke_list();
        if (r)
            return r;
    }
    if (check_sce_header((const vec_uchar16 *)&h, 0, 1, 8192))
        return 19;
    r = select_keyset(h.key_revision & 0xFFF);
    if (r)
        return r;
    m_auth->set_offset(m_req.image);
    r = load_header();
    if (r)
        return r;
    if (m_auth->get_app_info(&app))
        return 21;
    u64 a_class = m_req.auth_id_a >> 60;
    u64 b_class = m_req.auth_id >> 60;
    u64 app_class = app.auth_id >> 60;
    if (app_class != 1 || a_class != 1 || b_class != 1)
        return 37;
    r = m_auth->check_auth_id_class(m_req.auth_id_a, m_req.auth_id);
    if (r)
        return r;
    if (kind != 1) {
        r = m_list->is_revoked(app.type, app.auth_id, app.version);
        if (r != 1)
            return r;
    }
    r = m_auth->check_issuer(m_req.auth_id_a, m_req.auth_id);
    if (r)
        return r;
    load_state();
    if (m_qa && m_revision - 129 <= 1)
        apply_qa_flag();
    r = check_revision();
    if (r)
        return r;
    return load_segments(entry);
}

long loader::run(u64 *a0, u64 *a1, u64 *a2, u64 *a3,
                 spu_reg *r0, spu_reg *r1, spu_reg *r2, spu_reg *r3,
                 spu_reg *r4, spu_reg *r5, spu_reg *r6, spu_reg *r7,
                 spu_reg *r8, spu_reg *r9, spu_reg *r10, spu_reg *r11,
                 spu_reg *r12, spu_reg *r13, spu_reg *r14, spu_reg *r15,
                 void (**entry)(void), u64 *a4)
{
    unsigned int i;
    long r;

    if (ch72_compare_version(0x0004009300000000ULL))
        return 48;
    memcpy(g_ls_key, (const void *)0, 32);
    memcpy(&g_ls_iv, (const void *)32, 16);
    *a4 = *(const u64 *)0x30;
    memset((void *)0, 0, 64);
    r = read_request();
    if (r)
        return r;
    *a0 = m_req.arg[0];
    *a1 = m_req.arg[1];
    *a2 = m_req.arg[2];
    *a3 = m_req.arg[3];
    if (open_params())
        m_revision = 0xFFFF;
    else if (m_revision - 129 <= 1)
        check_qa_flag();
    if (ch72_unwrap_key((vec_uchar16 *)g_list_key, (const vec_uchar16 *)g_list_key, 32))
        return 48;
    if (ch72_unwrap_iv((vec_uchar16 *)g_list_iv, (const vec_uchar16 *)g_list_iv, 16))
        return 48;
    for (i = 0; i < 2; i++) {
        if (ch72_unwrap_key((vec_uchar16 *)g_keysets[i].key, (const vec_uchar16 *)g_keysets[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_keysets[i].iv, (const vec_uchar16 *)g_keysets[i].iv, 16))
            return 48;
        if (ch72_unwrap_key((vec_uchar16 *)g_debug_keysets[i].key,
                            (const vec_uchar16 *)g_debug_keysets[i].key, 32))
            return 48;
        if (ch72_unwrap_iv((vec_uchar16 *)g_debug_keysets[i].iv,
                           (const vec_uchar16 *)g_debug_keysets[i].iv, 16))
            return 48;
    }
    if (ch64_request())
        return 48;
    g_revoke_list.init();
    m_list = &g_revoke_list;
    m_auth = new (g_auth_area) authenticator;
    r = load(entry);
    if (r)
        return r;
    r = export_state(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);
    if (r)
        return r;
    if (takes_keys()) {
        r = export_keys(r13, r14, r15);
        if (r)
            return r;
    }
    if (mbox_out_write(2))
        return 21;
    if (mbox_out_intr_write(2))
        return 16;
    return r;
}
