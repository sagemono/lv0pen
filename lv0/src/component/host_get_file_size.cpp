#include "console.h"

int host_get_file_size(const char *name, unsigned long *out_size)
{
    if (is_host_file_io_unavailable() || get_ft_channels()->get_file_size(name, out_size))
        return -1;
    return 0;
}
