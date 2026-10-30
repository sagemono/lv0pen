#ifndef MC_MC_H
#define MC_MC_H

#include <spu_intrinsics.h>
#include "types.h"
#include "dma_channel.h"
#include "ss_iso_dma_data.h"
#include "des.h"

struct key128 {
    u64 hi;
    u64 lo;
};

#define CMD_ISSUE       0x2201
#define CMD_VERIFY      0x2202

#define STOP_OK         0x100
#define STOP_UNKNOWN    0x101
#define STOP_ERROR      0x103
#define STOP_MISMATCH   0x104

class mc_module {
public:
    unsigned int xor_bytes(const unsigned char *a, const unsigned char *b, unsigned char *out,
                           unsigned int n);
    unsigned int fill_random(unsigned char *p, unsigned int n);
    int unpack(const unsigned char *in, u64 *a, u64 *b, u64 *c, u64 *d);
    int pack(unsigned char *out, u64 a, u64 b, u64 c, u64 d);
    void init(key128 key, key128 iv);
    int check_chain(u64 k0, u64 k1, u64 a, u64 b, u64 c0, u64 c1, u64 c2, u64 *p0, u64 *p1,
                    unsigned char *p2);
    int chain(u64 k0, u64 k1, u64 a, u64 b, u64 c, unsigned char *o0, unsigned char *o1,
              unsigned char *o2);
    int derive(u64 a, u64 b, unsigned char *o0, unsigned char *o1);
    int serve(ss_iso_dma_data *t);

    unsigned char m_key[16];
    unsigned char m_iv[16];
};

#endif
