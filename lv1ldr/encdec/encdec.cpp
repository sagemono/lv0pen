#include <spu_intrinsics.h>
#include "types.h"
#include "mmio.h"
#include "aes.h"
#include "log.h"

bool is_sb_product_dx(void);
bool is_sb_product_2(void);

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

static u32 kbuf[8] __attribute__((aligned(16)));
static u32 kout[8] __attribute__((aligned(16)));

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

int highest_bit(unsigned int x)
{
    int i;

    if (x != 0)
        for (i = 1; i != 32; i++)
            if ((x >> i) == 0)
                return i - 1;
    return -1;
}

#define FIELD(x, shift, mask)   ((u64)((x) << (shift)) & (mask))

void set_window(u64 reg, u32 addr, u32 enable, u32 size)
{
    u32 n = highest_bit(size);
    u64 v;

    if (n == (u32)-1)
        return;
    v = FIELD(addr, 0, 0xFFFF0000) | FIELD(enable, 10, 0x400) | FIELD(n - 1, 3, 0x1F8) | FIELD(2, 0, 3);
    write32(reg, v);
}

void set_window_4k(u64 reg, u32 addr, u32 enable, u32 size)
{
    u32 n = highest_bit(size);
    u64 v;

    if (n == (u32)-1)
        return;
    v = FIELD(addr, 0, 0xFFFFF000) | FIELD(enable, 10, 0x400) | FIELD(n - 1, 3, 0x1F8) | FIELD(1, 0, 3);
    write32(reg, v);
}

int cbc_decrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                const vec_uchar16 *iv)
{
    return aes_cbc_decrypt(out, in, len, key, bits, iv) ? -1 : 0;
}

int cbc_encrypt(vec_uchar16 *out, const vec_uchar16 *in, int len, const unsigned char *key, int bits,
                const vec_uchar16 *iv)
{
    return aes_cbc_encrypt(out, in, len, key, bits, iv) ? -1 : 0;
}

int kgen::gen(u64 base, const u32 *iv, const u32 *key, const unsigned char *kek1,
              const unsigned char *kek2, u32 *out, u64 regs, u64 unused, bool discard)
{
    u64 r = base + regs;
    int i;

    write32(r + 0x80, iv[0]);
    write32(r + 0x84, iv[1]);
    write32(r + 0x88, iv[2]);
    write32(r + 0x8C, iv[3]);
    for (i = 0; i < 6; i++)
        kbuf[i] = key[i];
    kbuf[6] = 0;
    kbuf[7] = 0;
    if (cbc_encrypt((vec_uchar16 *)kbuf, (vec_uchar16 *)kbuf, 32, kek1, 192, (const vec_uchar16 *)iv))
        return -2;
    write32(r + 0x00, kbuf[0]);
    write32(r + 0x04, kbuf[1]);
    write32(r + 0x08, kbuf[2]);
    write32(r + 0x0C, kbuf[3]);
    write32(r + 0x10, kbuf[4]);
    write32(r + 0x14, kbuf[5]);
    write32(r + 0x18, kbuf[6]);
    write32(r + 0x1C, kbuf[7]);
    while (read32(r + 0xA0) & 0x10000)
        ;
    kbuf[0] = read32(r + 0x40);
    kbuf[1] = read32(r + 0x44);
    kbuf[2] = read32(r + 0x48);
    kbuf[3] = read32(r + 0x4C);
    kbuf[4] = read32(r + 0x50);
    kbuf[5] = read32(r + 0x54);
    kbuf[6] = read32(r + 0x58);
    kbuf[7] = read32(r + 0x5C);
    if (cbc_decrypt((vec_uchar16 *)kbuf, (vec_uchar16 *)kbuf, 32, kek2, 192, (const vec_uchar16 *)iv))
        return -5;
    for (i = 0; i < 6; i++)
        if (kbuf[i] != key[i])
            return -6;
    write32(r + 0xA0, 1);
    while (read32(r + 0xA0) & 1)
        ;
    kbuf[0] = read32(r + 0x60);
    kbuf[1] = read32(r + 0x64);
    kbuf[2] = read32(r + 0x68);
    kbuf[3] = read32(r + 0x6C);
    kbuf[4] = read32(r + 0x70);
    kbuf[5] = read32(r + 0x74);
    kbuf[6] = read32(r + 0x78);
    kbuf[7] = read32(r + 0x7C);
    if (cbc_decrypt((vec_uchar16 *)kout, (vec_uchar16 *)kbuf, 32, kek2, 192, (const vec_uchar16 *)iv))
        return -11;
    if (discard) {
        write32(r + 0xA0, 2);
        return 0;
    }
    cbc_encrypt((vec_uchar16 *)kbuf, (vec_uchar16 *)kout, 32, kek1, 192, (const vec_uchar16 *)iv);
    write32(r + 0x20, kbuf[0]);
    write32(r + 0x24, kbuf[1]);
    write32(r + 0x28, kbuf[2]);
    write32(r + 0x2C, kbuf[3]);
    write32(r + 0x30, kbuf[4]);
    write32(r + 0x34, kbuf[5]);
    write32(r + 0x38, kbuf[6]);
    write32(r + 0x3C, kbuf[7]);
    for (;;) {
        u32 st = read32(r + 0xA0);

        if (st & 0x40000)
            break;
        if (st & 0x20000)
            return -12;
    }
    for (i = 0; i < 6; i++)
        out[i] = key[i] ^ kout[i];
    return 0;
}

