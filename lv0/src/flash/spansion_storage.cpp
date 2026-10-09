#include "spansion_storage.h"
#include "iommu.h"
#include "memory.h"
#include "platform.h"
#include "clock.h"
#include "log.h"
#include "lv0.h"
#include "mmio.h"

struct nor_chip g_nor_chips[9] = {
    { 0x00000, 0x800000, 1, 4, 3, 0,
      { { 0x0, 0x2000, 8 }, { 0x10000, 0x10000, 63 }, { 0x400000, 0x10000, 63 }, { 0x7F0000, 0x2000, 8 } } },
    { 0x10000, 0x1000000, 1, 1, 4, 0, { { 0x0, 0x10000, 256 } } },
    { 0x20000, 0x1000000, 1, 1, 4, 16, { { 0x0, 0x20000, 128 } } },
    { 0x30000, 0x1000000, 1, 1, 5, 0, { { 0x0, 0x10000, 256 } } },
    { 0x40000, 0x800000, 1, 3, 5, 0,
      { { 0x0, 0x10000, 63 }, { 0x3F0000, 0x2000, 16 }, { 0x410000, 0x10000, 63 } } },
    { 0x50000, 0x800000, 2, 5, 3, 0,
      { { 0x0, 0x2000, 8 }, { 0x10000, 0x10000, 126 }, { 0x7F0000, 0x2000, 16 }, { 0x810000, 0x10000, 126 },
        { 0xFF0000, 0x2000, 8 } } },
    { 0x60000, 0x800000, 2, 5, 5, 0,
      { { 0x0, 0x10000, 63 }, { 0x3F0000, 0x2000, 16 }, { 0x410000, 0x10000, 126 }, { 0xBF0000, 0x2000, 16 },
        { 0xC10000, 0x10000, 63 } } },
    { 0x70000, 0x800000, 2, 5, 2, 0,
      { { 0x0, 0x2000, 8 }, { 0x10000, 0x10000, 126 }, { 0x7F0000, 0x2000, 16 }, { 0x810000, 0x10000, 126 },
        { 0xFF0000, 0x2000, 8 } } },
    { 0x80000, 0x1000000, 1, 1, 4, 32, { { 0x0, 0x20000, 128 } } },
};

#define NOR_EIEIO() __asm__ volatile ("eieio" ::: "memory")

#define NOR_DQ1 0x02
#define NOR_DQ5 0x20

#define CHIP_BASE(addr) \
    (((addr) - base) / g_nor_chips[type].chip_size * g_nor_chips[type].chip_size + base)

#define NOR_WRITE(addr, value) \
    do { *(volatile unsigned short *)(addr) = (value); NOR_EIEIO(); } while (0)

int spansion_storage::open()
{
    return type == -1 ? -99 : 0;
}

unsigned long spansion_storage::sector_start(unsigned long offset)
{
    unsigned long off = translate(offset) - base;
    const struct nor_region *r = g_nor_chips[type].regions;

    for (unsigned int i = 0; i < g_nor_chips[type].region_count; i++, r++) {
        unsigned long start = r->start;
        unsigned long size = r->sector_size;
        unsigned long count = r->sector_count;
        if (start <= off && off < start + size * count)
            return base + start + (off - start) / size * size;
    }
    return 0;
}

unsigned long spansion_storage::sector_size(unsigned long addr)
{
    unsigned long off = addr - base;
    const struct nor_region *r = g_nor_chips[type].regions;

    for (unsigned int i = 0; i < g_nor_chips[type].region_count; i++, r++) {
        unsigned long start = r->start;
        unsigned long size = r->sector_size;
        unsigned long count = r->sector_count;
        if (start <= off && off < start + size * count)
            return size;
    }
    return 0;
}

int spansion_storage::unlock_bypass_enter(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0x2020);
    return 0;
}

int spansion_storage::unlock_bypass_exit(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip, 0x9090);
    NOR_WRITE(chip, 0);
    return 0;
}

