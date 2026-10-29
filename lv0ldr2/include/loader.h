#ifndef LDR_LOADER_H
#define LDR_LOADER_H

#include "types.h"

long set_livelock_detection_mode(int mode);

struct flash_format_header {
    unsigned int magic;
    unsigned int version;
    unsigned int format;
};
extern struct flash_format_header g_flash_format;

int init_nand_flash_dma(u64 flash_base, u64 buf, u64 size);

unsigned char get_boot_fir_config(void);
bool get_bootrom_diag(unsigned char *mode, unsigned char *second);
const char *get_debug_device_name(void);
int get_boot_memory_type(void);
bool is_boot_memory_type_nor(void);
bool is_boot_memory_type_nand(void);
void release_dma_io_address(void);
int map_flash_format_window(u64 flash_base, unsigned int w2);
bool is_config_2_zero(void);
bool setup_livelock_detection(void);
long handle_fatal_error(unsigned int code);
void post_code_aa(void);
void post_code_01(void);
void post_code_02(void);
void post_code_03(void);
void post_code_05(void);
void post_code_06(void);
void post_code_07(bool f80, bool f40);
void post_code_08(void);
void post_code_30(void);
void post_lv0_auth_fail(void);
void post_lv0_not_found(void);
u64 get_first_spu_priv1_20(void);

extern u64 g_dma_io_addr;

class loader_base {
public:
    void print(const char *msg);
    int get_os_bank_indicator(void);
    int is_sc_protocol_ver1(void);
    void initialize(void);
    void finalize(bool fatal, long rc);
    int get_flash_layout(void);
    u64 get_flash_rom_base(void);
    u64 copy_to_main_memory(u64 src, unsigned int size);
    long init_memory(void);
    long check_config_ring(void);
};

class loader : public loader_base {
public:
    loader();
    virtual long run(void);
    virtual void finalize(bool fatal, long rc);
    virtual void halt(void);
    virtual void start(void);
    virtual long load_lv0(u64 lv0, u64 lv0_2);
    virtual void handoff(void);
    virtual void print_banner(void);
    long find_lv0(u64 *lv0, u64 *lv0_2);
    long find_file(u64 toc, const char *name, u64 *off, u64 *size);
    long find_lv0_ros(u64 base, u64 *lv0, u64 *lv0_2);
    long find_lv0_flash(u64 base, u64 *lv0, u64 *lv0_2);
};

struct ros_toc_header {
    u32 version;
    u32 count;
    u64 size;
};

struct ros_toc_entry {
    u64 offset;
    u64 size;
    char name[32];
};

class config_ring {
public:
    long verify(void);
    bool cmp_bits(const unsigned char *a, const unsigned char *b,
                  unsigned short start, unsigned short count,
                  int offset);
};

long ros_read_header(u64 addr, ros_toc_header *hdr);
long ros_find_entry(u64 addr, u32 count, const char *name, ros_toc_entry *out);

struct memory_config {
    unsigned char basic[128];
    bool str;
    unsigned int f_84;
    unsigned int xio_ref_clk;
    unsigned int be_ref_clk;
    s64 be_pll_multiplier;
    unsigned int f_98;
    unsigned int f_9c;
    unsigned int f_a0;
    unsigned int f_a4;
    unsigned int f_a8;
    unsigned short f_ac;
    unsigned int f_b0;
    unsigned int f_b4;
    unsigned int f_b8;
    bool ecc_off_ch0;
    bool ecc_off_ch1;
    bool ecc_on;
    bool xio_ch0_on;
    bool xio_ch1_on;
};

u64 get_memory_size(void);
bool memory_diag(u64 size, unsigned int unused1, u64 unused2, unsigned char mode);
long ata_activation(u64 addr, u64 size);

class memory_config_query {
public:
    void initialize(void);
    long is_str(bool *str);
    long query_config(memory_config *cfg);
};

#endif
