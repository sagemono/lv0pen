#include "sb.h"

systemio_dmac_dx::systemio_dmac_dx() : base(0), ioid(0), dev_addr(0), desc(0), desc_io(0)
{
}

systemio_dmac_dx *get_systemio_dmac_dx()
{
    static systemio_dmac_dx dmac;
    return &dmac;
}
