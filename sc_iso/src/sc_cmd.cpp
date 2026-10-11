#include "sc_cmd.h"

sc_cmd::sc_cmd()
{
}

unsigned int sc_cmd::set(const quad_uint &, const quad_uint &)
{
    return 10;
}

unsigned int sc_cmd::make(const quad_ptr &, const quad_uint &, quad_uint &, quad_uint &,
                          const unsigned int &, void *)
{
    return 10;
}

unsigned int sc_cmd::check(const quad_ptr &, const quad_uint &, const unsigned int &, void *)
{
    return 10;
}

unsigned int sc_cmd::get(const quad_ptr &, const quad_uint &, quad_uint &)
{
    return 10;
}

sc_cmd::~sc_cmd()
{
}
