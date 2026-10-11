#ifndef SCISO_SC_ISO_MODULE_H
#define SCISO_SC_ISO_MODULE_H

#include "types.h"
#include "ss_iso_dma_data.h"
#include "sc_pkt_hdr.h"
#include "sc_proxy_hdr.h"
#include "sc_cmd.h"
#include "sc_crypto.h"

struct op32_msg {
    u8 magic;
    u8 b1;
    u8 b2;
    u8 body[64];
} __attribute__((aligned(16)));

struct op32_body {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 data[60];
} __attribute__((aligned(16)));

struct op33_msg {
    u8 b0;
    u8 magic;
    u8 type;
    u8 ver;
    u8 b4;
    u8 b5;
    u8 body[64];
} __attribute__((aligned(16)));

struct op33_body {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 sig[48];
    u8 rest[12];
} __attribute__((aligned(16)));

struct sc_data_hdr {
    u64 size;
    unsigned int m_8;
    unsigned int m_12;
} __attribute__((aligned(16)));
typedef sc_data_hdr *quad_hdr_ptr __attribute__((aligned(16)));

class sc_iso_module {
public:
    sc_iso_module(ss_iso_dma_data *dma, const quad_ptr &buf, const quad_uint &size,
                  const quad_ptr &data, const quad_uint &data_size, const u8 *keys,
                  const u8 *key2, const quad_u64 &area, const quad_uint &id);
    ~sc_iso_module();

    unsigned int run(unsigned int &cmd, quad_ptr &buf, quad_uint &len);

#define SC_HANDLER(name)     unsigned int name(const quad_ptr &in, const quad_uint &in_len, const quad_ptr &out,                       const quad_uint &out_size, quad_uint &out_len, unsigned int &status)
    SC_HANDLER(cmd_encrypt);
    SC_HANDLER(cmd_decrypt);
    SC_HANDLER(cmd_get_srh);
    SC_HANDLER(cmd_set_srh);
    SC_HANDLER(cmd_init_srk);
    SC_HANDLER(cmd_get_region_data);
    SC_HANDLER(cmd_set_region_data);
    SC_HANDLER(cmd_get_time);
    SC_HANDLER(cmd_reset_records);
    SC_HANDLER(cmd_get_sc_status);
    SC_HANDLER(cmd_set_time);
    SC_HANDLER(cmd_rekey);
    SC_HANDLER(cmd_sc_binary_patch);
    SC_HANDLER(cmd_set_sc_status);
    SC_HANDLER(cmd_backup_root_info);
    SC_HANDLER(cmd_restore_root_info);
    SC_HANDLER(cmd_pmode_srv_auth);
    SC_HANDLER(cmd_get_property);
#undef SC_HANDLER

