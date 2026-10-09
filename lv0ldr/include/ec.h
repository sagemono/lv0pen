#ifndef LDR_EC_H
#define LDR_EC_H

#include "bn.h"

#ifdef __cplusplus
extern "C" {
#endif

int ecdsa_verify_id(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub, unsigned int id);
void ec_curve_params(vec_uint4 *p, vec_uint4 *a, vec_uint4 *b, vec_uint4 *n, unsigned int id);
void ec_curve_base(vec_uint4 *G, unsigned int id);
int ecdsa_verify(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub,
                 const vec_uint4 *p, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *n, const vec_uint4 *G);
int ecdsa_verify_curve(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub,
                       const unsigned char *curve);
unsigned int ec_bit(const unsigned int *w, int i);
int ec_bits(const unsigned int *w, int n);
int ec_is_inf(const vec_uint4 *P);
void ec_set_inf(vec_uint4 *P);
void ec_copy(vec_uint4 *P, const vec_uint4 *Q);
void ec_from_affine(vec_uint4 *P, const vec_uint4 *A, const vec_uint4 *p);
void ec_to_affine(vec_uint4 *A, const vec_uint4 *P, const vec_uint4 *p, unsigned int n0);
void ec_double(vec_uint4 *P, const vec_uint4 *a, const vec_uint4 *p, unsigned int n0);
void ec_add(vec_uint4 *P, const vec_uint4 *Q, const vec_uint4 *a, const vec_uint4 *p, unsigned int n0);
void ec_mul2(vec_uint4 *R, const vec_uint4 *G, const vec_uint4 *Q, const unsigned int *k1, const unsigned int *k2,
             const vec_uint4 *a, const vec_uint4 *p, unsigned int n0);
int ec_on_curve(const vec_uint4 *P, const vec_uint4 *p, const vec_uint4 *a, const vec_uint4 *b);
int ec_curve_valid(unsigned int id);

#ifdef __cplusplus
}
#endif

#endif
