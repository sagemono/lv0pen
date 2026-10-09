#ifndef LV0_PLATFORM_H
#define LV0_PLATFORM_H

#include "lv0.h"
#include "cxx.h"

struct platform;

struct boot_parm {
    int version;
    int header_size;
    long reserved8;
    long parm_txt_addr;
    long parm_txt_size;
    long reserved32;
    long reserved40;
    long os_image_addr;
    long os_image_size;
    long sysctl_txt_addr;
    long sysctl_txt_size;
    long alt_sys_parm_addr;
    long alt_sys_parm_size;
    char alt_sys_parm[0x800];
    unsigned int alt_sys_parm_len;
    long reserved_868;
    long reserved_870;
    char body[0x1400];
#ifdef __cplusplus
    boot_parm();
#endif
};

struct boot_parm *get_boot_parm(void);

#ifndef PLATFORM_UNIT
#define PLATFORM_UNIT
#endif

#ifdef __cplusplus
class platform;
extern platform *g_platform_ptr;

class platform {
public:
    platform() : dma_io_addr(0) { g_platform_ptr = this; }
    long dma_io_addr;
    int set_post_code(long mask, long code);
    virtual void post_init_done();
    virtual void post_load_lv1_start();
    virtual void post_load_lv1_done();
    virtual void post_load_lv1_failed();
    virtual int stub_slot4(void *buf, long size);
    virtual long stub_slot5(void *buf);
    virtual long stub_slot6();
    virtual long print_system_info();
    virtual long initialize();
    virtual int initialize_io();
    virtual void finalize();
    void print_memory_info(int level);
    void log(unsigned int level, const char *fmt, ...) PLATFORM_UNIT;
    void print_chip_revisions(long level);
    void print_spu_enable(long level);
};

class application_base {
public:
    application_base() {}
};

class application : public application_base {
public:
    application();
    void finalize_devices();
    int init_device_1();
    void reset_timebase_and_hid6();
    void reset_timebase_and_hid6_rev16_18();
    void reset_timebase_and_hid6_other();
    int init_device_0();
    virtual long run() = 0;
};

class boot_loader : public application {
public:
    boot_loader();
    long load_and_boot_lv1();
    void print_boot_banner();
    int component_init();
    int boot_cell_os(unsigned long lv1_addr, unsigned int *out_stop_code);
    inline virtual long run();
};

class platform_pwrmgr;
extern platform *g_platform_ptr;
extern application *g_application_ptr;
extern platform_pwrmgr *g_platform_pwrmgr_ptr;
extern char g_sys_ac_sd;
extern long g_sys_ac_misc;
extern unsigned long g_sys_ac_misc2;
extern unsigned long g_sys_ac_product_code;
extern unsigned int g_sys_hw_model_emulate;
#endif

long get_platform_id(void);
void install_ext_file_hooks(void);
void set_debug_interface(int iface);
EXTERN_C void flush_dcache_signal_and_halt(long size, int *flag);
void fill_boot_parm_header(long parm);
int boot_parm_nop(struct boot_parm *parm);
long setup_boot_parm(struct boot_parm *parm);

void finalize_debug_interface(char *iface);

int load_nv_entry(unsigned int idx);
void hang_forever(void) __attribute__((noreturn));

#endif
