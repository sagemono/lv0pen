#include "console.h"
#include "log.h"
#include "clock.h"
#include "memory.h"

extern file_transfer g_ft_channels;
extern log_cons_channel g_log_cons_ctl;
extern log_cons_channel g_log_cons_data;

enum { FT_SEND_DATA = 7 };

inline file_transfer::file_transfer()
{
    g_log_cons_ctl.id = 0xFFFE;
    g_log_cons_data.id = 0xFFFF;
    ctl = &g_log_cons_ctl;
    data = &g_log_cons_data;
}

int init_log_cons_ctl_and_data(void)
{
    void *ctl_buf = g_memory_budget_high_ptr->allocate(0x12000, 0x1000);
    if (!ctl_buf) {
        log_printf(2, 1, "ERROR: log_cons_ctl buf\n");
        return -16;
    }
    long rc = g_log_cons_ctl.init((char *)ctl_buf, 0x12000);
    if (rc) {
        log_printf(2, 1, "ERROR: log_cons_ctl init %d\n", rc);
        return -16;
    }
    void *data_buf = g_memory_budget_high_ptr->allocate(0x12000, 0x1000);
    if (!data_buf) {
        log_printf(2, 1, "ERROR: log_cons_data buf\n");
        return -16;
    }
    rc = g_log_cons_data.init((char *)data_buf, 0x12000);
    if (rc) {
        log_printf(2, 1, "ERROR: log_cons_data init %d\n", rc);
        return -16;
    }
    return 0;
}

file_transfer *get_ft_channels(void)
{
    return &g_ft_channels;
}

int file_transfer::status_to_error(unsigned long status)
{
    switch (status - 1) {
    case 0: return 0;
    case 1: return -6;
    case 2: case 3: case 4: return -5;
    case 5: case 6: return -9;
    }
    return -13;
}

long file_transfer::write_channel(log_cons_channel *chan, void *buf, long len)
{
    ring_buffer *tx_ring = chan->tx_ring;

    while (len) {
        int rc = tx_ring->push(*(unsigned char *)buf);
        if (rc == 0) {
            buf = (unsigned char *)buf + 1;
            len--;
        }
    }
    chan->flush();
    return 0;
}

int file_transfer::read_channel(log_cons_channel *chan, void *buf, long len)
{
    ring_buffer *rx_ring = chan->rx_ring;
    unsigned long deadline = get_time_us() + 5000000;
    while (len) {
        if (!rx_ring->pop((unsigned char *)buf)) {
            buf = (unsigned char *)buf + 1;
            --len;
        } else {
            chan->poll();
        }
        if (get_time_us() > deadline)
            return -13;
    }
    return 0;
}

template <class T> T file_transfer::bswap(T x)
{
    T out = 0;
    unsigned char *dst = (unsigned char *)&out;
    unsigned char *src = (unsigned char *)&x + sizeof(T) - 1;
    int count = sizeof(T);
    do { *dst++ = *src--; } while (--count);
    return out;
}

void file_transfer::decode_message(long *msg, long *cmd, long *param)
{
    *cmd = bswap(msg[0]);
    *param = bswap(msg[1]);
}

void file_transfer::encode_message(void *msg, long cmd, long param)
{
    long *m = (long *)msg;

    m[0] = bswap(cmd);
    m[1] = bswap(param);
}

long file_transfer::send_file_name(const char *name)
{
    unsigned char msg[16];
    long len = lv0_strlen(name);
    write_channel(data, (void *)name, len);
    encode_message(msg, 10, len);
    return write_channel(ctl, msg, sizeof msg);
}

int file_transfer::get_file(const char *name, unsigned long offset, void *buf, unsigned long size,
                             unsigned long *out_size)
{
    long msg[2];
    long cmd = 0;
    unsigned long param = 0;
    int result;

    send_file_name(name);
    encode_message(msg, 6, offset);
    write_channel(ctl, msg, sizeof msg);
    encode_message(msg, 1, size);
    write_channel(ctl, msg, sizeof msg);
    *out_size = size;

    while (1) {
        result = read_channel(ctl, msg, sizeof msg);
        if (result)
            return result;
        decode_message(msg, &cmd, (long *)&param);
        if (cmd == 4 || !size)
            break;
        if (cmd != FT_SEND_DATA) {
            log_printf(2, 0, "[WARN]: %s(%d) command(%d) == FT_SEND_DATA(%d)\n", __FUNCTION__, 159, cmd, FT_SEND_DATA);
            return -17;
        }
        if (param > g_physical_console_frame_size - 16) {
            log_printf(2, 0, "[WARN]: %s(%d) parameter(%d) <= %d\n", __FUNCTION__, 166, param, g_physical_console_frame_size - 16);
            return -17;
        }
        result = read_channel(data, buf, param);
        if (result)
            return result;
        buf = (char *)buf + param;
        size -= param;
        encode_message(msg, 8, param);
        write_channel(ctl, msg, sizeof msg);
    }

    result = -13;
    switch (param) {
    case 1: result = 0;  break;
    case 2: result = -6; break;
    case 3:
    case 4:
    case 5: result = -5; break;
    case 6:
    case 7: result = -9; break;
    }
    return result;
}

