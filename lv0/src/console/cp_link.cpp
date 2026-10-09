#include "console.h"
#include "mmio_accessor.h"
#include "clock.h"

cp_link::cp_link() : console(&g_physical_console), mmio(get_mmio_accessor())
{
}

void cp_link::delay_us(unsigned long us)
{
    unsigned long deadline = get_time_us() + us;
    while (deadline > get_time_us())
        ;
}

void cp_link::delay_ms(unsigned long ms)
{
    unsigned long deadline = get_time_ms() + ms;
    while (deadline > get_time_ms())
        ;
}

unsigned int cp_link::read32(long addr)
{
    mmio_accessor *mmio = this->mmio;
    if (mmio == 0)
        return 0;
    return mmio->read32(addr);
}

void cp_link::write32(long addr, long val)
{
    mmio_accessor *mmio = this->mmio;
    if (mmio == 0)
        return;
    mmio->write32(addr, val);
}
