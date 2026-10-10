#include <spu_intrinsics.h>
#include "utoken.h"
#include "util.h"

const quad_uint g_eid0_ls = 0x3E000;

static const unsigned char g_default_idps[16] __attribute__((aligned(16))) = {
    0x00, 0x00, 0x00, 0x01, 0x00, 0x81, 0x00, 0x01, 0x03, 0xFF, 0xFF, 0xFF, 0x18, 0x43, 0xC1, 0x4D,
};

static const unsigned char g_token_key[32] __attribute__((aligned(16))) = {
    0xD1, 0xCB, 0x1C, 0x81, 0xAC, 0xE3, 0x5F, 0x3D, 0x97, 0x0D, 0xDE, 0x72, 0x3A, 0x62, 0x29, 0x35,
    0x51, 0x6F, 0x98, 0xD0, 0xF0, 0xDB, 0x3E, 0x15, 0x1D, 0xE2, 0xB7, 0xA2, 0xE3, 0x4B, 0xD7, 0x36,
};
static const unsigned char g_token_hmac_key[64] __attribute__((aligned(16))) = {
    0x57, 0x2C, 0x98, 0x77, 0x47, 0xA4, 0xA0, 0xA6, 0xA1, 0xE7, 0x15, 0x96, 0x3D, 0x0D, 0xCC, 0xCA,
    0x28, 0xA8, 0xA9, 0x4B, 0x5B, 0x52, 0x94, 0x72, 0xEF, 0x1A, 0x4E, 0xFF, 0xEB, 0x29, 0x78, 0xF9,
    0x9B, 0xD0, 0xA9, 0xD4, 0x24, 0x38, 0xDB, 0x73, 0x1B, 0x44, 0x3C, 0x9D, 0xC7, 0x94, 0x4A, 0x13,
    0xAC, 0x7B, 0x40, 0xFC, 0xA5, 0x7D, 0xFE, 0x33, 0xD2, 0x12, 0xFB, 0xA8, 0x6C, 0xBE, 0xBC, 0xBA,
};
unsigned char g_token_iv[16] __attribute__((aligned(16))) = {
    0xA6, 0x52, 0x3E, 0x54, 0x26, 0x47, 0x09, 0x53, 0xFE, 0x8C, 0x90, 0xF6, 0x1B, 0xCA, 0x92, 0x7A,
};

unsigned char g_token[UTOKEN_SIZE] __attribute__((aligned(16)));

int read_params(u64 ea, u64 size, utoken_params *p)
{
    dma_channel dma;
    int r = DMA_EINVAL;

    if (size == sizeof *p) {
        u64 ls = (ea & 0x7F) + LS_READ;

        r = 16;
        if (dma.issue(ls, ea, sizeof *p, TAG_READ, 0, MFC_GET) == 0) {
            dma.wait(TAG_READ);
            memcpy(p, (const void *)(unsigned int)ls, sizeof *p);
            r = 0;
        }
    }
    return r;
}

int main(u64 ea, u64 size, u64 arg3, u64 arg4, key128 a, key128 iv, key128 key_hi,
         key128 key_lo)
{
    utoken_params params;
    unsigned char idps[16] __attribute__((aligned(16)));
    unsigned char eid0[0x860] __attribute__((aligned(16)));
    int r;

    memset(idps, 0, sizeof idps);
    memset(eid0, 0, sizeof eid0);
    memset(g_token, 0, sizeof g_token);
    if (read_params(ea, size, &params))
        spu_stop(0x101);
    switch (params.mode) {
    case UTOKEN_SEAL:
    case UTOKEN_OPEN:
        if (!(params.arg5 != 0 && params.arg6 != 0 && params.eid0_ea != 0 && params.eid0_size != 0 &&
              params.token_ea != 0 && params.token_size != 0 && params.out_ea != 0 && params.out_size != 0))
            spu_stop(0x101);
        break;
    default:
        spu_stop(0x101);
    }

    quad_u64 eid0_ea = params.eid0_ea;
    quad_uint eid0_size = params.eid0_size;
    quad_ptr buf = eid0;
    quad_uint buf_size = sizeof eid0;
    dma_channel dma;
    eid0_reader reader(&dma, a, iv, key_hi, key_lo);

    if (eid0_size == sizeof eid0) {
        if (reader.read(eid0_ea, eid0_size, g_eid0_ls, 0x1000, buf, buf_size))
            spu_stop(0x101);
        memcpy(idps, buf, 16);
    } else {
        memcpy(idps, g_default_idps, 16);
    }
    utoken token(g_token_key, g_token_iv, g_token_hmac_key, idps);

    switch (params.mode) {
    case UTOKEN_SEAL:
        r = token.seal(&params, g_token);
        if (r == 6)
            spu_stop(0x108);
        else if (r == 8)
            spu_stop(0x10C);
        else if (r == 27)
            spu_stop(0x10A);
        else if (r != 0)
            spu_stop(0x104);
        break;
    case UTOKEN_OPEN:
        r = token.open(&params, g_token);
        if (r == 6)
            spu_stop(0x108);
        else if (r == 8)
            spu_stop(0x10C);
        else if (r == 27)
            spu_stop(0x10A);
        else if (r != 0)
            spu_stop(0x104);
        break;
    default:
        spu_stop(0x101);
        break;
    }
    spu_stop(0x100);
    return 0x100;
}
