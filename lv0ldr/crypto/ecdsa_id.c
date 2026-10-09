#include "ec.h"

int ecdsa_verify_id(const unsigned char *sig, const unsigned char *hash, const unsigned char *pub, unsigned int id)
{
    vec_uint4 p[2], a[2], b[2], n[2], G[6];

    if (!ec_curve_valid(id))
        return -1;
    ec_curve_params(p, a, b, n, id);
    ec_curve_base(G, id);
    return ecdsa_verify(sig, hash, pub, p, a, b, n, G);
}
