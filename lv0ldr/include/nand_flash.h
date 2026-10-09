#ifndef LDR_NAND_FLASH_H
#define LDR_NAND_FLASH_H

#include "mmio.h"

typedef bool (*nand_flash_ready_fn)(void);

class nand_flash_ctrl {
public:
    nand_flash_ctrl() : base(0) {}
    int set_base(u64 base);
    unsigned short get_status();
    unsigned short get_intr();
    long idle();
    long start_read_sector(unsigned int sector, unsigned short count, unsigned short cmd);
    long start_idle();
    long read_sector(unsigned short *buf, int unused);

    static long read_sectors_poll(char *buf, u32 sector, u32 count, nand_flash_ready_fn ready);
    static long read_sectors_dma(unsigned long io_addr, u32 sector, u32 count, nand_flash_ready_fn ready);
    static long read_sector_dma(unsigned long io_addr, unsigned int sector, unsigned int count);

    u64 base;
    unsigned int reserved;
};

nand_flash_ctrl *get_nand_flash_ctrl(void);
int set_nand_flash_ready_callback(nand_flash_ready_fn callback);

#endif
