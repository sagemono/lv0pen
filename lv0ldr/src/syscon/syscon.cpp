#include "mmio.h"
#include "syscon.h"
#include "util.h"

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

struct __attribute__((packed)) sc_trailer { u16 zero; u16 cs; };

char g_sc_msg_buffer[2048];
u32 g_sc_communication_tag = 0x10101010;

enum sc_mmio {
    SC_MMIO_RX_VER1  = 0x8C000,
    SC_MMIO_SC_TC    = 0x8CFF0,
    SC_MMIO_BE_RC    = 0x8CFF4,
    SC_MMIO_RX_STAT  = 0x8CFF8,
    SC_MMIO_TX       = 0x8D000,
    SC_MMIO_BE_TC    = 0x8DFF0,
    SC_MMIO_SC_RC    = 0x8DFF4,
    SC_MMIO_SWBINT   = 0x8E004,
    SC_MMIO_DOORBELL = 0x8E100,
};

unsigned int syscon::read_swbint()
{
    return read32(this->base + SC_MMIO_SWBINT);
}

unsigned int syscon::get_protocol_version()
{
    return this->protocol;
}

void syscon::set_f8(u8 v)
{
    this->f8 = v;
}

inline int syscon::read_stat_msg(int *id_out)
{
    if (this->protocol == 1)
        return 0;
    return -8;
}

inline int syscon::write_stat_msg(unsigned int id)
{
    if (this->protocol == 1)
        return 0;
    return -8;
}

unsigned long syscon::sc_calc_checksum(unsigned char *buf, unsigned int n, unsigned short *out)
{
    unsigned int i;

    *out = 0;
    for (i = 0; i < n; i++)
        *out += buf[i];
    return 0;
}

unsigned long syscon::sc_calc_checksum_neg(unsigned char *buf, unsigned int n, unsigned short *out)
{
    unsigned int i;

    *out = 0;
    for (i = 0; i < n; i++)
        *out -= buf[i];
    return 0;
}

long syscon::raise_sc_interrupt(unsigned long ch)
{
    if (this->protocol == 1)
        ch = 0;
    write32(this->base + 4 * ch + SC_MMIO_DOORBELL, 1);
    return 0;
}

u32 syscon::get_swbint_status_ver1()
{
    u32 tc;
    u16 rc_next, peer_ctr;
    u32 status;

    if (read32(this->base + SC_MMIO_SWBINT) == 0)
        return 0;
    tc = read16(this->base + SC_MMIO_SC_TC + 2);
    rc_next = read16(this->base + SC_MMIO_SC_RC + 2) + 1;
    status = tc == rc_next;
    peer_ctr = read16(this->base + SC_MMIO_BE_RC + 2);
    if ((u16)(this->rc + 1) == peer_ctr)
        status |= 2;
    return status;
}

unsigned int syscon::get_swbint_status()
{
    if (this->protocol == 1)
        return get_swbint_status_ver1();
    return 0;
}

int syscon::wait_swbint(long mask)
{
    unsigned int flags = g_sc_swbint_pending;

    if (!(flags & mask)) {
        do {
            flags = g_sc_swbint_pending | get_swbint_status();
            g_sc_swbint_pending = flags;
        } while (!(flags & mask));
    }
    g_sc_swbint_pending = flags & ~mask;
    return 0;
}

int syscon::read_cmpl_msg_ver1(int *id_out, void *data, u32 max_len, int *len_out, struct sc_msg_ids *ids)
{
    FUNCTION_NAME("read_cmpl_msg_ver1");
    u16 tc = read16(this->base + SC_MMIO_SC_TC + 2);
    u16 rc = read16(this->base + SC_MMIO_SC_RC + 2);
    if (tc == rc) {
        uart_printf("%s: sc_tc(0x%04x) == sc_rc(0x%04x)\n", function_name, tc, rc);
        return -16;
    }

    unsigned char *buf = (unsigned char *)g_sc_msg_buffer;
    struct sc_msg_ver1 *m = (struct sc_msg_ver1 *)buf;
    copy_qwords_from_mmio(this->base + SC_MMIO_RX_VER1, (char *)buf, 16);
    u16 psize = m->payload_len;
    if (psize > max_len) {
        uart_printf("%s: too small payload_size %d, body_len %d\n", function_name, max_len, psize);
        return -7;
    }
    if ((unsigned long)psize + 20 > 0x800) {
        uart_printf("%s: too large payload_len %d\n", function_name, psize);
        return -6;
    }
    if (psize != m->payload_len2) {
        uart_printf("%s: body_len(%04x) != body_len2(%04x)\n", function_name, psize, m->payload_len2);
        return -6;
    }

    u16 cs;
    u16 npad = ((u64)psize + 3) & ~3;
    sc_calc_checksum(buf, 6, &cs);
    if (m->hdr_cs != (cs | 0x8000)) {
        uart_printf("%s: header checksum %d != %d\n", function_name, m->hdr_cs, cs);
        return -6;
    }

    copy_qwords_from_mmio(this->base + SC_MMIO_RX_VER1, (char *)buf, ((u64)(npad + 20u) + 15) & ~15);
    sc_calc_checksum_neg(buf, npad + 16, &cs);
    unsigned char *trailer = buf + npad;
    u16 body_cs = ((struct sc_trailer *)(trailer + 16))->cs;
    if (body_cs != cs) {
        uart_printf("%s: payload checksum %d != %d\n", function_name, body_cs, cs);
        return -6;
    }

    memcpy(data, m->payload, m->payload_len);
    *id_out = m->id;
    ids->transaction_id = m->transaction_id;
    ids->communication_tag = m->communication_tag;
    *len_out = m->payload_len;
    write32(this->base + SC_MMIO_SC_RC, (u16)(rc + 1) | ((u16)(rc + 1) << 16));
    return 0;
}