long spansion_storage::get_size(unsigned long *out_size)
{
    int rc = -99;

    if (type != -1) {
        rc = 0;
        *out_size = (unsigned int)(g_nor_chips[type].chip_count * g_nor_chips[type].chip_size);
    }
    return rc;
}

long spansion_storage::get_block_size(unsigned long *block_size)
{
    *block_size = 0x10000;
    return 0;
}

int spansion_storage::swap16(unsigned int value)
{
    if (swap)
        value = (unsigned short)((value >> 8) | (value << 8));
    return value;
}

int spansion_storage::identify()
{
    volatile unsigned short *flash = (volatile unsigned short *)base;
    unsigned short id, dev_type, size;

    *flash = 0xF0F0;
    NOR_EIEIO();
    flash[0x55] = 0x9898;
    NOR_EIEIO();

    {
        unsigned short s16, s17, s18;
        s16 = flash[16];
        NOR_EIEIO();
        s17 = flash[17];
        NOR_EIEIO();
        s18 = flash[18];
        NOR_EIEIO();
        if ((unsigned char)(s16 >> 8) == 'Q' && (unsigned char)(s17 >> 8) == 'R' && (unsigned char)(s18 >> 8) == 'Y')
            swap = 1;
        else if ((unsigned char)s16 == 'Q' && (unsigned char)s17 == 'R' && (unsigned char)s18 == 'Y')
            swap = 0;
        else
            return -1;
    }

    *flash = 0xF0F0;
    NOR_EIEIO();
    flash[0x555] = 0xAAAA;
    NOR_EIEIO();
    flash[0x2AA] = 0x5555;
    NOR_EIEIO();
    flash[0x555] = 0x9090;
    NOR_EIEIO();
    (void)*flash;
    *flash = 0xF0F0;
    NOR_EIEIO();
    flash[0x555] = 0xAAAA;
    NOR_EIEIO();
    flash[0x2AA] = 0x5555;
    NOR_EIEIO();
    flash[0x555] = 0x9090;
    NOR_EIEIO();
    id = flash[1];
    dev_type = flash[14];
    size = flash[15];
    *flash = 0xF0F0;
    NOR_EIEIO();

    id = swap16(id);
    dev_type = swap16(dev_type);
    size = swap16(size);

    if ((unsigned char)id == 0x7E && (unsigned char)dev_type == 0x02 && (unsigned char)size == 0x01)
        return 0;
    if ((unsigned char)id == 0x7E && (unsigned char)dev_type == 0x12 && (unsigned char)size == 0x00)
        return 1;
    if ((unsigned char)id == 0x7E && (unsigned char)dev_type == 0x21 && (unsigned char)size == 0x01)
        return 2;
    if ((unsigned char)id == 0x7E && (unsigned char)dev_type == 0x12 && (unsigned char)size == 0x01)
        return 3;
    if (id == 0x257E && dev_type == 0x2506 && size == 0x2501)
        return 7;
    if (id == 0x227E && dev_type == 0x2266 && size == 0x2260)
        return 8;
    return -1;
}

int spansion_storage::for_each_sector(unsigned long offset, unsigned long size,
                                      int (spansion_storage::*op)(unsigned long))
{
    unsigned long left = size;

    while (left) {
        unsigned long sector = sector_start(offset);
        unsigned long n = sector_size(sector);
        offset += n;
        left -= n;
        int rc = (this->*op)(sector);
        if (rc)
            return rc;
    }
    return 0;
}

int spansion_storage::protect_range(unsigned long addr, unsigned long size)
{
    long off = addr - base;
    unsigned long dev_size;
    int rc;

    if (off < 0)
        goto range;
    rc = get_size(&dev_size);
    if (rc)
        return rc;
    if (dev_size <= (unsigned long)off || dev_size - off < size)
        goto range;
    switch (type) {
    case 2:
        return for_each_sector(off, size, &spansion_storage::dyb_protect);
    case 7:
        return for_each_sector(off, size, &spansion_storage::protect_type7);
    case 8:
        return for_each_sector(off, size, &spansion_storage::protect_type8);
    default:
        return -16;
    }
range:
    return -14;
}

