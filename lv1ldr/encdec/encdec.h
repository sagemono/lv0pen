#ifndef LV1LDR_ENCDEC_H
#define LV1LDR_ENCDEC_H

#include <spu_intrinsics.h>
#include "types.h"
#include "mmio.h"
#include "aes.h"
#include "log.h"

bool is_sb_product_dx(void);
bool is_sb_product_2(void);
void set_window(u64 reg, u32 addr, u32 enable, u32 size);

inline void write32(u64 ea, u32 val)
{
    volatile u32 *ls = (volatile u32 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 4, DMA_TAG, 0, 0);
    dma_wait();
}

inline u16 read16(u64 ea)
{
    volatile u16 *ls = (volatile u16 *)(u32)(DMA_BUF + (ea & 15));
    mfc_get(ls, ea, 2, DMA_TAG, 0, 0);
    dma_wait();
    return *ls;
}

inline void write16(u64 ea, u16 val)
{
    volatile u16 *ls = (volatile u16 *)(u32)(DMA_BUF + (ea & 15));
    *ls = val;
    mfc_put(ls, ea, 2, DMA_TAG, 0, 0);
    dma_wait();
}

class kgen {
public:
    int gen(u64 base, const u32 *iv, const u32 *key, const unsigned char *kek1,
            const unsigned char *kek2, u32 *out, u64 regs, u64 unused, bool discard);
    int init(u64 base, u64 in, u64 out);
};

struct key192 {
    u32 w[6];
};

struct key192_slot {
    key192 key;
} __attribute__((aligned(32)));

struct iv128 {
    u32 w[4];
};

struct dma_desc {
    u32 src;
    u32 len;
    u32 rsvd;
    u32 key;
    u32 next;
    u32 pad;
    u16 ctl;
    u16 count;
    u32 pad2;
} __attribute__((aligned(32)));

struct key_desc {
    key192 key1;
    u32 a;
    u32 b;
    key192 key2;
    u32 zero[2];
} __attribute__((aligned(32)));

class io_map {
public:
    void set(u64 ea, u64 io);
    int get_dma_status(u64 base, u64 reg, u64 tail);
    int put_key(u64 dest, const key192 *key1, u32 a, u32 b, const unsigned char *kek,
                const vec_uchar16 *iv, const key192 *key2, iv128 *next_iv);
    int put_key2(u64 dest, const key192 *key1, u32 a, u32 b, const unsigned char *kek,
                 const vec_uchar16 *iv, const key192 *key2, iv128 *next_iv);
    int put_desc(u64 dest, u32 src, u32 len, u32 next, u16 count, u32 key);
    int start(u64 base, u64 reg, u64 ea);

    u32 m_ea;
    u32 m_io;
};

struct key192_node {
    key192 key;
    key192_node *next;
};

struct ata_cfg {
    u32 flag;
    key192 key0;
    key192 key1;
    key192 key2;
    key192 key3;
};

struct encdec_cfg {
    u32 flag;
    key192_node *keys;
};

class encdec {
public:
    encdec();
    virtual ~encdec() {}

    int bit_count(u32 x);
    int next_bit(u32 x, int i);
    key192_node *get_key192_by_index(key192_node *d, int n);
    u64 enable(u64 base);
    int activate(const unsigned char *kek1, const unsigned char *kek2, const key192 *key1,
                 const u32 *key, const u32 *iv, u64 ea, u64 io, const ata_cfg *ata,
                 const encdec_cfg *enc);
};

#endif
