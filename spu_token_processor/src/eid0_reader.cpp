#include <spu_intrinsics.h>
#include "eid0_reader.h"
#include "aes.h"
#include "util.h"

extern "C" int aes_cmac(vec_uchar16 *mac, const vec_uchar16 *in, int len, const unsigned char *key, int bits);
int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

static const unsigned char eid0_seed_a[16] __attribute__((aligned(16))) = {
    0x30, 0xb0, 0x39, 0x5d, 0xc5, 0x83, 0x5a, 0xaa, 0x3a, 0x79, 0x86, 0xb4, 0x4a, 0xfa, 0xe6, 0x84,
};
static const unsigned char eid0_seed_0[16] __attribute__((aligned(16))) = {
    0x2e, 0xd7, 0xce, 0x8d, 0x1d, 0x55, 0x45, 0x45, 0x85, 0xbf, 0x6a, 0x32, 0x81, 0xcd, 0x03, 0xaf,
};

#define TAG_EID0        7
#define EID0_SECTION    0xC0
#define EID0_CMAC       0xA8

eid0_reader::~eid0_reader()
{
}

eid0_reader::eid0_reader(dma_channel *dma, key128 arg, key128 iv, key128 key_hi, key128 key_lo)
{
    m_dma = dma;
    memcpy(&m_arg, &arg, 16);
    memcpy(&m_iv, &iv, 16);
    memcpy(&m_key[0], &key_hi, 16);
    memcpy(&m_key[1], &key_lo, 16);
}

struct idps_view {
    unsigned int id_hi : 16, company : 16;
    unsigned int product : 16, sub_product : 16;
    unsigned int board : 6, rest : 26;
};

unsigned int eid0_reader::read_id_type(const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                                       const quad_uint &unused)
{
    unsigned int type __attribute__((aligned(16)));
    unsigned int r;

    r = m_dma->issue(ls, ea, 32, TAG_EID0, 0, MFC_GET);
    if (r)
        return 15;
    m_dma->wait(TAG_EID0);
    memcpy(&type, (const unsigned char *)ls + 16, 4);
    if (type - 1 > 3)
        return 15;
    m_section = type;
    return r;
}

unsigned int eid0_reader::read(const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                               const quad_uint &unused, const quad_ptr &buf, const quad_uint &buf_size)
{
    vec_uchar16 mac;
    vec_uchar16 section_key;
    vec_uchar16 iv;
    unsigned char key[32] __attribute__((aligned(16)));
    vec_uchar16 rk[15];
    unsigned char *section;
    unsigned int r;
    int err;

    if (size < 0xE0)
        return 15;
    r = m_dma->issue(ls, ea, size, TAG_EID0, 0, MFC_GET);
    if (r)
        return 15;
    m_dma->wait(TAG_EID0);
    memcpy(key, &m_key[0], 16);
    memcpy(key + 16, &m_key[1], 16);
    memcpy(&iv, &m_iv, 16);
    if (aes_set_encrypt_key(rk, key, 256) != 14)
        return 21;
    if (m_section == 4)
        err = aes_encrypt_block(&section_key, (const vec_uchar16 *)eid0_seed_a, rk, 14);
    else
        err = aes_encrypt_block(&section_key, (const vec_uchar16 *)eid0_seed_0, rk, 14);
    if (err)
        return 21;
    if (m_section == 4)
        memcpy(buf, (const unsigned char *)ls + 0x7A0, EID0_SECTION);
    else
        memcpy(buf, (const unsigned char *)ls + 0x20, EID0_SECTION);
    if (aes_cbc_decrypt((vec_uchar16 *)buf, (const vec_uchar16 *)buf, EID0_SECTION,
                        (const unsigned char *)&section_key, 128, &iv))
        return 21;
    section = buf;
    if (aes_cmac(&mac, (const vec_uchar16 *)section, EID0_CMAC, (const unsigned char *)&section_key, 128))
        return 21;
    if (bytes_differ(section + EID0_CMAC, (const unsigned char *)&mac, 16))
        return 20;
    return r;
}

unsigned int eid0_reader::put_id(const quad_u64 &ea, const quad_uint &size, const quad_uint &ls,
                                 const quad_uint &unused, const quad_ptr &buf, const quad_uint &buf_size)
{
    u16 code[4] __attribute__((aligned(16)));
    key128 type;
    const idps_view *idps = (const idps_view *)(unsigned char *)buf;
    unsigned int r;

    memset((void *)ls, 0, 16);
    if (m_section == ID_DEVICE_TYPE) {
        type.hi = 0;
        type.lo = idps->product;
        memcpy((void *)ls, &type, 16);
        r = m_dma->issue(ls, ea, 16, TAG_EID0, 0, MFC_PUT);
    } else if (m_section == ID_DEVICE_ID || m_section == ID_OPEN_PS_ID) {
        memcpy((void *)ls, idps, 16);
        r = m_dma->issue(ls, ea, 16, TAG_EID0, 0, MFC_PUT);
    } else if (m_section == ID_PS_CODE) {
        code[0] = idps->company;
        code[1] = idps->product;
        code[2] = idps->sub_product;
        code[3] = idps->board;
        memcpy((void *)ls, code, 8);
        r = m_dma->issue(ls, ea, 8, TAG_EID0, 0, MFC_PUT);
    } else
        return 15;
    if (r)
        return 15;
    m_dma->wait(TAG_EID0);
    spu_stop(0x100);
    return r;
}
