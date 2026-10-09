#include "cipher.h"
#include "util.h"

aes128_ctr_cipher g_aes128_ctr;

void aes128_ctr_cipher::set_key(const unsigned char *key, const unsigned char *iv)
{
    m_key = key;
    memcpy(&m_iv, iv, sizeof m_iv);
}

int aes128_ctr_cipher::decrypt(const void *in, int len, void *out)
{
    return aes_ctr((vec_uchar16 *)out, (const vec_uchar16 *)in, len, m_key, m_bits,
                   (vec_uchar16 *)&m_iv, 0) < 0;
}
