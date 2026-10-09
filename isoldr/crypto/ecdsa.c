#include "ec.h"

int ecdsa_verify(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub,
                 const vec_uint4 *p, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *n, const vec_uint4 *G)
{
    vec_uint4 h[2], r[2], s[2], w[2], t[2];
    unsigned int u1[6], u2[6];
    vec_uint4 Q[6], am[2], GJ[6], QJ[6];
    unsigned int n0;

    bn_from_bytes21(r, sig);
    bn_from_bytes21(s, sig + 21);
    bn_from_bytes(h, hash);
    bn_from_bytes(Q, pub);
    bn_from_bytes(Q + 2, pub + 20);
    if (bn_is_zero(p, 2) == 1)
        return -1;
    bn_mod2(Q, Q, p);
    bn_mod2(Q + 2, Q + 2, p);
    if (!bn_in_range(r, n))
        return -1;
    if (!bn_in_range(s, n))
        return -1;
    bn_inv_mod2(w, s, n);
    bn_mod2(h, h, n);
    bn_mul_mod2(t, h, w, n);
    bn_to_words(u1, t);
    bn_mul_mod2(t, r, w, n);
    bn_to_words(u2, t);
    n0 = bn_mont_word(spu_extract(p[0], 3));
    bn_to_mont2(am, a, p);
    ec_from_affine(GJ, G, p);
    ec_from_affine(QJ, Q, p);
    ec_mul2(GJ, GJ, QJ, u1, u2, am, p, n0);
    ec_to_affine(Q, GJ, p, n0);
    bn_mod2(t, Q, n);
    if (bn_cmp(t, r, 2) == 0)
        return 0;
    return -1;
}

int ecdsa_verify_curve(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub,
                       const unsigned char *curve)
{
    vec_uint4 p[2], a[2], b[2], n[2], G[6];

    bn_from_bytes(p, curve);
    bn_from_bytes(a, curve + 20);
    bn_from_bytes(b, curve + 40);
    bn_from_bytes21(n, curve + 60);
    bn_from_bytes(G, curve + 81);
    bn_from_bytes(G + 2, curve + 101);
    return ecdsa_verify(sig, hash, pub, p, a, b, n, G);
}
