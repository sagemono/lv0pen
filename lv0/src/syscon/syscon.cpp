#include "syscon.h"
#include "memory.h"
#include "log.h"

struct __attribute__((packed)) sc_msg_header {
    u8 id;
    u8 b1;
    u16 w2;
    u16 len;
    u16 cs;
    u8 data[];
};

struct __attribute__((packed)) sc_stat_msg {
    u8 id;
    u8 b1;
    u32 w;
    u16 cs;
};

struct sc_msg_ver1 {
    u8 id;
    u8 type;
    u16 transaction_id;
    u16 zero;
    u16 hdr_cs;
    u32 communication_tag;
    u16 payload_len;
    u16 payload_len2;
    u8 payload[];
};

struct sc_msg_ids {
    u16 transaction_id;
    u16 pad;
    u32 communication_tag;
};

char g_sc_msg_buffer[2048];
u32 g_sc_communication_tag = 0x10101010;
u32 g_unused_41748[4] = { 0x00020000, 0, 0x00010000, 0 };

#define BE16(p, i) ((p)[(i) + 1] | (u64)(p)[i] << 8)

enum sc_mmio {
    SC_MMIO_RX_VER1  = 0x8C000,
    SC_MMIO_RX       = 0x8C800,
    SC_MMIO_SC_TC    = 0x8CFF0,
    SC_MMIO_BE_RC    = 0x8CFF4,
    SC_MMIO_RX_STAT  = 0x8CFF8,
    SC_MMIO_TX       = 0x8D000,
    SC_MMIO_TX_STAT  = 0x8D7F8,
    SC_MMIO_BE_TC    = 0x8DFF0,
    SC_MMIO_SC_RC    = 0x8DFF4,
    SC_MMIO_SWBINT   = 0x8E004,
    SC_MMIO_DOORBELL = 0x8E100,
};

long syscon::raise_sc_interrupt(unsigned int ch)
{
    if (this->protocol == 1)
        ch = 0;
    *(volatile int *)(this->base + 4 * (unsigned long)ch + SC_MMIO_DOORBELL) = 1;
    return 0;
}

unsigned int syscon::get_protocol_version()
{
    return this->protocol;
}

unsigned long syscon::sc_calc_checksum(unsigned char *buf, unsigned int n, unsigned short *out)
{
    unsigned int i;

    *out = 0;
    for (i = 0; i < n; i++)
        *out += *buf++;
    return 0;
}

unsigned long syscon::sc_calc_checksum_neg(unsigned char *buf, unsigned int n, unsigned short *out)
{
    unsigned int i;

    *out = 0;
    for (i = 0; i < n; i++)
        *out -= *buf++;
    return 0;
}

static inline int sc_read16(volatile u16 *p)
{
    return *p;
}

int syscon::write_init_msg_ver1(unsigned int id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids, int check_ctr)
{
    FUNCTION_NAME("write_init_msg_ver1");
    unsigned short cs;
    long n;

    if ((unsigned long)payload_len + 20 > 0x800) {
        uart_printf("%s: too large payload_size %d\n", function_name, payload_len);
        return -7;
    }

    long dev = this->base;
    u16 be_tc = sc_read16((volatile u16 *)(dev + SC_MMIO_BE_TC + 2));
    u16 be_rc = sc_read16((volatile u16 *)(dev + SC_MMIO_BE_RC + 2));
    u16 tc, rc;
    if (check_ctr) {
        if (be_tc != be_rc) {
            uart_printf("%s: be_tc(0x%04x) != be_rc(0x%04x)\n", function_name, be_tc, be_rc);
            return -16;
        }
        tc = be_tc;
        rc = be_rc;
    } else {
        tc = 0;
        rc = 0;
    }

    unsigned int pad = 0;
    if (payload_len & 3)
        pad = 4 - (payload_len & 3);

    unsigned char *hdr = (unsigned char *)g_sc_msg_buffer;
    volatile struct sc_msg_ver1 *m = (volatile struct sc_msg_ver1 *)hdr;
    m->id = id;
    m->type = 1;
    unsigned short txid = ids->transaction_id;
    m->transaction_id = txid;
    m->zero = 0;
    unsigned short len = payload_len | (txid >> 16);
    sc_calc_checksum(hdr, 6, &cs);
    int hdr_cs = cs | (short)0x8000;
    unsigned char *cur = (unsigned char *)m->payload;
    m->hdr_cs = hdr_cs;
    m->communication_tag = ids->communication_tag;
    m->payload_len2 = len;
    m->payload_len = len;
    lv0_memmove((char *)cur, (const char *)payload, payload_len);
    cur += payload_len;

    {
        unsigned int i;
        for (i = 0; i < pad; i++)
            *cur++ = 0;
    }

    n = cur - (unsigned char *)g_sc_msg_buffer;
    sc_calc_checksum_neg(hdr, (unsigned long)cur - (unsigned long)g_sc_msg_buffer, &cs);
    cur[0] = 0;
    cur[1] = 0;
    cur[2] = ((unsigned char *)&cs)[0];
    cur[3] = ((unsigned char *)&cs)[1];
    n += 19;
    copy_qwords_to_mmio((char *)(this->base + SC_MMIO_TX), g_sc_msg_buffer, n & ~15);

    this->rc = rc;
    unsigned short next = tc + 1;
    *(unsigned int *)(this->base + SC_MMIO_BE_TC) = next | ((unsigned int)next << 16);
    return 0;
}

