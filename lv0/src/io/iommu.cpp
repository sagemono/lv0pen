#include "iommu.h"
#include "log.h"

#define ROR8(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define ROL8(x, n) (((x) << (n)) | ((x) >> (64 - (n))))

#define IOC_IOST_CACHE_INVD 0x510900
#define IOC_IOPT_CACHE_INVD 0x510908
#define IOC_IOST_ORIGIN     0x510918
#define   IOC_IOST_ORIGIN_E     0x8000000000000000UL
#define   IOC_IOST_ORIGIN_HW    0x0000000000000800UL
#define IOC_IOCMD_CFG       0x511C00
#define   IOC_IOCMD_CFG_TE      0x0000800000000000UL

inline unsigned long iommu_context::get_iost_entry_io_address(long iost_entry)
{
    return ((iost_entry - iost) << 25) & 0xFFFFFFFFF0000000UL;
}

void iommu_context::clear_iopt_entries(unsigned long iopt, unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; i++)
        *(unsigned long *)(iopt + ((unsigned long)i << 3)) = 0;
}

int iommu_context::allocate_iost_entry(long **out)
{
    unsigned int off;

    for (off = 0; off < iost_size; off += 8) {
        long base = iost;
        if (!*(long *)(base + off)) { *out = (long *)(base + off); return 0; }
    }
    return -3;
}

int iommu_context::allocate_iopt_entry(unsigned long *out, unsigned int size)
{
    unsigned int limit = iopt_size;
    unsigned int need = size >> 9;
    unsigned int cnt = 0;
    unsigned long off = 0;

    while (off < limit) {
        unsigned long base = iopt;
        unsigned long slot = off + base;
        unsigned long entry = *(volatile unsigned long *)slot;
        off += 0x1000;
        if (entry != 0) { cnt = 0; continue; }
        if (++cnt >= need) { *out = slot; return 0; }
    }
    return -3;
}

void iommu_context::disable()
{
    unsigned long *cfg = (unsigned long *)(ioc_base + IOC_IOCMD_CFG);

    *cfg = ROR8(ROL8(*cfg, 16) & 0x7FFFFFFFFFFFFFFFUL, 16);
    *(unsigned long *)(ioc_base + IOC_IOST_ORIGIN) = 0;
    __asm__ volatile ("sync" ::: "memory");
    ioc_base = 0;
}

void flush_dcache_range(unsigned long addr, unsigned int size)
{
    unsigned int off;

    for (off = 0; off < size; off += 128)
        __asm__ volatile ("dcbf %0,%1" :: "b"(addr), "r"(off) : "memory");
    __asm__ volatile ("sync" ::: "memory");
}

int iommu_context::invalidate4_iopt_cache(unsigned long iopt, unsigned int count)
{
    unsigned long reg;

    if (count > 2048)
        return -2;
    flush_dcache_range(iopt, count * 8);

    reg = ioc_base + IOC_IOPT_CACHE_INVD;
    while (*(volatile unsigned long *)reg & 1)
        ;
    *(volatile unsigned long *)reg =
        iopt | (((unsigned long)count - 1) << 53) | 1;

    reg = ioc_base + IOC_IOPT_CACHE_INVD;
    while (*(volatile unsigned long *)reg & 1)
        ;
    __asm__ volatile ("sync" ::: "memory");
    return 0;
}

void iommu_context::invalidate_iost_cache(unsigned long iost_entry)
{
    unsigned long reg;

    flush_dcache_range(iost_entry, 8);

    reg = ioc_base + IOC_IOST_CACHE_INVD;
    while (*(volatile unsigned long *)reg & 1)
        ;
    *(volatile unsigned long *)reg =
        (((iost_entry - iost) >> 3) << 28) | 1;

    reg = ioc_base + IOC_IOST_CACHE_INVD;
    while (*(volatile unsigned long *)reg & 1)
        ;
    __asm__ volatile ("sync" ::: "memory");
}