int spansion_storage::dyb_enter(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0xE0E0);
    return 0;
}

int spansion_storage::cmdset_exit(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip, 0x9090);
    NOR_WRITE(chip, 0);
    return 0;
}

bool spansion_storage::dyb_is_protected(unsigned long addr)
{
    dyb_enter(addr);
    unsigned short v = *(volatile unsigned short *)addr;
    cmdset_exit(addr);
    return ~v & 1;
}

int spansion_storage::dyb_protect(unsigned long addr)
{
    dyb_enter(addr);
    NOR_WRITE(addr, 0xA0A0);
    NOR_WRITE(addr, 0);
    cmdset_exit(addr);
    return 0;
}

int spansion_storage::dyb_unprotect(unsigned long addr)
{
    dyb_enter(addr);
    NOR_WRITE(addr, 0xA0A0);
    NOR_WRITE(addr, 0x0101);
    cmdset_exit(addr);
    return 0;
}

unsigned char spansion_storage::is_protected_type8(unsigned long addr)
{
    return dyb_is_protected(addr);
}

int spansion_storage::protect_type8(unsigned long addr)
{
    return dyb_protect(addr);
}

int spansion_storage::unprotect_type8(unsigned long addr)
{
    return dyb_unprotect(addr);
}

unsigned long spansion_storage::sector_group(unsigned long addr)
{
    unsigned long base = this->base;
    unsigned long mb = (sector_start(addr - base) - base) >> 20;
    unsigned long group;

    if (mb == 0)
        group = 0;
    else if (mb <= 3)
        group = 0x100000;
    else if (mb <= 6)
        group = 0x400000;
    else if (mb == 7)
        group = 0x700000;
    else if (mb == 8)
        group = 0x800000;
    else if (mb <= 11)
        group = 0x900000;
    else if (mb <= 14)
        group = 0xC00000;
    else
        group = 0xF00000;
    return base + group;
}

bool spansion_storage::is_protected_type7(unsigned long addr)
{
    unsigned long base = this->base;
    unsigned long chip_off = (addr - base) / g_nor_chips[type].chip_size * g_nor_chips[type].chip_size;
    unsigned long chip = chip_off + base;
    unsigned long group = sector_group(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(group + 0xAAA, 0x5858);
    unsigned short v = *(volatile unsigned short *)addr;
    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0x9090);
    NOR_WRITE(base + chip_off, 0);
    return v & 1;
}

void spansion_storage::progress_tick()
{
    static unsigned long count;

    count++;
    if (count == 1) {
        g_platform_ptr->set_post_code(0xC0, 0x21);
    } else if (count == 2) {
        g_platform_ptr->set_post_code(0xC0, 0x22);
    } else if (count == 3) {
        g_platform_ptr->set_post_code(0xC0, 0x24);
    } else if (count == 4) {
        g_platform_ptr->set_post_code(0xC0, 0x28);
        count = 0;
    }
}

void spansion_storage::halt_blink()
{
    for (;;) {
        g_platform_ptr->set_post_code(0xC0, 0x20);
        unsigned long deadline = get_time_ms() + 500;
        while (get_time_ms() < deadline)
            ;
        g_platform_ptr->set_post_code(0xC0, 0x23);
        deadline = get_time_ms() + 500;
        while (get_time_ms() < deadline)
            ;
    }
}

