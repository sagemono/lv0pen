#include <spu_intrinsics.h>
#include "token.h"
#include "aes.h"
#include "sha1.h"
#include "ec.h"
#include "util.h"

extern "C" int aes_cmac(vec_uchar16 *mac, const vec_uchar16 *in, int len, const unsigned char *key, int bits);
extern "C" int hmac_sha1(unsigned char *md, const unsigned char *data, int len, const unsigned char *key,
                         unsigned int keylen);
int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

#define LS_IN           0x3E000ULL
#define LS_OUT          0x3F000ULL

token_processor::~token_processor()
{
}

token_processor::token_processor(const unsigned char *key, const unsigned char *iv,
                                 const unsigned char *hmac_key, const unsigned char *pub,
                                 unsigned int curve, const unsigned char *idps)
{
    m_key = key;
    m_iv = iv;
    m_hmac_key = hmac_key;
    m_pub = pub;
    m_curve = curve;
    memcpy(m_idps, idps, 16);
}

unsigned int token_processor::verify(const token_request *req)
{
    unsigned char token[128] __attribute__((aligned(16))) = { 0 };
    qa_token t = { 0 };
    dma_channel dma;
    unsigned int ls = LS_IN + (req->token_ea & 127);
    unsigned int r;

    r = dma.issue(ls, req->token_ea, 128, 1, 0, MFC_GET);
    if (r == 0) {
        dma.wait(1);
        memcpy(token, (void *)ls, 128);
        r = decrypt(token, &t);
        if (r == 0) {
            r = check_idps(&t);
            if (r == 0)
                r = verify_signature(token);
        }
    }
    return r;
}

unsigned int token_processor::update(const token_request *req, unsigned char *out)
{
    unsigned char token[128] __attribute__((aligned(16))) = { 0 };
    qa_token t = { 0 };
    dma_channel dma;
    unsigned int ls = LS_IN + (req->token_ea & 127);
    unsigned int r;

    r = dma.issue(ls, req->token_ea, 128, 1, 0, MFC_GET);
    if (r)
        return r;
    dma.wait(1);
    memcpy(token, (void *)ls, 128);
    if (decrypt(token, &t) || check_idps(&t) || verify_signature(token)) {
        memset(token, 0, 128);
        memset(&t, 0, 128);
        t.version = 1;
        memcpy(t.idps, m_idps, 16);
        if (hmac_sha1(t.md, (const unsigned char *)&t, 60, m_hmac_key, 64))
            return 20;
        if (out)
            memcpy(out, &t, 80);
        if (aes_cbc_encrypt((vec_uchar16 *)token, (const vec_uchar16 *)&t, 80, m_key, 256,
                            (const vec_uchar16 *)m_iv))
            return 21;
    }
    memset(t.md, 0, 20);
    ls = LS_OUT + (req->token_ea & 127);
    memcpy((void *)ls, token, 128);
    r = dma.issue(ls, req->token_ea, 128, 2, 0, MFC_PUT);
    if (r)
        return r;
    dma.wait(2);
    ls = LS_OUT + (req->result_ea & 127);
    memcpy((void *)ls, &t, 128);
    r = dma.issue(ls, req->result_ea, 128, 2, 0, MFC_PUT);
    if (r)
        return r;
    dma.wait(2);
    return r;
}

unsigned int token_processor::decrypt(const unsigned char *token, qa_token *t)
{
    unsigned char md[20] __attribute__((aligned(16)));

    if (aes_cbc_decrypt((vec_uchar16 *)t, (const vec_uchar16 *)token, 80, m_key, 256,
                        (const vec_uchar16 *)m_iv))
        return 20;
    if (hmac_sha1(md, (const unsigned char *)t, 60, m_hmac_key, 64))
        return 20;
    if (bytes_differ(md, t->md, 20))
        return 20;
    if (t->version != 1)
        return 19;
    return 0;
}

unsigned int token_processor::verify_signature(const unsigned char *token)
{
    sha1_ctx ctx;
    unsigned char md[20] __attribute__((aligned(16)));

    if (sha1_init(&ctx) < 0)
        return 15;
    if (sha1_update(&ctx, token, 80) < 0)
        return 15;
    if (sha1_final(md, &ctx) < 0)
        return 15;
    if (ecdsa_verify_id(token + 80, md, m_pub, m_curve) < 0)
        return 20;
    return 0;
}

unsigned int token_processor::check_idps(const qa_token *t)
{
    if (bytes_differ(m_idps, t->idps, 16))
        return 6;
    return 0;
}
