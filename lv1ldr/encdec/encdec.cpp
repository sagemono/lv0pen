#include "encdec.h"

static u32 kbuf[8] __attribute__((aligned(16)));
static u32 kout[8] __attribute__((aligned(16)));

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
