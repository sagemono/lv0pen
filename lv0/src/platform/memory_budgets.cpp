#include "cxx.h"
#include "memory.h"
#include "log.h"

memory_budget::memory_budget()
{
    addr = 0;
    size = 0;
    cursor = 0;
    initialized = 0;
}

memory_budget::~memory_budget()
{
}

long memory_budget::initialize(unsigned long addr, unsigned long size)
{
    log_printf(2, 2, "memory_budget::initialize: addr = 0x%llx, size = 0x%llx\n", addr, size);
    this->addr = addr;
    this->size = size;
    this->cursor = addr;
    this->initialized = 1;
    return 0;
}

void *memory_budget::allocate(unsigned long size, unsigned long align)
{
    unsigned long ret = 0;
    if (initialized != 0) {
        unsigned long aligned = (cursor + align - 1) & -align;
        unsigned long end = addr + this->size;
        unsigned long next = aligned + size;
        if (end >= next) {
            ret = aligned;
            cursor = next;
        }
    }
    return (void *)ret;
}

int memory_budget::get_used_size(long *out)
{
    if (initialized == 0)
        return 1;
    *out = cursor - addr;
    return 0;
}

int memory_budget::get_free_size(long *out)
{
    if (initialized == 0)
        return 1;
    *out = (addr + size) - cursor;
    return 0;
}

memory_budget g_memory_budget_low;
memory_budget g_memory_budget_high;

memory_budget *g_memory_budget_low_ptr = &g_memory_budget_low;
memory_budget *g_memory_budget_high_ptr = &g_memory_budget_high;
