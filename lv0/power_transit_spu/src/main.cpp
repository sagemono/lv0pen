#include "syscon.h"
#include "uart.h"
#include "util.h"

struct transit_request {
    unsigned int state;
    unsigned int memory;
    unsigned int graphic;
    unsigned int wake_source;
};

#define REQUEST ((transit_request *)0)
#define SB_MMIO_BASE 0x24000000000ULL

void start_decrementer(unsigned int count);
void quiesce_memory_controller(void);
void syscon_power_off_with_code(int p1, int p2, int p3);
long syscon_set_wake_source(unsigned int source);
void syscon_printf(const char *fmt, ...);

int main(void)
{
    get_uart()->init(SB_MMIO_BASE, 23, 0, 0, 0, 0);
    get_syscon_device()->initialize(SB_MMIO_BASE, 1, 0);
    start_decrementer(0x7FFFFFFF);
    syscon_printf("[INFO]: Transit to %d status.\n", REQUEST->state);
    syscon_printf("[INFO]: context.memory %d, context.graphic %d\n", REQUEST->memory, REQUEST->graphic);
    syscon_printf("[INFO]: Wake source 0x%08x\n", REQUEST->wake_source);
    long rc = syscon_set_wake_source(REQUEST->wake_source | 4);
    if (rc)
        syscon_printf("[ERROR]: set_wake_source fail %d\n", rc);
    quiesce_memory_controller();
    syscon_power_off_with_code(REQUEST->memory, REQUEST->graphic, REQUEST->state);
    return 0;
}
