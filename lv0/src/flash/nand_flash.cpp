#include "iommu.h"
#include "nand_flash.h"
#include "storage.h"
#include "memory.h"
#include "log.h"
#include "mmio.h"

extern const long nand_flash_dma_iopt_desc_template[6];

nand_flash::nand_flash()
{
    state = 0;
    dma_buf = 0;
    dma_io_addr = 0;
    dma_buf_sectors = 0;
    cached_size = 0;
    use_dma = false;
}

void nand_flash::configure_dma(long flash_addr, long iopt_index, bool use_dma, char *dma_buf, unsigned long dma_io_addr,
                               unsigned long dma_buf_size)
{
    iopt_codec codec;
    struct iopt_window window;

    lv0_memset(&window, 0, sizeof window);
    window.w[1] = 2;
    window.w[2] = 2;
    window.w[3] = 1;
    window.w[8] = 1;
    window.addr = (unsigned int)flash_addr;
    codec.pack_entry(sb_mmio_base, iopt_index, &window);
    this->dma_buf_sectors = dma_buf_size >> 9;
    this->dma_buf = dma_buf;
    this->dma_io_addr = dma_io_addr;
    this->use_dma = use_dma;
}

int nand_flash::open()
{
    return 0;
}

int nand_flash::close()
{
    return 0;
}

long nand_flash::get_size(unsigned long *out_size)
{
    unsigned long size;
    unsigned long dev_size = cached_size;

    if (dev_size != 0) {
        *out_size = dev_size;
    } else if (get_device_size(&size) != 0 || size <= 0x40000) {
        *out_size = dev_size;
    } else {
        dev_size = size - 0x40000;
        cached_size = dev_size;
        *out_size = dev_size;
    }
    return 0;
}

int nand_flash::read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read)
{
    unsigned long device_size;
    int rc;
    unsigned long pos;
    unsigned long tail_len, mid_len, tail_off, mid_off;
    unsigned long head_len;
    unsigned long dst_cur;

    rc = nand_flash::get_size(&device_size);
    if (rc) {
        log_message("[ERROR] %s(%d) get_size %d\n", __FUNCTION__, 82, rc);
        goto out;
    }
    if (device_size <= offset) {
        rc = -14;
        log_message("[WARN] %s(%d) device_size <= offset\n", __FUNCTION__, 87);
        goto out;
    }
    {
        unsigned long avail = device_size - offset;
        if (avail < size)
            size = avail;
    }

    pos = offset & 0x1FF;
    *bytes_read = 0;
    dst_cur = (unsigned long)buf;
    if (pos != 0) {
        head_len = size > 512 - pos ? 512 - pos : size;
        tail_len = (size - head_len) & 0x1FF;
        mid_len = size - head_len - tail_len;
        mid_off = offset + head_len;
        tail_off = mid_off + mid_len;
    } else {
        head_len = 0;
        tail_len = size & 0x1FF;
        mid_len = size - tail_len;
        mid_off = offset;
        tail_off = mid_off + mid_len;
    }

    if (head_len) {
        unsigned long lba = offset_to_lba(offset);
        if (!use_dma) {
            int err = nand_flash_ctrl::read_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_poll\n", __FUNCTION__, 133, err);
                goto out;
            }
            lv0_memmove(buf, (const char *)&sector_buf[pos], head_len);
        } else {
            int err = nand_flash_ctrl::read_sector_dma(dma_io_addr, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_dma\n", __FUNCTION__, 141, err);
                goto out;
            }
            lv0_memmove(buf, dma_buf + pos, head_len);
        }
        dst_cur = (unsigned long)buf + head_len;
        *bytes_read += head_len;
    }

    if (mid_len) {
        unsigned int n = mid_len >> 9;
        unsigned int lba = offset_to_lba(mid_off);
        if (!use_dma) {
            int err = nand_flash_ctrl::read_sector_poll((char *)dst_cur, lba, n);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_poll\n", __FUNCTION__, 160, err);
                goto out;
            }
            dst_cur += mid_len;
            *bytes_read += mid_len;
        } else {
            do {
                unsigned long nsecs = dma_buf_sectors < n ? dma_buf_sectors : n;
                unsigned int nbytes = nsecs << 9;
                int err = nand_flash_ctrl::read_sector_dma(dma_io_addr, lba, nsecs);
                if (err) {
                    rc = -99;
                    log_message("[ERROR] %s(%d) read_sector_dma\n", __FUNCTION__, 172, err);
                    goto out;
                }
                lv0_memmove((char *)dst_cur, dma_buf, nbytes);
                dst_cur += nbytes;
                n -= nsecs;
                *bytes_read += nbytes;
                lba += nsecs;
            } while (n);
        }
    }

    if (tail_len) {
        unsigned long lba = offset_to_lba(tail_off);
        if (!use_dma) {
            int err = nand_flash_ctrl::read_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_poll\n", __FUNCTION__, 197, err);
                goto out;
            }
            lv0_memmove((char *)dst_cur, (const char *)sector_buf, tail_len);
        } else {
            int err = nand_flash_ctrl::read_sector_dma(dma_io_addr, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_dma\n", __FUNCTION__, 205, err);
                goto out;
            }
            lv0_memmove((char *)dst_cur, dma_buf, tail_len);
        }
        *bytes_read += tail_len;
    }
