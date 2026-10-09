#include "mmio_accessor.h"
#include "mmio.h"

typedef long long_ma __attribute__((may_alias));
typedef int int_ma __attribute__((may_alias));

char g_mmio_accessor[sizeof(mmio_accessor)];
mmio_accessor *g_mmio_accessor_ptr;

unsigned char mmio_accessor::is_trace_enabled()
{
    return trace;
}

void mmio_accessor::write64(long off, long val)
{
    *(volatile long *)(base + off) = val;
    if (is_trace_enabled())
        on_write(off, 8, val);
}

void mmio_accessor::write32(long off, unsigned int val)
{
    *(volatile unsigned int *)(base + off) = val;
    if (is_trace_enabled())
        on_write(off, 4, val);
}

long mmio_accessor::read64(long off)
{
    long val = *(volatile long *)(base + off);
    if (is_trace_enabled())
        on_read(off, 8, val);
    return val;
}

long mmio_accessor::read32(long off)
{
    unsigned int val = *(volatile unsigned int *)(base + off);
    if (is_trace_enabled())
        on_read(off, 4, val);
    return val;
}

void mmio_accessor::on_write(long off, long size, long val)
{
}

void mmio_accessor::on_read(long off, long size, long val)
{
}

mmio_accessor::mmio_accessor(char *base) : base(base), trace(0)
{
}

mmio_accessor *get_mmio_accessor(void)
{
    if (!g_mmio_accessor_ptr)
        g_mmio_accessor_ptr = new (g_mmio_accessor) mmio_accessor((char *)sb_mmio_base);
    return g_mmio_accessor_ptr;
}

mmio_accessor::~mmio_accessor()
{
}
