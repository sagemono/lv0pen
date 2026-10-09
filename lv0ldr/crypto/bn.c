#include "bn.h"

void bn_zero(vec_uint4 *a, int n)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = spu_splats(0u);
}

void bn_set_word(vec_uint4 *a, unsigned int w, int n)
{
    bn_zero(a, n);
    a[0] = spu_insert(w, a[0], 3);
}

int bn_is_zero(const vec_uint4 *a, int n)
{
    vec_uint4 zero = spu_splats(0u);
    int i;

    for (i = 0; i < n; i++)
        if (spu_extract(spu_gather(spu_cmpgt(a[i], zero)), 0) != spu_extract(spu_gather(spu_cmpgt(zero, a[i])), 0))
            return 0;
    return 1;
}

int bn_is_one(const vec_uint4 *a, int n)
{
    vec_uint4 one = (vec_uint4){0, 0, 0, 1};
    vec_uint4 zero = spu_splats(0u);
    int i;

    if (spu_extract(spu_gather(spu_cmpgt(a[0], one)), 0) != spu_extract(spu_gather(spu_cmpgt(one, a[0])), 0))
        return 0;
    for (i = 1; i < n; i++)
        if (spu_extract(spu_gather(spu_cmpgt(a[i], zero)), 0) != spu_extract(spu_gather(spu_cmpgt(zero, a[i])), 0))
            return 0;
    return 1;
}

void bn_copy(vec_uint4 *d, const vec_uint4 *s, int n)
{
    int i;

    if (d == s)
        return;
    for (i = 0; i < n; i++)
        d[i] = s[i];
}

int bn_cmp(const vec_uint4 *a, const vec_uint4 *b, int n)
{
    unsigned int gt, lt;
    int i;

    for (i = n - 1; i >= 0; i--) {
        gt = spu_extract(spu_gather(spu_cmpgt(a[i], b[i])), 0);
        lt = spu_extract(spu_gather(spu_cmpgt(b[i], a[i])), 0);
        if (gt > lt)
            return 1;
        if (lt > gt)
            return -1;
    }
    return 0;
}

int bn_is_even(const vec_uint4 *a)
{
    unsigned int w = spu_extract(a[0], 3);

    if (w & 1)
        return 0;
    return 1;
}

void bn_add2(vec_uint4 *s, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 sum, c0, c1, c2, c3, c;

    c0 = spu_genc(a[0], b[0]);
    sum = spu_add(a[0], b[0]);
    c1 = spu_genc(sum, spu_slqwbyte(c0, 4));
    sum = spu_add(sum, spu_slqwbyte(c0, 4));
    c2 = spu_genc(sum, spu_slqwbyte(c1, 4));
    sum = spu_add(sum, spu_slqwbyte(c1, 4));
    c3 = spu_genc(sum, spu_slqwbyte(c2, 4));
    s[0] = spu_add(sum, spu_slqwbyte(c2, 4));
    c = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(c0, c1), c2), c3), -12);
    c0 = spu_gencx(a[1], b[1], c);
    sum = spu_addx(a[1], b[1], c);
    c1 = spu_genc(sum, spu_slqwbyte(c0, 4));
    sum = spu_add(sum, spu_slqwbyte(c0, 4));
    c2 = spu_genc(sum, spu_slqwbyte(c1, 4));
    sum = spu_add(sum, spu_slqwbyte(c1, 4));
    s[1] = spu_add(sum, spu_slqwbyte(c2, 4));
}

void bn_add2p(vec_uint4 *s, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 sum, c0, c1, c2, c3, c;

    c0 = spu_genc(a[0], b[0]);
    sum = spu_add(a[0], b[0]);
    c1 = spu_genc(sum, spu_slqwbyte(c0, 4));
    sum = spu_add(sum, spu_slqwbyte(c0, 4));
    c2 = spu_genc(sum, spu_slqwbyte(c1, 4));
    sum = spu_add(sum, spu_slqwbyte(c1, 4));
    c3 = spu_genc(sum, spu_slqwbyte(c2, 4));
    s[0] = spu_add(sum, spu_slqwbyte(c2, 4));
    c = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(c0, c1), c2), c3), -12);
    c0 = spu_gencx(a[1], b[1], c);
    sum = spu_addx(a[1], b[1], c);
    s[1] = spu_add(sum, spu_slqwbyte(c0, 4));
}

