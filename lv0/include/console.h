#ifndef LV0_CONSOLE_H
#define LV0_CONSOLE_H

#include "lv0.h"
#include "cxx.h"
#include "pci.h"

typedef int get_file_fn(const char *name, unsigned long offset, void *buf, unsigned long size,
                        unsigned long *out_len);
typedef int put_file_fn(const char *name, unsigned long offset, const void *buf, unsigned long size,
                        unsigned long *out_len);
typedef int get_file_size_fn(const char *name, unsigned long *out_size);

struct ring_buffer_header {
    long capacity;
    long count;
    long write_index;
    long read_index;
};

#ifdef __cplusplus
class mmio_accessor;
class physical_console;

class ring_buffer {
public:
    ring_buffer();
    ~ring_buffer();
    void attach(long size, ring_buffer_header *header);
    long push(unsigned char byte);
    int pop(unsigned char *out);
    int write(const unsigned char *src, unsigned long len, unsigned long *done);
    int read(unsigned char *dst, unsigned long len, unsigned long *done);

    long *capacity;
    long *count;
    long *write_index;
    long *read_index;
    unsigned char *data;
    unsigned char overwrite;
};

class log_cons_channel {
public:
    log_cons_channel();
    ~log_cons_channel();
    int init(char *buf, unsigned long size);
    int flush();
    long receive();
    void on_receive();
    int poll();

    long tag;
    unsigned short id;
    physical_console *console;
    ring_buffer tx;
    ring_buffer rx;
    ring_buffer *tx_ring;
    ring_buffer *rx_ring;
};

class internal_console {
public:
    internal_console() CXX_DROPPED;
    static int initialize();
    int write(const char *buf, long len);

    log_cons_channel *channel;
    unsigned char flag;
};

class cp_bridge {
public:
    cp_bridge();
    unsigned short cfg_read16(unsigned int reg);
    void cfg_write32(unsigned int reg, unsigned int val);
    unsigned long cfg_read32(unsigned int reg);
    void bridge_cfg_write32(unsigned int reg, unsigned int val);
    unsigned int bridge_cfg_read32(unsigned int reg);
    unsigned long map(unsigned int addr);
    unsigned int read32(unsigned int addr);
    void write32(unsigned int addr, unsigned int val);
    void set_window(unsigned int id, unsigned int addr);
    void window_low();
    void window_high();
    bool init();
    void write_raw(unsigned int off, unsigned int val);
    void write_ctl(unsigned int addr, unsigned int val);
    unsigned int read_raw(unsigned int off);
    unsigned int read_ctl(unsigned int addr);

    unsigned long base;
    unsigned int window;
};

cp_bridge *get_cp_bridge(void);

class cp_link {
public:
    cp_link();
    virtual ~cp_link() {}
    virtual int receive(void *buf, long size, unsigned short *out_len) = 0;
    virtual int send(void *buf, unsigned short len, unsigned short *out_len) = 0;
    virtual int connect() = 0;
    virtual long disconnect() = 0;
    virtual long vslot6() = 0;
    virtual long vslot7() = 0;
    virtual unsigned int read32(long addr);
    virtual void write32(long addr, long val);
    void delay_us(unsigned long us);
    void delay_ms(unsigned long ms);

    physical_console *console;
    mmio_accessor *mmio;
};

struct cp_devparam : pci_devparam {
    cp_devparam() { cfg_base = 0; bus = 0; dev = 0; fn = 0; pad = 0; vendor = 0; device = 0; }
};

static inline unsigned int ld_le32(const void *p)
{
    unsigned int v;
    __asm__ __volatile__("lwbrx %0,0,%1" : "=&r"(v) : "r"(p) : "memory");
    return v;
}

static inline unsigned short ld_le16(const void *p)
{
    unsigned short v;
    __asm__ __volatile__("lhbrx %0,0,%1" : "=&r"(v) : "r"(p) : "memory");
    return v;
}

static inline unsigned long ld_le64(const void *p)
{
    unsigned long v;
    __asm__ __volatile__("ldbrx %0,0,%1" : "=&r"(v) : "r"(p) : "memory");
    return v;
}

