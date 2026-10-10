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
    if (m_section == 4) {
        if (aes_encrypt_block(&section_key, (const vec_uchar16 *)eid0_seed_a, rk, 14))
            return 21;
    } else {
        if (aes_encrypt_block(&section_key, (const vec_uchar16 *)eid0_seed_0, rk, 14))
            return 21;
    }
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