void bn_add_c(vec_uint4 *s, vec_uint4 *cout, const vec_uint4 *a, const vec_uint4 *b, vec_uint4 c, int n)
{
    vec_uint4 carry = spu_splats(0u);
    vec_uint4 sum, c0, c1, c2, c3;
    int i;

    for (i = 0; i < n; i++) {
        c0 = spu_gencx(a[i], b[i], c);
        sum = spu_addx(a[i], b[i], c);
        c1 = spu_genc(sum, spu_slqwbyte(c0, 4));
        sum = spu_add(sum, spu_slqwbyte(c0, 4));
        c2 = spu_genc(sum, spu_slqwbyte(c1, 4));
        sum = spu_add(sum, spu_slqwbyte(c1, 4));
        c3 = spu_genc(sum, spu_slqwbyte(c2, 4));
        s[i] = spu_add(sum, spu_slqwbyte(c2, 4));
        carry = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(c0, c1), c2), c3), -12);
        c = carry;
    }
    *cout = carry;
}

#define SHL4 ((vec_uchar16){4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19})

void bn_sub_q(vec_uint4 *d, vec_uint4 *bout, vec_uint4 a, vec_uint4 b, vec_uint4 bin)
{
    vec_uint4 dif, b0, b1, b2, b3, t0, t1, t2;
    vec_uint4 zero = spu_splats(0u);
    vec_uint4 one = spu_splats(1u);
    vec_uchar16 shl4 = SHL4;

    b0 = spu_genbx(a, b, bin);
    dif = spu_subx(a, b, bin);
    t0 = spu_shuffle(b0, one, shl4);
    b1 = spu_genbx(dif, zero, t0);
    dif = spu_subx(dif, zero, t0);
    t1 = spu_shuffle(b1, one, shl4);
    b2 = spu_genbx(dif, zero, t1);
    dif = spu_subx(dif, zero, t1);
    t2 = spu_shuffle(b2, one, shl4);
    b3 = spu_genbx(dif, zero, t2);
    *d = spu_subx(dif, zero, t2);
    *bout = spu_shuffle(one, spu_and(spu_and(b0, b1), spu_and(b2, b3)), shl4);
}

void bn_sub_q1(vec_uint4 *d, vec_uint4 a, vec_uint4 b, vec_uint4 bin)
{
    vec_uint4 dif, b0, t0;
    vec_uint4 zero = spu_splats(0u);
    vec_uint4 one = spu_splats(1u);
    vec_uchar16 shl4 = SHL4;

    b0 = spu_genbx(a, b, bin);
    dif = spu_subx(a, b, bin);
    t0 = spu_shuffle(b0, one, shl4);
    *d = spu_subx(dif, zero, t0);
}

void bn_sub_n(vec_uint4 *d, vec_uint4 *bout, const vec_uint4 *a, const vec_uint4 *b, vec_uint4 bin, int n)
{
    vec_uint4 borrow = spu_splats(1u);
    int i;

    for (i = 0; i < n; i++) {
        bn_sub_q(&d[i], &borrow, a[i], b[i], bin);
        bin = borrow;
    }
    *bout = borrow;
}

void bn_sub2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 borrow;

    bn_sub_q(&d[0], &borrow, a[0], b[0], spu_splats(1u));
    bn_sub_q(&d[1], &borrow, a[1], b[1], borrow);
}

void bn_sub2p(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 borrow;

    bn_sub_q(&d[0], &borrow, a[0], b[0], spu_splats(1u));
    bn_sub_q1(&d[1], a[1], b[1], borrow);
}

