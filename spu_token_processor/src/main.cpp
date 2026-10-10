#include <spu_intrinsics.h>
#include "token.h"
#include "util.h"

static const quad_uint g_eid_ls = 0x3E000;

static const unsigned char g_default_idps[16] __attribute__((aligned(16))) = {
    0x00, 0x00, 0x00, 0x01, 0x00, 0x81, 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0x18, 0x43, 0xc1, 0x4d,
};

static const unsigned char g_token_key[32] __attribute__((aligned(16))) = {
    0x34, 0x18, 0x12, 0x37, 0x62, 0x91, 0x37, 0x1c, 0x8b, 0xc7, 0x56, 0xff, 0xfc, 0x61, 0x15, 0x25,
    0x40, 0x3f, 0x95, 0xa8, 0xef, 0x9d, 0x0c, 0x99, 0x64, 0x82, 0xee, 0xc2, 0x16, 0xb5, 0x62, 0xed,
};

static const unsigned char g_token_hmac_key[64] __attribute__((aligned(16))) = {
    0xcc, 0x30, 0xc4, 0x22, 0x91, 0x13, 0xdb, 0x25, 0x73, 0x35, 0x53, 0xaf, 0xd0, 0x6e, 0x87, 0x62,
    0xb3, 0x72, 0x9d, 0x9e, 0xfa, 0xa6, 0xd5, 0xf3, 0x5a, 0x6f, 0x58, 0xbf, 0x38, 0xff, 0x8b, 0x5f,
    0x58, 0xa2, 0x5b, 0xd9, 0xc9, 0xb5, 0x0b, 0x01, 0xd1, 0xab, 0x40, 0x28, 0x67, 0x69, 0x68, 0xea,
    0xc7, 0xf8, 0x88, 0x33, 0xb6, 0x62, 0x93, 0x5d, 0x75, 0x06, 0xa6, 0xb5, 0xe0, 0xf9, 0xd9, 0x7a,
};

static const unsigned char g_token_pub[40] __attribute__((aligned(16))) = {
    0xa5, 0x54, 0x76, 0xc9, 0xe6, 0xdf, 0xb8, 0x90, 0xfa, 0xaf, 0x5f, 0xbf, 0xfd, 0x96, 0x1b, 0x64,
    0x9d, 0x0a, 0xbf, 0x1d, 0x0c, 0xd7, 0x60, 0x00, 0xbd, 0x4b, 0x5f, 0x5a, 0xfe, 0x0a, 0xb8, 0x7d,
    0xce, 0xa2, 0x21, 0xe2, 0x52, 0xa3, 0x74, 0x04,
};

unsigned char g_token_iv[16] __attribute__((aligned(16))) = {
    0xe8, 0x66, 0x3a, 0x69, 0xcd, 0x1a, 0x5c, 0x45, 0x4a, 0x76, 0x1e, 0x72, 0x8c, 0x7c, 0x25, 0x4e,
};

unsigned int g_token_curve = 9;

#define LS_REQUEST      0x3E000ULL

int read_request(u64 ea, u64 size, token_request *req)
{
    dma_channel dma;
    unsigned int ls;
    int r = 9;

    if (size == sizeof(token_request)) {
        ls = LS_REQUEST + (ea & 127);
        r = 16;
        if (dma.issue(ls, ea, sizeof(token_request), 1, 0, MFC_GET) == 0) {
            dma.wait(1);
            memcpy(req, (void *)ls, sizeof(token_request));
            r = 0;
        }
    }
    return r;
}

int main(u64 ea, u64 size, u64 arg2, u64 arg3, key128 arg4, key128 eid_iv, key128 eid_key_hi,
         key128 eid_key_lo)
{
    token_request req;
    unsigned char idps[16] __attribute__((aligned(16)));
    unsigned char eid0[0x860] __attribute__((aligned(16)));
    unsigned char result[80] __attribute__((aligned(16)));
    unsigned int r;

    memset(idps, 0, 16);
    memset(eid0, 0, 0x860);
    if (read_request(ea, size, &req))
        spu_stop(0x101);
    switch (req.cmd) {
    case CMD_UPDATE:
        if (!(req.unknown_ea != 0 && req.unknown_size != 0 && req.eid0_ea != 0 && req.eid0_size != 0 &&
              req.token_ea != 0 && req.token_size != 0 && req.result_ea != 0 && req.result_size != 0))
            spu_stop(0x101);
        break;
    case CMD_VERIFY:
        if (!(req.unknown_ea != 0 && req.unknown_size != 0 && req.eid0_ea != 0 && req.eid0_size != 0 &&
              req.token_ea != 0 && req.token_size != 0))
            spu_stop(0x101);
        break;
    default:
        spu_stop(0x101);
    }

    quad_u64 eid0_ea = req.eid0_ea;
    quad_uint eid0_size = req.eid0_size;
    quad_ptr eid0_buf = eid0;
    quad_uint eid0_buf_size = sizeof(eid0);
    dma_channel dma;
    eid0_reader eid(&dma, arg4, eid_iv, eid_key_hi, eid_key_lo);

    if (eid0_size == sizeof(eid0)) {
        if (eid.read(eid0_ea, eid0_size, g_eid_ls, 0x1000, eid0_buf, eid0_buf_size))
            spu_stop(0x101);
        memcpy(idps, eid0_buf, 16);
    } else
        memcpy(idps, g_default_idps, 16);

    token_processor tp(g_token_key, g_token_iv, g_token_hmac_key, g_token_pub, g_token_curve, idps);

    switch (req.cmd) {
    case CMD_UPDATE:
        if (tp.update(&req, result))
            spu_stop(0x101);
        break;
    case CMD_VERIFY:
        r = tp.verify(&req);
        if (r == 6)
            spu_stop(0x108);
        else if (r == 19)
            spu_stop(0x109);
        else if (r)
            spu_stop(0x104);
        break;
    default:
        spu_stop(0x101);
    }
    spu_stop(0x100);
    return 0x100;
}
