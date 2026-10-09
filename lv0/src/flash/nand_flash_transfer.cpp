#include "nand_flash.h"
#include "sb.h"
#include "log.h"

nand_flash_ready_fn g_nand_flash_ready_callback;

inline int kick_dma(unsigned long io_addr, u32 size)
{
    if ((mmio_sb_product_code & 0x7F000000) == 0x01000000)
        return get_systemio_dmac_dx()->kick_dma(io_addr, size);
    return get_systemio_dmac_px()->kick_dma(io_addr, size);
}

int set_nand_flash_ready_callback(nand_flash_ready_fn callback)
{
    g_nand_flash_ready_callback = callback;
    return 0;
}

long nand_flash_ctrl::read_sectors_poll(char *buf, u32 sector, u32 count, nand_flash_wait_fn ready)
{
    nand_flash_ctrl *dev;
    int rc;
    int ret = -3;
    int i;
    if (g_nand_flash_ready_callback == 0)
        goto out;
    dev = get_nand_flash_ctrl();
    ret = 0;
    rc = dev->start_read_sector(sector, count, 9);
    if (__builtin_expect(rc != 0, 0)) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_read_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 39, rc, dev->get_status());
        goto out;
    }
    for (i = 0; (u32)i < count; i++) {
        while (!ready())
            ;
        rc = dev->read_sector((unsigned short *)(buf + ((u32)i << 9)), 0);
        if (rc != 0) {
            ret = -1;
            syscon_printf("[ERROR] 0x%08x %s(%d) read_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 48, rc, dev->get_status());
            goto out;
        }
    }
    while (!ready())
        ;
    rc = dev->start_idle();
    if (rc != 0) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 58, rc, dev->get_status());
        goto out;
    }
    while (!ready())
        ;
    ret = dev->idle();
    if (ret != 0)
        syscon_printf("[ERROR] 0x%08x %s(%d) idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 65, ret, dev->get_status());
out:
    return ret;
}

long nand_flash_ctrl::read_sector_poll(char *buf, unsigned int sector, unsigned int count)
{
    return read_sectors_poll(buf, sector, count, (nand_flash_wait_fn)g_nand_flash_ready_callback);
}

unsigned char is_nand_flash_ready(void)
{
    return g_nand_flash_ready_callback();
}

