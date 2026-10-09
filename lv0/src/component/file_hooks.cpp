#include "log.h"
#include "platform.h"
#include "console.h"
#include "printf.h"

get_file_fn *g_get_file_hook = host_get_file;
put_file_fn *g_put_file_hook = host_put_file;
get_file_size_fn *g_get_file_size_hook = host_get_file_size;
long (*g_print_hook)(const char *msg) = (long (*)(const char *))default_print_hook;

void install_ext_file_hooks(void)
{
}

int call_get_file_hook(const char *name, unsigned long offset, void *buf, unsigned long size,
                       unsigned long *out_len)
{
    return g_get_file_hook(name, offset, buf, size, out_len);
}

int call_get_file_size_hook(const char *name, unsigned long *out_size)
{
    return g_get_file_size_hook(name, out_size);
}
