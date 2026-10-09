#include "data_decryptor.h"
#include "mac.h"
#include "chain_cipher.h"
#include "plain_cipher.h"
#include "mfc_atomic.h"
#include "aes.h"
#include "hmac_sha1.h"
#include "util.h"

extern const unsigned char g_keys_a[][16];
extern const unsigned char g_keys_b[][16];
extern const unsigned char g_key_ivs[][16];
extern const unsigned char g_state_seed_a[16];
extern const unsigned char g_state_seed_b[16];

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

static inline bool index_ok(unsigned int index)
{
    if (index > 1)
        return false;
    return true;
}

void data_decryptor::reset()
{
    m_open = false;
}

int data_decryptor::set_cipher(unsigned int flags)
{
    m_cipher_flags = flags;
    switch (flags & 0xFF) {
    case 1:
        m_cipher = &g_plain_cipher;
        break;
    case 2:
        m_cipher = &g_aes128_cbc_chain;
        break;
    default:
        return 46;
    }
    if ((flags & 0x0F000000) == 0x01000000)
        m_in_place = true;
    else
        m_in_place = false;
    return 0;
}

int data_decryptor::set_mac(unsigned int flags)
{
    m_mac_flags = flags;
    switch (flags & 0xFF) {
    case 3:
        return 47;
    case 4:
        m_mac = &g_mac_hmac_sha1;
        if (m_mac->set_mac_size(16))
            return 21;
        break;
    case 2:
        m_mac = &g_mac_aes_cmac;
        break;
    case 1:
        m_mac = &g_mac_hmac_sha1;
        if (m_mac->set_mac_size(20))
            return 21;
        break;
    default:
        return 46;
    }
    if ((flags & 0x0F000000) == 0x01000000)
        m_no_verify = true;
    else
        m_no_verify = false;
    return 0;
}

int data_decryptor::check(unsigned int revision, unsigned int mac_flags, unsigned int cipher_flags)
{
    if ((cipher_flags & 0x01000000)
        && !(revision == 129 || revision == 130 || revision == 160))
        return 41;
    if ((mac_flags & 0x01000000)
        && !(revision == 129 || revision == 130 || revision == 160))
        return 41;
    return 0;
}

