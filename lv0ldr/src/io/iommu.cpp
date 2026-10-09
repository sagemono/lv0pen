#include "iommu.h"
#include "util.h"

void syscon_printf(const char *fmt, ...);

#define IOC_IOST_CACHE_INVD 0x510900
#define IOC_IOPT_CACHE_INVD 0x510908
#define IOC_IOST_ORIGIN     0x510918
#define   IOC_IOST_ORIGIN_E     0x8000000000000000ULL
#define   IOC_IOST_ORIGIN_HW    0x0000000000000800ULL
#define IOC_IOCMD_CFG       0x511C00
#define   IOC_IOCMD_CFG_TE      0x0000800000000000ULL

void iommu_context::clear_iopt_entries(u64 iopt, unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; i++)
        write64(iopt + (i << 3), 0);
}

void iommu_context::clear_iost_entry(u64 entry)
{
    write64(entry, 0);
}

u64 iommu_context::get_iost_entry_io_address(u64 iost_entry)
{
    return ((iost_entry - iost) / 8) << 28;
}

long iommu_context::invalidate4_iopt_cache(u64 iopt, unsigned int count)
{
    if (count > 2048)
        return -2;
    while (read64(ioc_base + IOC_IOPT_CACHE_INVD) & 1)
        ;
    write64(ioc_base + IOC_IOPT_CACHE_INVD, iopt | (((u64)count - 1) << 53) | 1);
    while (read64(ioc_base + IOC_IOPT_CACHE_INVD) & 1)
        ;
    __asm__ volatile ("sync" ::: "memory");
    return 0;
}

long iommu_context::allocate_iopt_entry(u64 *out, unsigned int size)
{
    unsigned int need = size >> 9;
    unsigned int cnt = 0;
    u64 off;

    for (off = 0; off < iopt_size; off += 4096) {
        if (read64(iopt + off) != 0) {
            cnt = 0;
            continue;
        }
        if (++cnt >= need) {
            *out = iopt + off;
            return 0;
        }
    }
    return -3;
}

long iommu_context::allocate_iost_entry(u64 *out)
{
    unsigned int off;

    for (off = 0; off < iost_size; off += 8) {
        if (read64(iost + off) == 0) {
            *out = iost + off;
            return 0;
        }
    }
    return -3;
}

void iommu_context::disable()
{
    u64 cfg = ioc_base + IOC_IOCMD_CFG;
    write64(cfg, read64(cfg) & ~IOC_IOCMD_CFG_TE);
    write64(ioc_base + IOC_IOST_ORIGIN, 0);
    __asm__ volatile ("sync" ::: "memory");
    ioc_base = 0;
}

long iommu_context::initialize(u64 ioc, u64 iost_addr, unsigned int iost_len,
                               u64 iopt_addr, unsigned int iopt_len)
{
    unsigned int i;

    if (iost_addr & 0xFFF) {
        syscon_printf("iost_addr is not aligned 4KB %08llx\n", iost_addr);
        return -2;
    }
    if (iopt_addr & 0xFFF) {
        syscon_printf("iopt_addr is not aligned 4KB %08llx\n", iopt_addr);
        return -2;
    }
    ioc_base = ioc;
    iost = iost_addr;
    iost_size = iost_len;
    iopt = iopt_addr;
    iopt_size = iopt_len;

    for (i = 0; i < iost_size; i += 8)
        clear_iost_entry(iost + i);
    clear_iopt_entries(iopt, iopt_size >> 3);

    write64(ioc_base + IOC_IOST_ORIGIN, iost | IOC_IOST_ORIGIN_E | IOC_IOST_ORIGIN_HW);
    {
        u64 cfg = ioc_base + IOC_IOCMD_CFG;
        write64(cfg, read64(cfg) | IOC_IOCMD_CFG_TE);
    }
    __asm__ volatile ("sync" ::: "memory");
    return 0;
}

