#ifndef SCISO_SC_CRYPTO_H
#define SCISO_SC_CRYPTO_H

#include "types.h"

#define SC_CRYPTO_CTR   0
#define SC_CRYPTO_CBC   1
#define SC_CRYPTO_XTS   2

class sc_crypto {
public:
    sc_crypto(int mode);
    ~sc_crypto();

    unsigned int crypt(const u8 *in, const int &in_len, u8 *out, const int &len, const u8 *key,
                       const int &key_bits, u8 *iv, const int &iv_bits, const u8 &encrypt);
    unsigned int crypt(const u8 *in, const int &in_len, u8 *out, const int &len, const u8 *key1,
                       const int &key1_bits, const u8 *key2, const int &key2_bits,
                       const u8 *tweak, const int &tweak_bits, const u8 &encrypt);

private:
    unsigned int xts_block(const u8 *in, u8 *out, const int &len, const u8 *key1, const u8 *key2,
                           const int &bits, const u8 *tweak, const u8 &encrypt);
    unsigned int cbc_decrypt(const u8 *in, u8 *out, const int &len, const u8 *key,
                             const int &bits, u8 *iv);
    unsigned int cbc_encrypt(const u8 *in, u8 *out, const int &len, const u8 *key,
                             const int &bits, u8 *iv);
    unsigned int ctr(const u8 *in, u8 *out, const int &len, const u8 *key, const int &bits,
                     u8 *iv);

    int m_mode;
};

#endif
