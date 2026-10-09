#include "cipher.h"

aes128_ctr_cipher g_aes128_ctr;

void aes128_ctr_cipher::set_key(const unsigned char *key, const cipher_iv *iv)
{
    m_key = key;
    m_iv.a = iv->a;
    m_iv.b = iv->b;
}

int aes128_ctr_cipher::decrypt(const void *in, int len, void *out)
{
    aes_ctr((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
            (vec_uchar16 *)&m_iv, 0);
    return 0;
}