void iommu_context::invalidate_iost_cache(u64 iost_entry)
{
    while (read64(ioc_base + IOC_IOST_CACHE_INVD) & 1)
        ;
    write64(ioc_base + IOC_IOST_CACHE_INVD, (((iost_entry - iost) >> 3) << 28) | 1);
    while (read64(ioc_base + IOC_IOST_CACHE_INVD) & 1)
        ;
    __asm__ volatile ("sync" ::: "memory");
}

long iommu_context::free_io_address(u64 io_addr)
{
    u64 idx = io_addr >> 28;
    long ret = -1;
    if (ioc_base) {
        u64 entry = iost + 8 * idx;
        u64 val = read64(entry);
        u64 pages = val & 0x3FFFFFFFFFFFF000ULL;
        u64 count = ((val & 0xFE0) << 4) + 512;
        clear_iopt_entries(pages, count);
        ret = invalidate4_iopt_cache(pages, count);
        if (ret) {
            syscon_printf("invalidate4_iopt_cache fail %d\n", ret);
        } else {
            clear_iost_entry(entry);
            invalidate_iost_cache(entry);
        }
    }
    return ret;
}

long iommu_context::allocate_io_address(unsigned int ioid, u64 ea_addr, unsigned int ea_size,
                                        unsigned int pgsz_code, unsigned int prot,
                                        unsigned int coherent, unsigned int ordering, unsigned int hint)
{
    FUNCTION_NAME("allocate_io_address");
    static const unsigned int page_size[8] = { 0, 4096, 0, 0x10000, 0, 0x100000, 0, 0x1000000 };
    long ret = -1;
    u64 entry;
    u64 table;
    unsigned int pgsz, npg, nent, i;

    if (!ioc_base)
        goto out;

    ret = allocate_iost_entry(&entry);
    if (ret) {
        syscon_printf("%s: allocate_iost_entry fail %d\n", function_name, ret);
        goto out;
    }

    switch (pgsz_code) {
    case 5: pgsz = page_size[5]; break;
    case 7: pgsz = page_size[7]; break;
    case 1: pgsz = page_size[1]; break;
    case 3: pgsz = page_size[3]; break;
    default: goto bad_pgsz;
    }

    if (ea_addr & (pgsz - 1) > 0) {
        ret = -2;
        syscon_printf("%s: ea_addr is not aligned by page_size in byte\n", function_name);
        goto out;
    }

    npg = ea_size / pgsz;
    if (!npg) {
        ret = -2;
        syscon_printf("%s: invalid ea_size < page size in byte\n", function_name);
        goto out;
    }

    nent = (npg + 511ULL) & ~511u;
    if (nent > 2048) {
        ret = -2;
        syscon_printf("%s: iopt_entry_num > %d\n", function_name, 2048);
        goto out;
    }

    ret = allocate_iopt_entry(&table, nent);
    if (ret) {
        syscon_printf("%s: allocate_iopt_entry fail %d\n", function_name, ret);
        goto out;
    }

    write64(entry, table | (((u64)(nent >> 9) - 1) << 5) | pgsz_code | 0x8000000000000000ULL);
    for (i = 0; i < nent; i++) {
        unsigned int off = pgsz * i;
        if (ea_size > off)
            write64(table + (i << 3), ((u64)prot << 62) | ((u64)coherent << 61) | ((u64)ordering << 59) |
                                      (ea_addr + off) | ((u64)hint << 11) | ioid);
        else
            write64(table + (i << 3), 0);
    }
    invalidate4_iopt_cache(table, nent);
    invalidate_iost_cache(entry);
    ret = (int)get_iost_entry_io_address(entry);
    goto out;
bad_pgsz:
    ret = -2;
    syscon_printf("%s: invalid page_size\n", function_name);
out:
    return ret;
}

iommu_context *get_iommu_context(void)
{
    static iommu_context ctx;
    return &ctx;
}
