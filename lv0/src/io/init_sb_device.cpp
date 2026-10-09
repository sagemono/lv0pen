#include "iommu.h"
#include "sb.h"

struct iopt_desc { long w[6]; };

void sb_device::initialize(long sb_base, int init_hw)
{
    base = sb_base;
    if (init_hw) {
        iopt_codec codec;
        struct iopt_desc iopt_desc;
        const long *tmpl = sb_device_iopt_desc_template;

        sb_dx_write32_1f60(sb_base, 0x10004BA);
        sb_dx_write32_1c70(base, 0xFF00000);
        sb_dx_write32_1c74(base, 0x100000);
        iopt_desc.w[0] = tmpl[0];
        iopt_desc.w[1] = tmpl[1];
        iopt_desc.w[2] = tmpl[2];
        iopt_desc.w[3] = tmpl[3];
        iopt_desc.w[4] = tmpl[4];
        iopt_desc.w[5] = tmpl[5];
        codec.pack_entry(base, 6, &iopt_desc);
    }
    *(char *)(base + 0x1000038) = -1;
    *(char *)(base + 0x1000028) = 0;
}