void bn_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 t[2];

    bn_copy(t, a, 2);
    while (bn_cmp(t, m, 2) >= 0)
        bn_sub2p(t, t, m);
    bn_copy(d, t, 2);
}

void bn_add_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m)
{
    vec_uint4 t[2];

    bn_add2p(t, a, b);
    bn_mod2(d, t, m);
}

void bn_sub_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m)
{
    vec_uint4 t[2];

    bn_copy(t, a, 2);
    if (bn_cmp(t, b, 2) < 0)
        bn_add2(t, t, m);
    bn_sub2(d, t, b);
}

void bn_sub_mod2p(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m)
{
    vec_uint4 t[2];

    bn_copy(t, a, 2);
    if (bn_cmp(t, b, 2) < 0)
        bn_add2p(t, t, m);
    bn_sub2p(d, t, b);
}

#define BN_MUL_Q(lo, hi, a, b, c, sl, sh) {                                   \
    vec_uint4 _zero = spu_splats(0u);                                         \
    vec_uint4 _bl = spu_shuffle(b, b, sl);                                    \
    vec_uint4 _bh = spu_shuffle(b, b, sh);                                    \
    vec_uint4 _mid0, _mid1, _low, _high;                                      \
    vec_uint4 _s1, _k1, _s2, _k2, _s3, _k3, _sum, _c0, _c1, _c2, _c3, _t, _ct; \
                                                                              \
    _mid0 = spu_mule((vec_ushort8)(a), (vec_ushort8)_bl);                     \
    _mid1 = spu_mulo((vec_ushort8)(a), (vec_ushort8)_bh);                     \
    _high = spu_mule((vec_ushort8)(a), (vec_ushort8)_bh);                     \
    _low = spu_mulo((vec_ushort8)(a), (vec_ushort8)_bl);                      \
                                                                              \
    _c0 = spu_gencx(_mid0, _mid1, _zero);                                     \
    _sum = spu_addx(_mid0, _mid1, _zero);                                     \
    _c1 = spu_genc(_sum, spu_slqwbyte(_c0, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c0, 4));                               \
    _c2 = spu_genc(_sum, spu_slqwbyte(_c1, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c1, 4));                               \
    _c3 = spu_genc(_sum, spu_slqwbyte(_c2, 4));                               \
    _s1 = spu_add(_sum, spu_slqwbyte(_c2, 4));                                \
    _k1 = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(_c0, _c1), _c2), _c3), -12); \
                                                                              \
    _t = spu_slqwbyte(_high, 4);                                              \
    _c0 = spu_gencx(_low, _t, _zero);                                         \
    _sum = spu_addx(_low, _t, _zero);                                         \
    _c1 = spu_genc(_sum, spu_slqwbyte(_c0, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c0, 4));                               \
    _c2 = spu_genc(_sum, spu_slqwbyte(_c1, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c1, 4));                               \
    _c3 = spu_genc(_sum, spu_slqwbyte(_c2, 4));                               \
    _s2 = spu_add(_sum, spu_slqwbyte(_c2, 4));                                \
    _k2 = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(_c0, _c1), _c2), _c3), -12); \
                                                                              \
    _t = spu_slqwbyte(_s1, 2);                                                \
    _ct = spu_genc(_t, c);                                                    \
    _s3 = spu_add(_t, c);                                                     \
    _c0 = spu_gencx(_s2, _s3, spu_slqwbyte(_ct, 4));                          \
    _sum = spu_addx(_s2, _s3, spu_slqwbyte(_ct, 4));                          \
    _c1 = spu_genc(_sum, spu_slqwbyte(_c0, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c0, 4));                               \
    _c2 = spu_genc(_sum, spu_slqwbyte(_c1, 4));                               \
    _sum = spu_add(_sum, spu_slqwbyte(_c1, 4));                               \
    _c3 = spu_genc(_sum, spu_slqwbyte(_c2, 4));                               \
    lo = spu_add(_sum, spu_slqwbyte(_c2, 4));                                 \
    _k3 = spu_rlmaskqwbyte(spu_add(spu_add(spu_add(spu_add(_ct, _c0), _c1), _c2), _c3), -12); \
                                                                              \
    hi = spu_add(spu_add(spu_add(spu_add(spu_rlmaskqwbyte(_s1, -14), _k3),     \
                                 _k2),                                        \
                         spu_slqwbyte(_k1, 2)),                               \
                 spu_rlmaskqwbyte(_high, -12));                               \
}

