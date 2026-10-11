#include <spu_intrinsics.h>
#include "sc_crypto.h"
#include "aes.h"
#include "sc_log.h"

#define BAD_ARG(bits) \
    do { \
        SC_ERR(err, bits); \
        return 9; \
    } while (0)

#define MODE_ERR(bits) \
    do { \
        err |= (bits); \
        get_log()->put(err, 16); \
        get_log()->puts(", "); \
        get_log()->put((unsigned int)m_mode, 16); \
        get_log()->puts("\n"); \
    } while (0)

sc_crypto::sc_crypto(int mode)
    : m_mode(mode)
{
}

sc_crypto::~sc_crypto()
{
}

unsigned int sc_crypto::crypt(const u8 *in, const int &in_len, u8 *out, const int &len,
                              const u8 *key, const int &key_bits, u8 *iv, const int &iv_bits,
                              const u8 &encrypt)
{
    unsigned int err = 0x81020100;
    unsigned int r;

    if (in_len <= 0)
        BAD_ARG(0x80);
    if (len <= 0)
        BAD_ARG(0x81);
    if (in_len > len)
        BAD_ARG(0x82);
    if (key_bits != 128)
        BAD_ARG(0x83);
    if (iv_bits != 128)
        BAD_ARG(0x84);
    if (in == 0 || out == 0 || key == 0 || iv == 0)
        BAD_ARG(0x85);
    switch (m_mode) {
    case SC_CRYPTO_CTR:
        r = ctr(in, out, len, key, key_bits, iv);
        if (r != 0)
            MODE_ERR(0x01);
        break;
    case SC_CRYPTO_CBC:
        if (encrypt)
            r = cbc_encrypt(in, out, len, key, key_bits, iv);
        else
            r = cbc_decrypt(in, out, len, key, key_bits, iv);
        if (r != 0)
            MODE_ERR(0x01);
        break;
    default:
        r = 10;
        MODE_ERR(0x86);
        break;
    }
    return r;
}

unsigned int sc_crypto::crypt(const u8 *in, const int &in_len, u8 *out, const int &len,
                              const u8 *key1, const int &key1_bits, const u8 *key2,
                              const int &key2_bits, const u8 *tweak, const int &tweak_bits,
                              const u8 &encrypt)
{
    unsigned int err = 0x81020100;
    unsigned int r;

    if (in_len <= 0)
        BAD_ARG(0x80);
    if (len <= 0)
        BAD_ARG(0x81);
    if (in_len > len)
        BAD_ARG(0x82);
    if (key1_bits != 128 || key2_bits != 128)
        BAD_ARG(0x83);
    if (tweak_bits != 128)
        BAD_ARG(0x84);
    if (in == 0 || out == 0 || key1 == 0 || key2 == 0 || tweak == 0)
        BAD_ARG(0x85);
    if (m_mode == SC_CRYPTO_XTS) {
        r = xts_block(in, out, len, key1, key2, key1_bits, tweak, encrypt);
        if (r != 0)
            MODE_ERR(0x01);
    } else {
        r = 10;
        MODE_ERR(0x86);
    }
    return r;
}

unsigned int sc_crypto::ctr(const u8 *in, u8 *out, const int &len, const u8 *key,
                            const int &bits, u8 *iv)
{
    if (aes_ctr((vec_uchar16 *)out, (const vec_uchar16 *)in, len, key, bits, (vec_uchar16 *)iv,
                0) != 0)
        return 15;
    return 0;
}

unsigned int sc_crypto::cbc_encrypt(const u8 *in, u8 *out, const int &len, const u8 *key,
                                    const int &bits, u8 *iv)
{
    if (aes_cbc_encrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, key, bits,
                        (const vec_uchar16 *)iv) != 0)
        return 15;
    return 0;
}

unsigned int sc_crypto::cbc_decrypt(const u8 *in, u8 *out, const int &len, const u8 *key,
                                    const int &bits, u8 *iv)
{
    if (aes_cbc_decrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, key, bits,
                        (const vec_uchar16 *)iv) != 0)
        return 15;
    return 0;
}

unsigned int sc_crypto::xts_block(const u8 *in, u8 *out, const int &len, const u8 *key1,
                                  const u8 *key2, const int &bits, const u8 *tweak,
                                  const u8 &encrypt)
{
    u8 t[16] __attribute__((aligned(16)));
    u8 ks[176] __attribute__((aligned(16)));
    vec_uchar16 *rk = (vec_uchar16 *)ks;
    int nr, i;

    if (len != 16)
        return 9;
    nr = aes_set_encrypt_key(rk, key2, bits);
    if (aes_encrypt_block((vec_uchar16 *)t, (const vec_uchar16 *)tweak, rk, nr) != 0)
        return 15;
    if (encrypt)
        nr = aes_set_encrypt_key(rk, key1, bits);
    else
        nr = aes_set_decrypt_key(rk, key1, bits);
    for (i = 0; i < 16; i++)
        out[i] = in[i] ^ t[i];
    if (encrypt) {
        if (aes_encrypt_block((vec_uchar16 *)out, (const vec_uchar16 *)out, rk, nr) != 0)
            return 15;
    } else {
        if (aes_decrypt_block((vec_uchar16 *)out, (const vec_uchar16 *)out, rk, nr) != 0)
            return 15;
    }
    for (i = 0; i < 16; i++)
        out[i] = out[i] ^ t[i];
    return 0;
}
