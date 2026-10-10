#include "encdec.h"

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
    r = k.init(0x24000000000ULL, 0x3006000, 0x3005000);
    if (r) {
        log_message("[ERROR]: kgen.init fail %08x\n", r);
        return -1;
    }
    for (i = 0; i < 9; i++) {
        r = k.gen(0x24000000000ULL, iv, key, kek1, kek2, genkey, 0x3006000, 0x3005000, true);
        if (r) {
            log_message("[ERROR]: kgen (gen_rand) fail %08x\n", r);
            return -1;
        }
    }
    r = k.gen(0x24000000000ULL, iv, key, kek1, kek2, genkey, 0x3006000, 0x3005000, false);
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
    log_debug("ATAGPEST (0) %08x\n", read32(0x24003005084ULL));
    m.start(0x24000000000ULL, 0x3005000, ea);
    r = m.get_dma_status(0x24000000000ULL, 0x3005000, desc_tail);
    if (r) {
        log_message("[ERROR]: get_dma_status fail %08x\n", r);
        return -2;
    }
    log_debug("ATAGPEST (1) %08x\n", read32(0x24003005084ULL));
    write32(0x24003005084ULL, ~ata->flag & 15);
    log_debug("ATAGPEST (2) %08x\n", read32(0x24003005084ULL));
    log_info("ATA Activation SUCCESS\n");
    return 0;
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
