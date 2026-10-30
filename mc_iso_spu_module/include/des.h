#ifndef MC_DES_H
#define MC_DES_H

#ifdef __cplusplus
extern "C" {
#endif

int des_set_key(unsigned int *sk, const unsigned char *key);

int des3_set_encrypt_key(unsigned int *sk, const unsigned char *key, int bits);
int des3_set_decrypt_key(unsigned int *sk, const unsigned char *key, int bits);

int des3_crypt_block(unsigned char *out, const unsigned char *in, const unsigned int *sk,
                      int nkeys);

int des3_cbc_encrypt(unsigned char *out, const unsigned char *in, int len,
                     const unsigned char *key, int bits, const unsigned char *iv);
int des3_cbc_decrypt(unsigned char *out, const unsigned char *in, int len,
                     const unsigned char *key, int bits, const unsigned char *iv);

#ifdef __cplusplus
}
#endif

#endif
