#ifndef LDR_BN_H
#define LDR_BN_H

#include <spu_intrinsics.h>

void bn_zero(vec_uint4 *a, int n);
void bn_set_word(vec_uint4 *a, unsigned int w, int n);
int bn_is_zero(const vec_uint4 *a, int n);
int bn_is_one(const vec_uint4 *a, int n);
void bn_copy(vec_uint4 *d, const vec_uint4 *s, int n);
int bn_cmp(const vec_uint4 *a, const vec_uint4 *b, int n);
int bn_is_even(const vec_uint4 *a);
void bn_add2(vec_uint4 *s, const vec_uint4 *a, const vec_uint4 *b);
void bn_add2p(vec_uint4 *s, const vec_uint4 *a, const vec_uint4 *b);
void bn_add_c(vec_uint4 *s, vec_uint4 *cout, const vec_uint4 *a, const vec_uint4 *b, vec_uint4 c, int n);
void bn_sub_q(vec_uint4 *d, vec_uint4 *bout, vec_uint4 a, vec_uint4 b, vec_uint4 bin);
void bn_sub_q1(vec_uint4 *d, vec_uint4 a, vec_uint4 b, vec_uint4 bin);
void bn_sub_n(vec_uint4 *d, vec_uint4 *bout, const vec_uint4 *a, const vec_uint4 *b, vec_uint4 bin, int n);
void bn_sub2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b);
void bn_sub2p(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b);
void bn_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
void bn_add_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m);
void bn_sub_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m);
void bn_sub_mod2p(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m);
void bn_mul_q(vec_uint4 *lo, vec_uint4 *hi, vec_uint4 a, vec_uint4 b, vec_uint4 c, vec_uchar16 sl, vec_uchar16 sh);
void bn_mul_n(vec_uint4 *d, vec_uint4 *cout, const vec_uint4 *a, vec_uint4 b, int n, vec_uchar16 sl, vec_uchar16 sh);
void bn_mul2(vec_uint4 *d, const vec_uint4 *a, vec_uint4 b, vec_uchar16 sl, vec_uchar16 sh);
void bn_mul2_pair(vec_uint4 *d, vec_uint4 *e, const vec_uint4 *a, vec_uint4 b, const vec_uint4 *f, vec_uint4 g, vec_uchar16 sl, vec_uchar16 sh);
vec_uint4 bn_shl_q(vec_uint4 x, int n);
vec_uint4 bn_shr_q(vec_uint4 x, int n);
void bn_shl(vec_uint4 *d, const vec_uint4 *a, int bits, int n);
void bn_mul_nq(vec_uint4 *d, vec_uint4 *hi, const vec_uint4 *a, vec_uint4 b, int n);
void bn_mul2x2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b);
void bn_shr(vec_uint4 *d, const vec_uint4 *a, int bits, int n);
void bn_mod2_full(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
void bn_mod4(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
void bn_div_mod2(vec_uint4 *d, const vec_uint4 *b, const vec_uint4 *a, const vec_uint4 *m);
void bn_inv_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
unsigned int bn_inv_word(unsigned int a);
unsigned int bn_mont_word(unsigned int a);
void bn_mont_mul2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m, unsigned int n0);
void bn_mont_reduce2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m, unsigned int n0);
void bn_to_mont2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
void bn_mul_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m);
int bn_mod4_is_zero(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m);
unsigned int bn_load_be32(const unsigned char *p);
void bn_from_bytes(vec_uint4 *d, const unsigned char *p);
void bn_from_bytes21(vec_uint4 *d, const unsigned char *p);
void bn_to_words(unsigned int *w, const vec_uint4 *a);
void bn_from_bytes_not(vec_uint4 *d, const unsigned char *p);
void bn_from_bytes21_not(vec_uint4 *d, const unsigned char *p);
int bn_in_range(const vec_uint4 *a, const vec_uint4 *b);
int bn_less(const vec_uint4 *a, const vec_uint4 *b);

#endif
