#include "ec.h"

unsigned int ec_bit(const unsigned int *w, int i)
{
    return w[i / 32] & 1 << i % 32;
}

int ec_bits(const unsigned int *w, int n)
{
    int i, j, b;
    unsigned int one = 1;

    for (i = n - 1; i > 0; i--)
        if (w[i] != 0)
            break;
    b = i * 32;
    for (j = 31; j >= 0; j--)
        if (one << j & w[i])
            break;
    if (j >= 0)
        return b + j + 1;
    return 0;
}

int ec_is_inf(const vec_uint4 *P)
{
    if (bn_is_one(P, 2) == 1 && bn_is_one(P + 2, 2) == 1 && bn_is_zero(P + 4, 2) == 1)
        return 1;
    return 0;
}

void ec_set_inf(vec_uint4 *P)
{
    bn_set_word(P, 1, 2);
    bn_set_word(P + 2, 1, 2);
    bn_zero(P + 4, 2);
}

void ec_copy(vec_uint4 *P, const vec_uint4 *Q)
{
    bn_copy(P, Q, 2);
    bn_copy(P + 2, Q + 2, 2);
    bn_copy(P + 4, Q + 4, 2);
}

void ec_from_affine(vec_uint4 *P, const vec_uint4 *A, const vec_uint4 *p)
{
    vec_uint4 one[2];

    bn_zero(one, 2);
    if (bn_is_zero(A, 2) && bn_is_zero(A + 2, 2)) {
        ec_set_inf(P);
    } else {
        bn_to_mont2(P, A, p);
        bn_to_mont2(P + 2, A + 2, p);
        bn_set_word(one, 1, 2);
        bn_to_mont2(P + 4, one, p);
    }
}

void ec_to_affine(vec_uint4 *A, const vec_uint4 *P, const vec_uint4 *p, unsigned int n0)
{
    vec_uint4 t[2], u[2];

    bn_set_word(t, 0, 2);
    bn_set_word(u, 0, 2);
    if (ec_is_inf(P)) {
        bn_zero(A, 2);
        bn_zero(A + 2, 2);
    } else {
        bn_mont_reduce2(t, P + 4, p, n0);
        bn_inv_mod2(t, t, p);
        bn_to_mont2(t, t, p);
        bn_mont_mul2(u, t, t, p, n0);
        bn_mont_mul2(A, P, u, p, n0);
        bn_mont_reduce2(A, A, p, n0);
        bn_mont_mul2(u, u, t, p, n0);
        bn_mont_mul2(A + 2, P + 2, u, p, n0);
        bn_mont_reduce2(A + 2, A + 2, p, n0);
        bn_set_word(A + 4, 1, 2);
    }
}

void ec_double(vec_uint4 *P, const vec_uint4 *a, const vec_uint4 *p, unsigned int n0)
{
    vec_uint4 *X = P, *Y = P + 2, *Z = P + 4;
    vec_uint4 t1[2], t2[2], t3[2], t4[2];

    bn_zero(t1, 2);
    bn_zero(t2, 2);
    bn_zero(t3, 2);
    bn_zero(t4, 2);
    if (ec_is_inf(X))
        return;
    bn_mont_mul2(t1, X, X, p, n0);
    bn_mont_mul2(t2, Z, Z, p, n0);
    bn_mont_mul2(t2, t2, t2, p, n0);
    bn_add_mod2(t3, t1, t1, p);
    bn_add_mod2(t1, t1, t3, p);
    bn_mont_mul2(t2, a, t2, p, n0);
    bn_add_mod2(t1, t1, t2, p);
    bn_mont_mul2(Z, Y, Z, p, n0);
    bn_add_mod2(Z, Z, Z, p);
    bn_mont_mul2(t2, X, Y, p, n0);
    bn_mont_mul2(t2, t2, Y, p, n0);
    bn_shl(t2, t2, 2, 2);
    bn_mod2(t2, t2, p);
    bn_mont_mul2(t4, t1, t1, p, n0);
    bn_copy(t3, t2, 2);
    bn_add_mod2(t3, t3, t3, p);
    bn_sub_mod2p(X, t4, t3, p);
    bn_mont_mul2(t3, Y, Y, p, n0);
    bn_mont_mul2(t3, t3, t3, p, n0);
    bn_shl(t3, t3, 3, 2);
    bn_mod2(t3, t3, p);
    bn_sub_mod2p(t2, t2, X, p);
    bn_mont_mul2(t1, t1, t2, p, n0);
    bn_sub_mod2p(Y, t1, t3, p);
}

void ec_add(vec_uint4 *P, const vec_uint4 *Q, const vec_uint4 *a, const vec_uint4 *p, unsigned int n0)
{
    vec_uint4 *X1 = P, *Y1 = P + 2, *Z1 = P + 4;
    const vec_uint4 *X2 = Q, *Y2 = Q + 2, *Z2 = Q + 4;
    vec_uint4 u2[2], u1[2], w[2], s2[2], s1[2], r[2];

    if (ec_is_inf(P)) {
        ec_copy(P, Q);
        return;
    }
    if (ec_is_inf(Q))
        return;
    bn_mont_mul2(s2, Z1, Z1, p, n0);
    bn_mont_mul2(s1, Z2, Z2, p, n0);
    bn_mont_mul2(u2, X2, s2, p, n0);
    bn_mont_mul2(u1, X1, s1, p, n0);
    bn_sub_mod2p(w, u2, u1, p);
    bn_mont_mul2(s2, s2, Z1, p, n0);
    bn_mont_mul2(s1, s1, Z2, p, n0);
    bn_mont_mul2(s2, s2, Y2, p, n0);
    bn_mont_mul2(s1, s1, Y1, p, n0);
    bn_sub_mod2p(r, s2, s1, p);
    if (bn_is_zero(w, 2)) {
        if (bn_is_zero(r, 2))
            ec_double(P, a, p, n0);
        else
            ec_set_inf(P);
        return;
    }
    bn_add_mod2(u2, u2, u1, p);
    bn_add_mod2(s2, s2, s1, p);
    bn_mont_mul2(Z1, Z2, Z1, p, n0);
    bn_mont_mul2(Z1, Z1, w, p, n0);
    bn_mont_mul2(u1, r, r, p, n0);
    bn_mont_mul2(s1, w, w, p, n0);
    bn_mont_mul2(u2, u2, s1, p, n0);
    bn_sub_mod2p(X1, u1, u2, p);
    bn_sub_mod2p(u2, u2, X1, p);
    bn_sub_mod2p(u2, u2, X1, p);
    bn_mont_mul2(u2, u2, r, p, n0);
    bn_mont_mul2(s1, s1, w, p, n0);
    bn_mont_mul2(s2, s2, s1, p, n0);
    bn_sub_mod2p(Y1, u2, s2, p);
    if (!bn_is_even(Y1))
        bn_add2p(Y1, Y1, p);
    bn_shr(Y1, Y1, 1, 2);
}
