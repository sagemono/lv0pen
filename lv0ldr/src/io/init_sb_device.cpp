#include "sb.h"
#include "iopt.h"

void sb_device::initialize(u64 sb_base, bool init_hw)
{
    base = sb_base;
    if (init_hw) {
        iopt_codec codec;
        iopt_desc desc;

        sb_dx_write32_1f60(sb_base, 0x10004BA);
        sb_dx_write32_1c70(base, 0xFF00000);
        sb_dx_write32_1c74(base, 0x100000);
        desc.addr = 0x1000000;
        desc.w[2] = 0;
        desc.w[3] = 3;
        desc.w[4] = 3;
        desc.w[5] = 0;
        desc.w[6] = 0;
        desc.w[7] = 0;
        desc.w[8] = 0;
        desc.w[9] = 0;
        desc.w[10] = 1;
        desc.w[11] = 2;
        codec.pack_entry(base, 6, &desc);
    }
    write8(base + 0x1000038, 0xFF);
    write8(base + 0x1000028, 0);
}
