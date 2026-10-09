#ifndef LDR_AES_H
#define LDR_AES_H

#include <spu_intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif

int aes_set_encrypt_key(vec_uchar16 *rk, const unsigned char *key, int bits);
int aes_set_decrypt_key(vec_uchar16 *rk, const unsigned char *key, int bits);
int aes_encrypt_block(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr);
int aes_encrypt8(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr);
int aes_decrypt_block(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr);
int aes_decrypt8(vec_uchar16 *out, const vec_uchar16 *in, const vec_uchar16 *rk, int nr);

int aes_cbc_decrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                    const vec_uchar16 *iv);
int aes_ctr(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
            vec_uchar16 *iv, int ctr_bits);

int aes_cbc_encrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                    const vec_uchar16 *iv);

#ifdef __cplusplus
}
#endif

#endif
