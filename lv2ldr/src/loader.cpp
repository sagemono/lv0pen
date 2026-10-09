#include "loader.h"
#include "revoke_list.h"
#include "auth.h"
#include "hash_reader.h"
#include "dma_stream.h"
#include "util.h"
#include "channels.h"
#include "mailbox.h"
#include <new>

loader::loader()
    : m_buf(0), m_list(0), m_auth(0)
{
    memset(&m_req, 0, sizeof m_req);
    memset(&m_file, 0, sizeof m_file);
}

long loader::read_file()
{
    unsigned int size;

    mfc_issue(0x3E000, m_file.auth_id, 32, 3, 0, MFC_GET_CMD);
    while (!mfc_tag_done(3))
        ;
    size = *(const u64 *)0x3E010 + *(const u64 *)0x3E018;
    if (size > 4096)
        return 17;
    if (size & 15)
        return 22;
    mfc_issue(0x3E000, m_file.auth_id, size, 3, 0, MFC_GET_CMD);
    while (!mfc_tag_done(3))
        ;
    return 0;
}

long loader::read_request()
{
    long r;

    while (*(volatile unsigned int *)0x3E000 != 0xFF)
        ;
    memcpy(&m_req, (const void *)0x3E800, sizeof m_req);
    if (m_req.arg_18 == ~0ULL) {
        m_buf = 0x3F000;
        if (m_req.image & 15)
            return 23;
        if (m_req.arg_10 & 15)
            return 23;
        if (m_req.image == 0)
            return 23;
        return 0;
    }
    memcpy(&m_file, (const void *)0x3E800, sizeof m_file);
    if ((m_file.auth_id & 15) != 0 || m_file.auth_id == 0)
        return 23;
    r = read_file();
    if (r == 0)
        m_buf = 0x3E000;
    return r;
}

extern unsigned char g_list_key[32], g_list_iv[16];
extern const unsigned char g_list_pub[48];
extern const unsigned int g_list_curve;

long loader::check_revoke_list()
{
    long r;

    r = m_list->load((const void *)m_buf);
    if (r)
        return r;
    r = m_list->open(g_list_key, g_list_iv, g_list_pub, &g_list_curve);
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

extern unsigned char g_lv2_key[32], g_lv2_iv[16];
extern const unsigned char g_lv2_pub[48];
extern const unsigned int g_lv2_curve;

long loader::load_header()
{
    tagged_dma_buffer buf(0);
    dma_buffer *bufs[1];

    buf.set_buffer(0x3E000, 8192);
    bufs[0] = &buf;
    return m_auth->load_header(3, g_lv2_key, g_lv2_iv, g_lv2_pub, &g_lv2_curve, bufs, 1);
}

long loader::load_segments()
{
    Elf64_Phdr ph;
    unsigned int i;
    long r;

    tagged_dma_buffer b0(0), b1(1), b2(2), b3(3);

    b0.set_buffer(0x3E000, 0x800);
    b1.set_buffer(0x3E800, 0x800);
    b2.set_buffer(0x3F000, 0x800);
    b3.set_buffer(0x3F800, 0x800);
    dma_buffer *in[2] = { &b0, &b1 };
    dma_buffer *out[2] = { &b2, &b3 };
    Elf64_Ehdr eh;
    r = m_auth->read_elf_header(&eh);
    if (r)
        return 19;
    if (eh.e_type != 2)
        return 29;
    for (i = 0; i < eh.e_phnum; i++) {
        if (m_auth->read_program_header(i, &ph))
            return 19;
        if (ph.p_filesz == 0 || ph.p_type != 1)
            continue;
        if (ph.p_align > 1 && ph.p_offset % ph.p_align != ph.p_vaddr % ph.p_align)
            return 29;
        r = m_auth->load_segment(2, i, (ph.p_vaddr & 0x3FFFFFFFFFFULL) + m_req.arg_10, ph.p_filesz,
                                 in, 2, out, 2);
        if (r)
            return r;
    }
    return 0;
}

extern const unsigned char g_segment_key[64];

long loader::hash_segment()
{
    hash_reader h;
    tagged_dma_buffer b0(0);
    Elf64_Phdr ph;
    tagged_dma_buffer b1(1);
    long r = 21;

    b0.set_buffer(0x3E000, 0x1000);
    b1.set_buffer(0x3F000, 0x1000);
    dma_buffer *bufs[2] = { &b0, &b1 };
    if (m_auth->read_program_header(0, &ph) == 0) {
        r = 46;
        if (h.open((ph.p_vaddr & 0x3FFFFFFFFFFULL) + m_req.arg_10, ph.p_filesz, bufs, 2,
                   g_segment_key, 64) == 0
            && h.read() == 0) {
            *(u64 *)0x3E000 = ph.p_vaddr;
            *(u64 *)0x3E008 = ph.p_filesz;
            if (h.get_digest((unsigned char *)0x3E010, 20) == 0) {
                memset((void *)0x3E024, 0, 12);
                r = 0;
            }
        }
    }
    return r;
}

extern unsigned char g_auth_area[];

long loader::run()
{
    u64 version;
    unsigned char payload[10];
    auth_app_info app;
    long r;

    ch72_get_version(&version);
    if (ch72_compare_version(0x0004009300000000ULL))
        return 48;
    mbox_in_drain();
    r = read_request();
    if (r)
        return r;
    if (ch72_unwrap_key((vec_uchar16 *)g_list_key, (const vec_uchar16 *)g_list_key, 32))
        return 48;
    if (ch72_unwrap_iv((vec_uchar16 *)g_list_iv, (const vec_uchar16 *)g_list_iv, 16))
        return 48;
    if (ch72_unwrap_key((vec_uchar16 *)g_lv2_key, (const vec_uchar16 *)g_lv2_key, 32))
        return 48;
    if (ch72_unwrap_iv((vec_uchar16 *)g_lv2_iv, (const vec_uchar16 *)g_lv2_iv, 16))
        return 48;
    if (ch64_request_40000())
        return 48;
    g_revoke_list.init();
    m_list = &g_revoke_list;
    if (m_req.arg_18 == ~0ULL)
        m_auth = new (g_auth_area) authenticator;
    r = check_revoke_list();
    if (r)
        return r;
    u64 t = m_req.arg_18 + 1;
    if (t)
        return 11;
    m_auth->set_offset(m_req.image + t);
    r = load_header();
    if (r)
        return r;
    if (m_auth->key_revision() != 0)
        return 19;
    if (m_auth->get_app_info(&app))
        return 21;
    u64 req_class = m_req.auth_id >> 60;
    u64 app_class = app.auth_id >> 60;
    if (app_class != 1 || req_class != 1)
        return 37;
    r = m_auth->check_auth_id_class(0, m_req.auth_id);
    if (r)
        return r;
    r = m_list->is_revoked(app.type, app.vendor[3], app.version);
    if (r != 1)
        return r;
    r = m_auth->check_issuer(0, m_req.auth_id);
    if (r)
        return r;
    r = load_segments();
    if (r)
        return r;
    u64 state[4] = { 0, 0, 0, 0 };
    m_auth->get_state(state);
    if (state[3] & 1) {
        memset(payload, 0, sizeof payload);
        ch72_put_request(version >> 32, payload);
    }
    if (ch64_request())
        return 48;
    r = hash_segment();
    if (r)
        return r;
    return 50;
}
