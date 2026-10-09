#include "ec.h"

extern const unsigned char ec_curves[][121];

void ec_curve_params(vec_uint4 *p, vec_uint4 *a, vec_uint4 *b, vec_uint4 *n, unsigned int id)
{
    const unsigned char *c = ec_curves[id];

    bn_from_bytes_not(p, c);
    bn_from_bytes_not(a, c + 20);
    bn_from_bytes_not(b, c + 40);
    bn_from_bytes21_not(n, c + 60);
}

void ec_curve_base(vec_uint4 *G, unsigned int id)
{
    const unsigned char *c = ec_curves[id];

    bn_from_bytes_not(G, c + 81);
    bn_from_bytes_not(G + 2, c + 101);
}