int kgen::init(u64 base, u64 in, u64 out)
{
    if (read32(base + 0x2D90) & 0x1800000) {
        write32(base + 0x2D98, 0x20);
        while (read32(base + 0x2D98) != 0x20)
            ;
        write32(base + 0x2D98, 0);
    }
    write32(base + 0x2D90, read32(base + 0x2D90) & ~0x2000000);
    write32(base + 0x2C10, 0);
    write32(base + 0x2C14, 31);
    write32(base + 0x2C18, 31);
    write32(base + 0x2C1C, 31);
    write32(base + 0x2C20, 31);
    write32(base + 0x2C24, 31);
    set_window_4k(base + 0x2D70, out, 1, 0x1000);
    set_window_4k(base + 0x2D68, in, 1, 0x1000);
    write32(base + 0x2D98, 0x30);
    while (read32(base + 0x2D98) != 0x30)
        ;
    write32(base + 0x2D98, 0);
    while (read32(base + 0x2D98) != 0)
        ;
    while (read32(base + 0x2D9C) & 4)
        ;
    write32(out + base + 0xFF0, 0x100);
    while (read32(out + base + 0xFF0) != 0x100)
        ;
    write32(out + base + 0xFF0, 0x10100);
    while (read32(out + base + 0xFF0) != 0x10100)
        ;
    return 0;
}

void io_map::set(u64 ea, u64 io)
{
    u32 *w = &m_ea;

    w[0] = ea;
    w[1] = io;
}

int io_map::get_dma_status(u64 base, u64 reg, u64 tail)
{
    u64 regs = base + reg;
    u32 t = tail;

    for (;;) {
        u16 ctl = read16(t + 24);

        if (ctl & 2) {
            write16(t + 24, ctl & ~2);
            log_debug("break ctlst %08x\n", ctl);
            write32(regs + 0x100, read32(regs + 0x100));
            return 0;
        }
        log_debug("ctlst %08x\n", ctl);
        u32 st = read32(regs + 0x100);
        log_debug("intst %08x\n", st);
        switch (st) {
        case 0:
        case 2:
            continue;
        }
        return -1;
    }
}

int io_map::put_key(u64 dest, const key192 *key1, u32 a, u32 b, const unsigned char *kek,
                    const vec_uchar16 *iv, const key192 *key2, iv128 *next_iv)
{
    key_desc d;

    d.key1 = *key1;
    d.a = a;
    d.b = b;
    d.key2 = *key2;
    d.zero[0] = d.zero[1] = 0;
    if (cbc_encrypt((vec_uchar16 *)&d, (vec_uchar16 *)&d, sizeof d, kek, 192, iv))
        return -1;
    copy_qwords_to_mmio(dest, (char *)&d, sizeof d);
    *next_iv = *(iv128 *)((char *)&d + 48);
    return 0;
}