int syscon::write_init_msg(unsigned int id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids)
{
    if (this->protocol == 1)
        return write_init_msg_ver1(id, payload, payload_len, ids, 1);
    return -8;
}

int syscon::read_cmpl_msg_ver1(enum sc_command *id_out, void *data, u32 max_len, int *len_out, struct sc_msg_ids *ids)
{
    FUNCTION_NAME("read_cmpl_msg_ver1");
    u16 cs;
    unsigned char hdr_buf[16];
    struct sc_msg_ver1 &hdr = *(struct sc_msg_ver1 *)hdr_buf;
    long dev = this->base;
    u16 tc = sc_read16((volatile u16 *)(dev + SC_MMIO_SC_TC + 2));
    u16 rc = sc_read16((volatile u16 *)(dev + SC_MMIO_SC_RC + 2));
    if (tc == rc) {
        uart_printf("%s: sc_tc(0x%04x) == sc_rc(0x%04x)\n", function_name, tc, rc);
        return -16;
    }

    copy_qwords_from_mmio((char *)(dev + SC_MMIO_RX_VER1), (char *)hdr_buf, 16);
    u16 len = hdr.payload_len;
    int psize = len;
    if ((u32)psize > max_len) {
        uart_printf("%s: too small payload_size %d, body_len %d\n", function_name, max_len, psize);
        return -7;
    }
    if ((u64)len + 20 > 0x800) {
        uart_printf("%s: too large payload_len %d\n", function_name, psize);
        return -6;
    }
    if (psize != hdr.payload_len2) {
        uart_printf("%s: body_len(%04x) != body_len2(%04x)\n", function_name, psize, hdr.payload_len2);
        return -6;
    }

    sc_calc_checksum(hdr_buf, 6, &cs);
    if (hdr.hdr_cs != (cs | 0x8000)) {
        uart_printf("%s: header checksum %d != %d\n", function_name, hdr.hdr_cs, cs);
        return -6;
    }

    u16 npad = (len + 3) & 0xFFFC;
    unsigned char *buf = (unsigned char *)g_sc_msg_buffer;
    copy_qwords_from_mmio((char *)(this->base + SC_MMIO_RX_VER1), (char *)buf, (npad + 35) & 0x1FFF0);
    *(struct sc_msg_ver1 *)buf = hdr;
    sc_calc_checksum_neg(buf, npad + 16, &cs);
    unsigned char *trailer = buf + npad;
    u64 body_cs = BE16(trailer, 18);
    int bcs = body_cs;
    if (bcs != cs) {
        uart_printf("%s: payload checksum %d != %d\n", function_name, bcs, cs);
        return -6;
    }

    lv0_memmove((char *)data, (const char *)(buf + 16), hdr.payload_len);
    ids->transaction_id = hdr.transaction_id;
    ids->communication_tag = hdr.communication_tag;
    *len_out = hdr.payload_len;
    *id_out = (enum sc_command)hdr.id;
    u16 next = rc + 1;
    *(u32 *)(this->base + SC_MMIO_SC_RC) = next | ((u32)next << 16);
    return 0;
}