int spansion_storage::program_write_buffer(unsigned long addr, const unsigned short *src, unsigned int count)
{
    unsigned long base = this->base;
    unsigned long off = addr - base;
    unsigned int chip_size = g_nor_chips[type].chip_size;
    unsigned long sector = sector_start(off);
    unsigned short *dst = (unsigned short *)addr;
    unsigned int i;

    if (count == 0 || count > g_nor_chips[type].write_buffer_words)
        return -14;

    unsigned long chip = off / chip_size * chip_size + base;
    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(addr, 0x2525);
    NOR_WRITE(sector, swap16((unsigned short)(count - 1)));
    for (i = 0; i < count; i++)
        NOR_WRITE(&dst[i], src[i]);
    NOR_WRITE(sector, 0x2929);

    bool timeout = false, abort = false;
    volatile unsigned short *last = &dst[count - 1];
    const unsigned short *last_src = &src[count - 1];

    for (;;) {
        int v = *last;
        int want = *last_src;
        if (v == want)
            break;
        if (timeout) {
            log_message("%s: NOR is corrupted, data %04x, expect %04x, addr %llx, data_num %d\n", "program_write_buffer",
                        v, want, addr, count);
            halt_blink();
            return -20;
        }
        if (abort) {
            log_message("%s: write to buffer was aborted.. data 0x%04x, expect 0x%04x\n", "program_write_buffer", v, want);
            halt_blink();
            return -20;
        }
        if (swap16(NOR_DQ5) & v)
            timeout = true;
        else if (swap16(NOR_DQ1) & v)
            abort = true;
    }

    for (i = 0; i < count; i++) {
        int d = dst[i];
        if (d != src[i]) {
            log_message("data verify failed...\n");
            log_message(" data: 0x%x\n", src[i]);
            log_message("rom address: 0x%x", &dst[i]);
            log_message(" data: 0x%x\n", d);
            halt_blink();
        }
    }
    return 0;
}

DEAD_STRING(program_half_word_name, "program_half_word");

int spansion_storage::unlock_bypass_program(unsigned long addr, unsigned short value)
{
    volatile unsigned short *p = (volatile unsigned short *)addr;
    bool timeout = false;

    NOR_WRITE(CHIP_BASE(addr), 0xA0A0);
    NOR_WRITE(addr, value);
    for (;;) {
        unsigned short v = *p;
        if (v == value)
            return 0;
        if (timeout) {
            log_message("%s: NOR is corrupted\n", "unlock_bypass_program");
            log_message("data verify failed...\n");
            log_message(" data: 0x%x\n", value);
            log_message("rom address: 0x%x", addr);
            log_message(" data: 0x%x\n", *p);
            halt_blink();
            return -20;
        }
        timeout = (swap16(NOR_DQ5) & v) != 0;
    }
}

int spansion_storage::program(unsigned long addr, const char *src, unsigned long size)
{
    unsigned long p = addr;
    unsigned long done;
    int ret;

    if (g_nor_chips[type].write_buffer_words == 0)
        unlock_bypass_enter(addr);
    for (done = 0; done < size;) {
        unsigned long words = g_nor_chips[type].write_buffer_words;
        unsigned long step = 2;
        int rc;
        if (words) {
            step = words * 2;
            rc = program_write_buffer(p, (const unsigned short *)src, words);
        } else {
            rc = unlock_bypass_program(p, *(const unsigned short *)src);
        }
        done += step;
        src += step;
        p += step;
        if (rc) {
            log_message("ERROR: program\n");
            ret = -99;
            goto out;
        }
    }
    ret = 0;
out:
    if (g_nor_chips[type].write_buffer_words == 0)
        unlock_bypass_exit(addr);
    return ret;
}

int spansion_storage::erase_sector(unsigned long addr)
{
    volatile unsigned short *p = (volatile unsigned short *)addr;
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0x8080);
    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(addr, 0x3030);
    while (*p != 0xFFFF && !(*p & (unsigned short)swap16(NOR_DQ5)))
        ;
    if (*p != 0xFFFF) {
        halt_blink();
        log_message("\nHalt system due to fatal error (2)\n");
        return -99;
    }
    return 0;
}

int spansion_storage::write_sector(unsigned long addr, unsigned long at, const char *src, unsigned long len)
{
    int rc;

    switch (type) {
    case 2:
        if (dyb_is_protected(addr))
            goto protect;
        break;
    case 7:
        if (is_protected_type7(addr))
            goto protect;
        break;
    case 8:
        if (is_protected_type8(addr))
            goto protect;
        break;
    }
    unsigned long n = sector_size(addr);
    if (len != n)
        lv0_memmove(sector_work, (const char *)addr, n);
    lv0_memmove(sector_work + at, src, len);
    rc = erase_sector(addr);
    if (rc)
        return rc;
    progress_tick();
    rc = program(addr, sector_work, n);
    progress_tick();
    return rc;
protect:
    return -19;
}