int io_map::put_key2(u64 dest, const key192 *key1, u32 a, u32 b, const unsigned char *kek,
                     const vec_uchar16 *iv, const key192 *key2, iv128 *next_iv)
{
    key_desc d;

    d.key1 = *key1;
    d.a = a;
    d.b = b;
    d.key2 = *key2;
    d.zero[0] = d.zero[1] = 0;
    if (cbc_encrypt((vec_uchar16 *)&d, (vec_uchar16 *)&d, sizeof d, kek, 192, iv))
        return -1;
    copy_qwords_to_mmio(dest, (char *)&d, sizeof d);
    *next_iv = *(iv128 *)((char *)&d + 48);
    return 0;
}

int io_map::put_desc(u64 dest, u32 src, u32 len, u32 next, u16 count, u32 key)
{
    dma_desc d;

    d.src = src - m_ea + m_io | 0x80000000;
    d.len = len;
    d.rsvd = 0;
    d.key = key;
    d.next = next ? next - m_ea + m_io | 0x80000000 : 0;
    d.ctl = 0x4000;
    d.count = count;
    copy_qwords_to_mmio(dest, (char *)&d, sizeof d);
    return 0;
}

int io_map::start(u64 base, u64 reg, u64 ea)
{
    write32(base + reg, (u32)ea - m_ea + m_io | 0x80000000);
    write32(base + reg + 4, 0x80000000);
    return 0;
}

encdec::encdec()
{
}

int encdec::bit_count(u32 x)
{
    int n = 0;

    for (int i = 0; i < 32; i++)
        if (x & (1 << i))
            n++;
    return n;
}

int encdec::next_bit(u32 x, int i)
{
    for (; i < 32; i++)
        if (x & (1 << i))
            return i;
    return -1;
}

key192_node *encdec::get_key192_by_index(key192_node *d, int n)
{
    for (int i = 0; i != n; i++) {
        d = d->next;
        if (!d)
            return 0;
    }
    return d;
}

u64 encdec::enable(u64 base)
{
    write32(base + 0x87020, read32(base + 0x87020) | 4);
    write32(base + 0x87030, read32(base + 0x87030) | 4);
    if (is_sb_product_dx() || is_sb_product_2()) {
        set_window(base + 0x3168, 0x3000000, 1, 0x800000);
        set_window(base + 0x3160, 0x3800000, 1, 0x800000);
    }
    return 0;
}

#define ALIGN32(x)      (((x) + 31) & ~31)

