#include "loader.h"
#include "sb.h"
#include "uart.h"
#include "log.h"

extern const u64 sb_mmio_base;

void loader::init_log(unsigned int flags)
{
    get_uart()->init(sb_mmio_base, 23, 0, 0, 0, 0);
    log_set_handler(0, uart_console_puts);
    log_set_threshold(flags >> 30);
    log_set_threshold(1);
}
