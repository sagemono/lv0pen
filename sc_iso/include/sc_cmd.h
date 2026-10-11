#ifndef SCISO_SC_CMD_H
#define SCISO_SC_CMD_H

#include "types.h"
#include "ss_iso_dma_data.h"

#define REPLY_LEN       688

#define BLOCKS(n)       ((n) / 16 + ((n) % 16 != 0))

class sc_cmd {
public:
    sc_cmd();
    virtual ~sc_cmd() = 0;
    virtual unsigned int set(const quad_uint &offset, const quad_uint &len);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);
    virtual unsigned int get(const quad_ptr &buf, const quad_uint &max, quad_uint &len);
};

class sc_cmd_08 : public sc_cmd {
public:
    sc_cmd_08(const unsigned int &seq);
    sc_cmd_08();
    virtual ~sc_cmd_08();
    virtual unsigned int set(const quad_uint &offset, const quad_uint &len);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);
    virtual unsigned int get(const quad_ptr &buf, const quad_uint &max, quad_uint &len);

    quad_uint m_offset;
    quad_uint m_len;
    unsigned char m_data[128];
    unsigned int m_seq;
    bool m_have_seq;
};

class sc_cmd_0d : public sc_cmd {
public:
    sc_cmd_0d(const unsigned int &seq);
    sc_cmd_0d();
    virtual ~sc_cmd_0d();
    unsigned int set(const quad_ptr &data, const quad_uint &offset, const quad_uint &len);
    unsigned int get(quad_uint &len);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    quad_uint m_offset;
    quad_uint m_len;
    unsigned char m_data[128];
    unsigned int m_seq;
    bool m_have_seq;
};

class sc_cmd_07 : public sc_cmd {
public:
    sc_cmd_07();
    virtual ~sc_cmd_07();
    unsigned int set(const u64 &mode);
    unsigned int get(u64 &value, u64 &value2);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    u64 m_value;
    u8 m_mode;
    u64 m_value2;
};

class sc_cmd_09 : public sc_cmd {
public:
    sc_cmd_09();
    virtual ~sc_cmd_09();
    unsigned int set(const u64 &mode, const s64 &value);
    unsigned int get();
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    s64 m_value;
    u8 m_mode;
};

class sc_cmd_0b : public sc_cmd {
public:
    sc_cmd_0b();
    virtual ~sc_cmd_0b();
    virtual unsigned int set(const quad_uint &data, const quad_uint &len);
    unsigned int get();
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    u8 m_data[32];
};

class sc_cmd_05 : public sc_cmd {
public:
    sc_cmd_05();
    virtual ~sc_cmd_05();
    unsigned int set();
    unsigned int get(u8 &a, u8 &b);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    u8 m_a;
    u8 m_b;
};

class sc_cmd_0c : public sc_cmd {
public:
    sc_cmd_0c();
    virtual ~sc_cmd_0c();
    unsigned int set(const u8 &v);
    unsigned int get();
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    u8 m_v;
};

class sc_cmd_04 : public sc_cmd {
public:
    sc_cmd_04();
    virtual ~sc_cmd_04();
    unsigned int set();
    unsigned int get(u8 *data);
    virtual unsigned int make(const quad_ptr &buf, const quad_uint &size, quad_uint &len,
                              quad_uint &reply_len, const unsigned int &id, void *ctx);
    virtual unsigned int check(const quad_ptr &buf, const quad_uint &size,
                               const unsigned int &id, void *ctx);

    u8 m_data[128];
};

#endif