int encdec::activate(const unsigned char *kek1, const unsigned char *kek2, const key192 *key1,
                     const u32 *key, const u32 *iv, u64 ea, u64 io, const ata_cfg *ata,
                     const encdec_cfg *enc)
{
    key192_slot ata_key;
    key192_slot enc_key;
    kgen k;
    io_map m;
    iv128 chain;
    u32 genkey[6] __attribute__((aligned(16)));
    u64 desc_tail = 0;
    u64 desc, data;
    u32 src, next;
    int ata_desc_num, encdec_desc_num;
    int i, bit, r;

    log_debug("ea %08llx, io %08llx \n", ea, io);
    log_debug("ata flag %08x, encdec flag %08x \n", ata->flag, enc->flag);
    ata_desc_num = bit_count((ata->flag & 0xF0) >> 4);
    encdec_desc_num = bit_count(enc->flag & 0xFFFF);
    log_debug("ata_desc_num %d, encdec_desc_num %d\n", ata_desc_num, encdec_desc_num);
    if (ata_desc_num == 0 && encdec_desc_num == 0)
        return 0;
    r = k.init(0x240000000ULL, 0x3006000, 0x3005000);
    if (r) {
        log_message("[ERROR]: kgen.init fail %08x\n", r);
        return -1;
    }
    for (i = 0; i < 9; i++) {
        r = k.gen(0x240000000ULL, iv, key, kek1, kek2, genkey, 0x3006000, 0x3005000, true);
        if (r) {
            log_message("[ERROR]: kgen (gen_rand) fail %08x\n", r);
            return -1;
        }
    }
    r = k.gen(0x240000000ULL, iv, key, kek1, kek2, genkey, 0x3006000, 0x3005000, false);
    if (r) {
        log_message("[ERROR]: kgen fail %08x\n", r);
        return -1;
    }
    m.set(ea, io);

    desc = ea;
    data = desc + 32;
    src = data;
    next = ALIGN32(src + 16);
    m.put_desc(desc, src, 16, next, 1024, 0);
    log_debug("desc %08x, src %08x, next %08x\n", (u32)desc, src, next);
    write32(data, iv[0]);
    write32(data + 4, iv[1]);
    write32(data + 8, iv[2]);
    write32(data + 12, iv[3]);

    bit = 0;
    for (i = 0; i != ata_desc_num; i++) {
        bool last = i == ata_desc_num - 1 && encdec_desc_num == 0;
        const key192 *p;
        int on;

        desc = next;
        data = desc + 32;
        src = data;
        next = last ? 0 : ALIGN32(src + 64);
        if (last)
            desc_tail = desc;
        m.put_desc(desc, src, 64, next, 0, 32);
        log_debug("desc %08x, src %08x, next %08x\n", (u32)desc, src, next);
        bit = next_bit((ata->flag & 0xF0) >> 4, bit);
        switch (bit) {
        case 0:
            p = &ata->key0;
            break;
        case 1:
            p = &ata->key1;
            break;
        case 2:
            p = &ata->key2;
            break;
        case 3:
            p = &ata->key3;
            break;
        default:
            log_message("[ERROR]: unknown ata device index\n");
            return -99;
        }
        ata_key.key = *p;
        on = (ata->flag & (1 << bit)) != 0;
        log_debug("ata enable device %d, enc_enable %d\n", bit, on);
        m.put_key2(data, key1, bit | 512, on, (const unsigned char *)genkey,
                   (const vec_uchar16 *)iv, &ata_key.key, &chain);
        bit++;
    }

    chain.w[0] = iv[0];
    chain.w[1] = iv[1];
    chain.w[2] = iv[2];
    chain.w[3] = iv[3];
    bit = 0;
    for (i = 0; i != encdec_desc_num; i++) {
        bool last = i == encdec_desc_num - 1;
        key192_node *p;

        desc = next;
        data = desc + 32;
        src = data;
        next = last ? 0 : ALIGN32(src + 64);
        if (last)
            desc_tail = desc;
        m.put_desc(desc, src, 64, next, 0, 32);
        log_debug("desc %08x, src %08x, next %08x\n", (u32)desc, src, next);
        bit = next_bit(enc->flag & 0xFFFF, bit);
        log_debug("encdec enable device %d\n", bit);
        p = get_key192_by_index(enc->keys, i);
        if (!p) {
            log_message("[ERROR]: get_key192_by_index returns NULL\n");
            return -3;
        }
        enc_key.key = p->key;
        m.put_key(data, key1, bit | 768, 0, (const unsigned char *)genkey,
                  (const vec_uchar16 *)&chain, &enc_key.key, &chain);
        bit++;
    }
    if (desc_tail == 0) {
        log_message("[ERROR]: desc_tail == 0\n");
        return -99;
    }
    log_debug("desc_tail %08llx\n", desc_tail);
    log_debug("ATAGPEST (0) %08x\n", read32(0x240003005084ULL));
    m.start(0x240000000ULL, 0x3005000, ea);
    r = m.get_dma_status(0x240000000ULL, 0x3005000, desc_tail);
    if (r) {
        log_message("[ERROR]: get_dma_status fail %08x\n", r);
        return -2;
    }
    log_debug("ATAGPEST (1) %08x\n", read32(0x240003005084ULL));
    write32(0x240003005084ULL, ~ata->flag & 15);
    log_debug("ATAGPEST (2) %08x\n", read32(0x240003005084ULL));
    log_info("ATA Activation SUCCESS\n");
    return 0;
}