int data_decryptor::restore(const decryptor_state *st)
{
    int r;
    bool ok;

    if (!st)
        return 21;
    if (st->version != 1)
        return 46;
    r = set_mac(st->mac_flags);
    if (r)
        return r;
    r = set_cipher(st->cipher_flags);
    if (r)
        return r;
    switch (st->mac_flags & 0xFF) {
    case 2:
        if (m_mac->restore(st->mac_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    case 4:
    case 1:
        if (m_mac->restore(st->mac_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    default:
        return 46;
    }
    if (!ok)
        return 21;
    switch (st->cipher_flags & 0xFF) {
    case 1:
        if (m_cipher->vslot6(st->cipher_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    case 2:
        if (m_cipher->vslot6(st->cipher_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    default:
        return 46;
    }
    if (!ok)
        return 21;
    m_open = true;
    return r;
}

int data_decryptor::save(decryptor_state *st)
{
    int r;
    bool ok;

    if (!m_cipher)
        return 21;
    if (!m_mac)
        return 21;
    if (!st)
        return 21;
    memset(st, 0, sizeof *st);
    st->version = 1;
    st->mac_flags = m_mac_flags;
    st->cipher_flags = m_cipher_flags;
    switch (st->mac_flags & 0xFF) {
    case 2:
        if (m_mac->save(st->mac_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    case 4:
    case 1:
        if (m_mac->save(st->mac_state) == 0)
            ok = true;
        else
            ok = false;
        break;
    default:
        return 46;
    }
    if (!ok)
        return 21;
    switch (st->cipher_flags & 0xFF) {
    case 1:
        r = m_cipher->vslot7(st->cipher_state);
        if (r)
            r = 21;
        break;
    case 2:
        r = m_cipher->vslot7(st->cipher_state);
        if (r)
            r = 21;
        break;
    default:
        r = 46;
        break;
    }
    if (r)
        return r;
    return 0;
}

int data_decryptor::derive_mac_key(const unsigned char *in, unsigned char *out,
                                   unsigned int flags, unsigned int index)
{
    int r;

    switch (flags & 0xF0000000) {
    case 0:
        memcpy(out, in, 16);
        r = 0;
        break;
    case 0x20000000:
        memcpy(out, g_keys_a[index], 16);
        r = 0;
        break;
    case 0x10000000:
        g_aes128_cbc_chain.set_key(g_keys_b[index], g_key_ivs[index]);
        r = g_aes128_cbc_chain.decrypt(in, 16, out) ? 21 : 0;
        break;
    default:
        r = 46;
        break;
    }
    return r;
}

int data_decryptor::derive_key(const unsigned char *key, const unsigned char *iv,
                               unsigned char *key_out, unsigned char *iv_out,
                               unsigned int flags, unsigned int index)
{
    int r;

    switch (flags & 0xF0000000) {
    case 0:
        memcpy(key_out, key, 16);
        memcpy(iv_out, iv, 16);
        r = 0;
        break;
    case 0x20000000:
        memcpy(key_out, g_keys_b[index], 16);
        memcpy(iv_out, g_key_ivs[index], 16);
        r = 0;
        break;
    case 0x10000000:
        g_aes128_cbc_chain.set_key(g_keys_b[index], g_key_ivs[index]);
        if (g_aes128_cbc_chain.decrypt(key, 16, key_out)) {
            r = 21;
            break;
        }
        memcpy(iv_out, iv, 16);
        r = 0;
        break;
    default:
        r = 46;
        break;
    }
    return r;
}

int data_decryptor::state_keys(unsigned char *k1, unsigned char *k2)
{
    if (!k1 || !k2)
        return 21;
    memset(k1, 0, 16);
    memset(k2, 0, 16);
    memcpy(k1, g_state_seed_a, 16);
    memcpy(k2, g_state_seed_b, 16);
    if (aes_cbc_encrypt((vec_uchar16 *)k1, (const vec_uchar16 *)k1, 16, g_keys_b[0], 128,
                        (const vec_uchar16 *)g_key_ivs[0]))
        return 21;
    if (aes_cbc_encrypt((vec_uchar16 *)k2, (const vec_uchar16 *)k2, 16, g_keys_b[0], 128,
                        (const vec_uchar16 *)g_key_ivs[0]))
        return 21;
    return 0;
}

int data_decryptor::seal(unsigned char *st)
{
    unsigned char k1[16], k2[16];
    unsigned char md[20];
    int r;

    r = state_keys(k1, k2);
    if (r)
        return r;
    memset(md, 0, 20);
    hmac_sha1(md, st, 332, k2, 128);
    memset(st + 332, 0, 20);
    memcpy(st + 332, md, 20);
    aes_cbc_encrypt((vec_uchar16 *)st, (const vec_uchar16 *)st, 352, k1, 128,
                    (const vec_uchar16 *)k1);
    return r;
}

int data_decryptor::unseal(unsigned char *st)
{
    unsigned char k1[16], k2[16];
    unsigned char md[20];
    int r;

    r = state_keys(k1, k2);
    if (r)
        return r;
    aes_cbc_decrypt((vec_uchar16 *)st, (const vec_uchar16 *)st, 352, k1, 128,
                    (const vec_uchar16 *)k1);
    memset(md, 0, 20);
    hmac_sha1(md, st, 332, k2, 128);
    if (bytes_differ(md, st + 332, 20))
        return 46;
    return r;
}

int data_decryptor::put_state(u64 ea, const unsigned char *st)
{
    if (ea == 0 || !st)
        return 46;
    return dma_put(ea, (void *)0x3E000, st, 352) ? 21 : 0;
}

int data_decryptor::get_state(u64 ea, unsigned char *st)
{
    if (ea == 0 || !st)
        return 46;
    return dma_get(ea, (void *)0x3E000, st, 352) ? 21 : 0;
}

int data_decryptor::finish(u64 ea, u64 state_ea)
{
    unsigned char tail[48];
    unsigned char st[352];
    int r;

    memset(tail, 0, sizeof tail);
    memset(st, 0, sizeof st);
    if (state_ea != 0 && (state_ea & 15))
        return 46;
    if (ea == 0)
        return 46;
    if (ea & 15)
        return 46;
    dma_get(ea, (void *)0x3E000, tail, 48);
    if (state_ea != 0) {
        r = get_state(state_ea, st);
        if (r)
            return r;
        if (unseal(st))
            return 46;
        r = restore((const decryptor_state *)st);
        if (r)
            return r;
    }
    if (!m_open)
        return 46;
    r = m_mac->verify(tail);
    if (m_no_verify)
        return 0;
    if (r)
        return 14;
    return r;
}

int data_decryptor::pump()
{
    dma_buffer *in = 0, *out = 0;
    bool last = false;
    int r;

    do {
        r = m_queue.dequeue_read(&in);
        if (r != 0) {
            if (r != 1)
                return 21;
        }
        if (in->offset & 15)
            return 21;
        if (r == 1)
            last = true;
        if (m_queue.dequeue_write(&out))
            return 21;
        out->length = in->length;
        out->offset = in->offset;
        if (m_mac->update((const void *)in->ls, in->length))
            return 21;
        if (in->length > out->size)
            return 21;
        if (m_cipher->decrypt((const void *)in->ls, in->length, (void *)out->ls))
            return 21;
        if (requeue(in, out))
            return 21;
    } while (!last);
    m_queue.close();
    return 0;
}

int data_decryptor::process(u64 src, u64 len, u64 state_ea, dma_buffer **in, unsigned int nin,
                            dma_buffer **out, unsigned int nout)
{
    unsigned char st[352];
    unsigned int i;
    int r;

    if (state_ea != 0 && (state_ea & 15))
        return 46;
    if (src == 0 || len == 0)
        return 46;
    if (src & 15)
        return 46;
    if (len & 15)
        return 46;
    if (state_ea != 0) {
        memset(st, 0, sizeof st);
        r = get_state(state_ea, st);
        if (r)
            return r;
        if (unseal(st))
            return 46;
        r = restore((const decryptor_state *)st);
        if (r)
            return r;
    }
    if (!m_open)
        return 46;
    if (m_queue.open(src, (unsigned int)len, src, (unsigned int)len))
        return 21;
    for (i = 0; i != nin; i++)
        if (m_queue.enqueue_read(in[i]))
            return 21;
    for (i = 0; i != nout; i++)
        if (m_queue.add_written(out[i]))
            return 21;
    r = pump();
    if (r)
        return r;
    if (state_ea == 0)
        return r;
    r = save((decryptor_state *)st);
    if (r)
        return r;
    r = seal(st);
    if (r)
        return r;
    return put_state(state_ea, st);
}

data_decryptor::data_decryptor()
    : m_open(false), m_cipher(0), m_mac(0), m_in_place(false), m_no_verify(false),
      m_mac_flags(~0U), m_cipher_flags(~0U)
{
}

int data_decryptor::open(const unsigned char *mac_key, const unsigned char *key,
                         const unsigned char *iv, unsigned int mac_flags,
                         unsigned int cipher_flags, u64 state_ea, unsigned int index)
{
    unsigned char mk[16];
    unsigned char k[16];
    unsigned char v[16];
    unsigned char st[352];
    int r;

    if (state_ea != 0 && (state_ea & 15))
        return 46;
    if (!mac_key || !key)
        return 46;
    if (!iv)
        return 46;
    if (!index_ok(index))
        return 46;
    r = derive_key(key, iv, k, v, cipher_flags, index);
    if (r)
        return r;
    r = derive_mac_key(mac_key, mk, mac_flags, index);
    if (r)
        return r;
    r = set_cipher(cipher_flags);
    if (r)
        return r;
    r = set_mac(mac_flags);
    if (r)
        return r;
    m_cipher->set_key(k, v);
    m_mac->init(mk, 0);
    if (state_ea != 0) {
        r = save((decryptor_state *)st);
        if (r)
            return r;
        r = seal(st);
        if (r)
            return r;
        r = put_state(state_ea, st);
        if (r)
            return r;
    }
    m_open = true;
    return r;
}

int data_decryptor::requeue(dma_buffer *in, dma_buffer *out)
{
    if (m_in_place) {
        if (m_queue.enqueue_write(in) || m_queue.enqueue_read(out))
            return 21;
    } else {
        if (m_queue.enqueue_write(out) || m_queue.enqueue_read(in))
            return 21;
    }
    return 0;
}
