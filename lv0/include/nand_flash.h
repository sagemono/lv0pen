#ifndef LV0_NAND_FLASH_H
#define LV0_NAND_FLASH_H

#include "lv0.h"
#include "cxx.h"

typedef long (*nand_flash_ready_fn)(void);
typedef unsigned char (*nand_flash_wait_fn)(void);

#ifdef __cplusplus
class nand_flash_ctrl {
public:
    nand_flash_ctrl() : base(0) {}
    int set_base(long base);
    int start_read_sector(unsigned int sector, unsigned short count, unsigned short cmd);
    int start_idle();
    int start_write_sector(unsigned int sector, unsigned short count);
    int start_erase_sector(unsigned int sector, unsigned short count);
    int start_identify_device();
    int read_sector(unsigned short *buf, int unused);
    int idle();
    unsigned short get_status();
    int get_reg_44016();
    unsigned short get_intr();
    int write_sector(const unsigned short *buf, int unused);
    int erase_sector();
    int identify_device(unsigned short *buf);
    void low_level_format();
    void erase_flash();
    void read_page();
    void read_block_condition();

    static long read_sectors_poll(char *buf, u32 sector, u32 count, nand_flash_wait_fn ready);
    static long read_sector_poll(char *buf, unsigned int sector, unsigned int count);
    static int write_sectors_poll_16(const char *buf, u32 sector, unsigned short count16, nand_flash_wait_fn ready);
    static int erase_sectors_poll_16(u32 sector, unsigned short count, nand_flash_wait_fn ready);
    static int write_sectors_poll(const char *buf, u32 sector, u32 count, nand_flash_wait_fn ready);
    static int write_sector_poll(const char *buf, unsigned int sector, unsigned int count);
    static int erase_sectors_poll(u32 sector, u32 count, nand_flash_wait_fn ready);
    static int erase_sector_poll(unsigned int sector, unsigned int count);
    static long read_sectors_dma(unsigned long io_addr, u32 sector, u32 count, nand_flash_wait_fn ready);
    static long read_sector_dma(unsigned long io_addr, unsigned int sector, unsigned int count);

    long base;
};
#endif

struct nand_flash_ctrl *get_nand_flash_ctrl(void);
unsigned char is_nand_flash_boot(void);
int set_nand_flash_ready_callback(nand_flash_ready_fn callback);
unsigned char is_nand_flash_ready(void);
int get_device_size(unsigned long *out);

#ifdef __cplusplus
#include "storage.h"

class nand_flash : public storage_device {
public:
    nand_flash();
    void configure_dma(long flash_addr, long iopt_index, bool use_dma, char *dma_buf, unsigned long dma_io_addr,
                       unsigned long dma_buf_size);
    int open();
    int close();
    unsigned long offset_to_lba(unsigned long offset) { return (offset >> 9) + 0x200; }
    long get_size(unsigned long *out_size);
    int write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written);
    int read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read);
    long get_block_size(unsigned long *block_size);

    unsigned char sector_buf[512];
    char *dma_buf;
    unsigned long dma_io_addr;
    unsigned long dma_buf_sectors;
    unsigned long cached_size;
    bool use_dma;
};

class nand_flash_abs : public nand_flash {
public:
    nand_flash_abs();
    int read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read);
    int write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written);
    long get_size(unsigned long *out_size);
};
#endif

int identify_nor_flash(volatile unsigned short *flash);
int probe_nand_flash(long dev);
int detect_flash_type(long base);

#endif
