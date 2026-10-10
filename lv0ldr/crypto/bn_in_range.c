#include "ec.h"

int bn_in_range(const vec_uint4 *a, const vec_uint4 *b)
{
    if (bn_is_zero(a, 2) == 1)
        return 0;
    if (bn_cmp(a, b, 2) == -1)
        return 1;
    return 0;
}
