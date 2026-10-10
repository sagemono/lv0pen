#include <spu_intrinsics.h>
#include "aim.h"
#include "util.h"

bool is_valid_ea(u64 ea)
{
    if (ea == 0)
        return false;
    return (ea & 15) == 0;
}

void stop_on_error(unsigned int r)
{
    switch (r) {
    case 9:
        spu_stop(0x101);
        break;
    case 20:
        spu_stop(0x104);
        break;
    case 10:
        spu_stop(0x107);
        break;
    default:
        spu_stop(0x103);
        break;
    }
}

int main(u64 dma_ea, u64 dma_size, u64 eid0_ea, u64 eid0_size, key128 arg4, key128 eid_iv,
         key128 eid_key_hi, key128 eid_key_lo)
{
    bool bad = false;
    unsigned char eid0[0x860] __attribute__((aligned(16)));
    unsigned int r;
    quad_uint size;
    quad_u64 eid_ea;
    quad_uint eid_size;
    quad_uint ls;
    quad_uint unused;
    quad_u64 ea;
    quad_ptr buf;
    quad_uint buf_size;

    get_log()->init(dma_ea, dma_size);
    get_log()->puts("(spu)start aim spu module!\n");

    ea = dma_ea;
    size = dma_size;
    eid_ea = eid0_ea;
    eid_size = eid0_size;
    ls = 0x3E000;
    unused = 0x1000;
    buf = eid0;
    buf_size = sizeof(eid0);
    dma_channel dma;

    if (!is_valid_ea(ea)) {
        get_log()->puts("(spu) PU DMA area start address is not align 16byte\n");
        bad = true;
    }
    if (!is_valid_ea(eid_ea)) {
        bad = true;
        get_log()->puts("(spu) PU EID area start address is not align 16byte\n");
    }
    if (size != AIM_DMA_SIZE) {
        get_log()->puts("(spu) PU DMA area size is not equall to AIM_DMA_SIZE\n");
        stop_on_error(9);
    } else if (bad) {
        stop_on_error(9);
    } else {
        eid0_reader eid(&dma, arg4, eid_iv, eid_key_hi, eid_key_lo);

        r = read_id_type(&eid, ea, size, ls, unused);
        if (r) {
            stop_on_error(r);
            return -1;
        }
        r = eid.read(eid_ea, eid_size, ls, unused, buf, buf_size);
        if (r) {
            stop_on_error(r);
            return -1;
        }
        r = put_id(&eid, ea, size, ls, unused, buf, buf_size);
        if (r) {
            stop_on_error(r);
            return -1;
        }
        spu_stop(0x103);
    }
    return -1;
}