int spansion_storage::write(unsigned long offset, unsigned long size, const char *src, unsigned long *bytes_written)
{
    unsigned long dev_size;
    int rc = get_size(&dev_size);

    if (rc == 0) {
        if (dev_size <= offset) {
            rc = -14;
        } else {
            unsigned long left = size;
            if (left > dev_size - offset)
                left = dev_size - offset;
            const char *p = src;
            unsigned long total = 0;
            while (left) {
                unsigned long n = left;
                unsigned long sector = sector_start(offset);
                unsigned long ssize = sector_size(sector);
                unsigned long at = translate(offset) - sector;
                if (at + left > ssize)
                    n = ssize - at;
                int err = write_sector(sector, at, p, n);
                offset += n;
                left -= n;
                p += n;
                total += n;
                if (err) {
                    rc = err;
                    goto out;
                }
            }
            *bytes_written = total;
        }
    }
out:
    return rc;
}

spansion_storage::spansion_storage()
{
    type = -1;
    swap = 0;
}

int spansion_storage::init(unsigned long base, unsigned long iopt_index, int type, unsigned int iopt_w0)
{
    mapped_storage::set_base(base);
    sector_work = (char *)g_memory_budget_high_ptr->allocate(0x20000, 16);
    if (__builtin_expect(!sector_work, 0))
        log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x allocate fail sector_work\n", LV0_ERR_INTERNAL);

    iopt_codec codec;
    struct iopt_window window;
    lv0_memset(&window, 0, sizeof window);
    window.w[1] = 62;
    window.w[2] = 2;
    window.w[8] = 1;
    window.addr = base;
    window.w[0] = iopt_w0;
    codec.pack_entry(sb_mmio_base, iopt_index, &window);
    NOR_EIEIO();

    this->type = identify();
    if (type != -1)
        this->type = type;
    if (this->type < 0)
        return -1;
    window.w[1] = g_nor_chips[this->type].iopt_field;
    codec.pack_entry(sb_mmio_base, iopt_index, &window);
    NOR_EIEIO();
    return 0;
}

unsigned long spansion_storage::translate(unsigned long offset)
{
    unsigned long base = this->base;
    unsigned long size;

    get_size(&size);
    if (offset < FLASH_ROTATE)
        return offset + base + size - FLASH_ROTATE;
    return base + offset - FLASH_ROTATE;
}

int spansion_storage::unprotect_type7(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0x4848);
    NOR_WRITE(addr, 0);
    return 0;
}

int spansion_storage::protect_type7(unsigned long addr)
{
    unsigned long chip = CHIP_BASE(addr);

    NOR_WRITE(chip + 0xAAA, 0xAAAA);
    NOR_WRITE(chip + 0x554, 0x5555);
    NOR_WRITE(chip + 0xAAA, 0x4848);
    NOR_WRITE(addr, 0x0101);
    return 0;
}

int spansion_storage_plain::read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read)
{
    unsigned long dev_size;
    int rc = get_size(&dev_size);

    if (rc == 0) {
        if (dev_size <= offset) {
            rc = -14;
        } else {
            unsigned long n = size;
            if (n > dev_size - offset)
                n = dev_size - offset;
            lv0_memmove(buf, (const char *)(offset + base), n);
            *bytes_read = n;
        }
    }
    return rc;
}

spansion_storage_plain::~spansion_storage_plain()
{
}

spansion_storage_plain::spansion_storage_plain()
{
}

unsigned long spansion_storage_plain::translate(unsigned long offset)
{
    return offset + base;
}

long spansion_storage_plain::get_mapped_address(unsigned long offset, unsigned long *out_addr)
{
    unsigned long size;
    int rc = get_size(&size);

    if (rc == 0) {
        if (size <= offset)
            rc = -14;
        else
            *out_addr = base + offset;
    }
    return rc;
}
