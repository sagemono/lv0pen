#ifndef LV0_SYSCON_H
#define LV0_SYSCON_H

#include "lv0.h"

enum sc_command {
    SC_CMD_DEV_ACCESS   = 3,
    SC_CMD_POWER_STATUS = 16,
    SC_CMD_CONFIG       = 18,
    SC_CMD_POWER        = 19,
    SC_CMD_NV_STORAGE   = 20,
    SC_CMD_VERSION      = 24,
    SC_CMD_CONSOLE      = 32,
};

struct syscon *get_syscon_device(void);

#ifdef __cplusplus
struct sc_msg_ids;
class syscon {
public:
    syscon() : base(0), f8(1), rc(0) {}
    long raise_sc_interrupt(unsigned int ch);
    unsigned int get_protocol_version();
    unsigned long sc_calc_checksum(unsigned char *buf, unsigned int n, unsigned short *out);
    unsigned long sc_calc_checksum_neg(unsigned char *buf, unsigned int n, unsigned short *out);
    int write_init_msg_ver1(unsigned int id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids, int check_ctr);
    int write_init_msg(unsigned int id, const void *payload, unsigned int payload_len, struct sc_msg_ids *ids);
    int read_cmpl_msg_ver1(enum sc_command *id_out, void *data, u32 max_len, int *len_out, struct sc_msg_ids *ids);
    int read_cmpl_msg(enum sc_command *id_out, void *data, unsigned int max_len, int *len_out, struct sc_msg_ids *ids);
    u32 get_swbint_status_ver1();
    unsigned int get_swbint_status();
    int wait_swbint(unsigned int mask);
    int read_stat_msg(enum sc_command *id_out);
    int write_stat_msg(unsigned int id);
    long send_and_receive_safe(u32 id, const void *req, u32 req_len, void *resp, u32 resp_max, int *resp_len, int flags);
    long initialize(long mmio_base, int proto_ver, int detect);
    int send_and_receive(unsigned int id, const void *req, unsigned int req_len, void *resp, unsigned int resp_max, int *resp_len, int flags);

    long base;
    u8 f8;
    int protocol;
    u16 rc;
};

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
    static int check_status(unsigned int status);
    static int write(unsigned int region, unsigned int off, unsigned int size, const void *data);
    static int read(unsigned int region, unsigned int off, unsigned int size, void *dst, unsigned int *outlen);
};

class nv_storage {
public:
    static int read(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base);
    static int write(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base);
};
#endif

int get_sc_version(unsigned char arg, unsigned short *out);
unsigned long get_syscon_protocol_version(void);
long sc_write_cmd32_chunked(const unsigned char *data, unsigned int size);
int syscon_get_core_clock_multiplier(long *out);
int syscon_get_platform_id(unsigned char *out);
int syscon_get_hw_config(unsigned long *out, unsigned long *out2);
int syscon_get_clock_value(unsigned char *out, unsigned char req0, unsigned char req1, int scale);
int syscon_get_ref_clock(unsigned int *out, int scale);
long syscon_write_string(const char *str);
long syscon_write_buffer(const unsigned char *data, unsigned long size);
unsigned long copy_qwords_to_mmio(char *dest, char *src, unsigned int count);
unsigned long copy_qwords_from_mmio(char *src, char *dst, unsigned int n);

long syscon_shutdown(void);
long syscon_reboot(void);
int syscon_get_wake_info(unsigned char *out1, unsigned char *out2, unsigned char *out3, unsigned char *out4, unsigned char *out5, unsigned int *out6);
void syscon_power_off_with_code(int p1, int p2, int p3);
int syscon_set_wake_source(int arg);
bool is_sc_nv_storage_supported(void);
int syscon_syspm_get_33(unsigned int *out);
int get_boot_gos(void);

extern const unsigned char sc_core_clock_multiplier_request[2];
extern const unsigned char sc_platform_id_request[2];

#endif
