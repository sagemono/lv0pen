#include "ec.h"

void ec_mul2(vec_uint4 *R, const vec_uint4 *G, const vec_uint4 *Q, const unsigned int *k1, const unsigned int *k2,
             const vec_uint4 *a, const vec_uint4 *p, unsigned int n0)
{
    vec_uint4 GQ[6], A[6];
    int i, b1, b2;

    ec_copy(GQ, G);
    ec_add(GQ, Q, a, p, n0);
    ec_set_inf(A);
    b1 = ec_bits(k1, 6) - 1;
    b2 = ec_bits(k2, 6) - 1;
    for (i = b1 > b2 ? b1 : b2; i >= 0; i--) {
        ec_double(A, a, p, n0);
        if (!ec_bit(k1, i)) {
            if (ec_bit(k2, i))
                ec_add(A, Q, a, p, n0);
        } else if (!ec_bit(k2, i))
            ec_add(A, G, a, p, n0);
        else
            ec_add(A, GQ, a, p, n0);
    }
    ec_copy(R, A);
}
