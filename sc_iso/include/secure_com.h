#ifndef SCISO_SECURE_COM_H
#define SCISO_SECURE_COM_H

#include <spu_intrinsics.h>
#include "types.h"

struct sc_buf {
    unsigned char *addr;
    unsigned int size;
    unsigned int *len;
};

class secure_com_lib {
public:
    secure_com_lib();

    unsigned int get_error();

    int create_get_state_cmd(const sc_buf *buf);
    int validate_get_state_cmd(const sc_buf *buf, u8 *state);
    int create_04_cmd(const sc_buf *buf);
    int validate_04_cmd(const sc_buf *buf, u8 *data);
    int create_auth1_cmd(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
    int validate_auth1_cmd(const sc_buf *buf, unsigned int id, const void *key, const u8 *rnd,
                           u8 *data);
    int create_auth2_cmd(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
    int validate_auth2_cmd(const sc_buf *buf, unsigned int id, unsigned int unused,
                           const u8 *rnd, const void *key, u8 *session_key);

    int create_08_cmd(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
                      unsigned int blocks, unsigned int seq);
    int validate_08_cmd(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                        unsigned int *blocks, unsigned int *seq, u8 *data, unsigned int max);
    int create_0d_cmd(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
                      unsigned int blocks, const u8 *data, unsigned int len, unsigned int seq);
    int validate_0d_cmd(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                        unsigned int *blocks, unsigned int *seq);
    int create_07_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 mode);
    int validate_07_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *mode,
                        unsigned int *value);
    int create_09_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 mode,
                      unsigned int neg, unsigned int value, unsigned int unused);
    int validate_09_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *mode);
    int create_0b_cmd(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
    int validate_0b_cmd(const sc_buf *buf, unsigned int id, const void *key);
    int create_05_cmd(const sc_buf *buf, unsigned int id, const void *key);
    int validate_05_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 *a, u8 *b);
    int create_0c_cmd(const sc_buf *buf, unsigned int id, const void *key, u8 v);
    int validate_0c_cmd(const sc_buf *buf, unsigned int id, const void *key);

private:
    int wrap(unsigned int len, unsigned int id, unsigned int type);
    int unwrap(unsigned int len, unsigned int id, unsigned int type);
    int send(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int type);
    int receive(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int type);
    int wrap_cmd(unsigned int len, unsigned int id, unsigned int cmd, const void *key);
    int unwrap_cmd(unsigned int len, unsigned int id, unsigned int cmd, const void *key);
    int send_cmd(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int cmd,
                 const void *key);
    int receive_cmd(const sc_buf *buf, unsigned int len, unsigned int id, unsigned int cmd,
                    const void *key);
    int send_09(const sc_buf *buf, unsigned int id, const void *key, u8 mode, const u8 *data);

    u64 m_seq;
    bool m_crypt;
    u8 m_hdr3;
    u8 m_hdr4;
    vec_uchar16 m_key_out;
    vec_uchar16 m_key_in;
    unsigned int m_error;
};

extern secure_com_lib g_secure_com;

unsigned int sc_lib_error();

int sc_create_get_state_cmd(const sc_buf *buf);
int sc_validate_get_state_cmd(const sc_buf *buf, u8 *state);
int sc_make_04(const sc_buf *buf);
int sc_check_04(const sc_buf *buf, u8 *data);
int sc_make_auth1(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
int sc_check_auth1(const sc_buf *buf, unsigned int id, const void *key, const u8 *rnd, u8 *data);
int sc_make_auth2(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
int sc_check_auth2(const sc_buf *buf, unsigned int key_index, unsigned int id, const u8 *rnd,
                   const void *key, u8 *session_key);

int sc_make_08(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
               unsigned int blocks, unsigned int seq);
int sc_check_08(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                unsigned int *blocks, unsigned int *seq, u8 *data, unsigned int max);
int sc_make_0d(const sc_buf *buf, unsigned int id, const void *key, unsigned int offset,
               unsigned int blocks, const u8 *data, unsigned int len, unsigned int seq);
int sc_check_0d(const sc_buf *buf, unsigned int id, const void *key, unsigned int *offset,
                unsigned int *blocks, unsigned int *seq);
int sc_make_07(const sc_buf *buf, unsigned int id, const void *key, u8 mode);
int sc_check_07(const sc_buf *buf, unsigned int id, const void *key, u8 *mode, unsigned int *value);
int sc_make_09(const sc_buf *buf, unsigned int id, const void *key, u8 mode, unsigned int neg,
               unsigned int value, unsigned int unused);
int sc_check_09(const sc_buf *buf, unsigned int id, const void *key, u8 *mode);
int sc_make_0b(const sc_buf *buf, unsigned int id, const void *key, const u8 *data);
int sc_check_0b(const sc_buf *buf, unsigned int id, const void *key);
int sc_make_05(const sc_buf *buf, unsigned int id, const void *key);
int sc_check_05(const sc_buf *buf, unsigned int id, const void *key, u8 *a, u8 *b);
int sc_make_0c(const sc_buf *buf, unsigned int id, const void *key, u8 v);
int sc_check_0c(const sc_buf *buf, unsigned int id, const void *key);

#endif