int syscon::read_cmpl_msg(enum sc_command *id_out, void *data, unsigned int max_len, int *len_out, struct sc_msg_ids *ids)
{
    if (this->protocol == 1)
        return read_cmpl_msg_ver1(id_out, data, max_len, len_out, ids);
    return -8;
}

u32 syscon::get_swbint_status_ver1()
{
    long base = this->base;
    u16 saved_next, rc_next;
    u32 status, peer_ctr;
    if (*(volatile u32 *)(base + SC_MMIO_SWBINT) == 0)
        return 0;
    saved_next = this->rc + 1;
    status = *(volatile u16 *)(base + SC_MMIO_SC_TC + 2);
    rc_next = *(volatile u16 *)(base + SC_MMIO_SC_RC + 2) + 1;
    peer_ctr = *(volatile u16 *)(base + SC_MMIO_BE_RC + 2);
    status = (u16)status == rc_next;
    if (saved_next == peer_ctr)
        status |= 2;
    return status;
}

unsigned int syscon::get_swbint_status()
{
    if (this->protocol == 1)
        return get_swbint_status_ver1();
    return 0;
}

int syscon::wait_swbint(unsigned int mask)
{
    unsigned int flags = g_sc_swbint_pending;

    if (flags & mask) {
        g_sc_swbint_pending = flags & ~mask;
    } else {
        do
            flags |= get_swbint_status();
        while ((flags & mask) == 0);
        g_sc_swbint_pending = flags & ~mask;
    }
    return 0;
}

inline int syscon::read_stat_msg(enum sc_command *id_out) {
    if (this->protocol == 1)
        return 0;
    return -8;
}

inline int syscon::write_stat_msg(unsigned int id) {
    if (this->protocol == 1)
        return 0;
    return -8;
}

long syscon::send_and_receive_safe(u32 id, const void *req, u32 req_len, void *resp, u32 resp_max, int *resp_len, int flags)
{
    FUNCTION_NAME("send_and_receive_safe");
    enum sc_command receive_id;
    int ok, ret, wait_rc;
    u32 sid;

    if (!this->base)
        return -1;

    ok = 0;
    struct sc_msg_ids ids = { ++g_sc_transaction_id };
    ids.communication_tag = ++g_sc_communication_tag;

    for (;;) {
        ret = write_init_msg(id, req, req_len, &ids);
        if (ret != -16) {
            if (ret != 0) {
                uart_printf("%s: write_init_msg fail %d\n", function_name, ret);
                return -4;
            }
            ok = 1;
        }
        raise_sc_interrupt(0);
        wait_rc = wait_swbint(2);
        if (wait_rc == 0) {
            if (ok)
                break;
        } else
            uart_printf("%s: wait_swbint(0) returns unknown value %d\n", function_name, wait_rc);
    }

    receive_id = (enum sc_command)id;
    ret = read_stat_msg(&receive_id);
    if (ret) {
        uart_printf("%s: read_stat_msg fail %d\n", function_name, ret);
        return -2;
    }
    sid = receive_id;
    if (sid != id) {
        uart_printf("%s: read_stat_msg unknown id %d\n", function_name, (int)sid);
        return -2;
    }

    for (;;) {
        int cmpl_rc;
        _Bool valid;
        wait_rc = wait_swbint(1);
        if (wait_rc) {
            uart_printf("%s: wait_swbint(0) returns unknown value %d\n", function_name, wait_rc);
            continue;
        }
        cmpl_rc = read_cmpl_msg(&receive_id, resp, resp_max, resp_len, &ids);
        if (cmpl_rc == -16) {
            uart_printf("%s: read_cmpl_msg fail %d\n", function_name, -16);
            return -16;
        }
        if (cmpl_rc == 0)
            valid = (receive_id == sid && ids.transaction_id == g_sc_transaction_id && ids.communication_tag == g_sc_communication_tag);
        else {
            long base = this->base;
            u16 rc_next = *(u16 *)(base + SC_MMIO_SC_RC + 2) + 1;
            *(u32 *)(base + SC_MMIO_SC_RC) = (((long)rc_next) << 16) | rc_next;
            valid = 0;
        }
        ret = write_stat_msg(sid);
        if (ret) {
            uart_printf("%s: write_stat_msg fail %d\n", function_name, ret);
            return -2;
        }
        raise_sc_interrupt(1);
        if (valid)
            return 0;
        uart_printf("%s: skip cmpl err %d, receive_id %d, transaction_id %d, communication_tag %d\n", function_name, 0, receive_id, g_sc_transaction_id, g_sc_communication_tag);
    }
}