void bn_mul_q(vec_uint4 *lo, vec_uint4 *hi, vec_uint4 a, vec_uint4 b, vec_uint4 c, vec_uchar16 sl, vec_uchar16 sh)
{
    BN_MUL_Q(*lo, *hi, a, b, c, sl, sh);
}

void bn_mul_n(vec_uint4 *d, vec_uint4 *cout, const vec_uint4 *a, vec_uint4 b, int n, vec_uchar16 sl, vec_uchar16 sh)
{
    vec_uint4 c = spu_splats(0u);
    vec_uint4 carry = spu_splats(0u);
    int i;

    for (i = 0; i < n; i++) {
        bn_mul_q(&d[i], &carry, a[i], b, c, sl, sh);
        c = carry;
    }
    *cout = carry;
}

void bn_mul2(vec_uint4 *d, const vec_uint4 *a, vec_uint4 b, vec_uchar16 sl, vec_uchar16 sh)
{
    vec_uint4 zero = spu_splats(0u);
    vec_uint4 a0 = a[0], a1 = a[1];
    vec_uint4 c;

    BN_MUL_Q(d[0], c, a0, b, zero, sl, sh);
    BN_MUL_Q(d[1], c, a1, b, c, sl, sh);
}

#define ADD_Q(s, k, x, y) {                                                   \
    vec_uint4 _c0, _c1, _c2, _c3, _s;                                         \
    _c0 = spu_genc(x, y);                                                     \
    _s = spu_add(x, y);                                                       \
    k = _c0;                                                                  \
    _c1 = spu_genc(_s, spu_slqwbyte(_c0, 4));                                 \
    _s = spu_add(_s, spu_slqwbyte(_c0, 4));                                   \
    k = spu_add(k, _c1);                                                      \
    _c2 = spu_genc(_s, spu_slqwbyte(_c1, 4));                                 \
    _s = spu_add(_s, spu_slqwbyte(_c1, 4));                                   \
    k = spu_add(k, _c2);                                                      \
    _c3 = spu_genc(_s, spu_slqwbyte(_c2, 4));                                 \
    s = spu_add(_s, spu_slqwbyte(_c2, 4));                                    \
    k = spu_add(k, _c3);                                                      \
}

#define ADDX_Q(s, k, x, y, cin) {                                             \
    vec_uint4 _c0, _c1, _c2, _c3, _s;                                         \
    _c0 = spu_gencx(x, y, cin);                                               \
    _s = spu_addx(x, y, cin);                                                 \
    k = _c0;                                                                  \
    _c1 = spu_genc(_s, spu_slqwbyte(_c0, 4));                                 \
    _s = spu_add(_s, spu_slqwbyte(_c0, 4));                                   \
    k = spu_add(k, _c1);                                                      \
    _c2 = spu_genc(_s, spu_slqwbyte(_c1, 4));                                 \
    _s = spu_add(_s, spu_slqwbyte(_c1, 4));                                   \
    k = spu_add(k, _c2);                                                      \
    _c3 = spu_genc(_s, spu_slqwbyte(_c2, 4));                                 \
    s = spu_add(_s, spu_slqwbyte(_c2, 4));                                    \
    k = spu_add(k, _c3);                                                      \
}

