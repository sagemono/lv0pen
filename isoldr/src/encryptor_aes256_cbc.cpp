#include "cipher.h"

aes256_cbc_encryptor g_aes256_cbc_encryptor;

void aes256_cbc_encryptor::set_key(const unsigned char *key, const cipher_iv *iv)
{
    m_key = key;
    m_iv.a = iv->a;
    m_iv.b = iv->b;
}

int aes256_cbc_encryptor::encrypt(const void *in, int len, void *out)
{
    return aes_cbc_encrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
                           (const vec_uchar16 *)&m_iv) < 0;
}
