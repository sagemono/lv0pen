#include "storage.h"
#include "platform.h"
#include "spu.h"
#include "clock.h"
#include "config.h"
#include "memory.h"
#include "mmio.h"

class platform_pwrmgr;

platform_pwrmgr *g_platform_pwrmgr_ptr;
platform *g_platform_ptr;
unsigned int g_sys_hw_model_emulate;

application::application()
{
}

void application::finalize_devices()
{
    platform *plat = g_platform_ptr;
    plat->finalize();

    unsigned long count = g_storage.get_device_count();
    unsigned long i;

    for (i = 0; i < count; ++i)
        g_storage.close(i);
}

int application::init_device_1()
{
    if (g_platform_ptr->initialize_io() == 0) {
        long spu_id;
        for (spu_id = 0; spu_id != 8; ++spu_id) {
            if (((0x80 >> spu_id) & get_spu_fault_mask()) == 0)
                set_spu_mfc_sr1(spu_id, 0x23);
        }
        return 0;
    }
    return -1;
}

static inline void set_decrementer(int dec)
{
    __asm__ volatile ("mtdec %0" :: "r"(dec));
}

static inline void reset_timebase()
{
    __asm__ volatile ("mttbl %0" :: "r"(0));
    __asm__ volatile ("mttbu %0" :: "r"(0));
    __asm__ volatile ("mttbl %0" :: "r"(0));
}

#define mtspr(n, v) __asm__ volatile ("mtspr " #n ",%0" :: "r"(v))

void application::reset_timebase_and_hid6_rev16_18()
{
    set_decrementer(0x7FFFFFFF);
    reset_timebase();
    mtspr(1017, 0x0001001400000000UL);
}

void application::reset_timebase_and_hid6_other()
{
    set_decrementer(0x7FFFFFFF);
    reset_timebase();
    mtspr(1017, 0x0001001400000000UL);
}

void application::reset_timebase_and_hid6()
{
    unsigned int rev = get_be_revision();

    if (rev >= 0x10 && rev <= 0x12)
        reset_timebase_and_hid6_rev16_18();
    else
        reset_timebase_and_hid6_other();
}

int application::init_device_0()
{
    g_memory_budget_low_ptr->initialize(memory_budget_low_base, memory_budget_low_size);
    g_memory_budget_high_ptr->initialize(0xC000000, 0x4000000);
    reset_timebase_and_hid6();
    g_platform_ptr->initialize();
    return 0;
}
