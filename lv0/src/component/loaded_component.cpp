#include "component.h"

loaded_component::loaded_component() : id(19), size(0), address(0)
{
}

loaded_component::~loaded_component()
{
}

int loaded_component::get(unsigned int id, unsigned long *out_addr, unsigned long *out_size)
{
    if (id != this->id)
        return -56;
    if (size == 0)
        return -55;
    if (address == 0)
        return -55;
    *out_addr = address;
    if (out_size)
        *out_size = size;
    return 0;
}

int loaded_component::initialize(unsigned int id, unsigned long address)
{
    this->id = id;
    this->address = address;
    return 0;
}