out:
    return rc;
}

int nand_flash::write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written)
{
    unsigned long device_size;
    int rc, err;

    rc = nand_flash::get_size(&device_size);
    if (rc) {
        log_message("[ERROR] %s(%d) get_size %d\n", __FUNCTION__, 226, rc);
        goto out;
    }
    if (device_size <= offset) {
        rc = -14;
        log_message("[WARN] %s(%d) device_size <= offset\n", __FUNCTION__, 231);
        goto out;
    }
    if (device_size - offset < size)
        size = device_size - offset;

    {
        unsigned long head_off = offset & 0x1FF;
        *bytes_written = 0;
        unsigned long src_cur = (unsigned long)src;

        unsigned long head_len, tail_len, mid_off, mid_len, tail_off;
        if (head_off) {
            head_len = size > 512 - head_off ? 512 - head_off : size;
            tail_len = (size - head_len) & 0x1FF;
            mid_len = size - head_len - tail_len;
            mid_off = offset + head_len;
            tail_off = mid_off + mid_len;
        } else {
            head_len = 0;
            tail_len = size & 0x1FF;
            mid_len = size - tail_len;
            mid_off = offset;
            tail_off = mid_off + mid_len;
        }

        if (head_len) {
            unsigned int lba = offset_to_lba(offset);
            err = nand_flash_ctrl::read_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_poll\n", __FUNCTION__, 277, err);
                goto out;
            }
            lv0_memmove((char *)&sector_buf[head_off], src, head_len);
            *bytes_written += head_len;
            err = nand_flash_ctrl::erase_sector_poll(lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) erase_sector_poll\n", __FUNCTION__, 285, err);
                goto out;
            }
            src_cur = (unsigned long)(src + head_len);
            err = nand_flash_ctrl::write_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) write_sector_poll\n", __FUNCTION__, 291, err);
                goto out;
            }
        }

        if (mid_len) {
            unsigned int lba = offset_to_lba(mid_off);
            unsigned int n = mid_len >> 9;
            err = nand_flash_ctrl::erase_sector_poll(lba, n);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) erase_sector_poll\n", __FUNCTION__, 304, err);
                goto out;
            }
            err = nand_flash_ctrl::write_sector_poll((const char *)src_cur, lba, n);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) write_sector_poll\n", __FUNCTION__, 310, err);
                goto out;
            }
            src_cur += mid_len;
            *bytes_written += mid_len;
        }

        if (tail_len) {
            unsigned int lba = offset_to_lba(tail_off);
            err = nand_flash_ctrl::read_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) read_sector_poll\n", __FUNCTION__, 326, err);
                goto out;
            }
            lv0_memmove((char *)sector_buf, (const char *)src_cur, tail_len);
            *bytes_written += tail_len;
            err = nand_flash_ctrl::erase_sector_poll(lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) erase_sector_poll\n", __FUNCTION__, 334, err);
                goto out;
            }
            err = nand_flash_ctrl::write_sector_poll((char *)sector_buf, lba, 1);
            if (err) {
                rc = -99;
                log_message("[ERROR] %s(%d) write_sector_poll\n", __FUNCTION__, 340, err);
                goto out;
            }
        }
    }
out:
    return rc;
}

long nand_flash::get_block_size(unsigned long *block_size)
{
    *block_size = 0x20000;
    return 0;
}
