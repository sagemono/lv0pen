#include "gbe.h"
#include "iommu.h"
#include "memory.h"
#include "log.h"

extern gbe_work g_gbe_work;

gbe_work::gbe_work() : region(0), io_addr(0), size(0), cursor(0), mapped(false)
{
}

void gbe_work::initialize(unsigned long size)
{
    void *region = g_memory_budget_high_ptr->allocate(0x100000, 0x1000);
    if (!region) {
        log_printf(1, 1, "[ERROR]: 0x%08x gbe_work_addr\n", LV0_ERR_INTERNAL);
        return;
    }
    if (size > 0x100000) {
        log_printf(1, 1, "[ERROR]: 0x%08x gbe_work_size 0x%llx\n", LV0_ERR_INTERNAL, 0x100000);
        return;
    }

    io_addr = 0;
    long io_addr = get_iommu_context()->allocate_io_address(0, (long)region, size, 1, 3, 1, 3, 0);
    this->io_addr = io_addr;
    mapped = true;
    this->size = (size + 0xFFF) & ~0xFFFUL;
    this->region = region;
    cursor = (unsigned long)region;
}

void gbe_work::finalize()
{
    if (mapped)
        get_iommu_context()->free_io_address(io_addr);
}

gbe_work *get_gbe_work(void)
{
    return &g_gbe_work;
}

unsigned long gbe_work::allocate(unsigned long size, unsigned long align)
{
    unsigned long aligned = (cursor + align - 1) & -align;
    cursor = aligned + size;
    return aligned;
}

gbe_work g_gbe_work;
