#include "console.h"
#include "gbe.h"
#include "mmio_accessor.h"
#include "memory.h"

extern cp_ch4_channel g_cp_ch4_channel;
extern cp_devparam g_cp_ch4_channel_devparam;

struct cp_message {
    u16 w0;
    u16 id;
    u32 w4;
    u64 tag;
    u64 payload[1];
};

struct cp_id_word { u32 status; u32 id; };

struct cp_header {
    u8  b0, b1;
    u16 status;
    u32 len;
    u8  m, h, z, f;
    u32 ctl;
};

static inline u32 ctl_read(u32 addr)
{
    mmio_accessor *mmio = get_mmio_accessor();
    return mmio->read32(addr);
}

static inline void ctl_write(u32 addr, u32 val)
{
    mmio_accessor *mmio = get_mmio_accessor();
    mmio->write32(addr, val);
}

static inline int connect_poll(cp_ch4_channel *self)
{
    mmio_accessor *mmio = get_mmio_accessor();
    return mmio->read32(self->bar1 + ((self->channel << 12) & 0xFFFFF000) + 0x20);
}

cp_ch4_channel::cp_ch4_channel()
{
}

cp_ch4_channel::~cp_ch4_channel()
{
}

int cp_ch4_channel::receive(void *buf, long size, unsigned short *out_len)
{
    *out_len = 0;
    return -10;
}

template <class T> T cp_ch4_channel::bswap(T x)
{
    T out;
    unsigned char *dst = (unsigned char *)&out;
    unsigned char *src = (unsigned char *)&x + sizeof(T) - 1;
    unsigned int count = sizeof(T);
    do { *dst++ = *src--; } while (--count);
    return out;
}

long cp_ch4_channel::disconnect()
{
    write_bar1(0x20, 4);
    write_ch(8, 1);
    reset();
    return 0;
}

cp_ch4_channel *get_cp_ch4_channel(void)
{
    return &g_cp_ch4_channel;
}

cp_ch4_channel g_cp_ch4_channel;
cp_devparam g_cp_ch4_channel_devparam;

int cp_ch4_channel::send(void *buf, unsigned short size, unsigned short *out_len)
{
    cp_message *frame = (cp_message *)buf;
    struct cp_id_word id_word;
    struct cp_header header;
    unsigned short chan_id;
    unsigned int hlen;
    unsigned long tag;
    int status;
    unsigned short len;
    const char *src;

    *out_len = 0;
    status = read_bar1(0x10) & 0xC0000000;
    if (status)
        return -9;
    *out_len = size;
    chan_id = ld_le16(&frame->id);
    tag = ld_le64(&frame->tag);
    len = size - 16;
    *out_len += 16;
    hlen = len + 24;
    src = (const char *)buf + 16;
    header.b0 = 0x30;
    header.b1 = 0x10;
    header.status = status;
    header.len = hlen;
    header.m = 'M';
    header.h = 'H';
    header.z = 0;
    header.f = 0x80;
    header.ctl = 0x00800300;
    id_word.status = status;
    id_word.id = ((unsigned int)tag << 16) | chan_id;

    if (bridge) {
        unsigned long tx = (long)tx_buf;
        unsigned long dst = tx;
        unsigned int *p = (unsigned int *)&header;
        do {
            bridge->write_raw(dst, ld_le32(p));
            dst += 4;
            p++;
        } while (p != (unsigned int *)(&header + 1));
        p = (unsigned int *)&id_word;
        for (unsigned int i = 0; i < 8; i += 4) {
            bridge->write_raw(dst + i, ld_le32(p));
            p++;
        }
        dst += 8;
        p = (unsigned int *)src;
        for (unsigned long j = 0; j < len; j += 4) {
            bridge->write_raw(dst, ld_le32(p));
            p++;
            dst += 4;
        }
    } else {
        unsigned long tx = (long)tx_buf;
        __builtin_memcpy((void *)tx, &header, sizeof header);
        __builtin_memcpy((void *)(tx + 16), &id_word, sizeof id_word);
        lv0_memmove((char *)(tx + 24), src, len);
    }

    __asm__ __volatile__("sync" ::: "memory");
    write_bar1(0x1C, hlen);
    write_bar0(0x1C0, 1UL << channel);
    for (;;) {
        if (read_bar0(0x18C) & (unsigned int)(1UL << channel))
            break;
        delay_us(10);
    }
    write_bar0(0x18C, 1UL << channel);
    return 0;
}

int cp_ch4_channel::connect()
{
    write_bar1(0x20, 1);
    write_ch(8, 1);
    while (read_bar1(0x20) == 1)
        delay_us(10);
    return 0;
}
