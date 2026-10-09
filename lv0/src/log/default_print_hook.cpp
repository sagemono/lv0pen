#include "printf.h"
#include "log.h"

int default_print_hook(const char *msg)
{
    log_message(msg);
    return 0;
}
