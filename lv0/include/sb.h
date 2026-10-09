#ifndef LV0_SB_H
#define LV0_SB_H

#include "lv0.h"

#ifdef __cplusplus
class sb_device {
public:
    sb_device() : base(0) {}
    int write8_100000a(unsigned int idx, long val);
    int read8_1000009(unsigned int idx, unsigned char *out);
    int write8_100003a(long b);
    int read8_1000039();
    void initialize(long sb_base, int init_hw);

    long base;
};
#endif

struct sb_device *get_sb_device(void);
extern const long sb_device_iopt_desc_template[6];

#define mmio_sb_product_code (*(volatile unsigned int *)0x24000087000UL)

#ifdef __cplusplus
class systemio_dmac_dx {
public:
    systemio_dmac_dx();
    int configure(unsigned long base, unsigned long desc, unsigned long desc_io, unsigned long ioid,
                  unsigned long dev_addr);
    int get_dma_status();
    int start_transfer(unsigned long dst, unsigned long src, unsigned int len);
    int kick_dma(unsigned long buf, unsigned int len);

    unsigned long base;
    unsigned long ioid;
    unsigned long dev_addr;
    unsigned long desc;
    unsigned long desc_io;
};
#endif

struct systemio_dmac_dx *get_systemio_dmac_dx(void);

#ifdef __cplusplus
class systemio_dmac_px {
public:
    systemio_dmac_px() : base(0), dev_addr(0), desc(0), desc_io(0) {}
    int configure(unsigned long base, unsigned long dev_addr, unsigned long desc, unsigned int desc_io);
    bool is_ebus_interrupt_asserted();
    int kick_dma(unsigned long dst, unsigned int len);
    int get_dma_status();

    unsigned long base;
    unsigned long dev_addr;
    unsigned long desc;
    unsigned int desc_io;
};
#endif

struct systemio_dmac_px *get_systemio_dmac_px(void);
int poll_ebus_interrupt(void);

#ifdef __cplusplus
class pio {
public:
    pio() : sb_base(0) {}
    int write_fff500(unsigned short value);
    int read_fff500(unsigned short *out);
    unsigned char reverse_bits8(unsigned char value);
    int read_output_byte();
    void configure(char *base, unsigned int cfg_508, unsigned int cfg_530);
    int read_fff504(unsigned short *out);
    bool is_ss2_interrupt_asserted();
    bool is_emmcbridge_interrupt_asserted();
    int write_output_byte(unsigned char value);

    char *sb_base;
};
#endif

struct pio *get_pio(void);
long poll_ss2_interrupt(void);
long poll_emmcbridge_interrupt(void);

#ifdef __cplusplus
class uart {
public:
    uart() : base(0) {}
    int putc(int ch);
    long puts(const char *str);
    uart *init(volatile unsigned int *regs, int baud_div, unsigned int mode, int a5, int a6, int a7);

    long base;
};
#endif

struct uart *get_uart(void);

unsigned long get_xio_channel_mask(void);
long get_xio_channel_size_mb(int channel);
void sb_dx_write32_1f60(long base, long val);
void sb_dx_write32_1c74(long base, long val);
void sb_dx_write32_1c70(long base, long val);
int kick_dma(unsigned long io_addr, unsigned int size);
int get_dma_status(void);
unsigned int get_sb_revision(void);

#endif