class cp_channel : public cp_link {
public:
    cp_channel();
    virtual ~cp_channel();
    int find_lowest_set_bit(unsigned int bits);
    virtual long vslot7();
    virtual int connect();
    virtual long vslot6();
    long reset();
    virtual long disconnect();
    void set_device(pci_devparam *src, cp_bridge *bridge, unsigned int channel);
    unsigned int read_bar0(unsigned int off);
    void write_bar0(unsigned int off, unsigned int val);
    void write_ch(unsigned int off, unsigned int val);
    unsigned int read_bar1(unsigned int off);
    void read_config(unsigned int *out);
    void write_bar1(unsigned int off, unsigned int val);
    unsigned long cfg_read32(unsigned int reg);
    void cfg_write32(unsigned int reg, unsigned int val);
    int initialize(pci_devparam *src, cp_bridge *bridge, unsigned int channel);
    void setup();
    virtual int receive(void *buf, long size, unsigned short *out_len);
    virtual int send(void *buf, unsigned short len, unsigned short *out_len);

    unsigned int bar0;
    unsigned int bar0_size;
    unsigned int bar1;
    unsigned int bar1_size;
    volatile u64 *tx_buf;
    u64 rx_buf;
    u64 status_buf;
    unsigned int channel;
    pci_devparam *devparam;
    cp_bridge *bridge;
};

class cp_ch4_channel : public cp_channel {
public:
    cp_ch4_channel();
    virtual ~cp_ch4_channel();
    virtual int receive(void *buf, long size, unsigned short *out_len);
    template <class T> T bswap(T x);
    virtual long disconnect();
    virtual int send(void *buf, unsigned short len, unsigned short *out_len);
    virtual int connect();
};

class physical_console {
public:
    physical_console();
    long deliver(ring_buffer *ring, unsigned short chan_id, long tag);
    template <class T> T bswap(T x);
    long decode_header(unsigned short *hdr, unsigned short *out_len, unsigned short *out_chan_id, long *out_tag);
    int receive();
    int poll();
    long encode_header(unsigned char *hdr, unsigned short len, unsigned short chan_id, long tag);
    int send(ring_buffer *ring, unsigned short chan_id, long tag);
    static int initialize();
    static void finalize();

    unsigned short rx_len;
    unsigned short rx_channel_id;
    long rx_tag;
    cp_link *link;
    long unused;
};

extern physical_console g_physical_console;
extern unsigned long g_physical_console_frame_size;

class file_transfer {
public:
    file_transfer();
    int status_to_error(unsigned long status);
    long write_channel(log_cons_channel *chan, void *buf, long len);
    int read_channel(log_cons_channel *chan, void *buf, long len);
    template <class T> T bswap(T x);
    void decode_message(long *msg, long *cmd, long *param);
    void encode_message(void *msg, long cmd, long param);
    long send_file_name(const char *name);
    int get_file(const char *name, unsigned long offset, void *buf, unsigned long size,
                  unsigned long *out_size);
    int put_file(const char *name, unsigned long offset, const void *buf, unsigned long size,
                  unsigned long *out_size);
    int get_file_size(const char *name, unsigned long *out_size);
    int get_file_name(void *buf, unsigned long size, unsigned long arg);
    int authorize_file(const void *src, unsigned long src_size, void *dst);
    int file_command_15(const char *name, unsigned long offset);

    log_cons_channel *ctl;
    log_cons_channel *data;
};
#endif

struct physical_console *get_physical_console(void);
struct file_transfer *get_ft_channels(void);
int init_debug_interface(struct physical_console *console);
struct cp_channel *get_cp_channel(void);
struct cp_ch4_channel *get_cp_ch4_channel(void);

get_file_fn call_get_file_hook;
get_file_size_fn call_get_file_size_hook;
bool is_host_file_io_unavailable(void);
get_file_fn host_get_file;
put_file_fn host_put_file;
get_file_size_fn host_get_file_size;
get_file_fn ext_get_file;
put_file_fn ext_put_file;
get_file_size_fn ext_get_file_size;
long ext_console_output(const char *msg);
long uart_console_puts(const char *str);
int init_log_cons_ctl_and_data(void);

#endif