int syscon::read_cmpl_msg(int *id_out, void *data, unsigned int max_len, int *len_out, struct sc_msg_ids *ids)
{
    if (this->protocol == 1)
        return read_cmpl_msg_ver1(id_out, data, max_len, len_out, ids);
    return -8;
}

int syscon::write_init_msg_ver1(long id, const void *payload, unsigned long payload_len, struct sc_msg_ids *ids, bool check_ctr)
{
    FUNCTION_NAME("write_init_msg_ver1");
    unsigned short cs;

    if (payload_len + 20 > 0x800) {
        uart_printf("%s: too large payload_size %d\n", function_name, payload_len);
        return -7;
    }

    u16 tc = read16(this->base + SC_MMIO_BE_TC + 2);
    u16 rc = read16(this->base + SC_MMIO_BE_RC + 2);
    if (check_ctr) {
        if (tc != rc) {
            uart_printf("%s: be_tc(0x%04x) != be_rc(0x%04x)\n", function_name, tc, rc);
            return -16;
        }
    } else {
        tc = 0;
        rc = 0;
    }

    unsigned int pad = 0;
    if (payload_len & 3)
        pad = 4 - (payload_len & 3);

    struct sc_msg_ver1 *m = (struct sc_msg_ver1 *)g_sc_msg_buffer;
    m->id = id;
    m->type = 1;
    m->transaction_id = ids->transaction_id;
    m->zero = 0;
    sc_calc_checksum((unsigned char *)g_sc_msg_buffer, 6, &cs);
    m->hdr_cs = cs | 0x8000;
    m->communication_tag = ids->communication_tag;
    m->payload_len = payload_len;
    m->payload_len2 = payload_len;
    memcpy(m->payload, payload, payload_len);
    unsigned char *cur = m->payload + payload_len;
    {
        unsigned int i;
        for (i = 0; i < pad; i++)
            *cur++ = 0;
    }
    sc_calc_checksum_neg((unsigned char *)g_sc_msg_buffer, cur - (unsigned char *)g_sc_msg_buffer, &cs);
    struct sc_trailer *t = (struct sc_trailer *)cur;
    t->zero = 0;
    t->cs = cs;
    memcpy(cur, t, sizeof(*t));
    cur += sizeof(*t);
    copy_qwords_to_mmio(this->base + SC_MMIO_TX, g_sc_msg_buffer, ((u64)(cur - (unsigned char *)g_sc_msg_buffer) + 15) & ~15);
    write32(this->base + SC_MMIO_BE_TC, (u16)(tc + 1) | ((u16)(tc + 1) << 16));
    this->rc = rc;
    return 0;
}

int syscon::write_init_msg(long id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids)
{
    if (this->protocol == 1)
        return write_init_msg_ver1(id, payload, payload_len, ids, 1);
    return -8;
}

long syscon::initialize(u64 mmio_base, int proto_ver, bool detect)
{
    this->base = mmio_base;
    read_swbint();
    if (detect) {
        char msg[4];
        struct sc_msg_ids ids;
        this->protocol = 1;
        write32(this->base + SC_MMIO_BE_TC, 0);
        write32(this->base + SC_MMIO_SC_RC, 0);
        msg[0] = 1;
        ids.transaction_id = 0;
        ids.communication_tag = 0;
        write_init_msg_ver1(255, msg, 1, &ids, 0);
        raise_sc_interrupt(0);
        while (read32(this->base + SC_MMIO_SWBINT) == 0)
            ;
        if (read64(this->base + SC_MMIO_RX_STAT) == (u64)-1)
            this->protocol = 1;
        else
            this->protocol = 0;
    } else {
        this->protocol = proto_ver;
    }
    return 0;
}

long syscon::send_and_receive(unsigned int id, const void *req, unsigned int req_len, void *resp, unsigned int resp_max, int *resp_len, int flags)
{
    FUNCTION_NAME("send_and_receive");
    long ret = -1;
    int receive_id;

    if (this->base) {
        read_swbint();
        struct sc_msg_ids ids = { ++g_send_and_receive_transaction_id };
        ids.communication_tag = g_send_and_receive_communication_tag;
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
        receive_id = id;
        int stat_rc = read_stat_msg(&receive_id);
        if (stat_rc) {
            uart_printf("%s: read_stat_msg fail %d\n", function_name, stat_rc);
            ret = -2;
            goto out;
        }
        unsigned int first = receive_id;
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
        unsigned int rid = receive_id;
        if (rid != first) {
            uart_printf("%s: read_cmpl_msg unknown id %d\n", function_name, (int)rid);
            ret = -6;
            goto out;
        }
        if (ids.transaction_id != g_send_and_receive_transaction_id) {
            uart_printf("%s: read_cmpl_msg unknown transaction_id %d\n", function_name, g_send_and_receive_transaction_id);
            ret = -6;
            goto out;
        }
        if (ids.communication_tag != g_send_and_receive_communication_tag) {
            uart_printf("%s: read_cmpl_msg unknown communication_tag %d\n", function_name, g_send_and_receive_communication_tag);
            ret = -6;
            goto out;
        }
        int ack_rc = write_stat_msg(rid);
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

u16 g_send_and_receive_transaction_id;
u32 g_send_and_receive_communication_tag;
u16 g_sc_transaction_id;
unsigned int g_sc_swbint_pending;

inline syscon *get_syscon_device()
{
    static syscon device;
    return &device;
}

CXX_DROPPED long get_syscon_device_dropped_user()
{
    return get_syscon_device()->base;
}