    static u8 *copy_bytes(u8 *dst, const u8 *src, unsigned int n);
    static int bytes_differ(const unsigned char *a, const unsigned char *b, int n);
    static void fill_bytes(u8 *dst, u8 c, unsigned int n);
    unsigned int get_random(u8 *buf, unsigned int n, unsigned int &status);
    unsigned int time_diff(const u64 &a, const u64 &b, u64 &diff);
    unsigned int use_old_auth_keys();
    static unsigned int xts_decrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                               const int &len, const u8 *key1, const int &key1_bits,
                               const u8 *key2, const int &key2_bits, const u8 *tweak,
                               const int &tweak_bits);
    static unsigned int xts_encrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                               const int &len, const u8 *key1, const int &key1_bits,
                               const u8 *key2, const int &key2_bits, const u8 *tweak,
                               const int &tweak_bits);
    void make_tweak(u8 *buf, unsigned int n, unsigned int v);
    unsigned int pmode_make_challenge(const op32_msg *in, op32_msg *out, u8 *rnd, unsigned int rnd_len,
                       unsigned int &status);
    unsigned int get_syscon_state(u8 *state);
    unsigned int authenticate(unsigned int id, const u8 *key1, const u8 *key2, void *ctx);
    unsigned int open_session();
    unsigned int exec_cmd(sc_cmd *cmd, u8 authed);
    unsigned int exec_plain(sc_cmd *cmd);
    unsigned int read_sc_info(u8 *data, unsigned int &status);
    unsigned int get_sc_version(u8 *out);
    unsigned int exec(sc_cmd *cmd);
    unsigned int derive_auth_key(u8 *out, const u8 *in);
    unsigned int derive_auth_keys();
    unsigned int set_state_byte(const u8 &v, unsigned int &status);
    unsigned int get_state_byte(u8 &a, unsigned int &status);
    unsigned int install_key_material(const quad_uint &data, const quad_uint &len, unsigned int &status);
    unsigned int set_time_offset(const u64 &mode, const s64 &value, unsigned int &status);
    unsigned int read_time(const u64 &mode, u64 &value, u64 &value2, unsigned int &status);
    unsigned int get_key(const u64 &id, const quad_ptr &buf, quad_uint &len);
    unsigned int pmode_check_response(const op33_msg *in, op33_msg *out, u8 *rnd, unsigned int rnd_len,
                       unsigned int &status);
    unsigned int read_data(const quad_ptr &buf, const quad_uint &offset, const quad_uint &len,
                       quad_uint &out_len);
    unsigned int encrypt_seed(const quad_ptr &buf, const quad_uint &len, const u8 *data);
    static unsigned int cbc_decrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                               const int &len, const u8 *key, const int &key_bits, u8 *iv,
                               const int &iv_bits);
    static unsigned int cbc_encrypt(sc_crypto *c, const u8 *in, const int &in_len, u8 *out,
                               const int &len, const u8 *key, const int &key_bits, u8 *iv,
                               const int &iv_bits);
    unsigned int decrypt_sectors(const quad_ptr &buf, const quad_uint &offset, const quad_uint &len);
    unsigned int read_slot_data(const quad_ptr &buf, const quad_uint &offset, const quad_uint &len,
                       const unsigned int &seq, quad_uint &out_len);
    unsigned int read_record(const u64 &id, const quad_ptr &buf, const quad_uint &len);
    unsigned int read_srh(const quad_ptr &buf, const quad_uint &len);
    unsigned int read_mngblk(void *buf);
    unsigned int encrypt_sectors(const quad_ptr &buf, const quad_uint &offset, const quad_uint &len);
    unsigned int write_slot_data(const quad_ptr &buf, const quad_uint &offset, const quad_uint &len,
                       const unsigned int &seq, quad_uint &out_len);
    unsigned int write_area(const u8 *data, const quad_uint &offset, const quad_uint &size,
                        const quad_uint &x);
    unsigned int patch_4k(const quad_hdr_ptr &buf, const quad_uint &len, const quad_ptr &buf2,
                       const quad_uint &size, quad_uint &out_len, unsigned int &status);
    unsigned int patch_areas(const quad_hdr_ptr &buf, const quad_uint &len, const quad_ptr &buf2,
                       const quad_uint &size, quad_uint &out_len, unsigned int &status);
    unsigned int write_data(const quad_ptr &data, const quad_uint &offset, const quad_uint &len,
                       quad_uint &out_len);
    unsigned int write_record(const u64 &id, const quad_ptr &buf, const quad_uint &len);
    unsigned int write_srh(const quad_ptr &buf, const quad_uint &len);
    unsigned int write_srk(const u64 &id, const quad_ptr &buf, const quad_uint &len);
    unsigned int write_mngblk(void *data);
    unsigned int erase_records(unsigned int &status);
    unsigned int crypt_data(const int &mode, const u8 &encrypt, const quad_ptr &buf,
                       const quad_uint &len, const quad_ptr &out, const quad_uint &size,
                       quad_uint &out_len, unsigned int &status);
    unsigned int derive_srk2_key(const quad_ptr &buf, const quad_uint &len);

    ss_iso_dma_data *m_dma;
    quad_ptr m_buf;
    quad_uint m_size;
    quad_ptr m_data;
    quad_uint m_data_size;
    u8 m_keys[32] __attribute__((aligned(16)));
    u8 m_key2[16] __attribute__((aligned(16)));
    quad_u64 m_area;
    quad_uint m_144;
    sc_pkt_hdr *m_pkt;
    u8 m_152;
    unsigned int m_id;
    quad_u64 m_ctx;
    u8 m_176 __attribute__((aligned(16)));
};

#endif
