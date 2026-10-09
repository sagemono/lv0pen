#include "console.h"
#include "log.h"
#include "clock.h"
#include "memory.h"

extern log_cons_channel g_internal_console_channel;

inline internal_console::internal_console() : channel(&g_internal_console_channel), flag(0)
{
    channel->id = 0xFFFC;
}

extern internal_console g_internal_console;

int internal_console::initialize()
{
    char *buf;
    long rc;

    buf = (char *)g_memory_budget_high_ptr->allocate(0x12000, 0x1000);
    if (!buf) {
        log_printf(1, 1, "ERROR: internal_console allocate memory\n");
        return -16;
    }
    rc = g_internal_console_channel.init(buf, 0x12000);
    if (rc) {
        log_printf(1, 1, "ERROR: log_cons_channel init %d\n", rc);
        return -16;
    }
    return 0;
}

int internal_console::write(const char *buf, long len)
{
    ring_buffer *ring = channel->tx_ring;
    if (!ring)
        return -2;

    const unsigned char *p = (const unsigned char *)buf;
    unsigned long deadline = get_time_us() + 5000000;

    while (len) {
        int rc = ring->push(*p);
        if (rc == 0) {
            p++;
            len--;
        } else {
            channel->flush();
            if (get_time_us() > deadline)
                return -13;
        }
    }
    channel->flush();
    return 0;
}

int log_to_internal_console(const char *s)
{
    return g_internal_console.write(s, lv0_strlen(s));
}

internal_console g_internal_console;
log_cons_channel g_internal_console_channel;