long syscon::initialize(long mmio_base, int proto_ver, int detect)
{
    this->base = mmio_base;
    *(volatile unsigned int *)(mmio_base + SC_MMIO_SWBINT);
    if (detect) {
        char msg[4];
        struct sc_msg_ids ids;
        long base;
        volatile unsigned int *ready;
        *(volatile int *)(mmio_base + SC_MMIO_BE_TC) = 0;
        this->protocol = 1;
        *(volatile int *)(mmio_base + SC_MMIO_SC_RC) = 0;
        msg[0] = 1;
        ids.transaction_id = 0;
        ids.communication_tag = 0;
        write_init_msg_ver1(255, msg, 1, &ids, 0);
        raise_sc_interrupt(0);
        base = this->base;
        ready = (volatile unsigned int *)(base + SC_MMIO_SWBINT);
        while (*ready == 0)
            ;
        if (*(long *)(base + SC_MMIO_RX_STAT) == -1)
            this->protocol = 1;
        else
            this->protocol = 0;
    } else {
        this->protocol = proto_ver;
    }
    return 0;
}

static u32 sr_communication_tag;

int syscon::send_and_receive(unsigned int id, const void *req, unsigned int req_len, void *resp, unsigned int resp_max, int *resp_len, int flags)
{
    FUNCTION_NAME("send_and_receive");
    int ret = -1;
    enum sc_command receive_id;

    long base = this->base;
    if (base) {
        *(volatile int *)(base + SC_MMIO_SWBINT);
        struct sc_msg_ids ids;
        ids.transaction_id = ++g_send_and_receive_transaction_id;
        ids.communication_tag = sr_communication_tag;
        long init_rc = write_init_msg(id, req, req_len, &ids);
        if (init_rc) {
            uart_printf("%s: write_init_msg fail %d\n", function_name, init_rc);
            ret = -4;
            goto out;
        }
        for (;;) {
            raise_sc_interrupt(0);
            long wait_rc = wait_swbint(2);
            if (!wait_rc)
                break;
            uart_printf("%s: wait_swbint(0) returns unknown value %d\n", function_name, wait_rc);
        }
        receive_id = (enum sc_command)id;
        long stat_rc = read_stat_msg(&receive_id);
        if (stat_rc) {
            uart_printf("%s: read_stat_msg fail %d\n", function_name, stat_rc);
            ret = -2;
            goto out;
        }
        if (receive_id != id) {
            uart_printf("%s: read_stat_msg unknown id %d\n", function_name, receive_id);
            ret = -2;
            goto out;
        }
        for (;;) {
            long wait_rc = wait_swbint(1);
            if (!wait_rc)
                break;
            uart_printf("%s: wait_swbint(0) returns unknown value %d\n", function_name, wait_rc);
        }
        long cmpl_rc = read_cmpl_msg(&receive_id, resp, resp_max, resp_len, &ids);
        if (cmpl_rc) {
            uart_printf("%s: read_cmpl_msg fail %d\n", function_name, cmpl_rc);
            ret = -3;
            goto out;
        }
        if (receive_id != id) {
            uart_printf("%s: read_cmpl_msg unknown id %d\n", function_name, receive_id);
            ret = -6;
            goto out;
        }
        if (ids.transaction_id != g_send_and_receive_transaction_id) {
            uart_printf("%s: read_cmpl_msg unknown transaction_id %d\n", function_name, g_send_and_receive_transaction_id);
            ret = -6;
            goto out;
        }
        if (ids.communication_tag != sr_communication_tag) {
            uart_printf("%s: read_cmpl_msg unknown communication_tag %d\n", function_name, 0);
            ret = -6;
            goto out;
        }
        long ack_rc = write_stat_msg(id);
        if (ack_rc) {
            uart_printf("%s: write_stat_msg fail %d\n", function_name, ack_rc);
            ret = -2;
            goto out;
        }
        raise_sc_interrupt(1);
        ret = 0;
    }
out:
    return ret;
}
