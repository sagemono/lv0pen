#ifndef LV0_AES_H
#define LV0_AES_H

#define AES_ROUNDS 10

#ifdef __cplusplus
extern "C" {
#endif

typedef struct aes_key {
    const unsigned int *rk;
} aes_key;

typedef struct aes_ctx {
    aes_key key;
    unsigned char stream[16];
} aes_ctx;

extern const unsigned int lv1ldr_aes_rk[4 * (AES_ROUNDS + 1)];
extern unsigned char lv1ldr_aes_iv[16];

extern const unsigned char aes_in_table[16][256];
extern const unsigned int aes_round_table[AES_ROUNDS][16][256];
extern const unsigned char aes_out_table[16][256];

int aes_encrypt_block(const unsigned char *in, unsigned char *out, const aes_key *key);
void aes_ctr_crypt(const unsigned int *rk, const unsigned char *ctr, unsigned int len,
                   const unsigned char *in, unsigned char *out);

#ifdef __cplusplus
}
#endif

#endif