#define MULW(lo, hi, a, bl, bh) {                                             \
    vec_uint4 _mid0, _mid1, _high, _low, _s1, _k1, _s2, _k2, _k3;             \
    _mid0 = spu_mule((vec_ushort8)(a), bl);                                   \
    _mid1 = spu_mulo((vec_ushort8)(a), bh);                                   \
    _high = spu_mule((vec_ushort8)(a), bh);                                   \
    _low = spu_mulo((vec_ushort8)(a), bl);                                    \
    ADD_Q(_s1, _k1, _mid0, _mid1);                                            \
    ADD_Q(_s2, _k2, _low, spu_slqwbyte(_high, 4));                            \
    ADD_Q(lo, _k3, _s2, spu_slqwbyte(_s1, 2));                                \
    hi = spu_add(spu_add(spu_add(spu_add(spu_rlmaskqwbyte(_s1, -14),          \
                                         spu_rlmaskqwbyte(_k3, -12)),         \
                                 spu_rlmaskqwbyte(_k2, -12)),                 \
                         spu_slqwbyte(spu_rlmaskqwbyte(_k1, -12), 2)),        \
                 spu_rlmaskqwbyte(_high, -12));                               \
}

#define MULW_C(lo, hi, a, bl, bh, c) {                                        \
    vec_uint4 _mid0, _mid1, _high, _low, _s1, _k1, _s2, _k2, _s3, _k3, _t, _ct; \
    _mid0 = spu_mule((vec_ushort8)(a), bl);                                   \
    _mid1 = spu_mulo((vec_ushort8)(a), bh);                                   \
    _high = spu_mule((vec_ushort8)(a), bh);                                   \
    _low = spu_mulo((vec_ushort8)(a), bl);                                    \
    ADD_Q(_s1, _k1, _mid0, _mid1);                                            \
    ADD_Q(_s2, _k2, _low, spu_slqwbyte(_high, 4));                            \
    _t = spu_slqwbyte(_s1, 2);                                                \
    _ct = spu_genc(_t, c);                                                    \
    _s3 = spu_add(_t, c);                                                     \
    ADDX_Q(lo, _k3, _s2, _s3, spu_slqwbyte(_ct, 4));                          \
    hi = spu_add(spu_add(spu_add(spu_add(spu_rlmaskqwbyte(_s1, -14),          \
                                         spu_rlmaskqwbyte(spu_add(_ct, _k3), -12)), \
                                 spu_rlmaskqwbyte(_k2, -12)),                 \
                         spu_slqwbyte(spu_rlmaskqwbyte(_k1, -12), 2)),        \
                 spu_rlmaskqwbyte(_high, -12));                               \
}

void bn_mul2_pair(vec_uint4 *d, vec_uint4 *e, const vec_uint4 *a, vec_uint4 b, const vec_uint4 *f, vec_uint4 g, vec_uchar16 sl, vec_uchar16 sh)
{
    vec_ushort8 bl = (vec_ushort8)spu_shuffle(b, b, sl);
    vec_ushort8 bh = (vec_ushort8)spu_shuffle(b, b, sh);
    vec_ushort8 gl = (vec_ushort8)spu_shuffle(g, g, sl);
    vec_ushort8 gh = (vec_ushort8)spu_shuffle(g, g, sh);
    vec_uint4 a0 = a[0], a1 = a[1], f0 = f[0], f1 = f[1];
    vec_uint4 c, k;

    MULW(d[0], c, a0, bl, bh);
    MULW(e[0], k, f0, gl, gh);
    MULW_C(d[1], c, a1, bl, bh, c);
    MULW_C(e[1], k, f1, gl, gh, k);
}

vec_uint4 bn_shl_q(vec_uint4 x, int n)
{
    return spu_slqw(n > 7 ? spu_slqwbyte(x, n / 8) : x, n % 8);
}

vec_uint4 bn_shr_q(vec_uint4 x, int n)
{
    return spu_rlmaskqw(n > 7 ? spu_rlmaskqwbyte(x, -(n / 8)) : x, -(n % 8));
}

