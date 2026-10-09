#include "console.h"

int host_put_file(const char *name, unsigned long offset, const void *buf, unsigned long size,
                  unsigned long *out_len)
{
    if (is_host_file_io_unavailable() || get_ft_channels()->put_file(name, offset, buf, size, out_len))
        return -1;
    return 0;
}
