#include <spu_intrinsics.h>
#include "aim.h"
#include "util.h"

#define TAG_EID0        7

struct idps_view {
    unsigned int id_hi : 16, company : 16;
    unsigned int product : 16, sub_product : 16;
    unsigned int board : 6, rest : 26;
};

unsigned int read_id_type(eid0_reader *eid, const quad_u64 &ea, const quad_uint &size,
                          const quad_uint &ls, const quad_uint &unused)
{
    unsigned int type __attribute__((aligned(16)));
    unsigned int r;

    r = eid->m_dma->issue(ls, ea, 32, TAG_EID0, 0, MFC_GET);
    if (r)
        return 15;
    eid->m_dma->wait(TAG_EID0);
    memcpy(&type, (const unsigned char *)ls + 16, 4);
    if (type - 1 > 3)
        return 15;
    eid->m_section = type;
    return r;
}

unsigned int put_id(eid0_reader *eid, const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                    const quad_uint &unused, const quad_ptr &buf, const quad_uint &buf_size)
{
    u16 code[4] __attribute__((aligned(16)));
    key128 type;
    const idps_view *idps = (const idps_view *)(unsigned char *)buf;
    unsigned int r;

    memset((void *)ls, 0, 16);
    if (eid->m_section == AIM_DEVICE_TYPE) {
        type.hi = 0;
        type.lo = idps->product;
        memcpy((void *)ls, &type, 16);
        r = eid->m_dma->issue(ls, ea, 16, TAG_EID0, 0, MFC_PUT);
    } else if (eid->m_section == AIM_DEVICE_ID || eid->m_section == AIM_OPEN_PS_ID) {
        memcpy((void *)ls, idps, 16);
        r = eid->m_dma->issue(ls, ea, 16, TAG_EID0, 0, MFC_PUT);
    } else if (eid->m_section == AIM_PS_CODE) {
        code[0] = idps->company;
        code[1] = idps->product;
        code[2] = idps->sub_product;
        code[3] = idps->board;
        memcpy((void *)ls, code, 8);
        r = eid->m_dma->issue(ls, ea, 8, TAG_EID0, 0, MFC_PUT);
    } else
        return 15;
    if (r)
        return 15;
    eid->m_dma->wait(TAG_EID0);
    spu_stop(0x100);
    return r;
}