void bn_shl(vec_uint4 *d, const vec_uint4 *a, int bits, int n)
{
    int i;

    if (bits == 0) {
        bn_copy(d, a, n);
        return;
    }
    for (i = n - 1; i > 0; i--)
        d[i] = spu_or(bn_shl_q(a[i], bits), bn_shr_q(a[i - 1], 128 - bits));
    d[0] = bn_shl_q(a[0], bits);
}

#define SPLAT_H(h) ((vec_uchar16)spu_splats((unsigned short)(h)))

void bn_mul_nq(vec_uint4 *d, vec_uint4 *hi, const vec_uint4 *a, vec_uint4 b, int n)
{
    vec_uint4 u[8], t[8], carry;

    bn_zero(t, n + n);
    bn_mul_n(t, &carry, a, b, n, SPLAT_H(0x0e0f), SPLAT_H(0x0c0d));
    t[n] = carry;
    bn_mul_n(u, &carry, a, b, n, SPLAT_H(0x0a0b), SPLAT_H(0x0809));
    u[n] = carry;
    bn_shl(u, u, 32, n + 1);
    bn_add_c(t, &carry, t, u, spu_splats(0u), n + 1);
    bn_mul_n(u, &carry, a, b, n, SPLAT_H(0x0607), SPLAT_H(0x0405));
    u[n] = carry;
    bn_shl(u, u, 64, n + 1);
    bn_add_c(t, &carry, t, u, spu_splats(0u), n + 1);
    bn_mul_n(u, &carry, a, b, n, SPLAT_H(0x0203), SPLAT_H(0x0001));
    u[n] = carry;
    bn_shl(u, u, 96, n + 1);
    bn_add_c(t, &carry, t, u, spu_splats(0u), n + 1);
    bn_copy(d, t, n);
    *hi = t[n];
}

void bn_mul2x2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 u[3], t[4], hi;

    bn_zero(t, 4);
    bn_mul_nq(t, &hi, a, b[0], 2);
    t[2] = hi;
    bn_mul_nq(u, &hi, a, b[1], 2);
    u[2] = hi;
    bn_add_c(&t[1], &hi, &t[1], u, spu_splats(0u), 3);
    bn_copy(d, t, 4);
}

void bn_shr(vec_uint4 *d, const vec_uint4 *a, int bits, int n)
{
    int i;

    if (bits == 0) {
        bn_copy(d, a, n);
        return;
    }
    for (i = 0; i < n - 1; i++)
        d[i] = spu_or(bn_shr_q(a[i], bits), bn_shl_q(a[i + 1], 128 - bits));
    d[n - 1] = bn_shr_q(a[n - 1], bits);
}

void bn_mod2_full(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 t[2];

    bn_copy(t, a, 2);
    while (bn_cmp(t, m, 2) >= 0)
        bn_sub2(t, t, m);
    bn_copy(d, t, 2);
}

void bn_mod4(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 t[4], s[4], borrow;
    int i;

    bn_copy(t, a, 4);
    bn_zero(s, 2);
    bn_copy(&s[2], m, 2);
    bn_shr(s, s, 95, 4);
    for (i = 161; i > 0; i--) {
        if (bn_cmp(t, s, 4) >= 0)
            bn_sub_n(t, &borrow, t, s, spu_splats(1u), 4);
        bn_shr(s, s, 1, 4);
    }
    bn_mod2_full(d, t, m);
}

