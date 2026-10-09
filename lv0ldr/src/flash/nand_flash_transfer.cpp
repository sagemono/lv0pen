#include "nand_flash.h"
#include "systemio_dmac.h"
#include "errors.h"
#include "util.h"

void syscon_printf(const char *fmt, ...);

nand_flash_ready_fn g_nand_flash_ready_callback;

inline long kick_dma(u64 io_addr, u32 size)
{
    if (is_sb_product_dx())
        return get_systemio_dmac_dx()->kick_dma(io_addr, size);
    return get_systemio_dmac_px()->kick_dma(io_addr, size);
}

inline int get_dma_status(void)
{
    if (is_sb_product_dx())
        return get_systemio_dmac_dx()->get_dma_status();
    return get_systemio_dmac_px()->get_dma_status();
}

int set_nand_flash_ready_callback(nand_flash_ready_fn callback)
{
    g_nand_flash_ready_callback = callback;
    return 0;
}

CXX_DROPPED long nand_flash_ctrl::read_sectors_poll(char *buf, u32 sector, u32 count, nand_flash_ready_fn ready)
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

long nand_flash_ctrl::read_sectors_dma(unsigned long io_addr, u32 sector, u32 count, nand_flash_ready_fn ready)
{
    nand_flash_ctrl *dev;
    long rc, intr;
    long ret = -3;
    u32 rem, off, n;
    int dma_status;
    if (g_nand_flash_ready_callback == 0)
        goto out;
    dev = get_nand_flash_ctrl();
    rc = dev->start_read_sector(sector, count, 0);
    rem = count << 9;
    off = 0;
    if (rc != 0) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_read_sector %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 87, rc, dev->get_status());
        goto out;
    }
    do {
        n = rem > 0x100000 ? 0x100000 : rem;
        rc = kick_dma(io_addr + off, n);
        if (rc != 0) {
            ret = -2;
            syscon_printf("[ERROR] 0x%08x %s(%d) kick_dma %d\n", LV0_ERR_DMA, __FUNCTION__, 100, rc);
            goto out;
        }
        for (;;) {
            dma_status = get_dma_status();
            if (dma_status == 1)
                break;
            if (dma_status == -1) {
                ret = -2;
                syscon_printf("[ERROR] 0x%08x %s(%d) get_dma_status %08x\n", LV0_ERR_DMA, __FUNCTION__, 110, -1);
                goto out;
            }
        }
        intr = dev->get_intr();
        if (intr & 8) {
            ret = -1;
            syscon_printf("[ERROR] 0x%08x %s(%d) interrupt %08x\n", LV0_ERR_FLASH, __FUNCTION__, 118, intr);
            goto out;
        }
        rem -= n;
        off += n;
    } while (rem != 0);
    rc = dev->start_idle();
    if (rc != 0) {
        ret = -1;
        syscon_printf("[ERROR] 0x%08x %s(%d) start_idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 167, rc, dev->get_status());
        goto out;
    }
    while (!ready())
        ;
    ret = dev->idle();
    if (ret != 0)
        syscon_printf("[ERROR] 0x%08x %s(%d) idle %d, stataus %08x\n", LV0_ERR_FLASH, __FUNCTION__, 174, ret, dev->get_status());
out:
    return ret;
}

long nand_flash_ctrl::read_sector_dma(unsigned long io_addr, unsigned int sector, unsigned int count)
{
    return read_sectors_dma(io_addr, sector, count, g_nand_flash_ready_callback);
}