int file_transfer::put_file(const char *name, unsigned long offset, const void *buf, unsigned long size,
                             unsigned long *out_size)
{
    long cmd = 0, param = 0;
    char msg[16];
    int result;

    send_file_name(name);
    encode_message(msg, 6, offset);
    write_channel(ctl, msg, sizeof msg);
    encode_message(msg, 2, size);
    write_channel(ctl, msg, sizeof msg);
    *out_size = size;

    while (size) {
        unsigned long chunk = g_physical_console_frame_size - 16;
        if (size < chunk)
            chunk = size;
        write_channel(data, (void *)buf, chunk);
        encode_message(msg, FT_SEND_DATA, chunk);
        write_channel(ctl, msg, sizeof msg);
        result = read_channel(ctl, msg, sizeof msg);
        if (result)
            goto out;
        buf = (const char *)buf + chunk;
        size -= chunk;
        decode_message((long *)msg, &cmd, &param);
    }

    encode_message(msg, 4, 1);
    write_channel(ctl, msg, sizeof msg);
    result = read_channel(ctl, msg, sizeof msg);
    if (result)
        goto out;
    decode_message((long *)msg, &cmd, &param);
    switch (param) {
    case 1: result = 0; break;
    case 2: result = -6; break;
    case 3: case 4: case 5: result = -5; break;
    case 6: case 7: result = -9; break;
    default: result = -13; break;
    }
out:
    return result;
}

int file_transfer::get_file_size(const char *name, unsigned long *out_size)
{
    long msg[2];
    long cmd = 0;
    long param = 0;
    int result, rc;

    send_file_name(name);
    encode_message(msg, 3, 0x7FFFFFFF);
    write_channel(ctl, msg, sizeof msg);
    result = read_channel(ctl, msg, sizeof msg);
    if (result)
        goto out;

    decode_message(msg, &cmd, &param);
    if (cmd == 9) {
        *out_size = param;
        rc = read_channel(ctl, msg, sizeof msg);
        if (rc) {
            result = rc;
            goto out;
        }
        decode_message(msg, &cmd, &param);
    } else {
        *out_size = 0;
    }

    switch (param) {
    case 1: result = 0; break;
    case 2: result = -6; break;
    case 3: case 4: case 5: result = -5; break;
    case 6: case 7: result = -9; break;
    default: result = -13; break;
    }
out:
    return result;
}

int file_transfer::get_file_name(void *buf, unsigned long size, unsigned long arg)
{
    long msg[2];
    unsigned long cmd = 0, param = 0;
    unsigned long max_len;
    int result;

    encode_message(msg, 13, arg);
    write_channel(ctl, msg, sizeof msg);
    for (;;) {
        result = read_channel(ctl, msg, sizeof msg);
        if (result)
            return result;
        decode_message(msg, (long *)&cmd, (long *)&param);
        if (cmd == 4)
            break;
        if (cmd != FT_SEND_DATA) {
            log_printf(2, 0, "[WARN]: %s(%d) command(%d) == FT_SEND_DATA(%d)\n", __FUNCTION__, 357, cmd, FT_SEND_DATA);
            return -17;
        }
        max_len = g_physical_console_frame_size - 16;
        if (param > max_len) {
            log_printf(2, 0, "[WARN]: %s(%d) parameter(%d) <= %d\n", __FUNCTION__, 364, param, max_len);
            return -17;
        }
        if (max_len < size || size < param)
            return -13;
        result = read_channel(data, buf, param);
        if (result)
            return result;
        encode_message(msg, 8, param);
        write_channel(ctl, msg, sizeof msg);
    }

    switch (param) {
    case 1: return 0;
    case 2: return -6;
    case 3: case 4: case 5: return -5;
    case 6: case 7: return -9;
    }
    return -13;
}

int file_transfer::authorize_file(const void *src, unsigned long src_size, void *dst)
{
    long msg[2];
    long cmd = 0;
    unsigned long param = 0;
    int result;

    write_channel(data, (void *)src, src_size);
    encode_message(msg, 16, src_size);
    write_channel(ctl, msg, sizeof msg);

    while (1) {
        result = read_channel(ctl, msg, sizeof msg);
        if (result)
            return result;
        decode_message(msg, &cmd, (long *)&param);
        if (cmd == 4)
            break;
        if (cmd != FT_SEND_DATA) {
            log_printf(2, 0, "[WARN]: %s(%d) command(%d) == FT_SEND_DATA(%d)\n", __FUNCTION__, 563, cmd, FT_SEND_DATA);
            return -17;
        }
        if (param > g_physical_console_frame_size - 16) {
            log_printf(2, 0, "[WARN]: %s(%d) parameter(%d) <= %d\n", __FUNCTION__, 570, param, g_physical_console_frame_size - 16);
            return -17;
        }
        if (param != src_size) {
            log_printf(2, 0, "[WARN]: %s(%d) parameter(%d) == src_size(%d)\n", __FUNCTION__, 576, param, src_size);
            return -17;
        }
        result = read_channel(data, dst, src_size);
        if (result)
            return result;
    }

    result = -13;
    switch (param) {
    case 1: result = 0;  break;
    case 2: result = -6; break;
    case 3:
    case 4:
    case 5: result = -5; break;
    case 6:
    case 7: result = -9; break;
    }
    return result;
}

int file_transfer::file_command_15(const char *name, unsigned long offset)
{
    long msg[2];
    long cmd = 0;
    long param = 0;
    int result;

    send_file_name(name);
    encode_message(msg, 6, offset);
    write_channel(ctl, msg, sizeof msg);
    encode_message(msg, 15, 0);
    write_channel(ctl, msg, sizeof msg);
    result = read_channel(ctl, msg, sizeof msg);
    if (!result) {
        decode_message(msg, &cmd, &param);
        result = -13;
        switch (param) {
        case 1: result = 0; break;
        case 2: result = -6; break;
        case 3: case 4: case 5: result = -5; break;
        case 6: case 7: result = -9; break;
        }
    }
    return result;
}

file_transfer g_ft_channels;
log_cons_channel g_log_cons_ctl;
log_cons_channel g_log_cons_data;
