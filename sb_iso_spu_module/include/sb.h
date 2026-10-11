#ifndef SB_SB_H
#define SB_SB_H

#include <spu_intrinsics.h>
#include "types.h"
#include "dma_channel.h"
#include "ss_iso_dma_data.h"

struct key128 {
    u64 hi;
    u64 lo;
};

#define CMD_REPLY       0x2001
#define CMD_PEER_OK     0x2002
#define CMD_ANSWER      0x2003
#define CMD_START_A     0x2004
#define CMD_START_C     0x2005

#define STOP_OK         0x100
#define STOP_ERROR      0x103

extern const unsigned char g_b_enc_key0[32], g_b_dec_key0[32], g_a_key0[32];
extern const unsigned char g_b_enc_key1[32], g_b_dec_key1[32], g_a_key1[32];
extern const unsigned char g_b_enc_key2[32], g_b_dec_key2[32], g_a_key2[32];
extern const unsigned char g_b_enc_key3[32], g_b_dec_key3[32], g_a_key3[32];

class session_b {
public:
    unsigned int xor_bytes(const unsigned char *a, const unsigned char *b, unsigned char *out,
                           unsigned int n);
    unsigned int put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                           quad_uint &len, const unsigned char *msg);
    int check(unsigned int token, unsigned int cmd, const unsigned char *msg, unsigned int len,
              unsigned int size);
    int finish_xor(unsigned int token, unsigned int cmd, const unsigned char *msg,
                   unsigned int len);
    int put_peer(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len);
    int put_challenge(unsigned int &token, unsigned int &cmd, unsigned char *out,
                      quad_uint &len);
    int verify(unsigned int token, unsigned int cmd, const unsigned char *msg, unsigned int len);
    int run();
    unsigned int fill_random(unsigned char *p, unsigned int n);
    void init(ss_iso_dma_data *t, unsigned char *buf, unsigned int cmd, unsigned int version,
              unsigned char *out);

    ss_iso_dma_data *m_t;
    unsigned char *m_buf __attribute__((aligned(16)));
    unsigned int m_cmd;
    const unsigned char *m_enc_key;
    const unsigned char *m_dec_key;
    unsigned char m_iv[16] __attribute__((aligned(16)));
    unsigned char m_rand[32] __attribute__((aligned(16)));
    unsigned char m_peer[32] __attribute__((aligned(16)));
    unsigned char m_xor[32] __attribute__((aligned(16)));
    unsigned char *m_out __attribute__((aligned(16)));
};

class session_a {
public:
    int check(unsigned int token, unsigned int cmd, const unsigned char *msg, unsigned int len,
              unsigned int size);
    int check_answer(unsigned int token, unsigned int cmd, const unsigned char *msg,
                     unsigned int len);
    int put_type(unsigned char *p, unsigned int type);
    unsigned int fill_random(unsigned char *p, unsigned int n);
    unsigned int put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                           quad_uint &len, const unsigned char *msg);
    int put_key(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len);
    int run(unsigned int cmd);
    void init(ss_iso_dma_data *t, unsigned int type, unsigned int version, unsigned int len,
              const unsigned char *key);

    ss_iso_dma_data *m_t;
    const unsigned char *m_key;
    unsigned char m_data[32] __attribute__((aligned(16)));
    unsigned char m_48[64] __attribute__((aligned(16)));
    unsigned int m_type __attribute__((aligned(16)));
    unsigned int m_version;
    unsigned char m_128[16] __attribute__((aligned(16)));
    unsigned char m_session_key[32] __attribute__((aligned(16)));
    unsigned char m_iv[16] __attribute__((aligned(16)));
};

class session_c {
public:
    void init(ss_iso_dma_data *t, unsigned int version);
    int check(unsigned int token, unsigned int cmd, const unsigned char *msg, unsigned int len,
              unsigned int size);
    int check_answer(unsigned int token, unsigned int cmd, const unsigned char *msg,
                     unsigned int len);
    unsigned int put_reply(unsigned int &token, unsigned int &cmd, unsigned char *out,
                           quad_uint &len, const unsigned char *msg);
    int put_done(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len);
    int put_key(unsigned int &token, unsigned int &cmd, unsigned char *out, quad_uint &len);
    int run();

    ss_iso_dma_data *m_t;
    unsigned int m_version;
    unsigned char m_key[24];
    unsigned char m_32[16];
};

class sb_module {
public:
    int serve(ss_iso_dma_data *t);
};

#define GET_BE32(p)     ((p)[0] << 24 | (p)[1] << 16 | (p)[2] << 8 | (p)[3])

#endif