int nand_flash_ctrl::write_sectors_poll_16(const char *buf, u32 sector, unsigned short count16, nand_flash_wait_fn ready)
{
    nand_flash_ctrl *dev = get_nand_flash_ctrl();
    int rc = dev->start_write_sector(sector, count16);
    if (rc != 0) {
        int status = dev->get_status();
        syscon_printf("[ERROR] 0x%08x %s(%d) start_write_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 277, rc, status);
        return -1;
    }
    unsigned int count = count16 ? count16 : 0x10000;
    unsigned int i;
    for (i = 0; i < count; i++) {
        while (ready() == 0)
            ;
        rc = dev->write_sector((const unsigned short *)(buf + (i << 9)), 0);
        if (rc == 1)
            return 0;
        if (rc) {
            int status = dev->get_status();
            syscon_printf("[ERROR] 0x%08x %s(%d) write_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 293, rc, status);
            return -1;
        }
    }
    while (ready() == 0)
        ;
    rc = dev->write_sector((const unsigned short *)buf, 0);
    if (rc == 1)
        return 0;
    {
        int status = dev->get_status();
        syscon_printf("[ERROR] 0x%08x %s(%d) write_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 303, rc, status);
    }
    return -1;
}

int nand_flash_ctrl::erase_sectors_poll_16(u32 sector, unsigned short count, nand_flash_wait_fn ready)
{
    nand_flash_ctrl *dev = get_nand_flash_ctrl();
    long start_rc = dev->start_erase_sector(sector, count);
    if (start_rc) {
        int st = dev->get_status();
        syscon_printf("[ERROR] 0x%08x %s(%d) start_erase_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 319, start_rc, st);
        return -1;
    }
    while (ready() == 0)
        ;
    long rc = dev->erase_sector();
    if (rc == 0)
        return 0;
    int st = dev->get_status();
    syscon_printf("[ERROR] 0x%08x %s(%d) erase_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 327, rc, st);
    return -1;
}

int nand_flash_ctrl::write_sectors_poll(const char *buf, u32 sector, u32 count, nand_flash_wait_fn ready)
{
    if (g_nand_flash_ready_callback == 0)
        return -3;
    u32 rem = count;
    do {
        u32 n = 0x10000;
        unsigned short n16 = 0;
        if (rem <= 0xFFFF) {
            n = rem;
            n16 = rem;
        }
        if (write_sectors_poll_16(buf, sector, n16, ready) != 0)
            return -1;
        sector += n;
        rem -= n;
        buf += n << 9;
    } while (rem != 0);
    return 0;
}

int nand_flash_ctrl::write_sector_poll(const char *buf, unsigned int sector, unsigned int count)
{
    return write_sectors_poll(buf, sector, count, (nand_flash_wait_fn)g_nand_flash_ready_callback);
}

int nand_flash_ctrl::erase_sectors_poll(u32 sector, u32 count, nand_flash_wait_fn ready)
{
    if (g_nand_flash_ready_callback == 0)
        return -3;
    u32 rem = count;
    do {
        u32 n = 0x10000;
        unsigned short n16 = 0;
        if (rem <= 0xFFFF) {
            n = rem;
            n16 = rem;
        }
        if (erase_sectors_poll_16(sector, n16, ready) != 0)
            return -1;
        sector += n;
        rem -= n;
    } while (rem != 0);
    return 0;
}

int nand_flash_ctrl::erase_sector_poll(unsigned int sector, unsigned int count)
{
    return erase_sectors_poll(sector, count, (nand_flash_wait_fn)g_nand_flash_ready_callback);
}

inline int get_dma_status(void)
{
    if ((mmio_sb_product_code & 0x7F000000) == 0x1000000)
        return get_systemio_dmac_dx()->get_dma_status();
    return get_systemio_dmac_px()->get_dma_status();
}

long nand_flash_ctrl::read_sectors_dma(unsigned long io_addr, u32 sector, u32 count, nand_flash_wait_fn ready)
{
    nand_flash_ctrl *dev;
    int rc;
    int intr;
    int ret = -3;
    u32 rem, off, n;
    int dma_status;
    if (g_nand_flash_ready_callback == 0)
        goto out;
    dev = get_nand_flash_ctrl();
    rc = dev->start_read_sector(sector, count, 0);
    rem = count << 9;
    off = 0;
    if (__builtin_expect(rc != 0, 0)) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_read_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 88, rc, dev->get_status());
        goto out;
    }
    do {
        n = rem;
        if (rem > 0x4000)
            n = 0x4000;
        rc = kick_dma(io_addr + off, n);
        if (rc != 0) {
            ret = -2;
            syscon_printf("[ERROR] 0x%08x %s(%d) kick_dma %d\n", LV0_ERR_DMA, __FUNCTION__, 101, rc);
            goto out;
        }
        for (;;) {
            dma_status = get_dma_status();
            if (dma_status == 1)
                break;
            if (dma_status == -1) {
                ret = -2;
                syscon_printf("[ERROR] 0x%08x %s(%d) get_dma_status %08x\n", LV0_ERR_DMA, __FUNCTION__, 111, -1);
                goto out;
            }
        }
        intr = dev->get_intr();
        if (intr & 8) {
            ret = -1;
            syscon_printf("[ERROR] 0x%08x %s(%d) interrupt %08x\n", LV0_ERR_FLASH, __FUNCTION__, 119, intr);
            goto out;
        }
        rem -= n;
        off += n;
    } while (rem != 0);
    rc = dev->start_idle();
    if (rc != 0) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 168, rc, dev->get_status());
        goto out;
    }
    while (!ready())
        ;
    ret = dev->idle();
    if (ret != 0)
        syscon_printf("[ERROR] 0x%08x %s(%d) idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 175, ret, dev->get_status());
out:
    return ret;
}

long nand_flash_ctrl::read_sector_dma(unsigned long io_addr, unsigned int sector, unsigned int count)
{
    return read_sectors_dma(io_addr, sector, count, (nand_flash_wait_fn)g_nand_flash_ready_callback);
}

int get_device_size(unsigned long *out)
{
    unsigned short info[256];
    nand_flash_ctrl *ctrl = get_nand_flash_ctrl();
    int rc;

    if (is_nand_flash_boot()) {
        long base = ctrl->base;
        long sectors = *(volatile unsigned short *)(base + 0x44040);
        sectors += (long)(*(volatile unsigned short *)(base + 0x44042) << 16);
        *out = sectors << 9;
        return 0;
    }
    ctrl->start_identify_device();
    do {
        while (!is_nand_flash_ready())
            ;
        rc = ctrl->identify_device(info);
        if (rc == 1)
            goto identified;
    } while (rc == 0);
    syscon_printf("[ERROR] %s(%d) identify_device %d\n", __FUNCTION__, 352, rc);
    return -1;

identified:
    if ((info[0] & 0xFF00) == 0xFF00) {
        syscon_printf("[ERROR] %s(%d) identify_info[0] %04x\n", __FUNCTION__, 359, info[0]);
        return -1;
    }
    *out = (((unsigned long)info[3] << 16) | info[2]) << 9;
    return 0;
}
