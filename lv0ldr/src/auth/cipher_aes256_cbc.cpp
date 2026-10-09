#include "cipher.h"

aes256_cbc_cipher g_aes256_cbc;

void aes256_cbc_cipher::set_key(const unsigned char *key, const cipher_iv *iv)
{
    m_key = key;
    m_iv.a = iv->a;
    m_iv.b = iv->b;
}

int aes256_cbc_cipher::decrypt(const void *in, int len, void *out)
{
    aes_cbc_decrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
                    (const vec_uchar16 *)&m_iv);
    return 0;
}
