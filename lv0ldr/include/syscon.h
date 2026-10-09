#ifndef PT_SYSCON_H
#define PT_SYSCON_H

#include "mmio.h"

enum sc_command {
    SC_CMD_DEV_ACCESS   = 3,
    SC_CMD_POWER_STATUS = 16,
    SC_CMD_CONFIG       = 18,
    SC_CMD_POWER        = 19,
    SC_CMD_NV_STORAGE   = 20,
    SC_CMD_VERSION      = 24,
    SC_CMD_CONSOLE      = 32,
};

struct sc_msg_ids;
class syscon : public mmio_device {
public:
    syscon() : f8(1), rc(0) {}
    unsigned int read_swbint();
    long raise_sc_interrupt(unsigned long ch);
    unsigned int get_protocol_version();
    void set_f8(u8 v);
    unsigned long sc_calc_checksum(unsigned char *buf, unsigned int n, unsigned short *out);
    unsigned long sc_calc_checksum_neg(unsigned char *buf, unsigned int n, unsigned short *out);
    int write_init_msg_ver1(long id, const void *payload, unsigned long payload_len, struct sc_msg_ids *ids, bool check_ctr);
    int write_init_msg(long id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids);
    int read_cmpl_msg_ver1(int *id_out, void *data, u32 max_len, int *len_out, struct sc_msg_ids *ids);
    int read_cmpl_msg(int *id_out, void *data, unsigned int max_len, int *len_out, struct sc_msg_ids *ids);
    u32 get_swbint_status_ver1();
    unsigned int get_swbint_status();
    int wait_swbint(long mask);
    int read_stat_msg(int *id_out);
    int write_stat_msg(unsigned int id);
    long send_and_receive_safe(u32 id, const void *req, u32 req_len, void *resp, u32 resp_max, int *resp_len, int flags);
    long initialize(u64 mmio_base, int proto_ver, bool detect);
    long send_and_receive(unsigned int id, const void *req, unsigned int req_len, void *resp, unsigned int resp_max, int *resp_len, int flags);

    u8 f8;
    int protocol;
    u16 rc;
};

syscon *get_syscon_device();

extern u16 g_send_and_receive_transaction_id;
extern u32 g_send_and_receive_communication_tag;
extern u16 g_sc_transaction_id;
extern unsigned int g_sc_swbint_pending;

class sc_dev_access {
public:
    static long read(unsigned int addr, void *dst, unsigned int size);
    static long write(unsigned int addr, const void *data, unsigned int size);
};

class sc_nv_storage {
public:
    static long check_status(unsigned char status);
    static long write(unsigned int region, unsigned int off, unsigned int size, const void *data);
    static long read(unsigned int region, unsigned int off, unsigned int size, void *dst, unsigned int *outlen);
};

class nv_storage {
public:
    static long read(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base);
    static long write(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base);
};

long get_sc_version(unsigned char arg, unsigned short *out);

long syscon_read_xdr_config(void *out);
long syscon_read_config_2(unsigned int *out);
long syscon_get_wake_info(unsigned char *out1, unsigned char *out2, unsigned char *out3,
                          unsigned char *out4, unsigned char *out5, unsigned int *out6);
long syscon_get_core_clock_multiplier(s64 *out);
long syscon_get_ref_clock(unsigned int *out, bool scale);
long syscon_get_xdr_clock(unsigned int *out, bool scale);
long syscon_set_xdr_rail(unsigned char a, unsigned char b);
long syscon_write_xdr_config(unsigned int a, unsigned int b);
void syscon_printf(const char *fmt, ...);
long syscon_set_wake_source(unsigned int source);
void syscon_power_off_with_code(int p1, int p2, int p3);
bool is_sc_nv_storage_supported(void);
long load_nv_entry(unsigned int idx);
long syscon_write_string(const char *str);

#endif
