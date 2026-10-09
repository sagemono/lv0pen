#include "console.h"

struct ext_io_table {
    get_file_fn *get_file;
    put_file_fn *put_file;
    get_file_size_fn *get_file_size;
    long (*console_output)(const char *msg);
};

struct ext_io_table **ext_io_table_slot = (struct ext_io_table **)0x20000;

int ext_get_file(const char *name, unsigned long offset, void *buf, unsigned long size,
                  unsigned long *out_len)
{
    return (*ext_io_table_slot)->get_file(name, offset, buf, size, out_len);
}

int ext_put_file(const char *name, unsigned long offset, const void *buf, unsigned long size,
                 unsigned long *out_len)
{
    return (*ext_io_table_slot)->put_file(name, offset, buf, size, out_len);
}

int ext_get_file_size(const char *name, unsigned long *out_size)
{
    return (*ext_io_table_slot)->get_file_size(name, out_size);
}

long ext_console_output(const char *msg)
{
    return (*ext_io_table_slot)->console_output(msg);
}