void bn_div_mod2(vec_uint4 *d, const vec_uint4 *b, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 u[2], v[2], x1[2], x2[2];

    if (bn_is_zero(a, 2))
        return;
    bn_copy(u, a, 2);
    bn_copy(v, m, 2);
    bn_copy(x1, b, 2);
    bn_zero(x2, 2);
    while (bn_cmp(u, v, 2) != 0) {
        if (bn_is_even(u)) {
            bn_shr(u, u, 1, 2);
            if (!bn_is_even(x1))
                bn_add2(x1, x1, m);
            bn_shr(x1, x1, 1, 2);
        } else if (bn_is_even(v)) {
            bn_shr(v, v, 1, 2);
            if (!bn_is_even(x2))
                bn_add2(x2, x2, m);
            bn_shr(x2, x2, 1, 2);
        } else if (bn_cmp(u, v, 2) > 0) {
            bn_sub2(u, u, v);
            bn_shr(u, u, 1, 2);
            bn_sub_mod2(x1, x1, x2, m);
            if (!bn_is_even(x1))
                bn_add2(x1, x1, m);
            bn_shr(x1, x1, 1, 2);
        } else {
            bn_sub2(v, v, u);
            bn_shr(v, v, 1, 2);
            bn_sub_mod2(x2, x2, x1, m);
            if (!bn_is_even(x2))
                bn_add2(x2, x2, m);
            bn_shr(x2, x2, 1, 2);
        }
    }
    bn_copy(d, x1, 2);
}

void bn_inv_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 one[2];

    one[0] = (vec_uint4){0, 0, 0, 1};
    one[1] = spu_splats(0u);
    bn_div_mod2(d, one, a, m);
}

unsigned int bn_inv_word(unsigned int a)
{
    unsigned int y = 1;
    int i;

    for (i = 2; i < 32; i++)
        if (a * y % (1u << i) > 1u << (i - 1))
            y += 1u << (i - 1);
    if (a * y > 0x80000000u)
        y += 0x80000000u;
    return y;
}

unsigned int bn_mont_word(unsigned int a)
{
    return -bn_inv_word(a);
}

void bn_mont_mul2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m, unsigned int n0)
{
    vec_uint4 t[2], p[2], q[2];
    vec_uint4 s, k, s0, k0, s1, k1;
    unsigned int b0 = spu_extract(b[0], 3);
    unsigned int ai, u;
    int i;

    bn_zero(t, 2);
    for (i = 0; i < 5; i++) {
        ai = spu_extract(a[i / 4], 3 - i % 4);
        u = (spu_extract(t[0], 3) + ai * b0) * n0;
        bn_mul2_pair(p, q, b, (vec_uint4){0, 0, 0, ai}, m, (vec_uint4){0, 0, 0, u},
                     SPLAT_H(0x0e0f), SPLAT_H(0x0c0d));
        ADD_Q(s, k, t[0], p[0]);
        ADDX_Q(s1, k1, t[1], p[1], spu_rlmaskqwbyte(k, -12));
        ADD_Q(s0, k0, s, q[0]);
        ADDX_Q(s1, k1, s1, q[1], spu_rlmaskqwbyte(k0, -12));
        t[0] = spu_or(spu_rlmaskqwbyte(s0, -4), spu_slqwbyte(s1, 12));
        t[1] = spu_rlmaskqwbyte(s1, -4);
    }
    if (bn_cmp(t, m, 2) >= 0)
        bn_sub2(t, t, m);
    bn_copy(d, t, 2);
}

void bn_mont_reduce2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m, unsigned int n0)
{
    vec_uint4 t[2], p[2];
    int i;

    bn_copy(t, a, 2);
    for (i = 0; i < 5; i++) {
        bn_mul2(p, m, (vec_uint4){0, 0, 0, spu_extract(t[0], 3) * n0}, SPLAT_H(0x0e0f), SPLAT_H(0x0c0d));
        bn_add2p(t, t, p);
        bn_shr(t, t, 32, 2);
    }
    if (bn_cmp(t, m, 2) >= 0)
        bn_sub2(t, t, m);
    bn_copy(d, t, 2);
}

void bn_to_mont2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    vec_uint4 t[2];
    int i;

    bn_copy(t, a, 2);
    for (i = 0; i < 160; i++) {
        bn_shl(t, t, 1, 2);
        bn_mod2_full(t, t, m);
    }
    bn_copy(d, t, 2);
}

void bn_mul_mod2(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *b, const vec_uint4 *m)
{
    vec_uint4 p[4], r[2];

    bn_zero(p, 4);
    bn_mul2x2(p, a, b);
    bn_mod4(r, p, m);
    bn_copy(d, r, 2);
}

