#include "cipher.h"
#include "util.h"

aes256_cbc_cipher g_aes256_cbc;

void aes256_cbc_cipher::set_key(const unsigned char *key, const unsigned char *iv)
{
    m_key = key;
    memcpy(&m_iv, iv, sizeof m_iv);
}

int aes256_cbc_cipher::decrypt(const void *in, int len, void *out)
{
    return aes_cbc_decrypt((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
                           (const vec_uchar16 *)&m_iv) < 0;
}
