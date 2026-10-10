#include "ec.h"

int bn_less(const vec_uint4 *a, const vec_uint4 *b)
{
    if (bn_cmp(a, b, 2) == -1)
        return 1;
    return 0;
}

int ec_on_curve(const vec_uint4 *P, const vec_uint4 *p, const vec_uint4 *a, const vec_uint4 *b)
{
    vec_uint4 t[2], u[2], x[2];

    bn_copy(x, P, 2);
    bn_mul_mod2(t, x, x, p);
    bn_add_mod2(t, t, a, p);
    bn_mul_mod2(t, x, t, p);
    bn_add_mod2(t, t, b, p);
    bn_mul_mod2(u, P + 2, P + 2, p);
    if (bn_cmp(u, t, 2) == 0)
        return 1;
    return 0;
}
