#include "console.h"
#include "log.h"
#include "platform.h"
#include "memory.h"

physical_console::physical_console() : rx_len(0), rx_channel_id(0), rx_tag(0), link(0), unused(0)
{
}

extern unsigned char *g_physical_console_tx_buffer;
extern unsigned char *g_physical_console_rx_buffer;

physical_console *get_physical_console(void)
{
    return &g_physical_console;
}

long physical_console::deliver(ring_buffer *ring, unsigned short chan_id, long tag)
{
    int ret = 0;

    if (rx_len && chan_id == rx_channel_id && tag == rx_tag) {
        unsigned long len = (unsigned long)rx_len - 16;
        unsigned long done = 0;

        ring->write(g_physical_console_rx_buffer + 16, len, &done);
        ret = len != done ? -11 : 0;
        rx_len = 0;
    }
    return ret;
}

template <class T> T physical_console::bswap(T x)
{
    T out;
    unsigned char *dst = (unsigned char *)&out;
    unsigned char *src = (unsigned char *)&x + sizeof(T) - 1;
    unsigned int count = sizeof(T);
    do { *dst++ = *src--; } while (--count);
    return out;
}

long physical_console::decode_header(unsigned short *hdr, unsigned short *out_len, unsigned short *out_chan_id,
                                     long *out_tag)
{
    *out_len = bswap(*hdr);
    unsigned short chan_id = bswap(hdr[1]);
    long raw_tag = *((long *)hdr + 1);
    *out_chan_id = chan_id;
    *out_tag = bswap(raw_tag);
    return 0;
}

int physical_console::receive()
{
    int rc = -6;
    cp_link *link = this->link;
    if (!link)
        goto done;

    {
        unsigned short out = 0;
        link->receive(g_physical_console_rx_buffer, (unsigned short)g_physical_console_frame_size, &out);
        if (out == 0)
            goto no_frame;

        decode_header((unsigned short *)g_physical_console_rx_buffer, &rx_len, &rx_channel_id, &rx_tag);
        if (rx_len == 0)
            goto no_frame;

        unsigned short chan_id = rx_channel_id;
        log_cons_channel *chan;
        if (chan_id == 0xFFFE)
            goto ctl_channel;
        chan = 0;
        if (chan_id != 0xFFFF)
            goto no_channel;
        goto data_channel;
    ctl_channel:
        chan = get_ft_channels()->ctl;
        goto have_channel;
    data_channel:
        chan = get_ft_channels()->data;
    have_channel:
        if (chan == 0)
            goto no_channel;
        chan->on_receive();
        rc = 0;
        goto done;
    no_channel:
        rx_len = (unsigned long)chan;
        rc = -6;
        goto done;
    }
no_frame:
    rc = -10;
done:
    return rc;
}

int physical_console::poll()
{
    if (!link)
        return -6;
    return receive();
}

long physical_console::encode_header(unsigned char *hdr, unsigned short len, unsigned short chan_id, long tag)
{
    *(unsigned short *)hdr = bswap(len);
    *(unsigned short *)(hdr + 2) = bswap(chan_id);
    *(int *)(hdr + 4) = 0;
    *(long *)(hdr + 8) = bswap(tag);
    return 0;
}

int physical_console::send(ring_buffer *ring, unsigned short chan_id, long tag)
{
    unsigned long done;
    unsigned short sent;

    if (link == 0)
        return -6;

    unsigned short remaining = *ring->count;

    while (remaining) {
        unsigned short max = (unsigned short)(g_physical_console_frame_size - 16);
        unsigned short chunk = max;
        if (remaining < max)
            chunk = remaining;
        encode_header(g_physical_console_tx_buffer, (unsigned short)(chunk + 16), chan_id, tag);
        remaining -= chunk;
        done = 0;
        ring->read(g_physical_console_tx_buffer + 16, chunk, &done);
        if (chunk != done)
            goto fail;
        sent = 0;
        link->send(g_physical_console_tx_buffer, (unsigned short)(chunk + 16), &sent);
        if ((unsigned short)(chunk + 16) != sent)
            goto fail;
    }
    return 0;
fail:
    return -11;
}

int physical_console::initialize()
{
    void *tx_buf = g_memory_budget_high_ptr->allocate(0x8000, 128);
    if (!tx_buf) {
        log_printf(1, 1, "ERROR: phy_console buf\n");
        return -16;
    }
    g_physical_console_tx_buffer = (unsigned char *)tx_buf;

    void *rx_buf = g_memory_budget_high_ptr->allocate(0x8000, 128);
    if (!rx_buf) {
        log_printf(1, 1, "ERROR: phy_console buf\n");
        return -16;
    }
    g_physical_console_rx_buffer = (unsigned char *)rx_buf;

    return init_debug_interface(&g_physical_console);
}

void physical_console::finalize()
{
    finalize_debug_interface((char *)&g_physical_console);
}

physical_console g_physical_console;
unsigned long g_physical_console_frame_size = 0x1000;
unsigned char *g_physical_console_tx_buffer;
unsigned char *g_physical_console_rx_buffer;
