#include "ec.h"

int ec_curve_valid(unsigned int id)
{
    if (id < 64)
        return 1;
    return 0;
}
