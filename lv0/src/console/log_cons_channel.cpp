#include "console.h"

log_cons_channel::log_cons_channel() : tag(1), console(0), tx_ring(&tx), rx_ring(&rx)
{
    console = get_physical_console();
}

log_cons_channel::~log_cons_channel()
{
}

int log_cons_channel::init(char *buf, unsigned long size)
{
    unsigned long half = size >> 1;

    tx.attach(half, (ring_buffer_header *)buf);
    rx.attach(half, (ring_buffer_header *)(buf + half));
    return 0;
}

int log_cons_channel::flush()
{
    if (*tx_ring->count)
        return console->send(tx_ring, id, tag);
    return 0;
}

long log_cons_channel::receive()
{
    return console->deliver(rx_ring, id, tag);
}

void log_cons_channel::on_receive()
{
    receive();
}

int log_cons_channel::poll()
{
    return console->poll();
}