int bn_mod4_is_zero(vec_uint4 *d, const vec_uint4 *a, const vec_uint4 *m)
{
    bn_mod4(d, a, m);
    return bn_is_zero(d, 2);
}

unsigned int bn_load_be32(const unsigned char *p)
{
    unsigned int w;

    w = *p++ << 24;
    w |= *p++ << 16;
    w |= *p++ << 8;
    w |= *p;
    return w;
}

void bn_from_bytes(vec_uint4 *d, const unsigned char *p)
{
    vec_uchar16 v = (vec_uchar16)spu_splats(0u);

    v = spu_insert(*p++, v, 12);
    v = spu_insert(*p++, v, 13);
    v = spu_insert(*p++, v, 14);
    v = spu_insert(*p++, v, 15);
    d[1] = (vec_uint4)v;
    v = spu_insert(*p++, v, 0);
    v = spu_insert(*p++, v, 1);
    v = spu_insert(*p++, v, 2);
    v = spu_insert(*p++, v, 3);
    v = spu_insert(*p++, v, 4);
    v = spu_insert(*p++, v, 5);
    v = spu_insert(*p++, v, 6);
    v = spu_insert(*p++, v, 7);
    v = spu_insert(*p++, v, 8);
    v = spu_insert(*p++, v, 9);
    v = spu_insert(*p++, v, 10);
    v = spu_insert(*p++, v, 11);
    v = spu_insert(*p++, v, 12);
    v = spu_insert(*p++, v, 13);
    v = spu_insert(*p++, v, 14);
    v = spu_insert(*p++, v, 15);
    d[0] = (vec_uint4)v;
}

void bn_from_bytes21(vec_uint4 *d, const unsigned char *p)
{
    vec_uchar16 v = (vec_uchar16)spu_splats(0u);

    v = spu_insert(*p++, v, 11);
    v = spu_insert(*p++, v, 12);
    v = spu_insert(*p++, v, 13);
    v = spu_insert(*p++, v, 14);
    v = spu_insert(*p++, v, 15);
    d[1] = (vec_uint4)v;
    v = spu_insert(*p++, v, 0);
    v = spu_insert(*p++, v, 1);
    v = spu_insert(*p++, v, 2);
    v = spu_insert(*p++, v, 3);
    v = spu_insert(*p++, v, 4);
    v = spu_insert(*p++, v, 5);
    v = spu_insert(*p++, v, 6);
    v = spu_insert(*p++, v, 7);
    v = spu_insert(*p++, v, 8);
    v = spu_insert(*p++, v, 9);
    v = spu_insert(*p++, v, 10);
    v = spu_insert(*p++, v, 11);
    v = spu_insert(*p++, v, 12);
    v = spu_insert(*p++, v, 13);
    v = spu_insert(*p++, v, 14);
    v = spu_insert(*p++, v, 15);
    d[0] = (vec_uint4)v;
}

void bn_to_words(unsigned int *w, const vec_uint4 *a)
{
    w[5] = spu_extract(a[1], 2);
    w[4] = spu_extract(a[1], 3);
    w[3] = spu_extract(a[0], 0);
    w[2] = spu_extract(a[0], 1);
    w[1] = spu_extract(a[0], 2);
    w[0] = spu_extract(a[0], 3);
}

void bn_from_bytes_not(vec_uint4 *d, const unsigned char *p)
{
    unsigned char buf[20];
    int i;

    for (i = 0; i < 20; i++) {
        unsigned char *q = buf + i;
        *q = ~*p++;
    }
    bn_from_bytes(d, buf);
}

void bn_from_bytes21_not(vec_uint4 *d, const unsigned char *p)
{
    unsigned char buf[21];
    int i;

    for (i = 0; i < 21; i++) {
        unsigned char *q = buf + i;
        *q = ~*p++;
    }
    bn_from_bytes21(d, buf);
}