int iommu_context::initialize(unsigned long ioc, unsigned long iost_addr, unsigned int iost_len,
                               unsigned long iopt_addr, unsigned int iopt_len)
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
    iopt = iopt_addr;
    iopt_size = iopt_len;
    iost_size = iost_len;

    for (i = 0; i < iost_size; i += 8)
        *(unsigned long *)(iost + i) = 0;

    flush_dcache_range(iost, iost_size);

    clear_iopt_entries(iopt, iopt_size >> 3);

    {
        unsigned long origin = iost | IOC_IOST_ORIGIN_E | IOC_IOST_ORIGIN_HW;
        *(volatile unsigned long *)(ioc_base + IOC_IOST_ORIGIN) = origin;
    }
    *(volatile unsigned long *)(ioc_base + IOC_IOCMD_CFG) |= IOC_IOCMD_CFG_TE;
    __asm__ volatile ("sync" ::: "memory");
    return 0;
}

int iommu_context::free_io_address(unsigned long io_addr)
{
    unsigned long idx = io_addr >> 28;
    int ret = -1;
    if (ioc_base) {
        long table = iost;
        unsigned long *entry = (unsigned long *)(table + 8 * idx);
        unsigned long val = *entry;
        long nppt = (val >> 5) & 0x7F;
        unsigned long pages = ROR8((unsigned long)(4 * val), 2) & 0xFFFFFFFFFFFFF000UL;
        unsigned long count = (nppt << 9) + 512;
        clear_iopt_entries(pages, count);
        ret = invalidate4_iopt_cache(pages, count);
        if (ret) {
            syscon_printf("invalidate4_iopt_cache fail %d\n", ret);
        } else {
            *entry = 0;
            invalidate_iost_cache((long)entry);
        }
    }
    return ret;
}

int iommu_context::allocate_io_address(unsigned int ioid, long ea_addr, unsigned int ea_size, int pgsz_code,
                                        unsigned int prot, unsigned int coherent, unsigned int ordering,
                                        unsigned int hint)
{
    FUNCTION_NAME("allocate_io_address");
    int ret = -1;
    u64 *entry;
    u64 table;
    unsigned int pgsz, npg, nent, i;

    if (!ioc_base)
        goto out;

    ret = allocate_iost_entry((long **)(&entry));
    if (ret) {
        syscon_printf("%s: allocate_iost_entry fail %d\n", function_name, ret);
        goto out;
    }

    switch (pgsz_code) {
    case 1: pgsz = 0x1000; break;
    case 3: pgsz = 0x10000; break;
    case 5: pgsz = 0x100000; break;
    case 7: pgsz = 0x1000000; break;
    default: pgsz = 0; break;
    }
    if (!pgsz) {
        ret = -2;
        syscon_printf("%s: invalid page_size\n", function_name);
        goto out;
    }

    if (ea_addr & ((int)pgsz | 1) > 0) {
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

    nent = (npg + 511) & ~511u;
    if (nent > 2048) {
        ret = -2;
        syscon_printf("%s: iopt_entry_num > %d\n", function_name, 2048L);
        goto out;
    }

    ret = allocate_iopt_entry(&table, nent);
    if (ret) {
        syscon_printf("%s: allocate_iopt_entry fail %d\n", function_name, ret);
        goto out;
    }

    *entry = table | (((u64)nent >> 4) - 32) | (unsigned int)pgsz_code | 0x8000000000000000UL;
    for (i = 0; i < nent; i++) {
        unsigned int off = pgsz * i;
        if (ea_size > off)
            ((u64 *)table)[i] = ((u64)prot << 62) | ((u64)coherent << 61) | ((u64)ordering << 59)
                              | (ea_addr + off) | ((u64)hint << 11) | ioid;
        else
            ((u64 *)table)[i] = 0;
    }
    invalidate4_iopt_cache(table, nent);
    invalidate_iost_cache((unsigned long)entry);
    ret = (int)get_iost_entry_io_address((long)entry);
out:
    return ret;
}
