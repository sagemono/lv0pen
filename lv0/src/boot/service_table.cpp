#include "svc.h"
#include "spu.h"
#include "storage.h"
#include "static_init.h"
#include "syscon.h"

class service_table {
public:
    service_table();

    int (*entry0_unsupported)(int arg);
    void *unused_8;
    long (*spu_load_lv2ldr)(long spu_id, long arg0, long arg1, long arg2);
    long (*spu_load_isoldr)(unsigned long spu_id, long arg0, long arg1, long arg2, long arg3,
                            long arg4, long arg5, long arg6);
    long (*spu_load_lv2ldr_mode4)(long spu_id, long arg0);
    long (*spu_exit_isolation)(long spu_id);
    void *unused_48;
    long (*storage_open)(long dev_index);
    long (*storage_close)(long dev_index);
    long (*storage_lock)(long dev_index);
    long (*storage_read)(unsigned long dev_index, unsigned long offset, unsigned long size,
                         char *buf, unsigned long *out_len);
    long (*storage_write)(unsigned long dev_index, unsigned long offset, unsigned long size,
                          const char *buf, unsigned long *out_len);
    long (*storage_get_size)(unsigned long dev_index, unsigned long *out_size);
    long (*storage_get_count)(void);
    long (*spu_handle_class0_interrupt)(long spu_id, long int_stat, long *out0, long *out1,
                                        long *out2);
    long (*spu_handle_class2_interrupt)(long spu_id, long int_stat, long *out0, long *out1,
                                        long *out2);
    void *unused_128;
    long (*spu_is_running)(long spu_id, char *out_running);
    long (*storage_get_block_size)(long dev_index, unsigned long *out_size);
    long (*entry19_unsupported)(void);
    long (*spu_run)(long spu_id);
    long (*transit_power_state)(int state, unsigned int *params, int extra);
    long (*spu_load_appldr)(long spu_id, long arg0, long arg1);
    long (*sc_console_write)(const unsigned char *data, unsigned int size);
    void *unused_192[8];
};

extern service_table *g_service_table_ptr;

service_table::service_table()
{
    g_service_table_ptr = this;
    entry0_unsupported = svc_entry0_unsupported;
    spu_load_lv2ldr = svc_spu_load_lv2ldr;
    spu_load_isoldr = svc_spu_load_isoldr;
    spu_load_lv2ldr_mode4 = svc_spu_load_lv2ldr_mode4;
    spu_exit_isolation = svc_spu_exit_isolation;
    storage_open = svc_storage_open;
    storage_close = svc_storage_close;
    storage_lock = svc_storage_lock;
    storage_read = svc_storage_read;
    storage_write = svc_storage_write;
    storage_get_size = svc_storage_get_size;
    storage_get_count = svc_storage_get_count;
    spu_handle_class0_interrupt = svc_spu_handle_class0_interrupt;
    spu_handle_class2_interrupt = svc_spu_handle_class2_interrupt;
    spu_is_running = svc_spu_is_running;
    entry19_unsupported = svc_entry19_unsupported;
    storage_get_block_size = svc_storage_get_block_size;
    spu_run = svc_spu_run;
    transit_power_state = svc_transit_power_state;
    spu_load_appldr = svc_spu_load_appldr;
    sc_console_write = svc_sc_console_write;
}

int svc_entry0_unsupported(int arg)
{
    return g_svc_entry0_target_ptr->unsupported(arg);
}

long svc_spu_load_appldr(long spu_id, long arg0, long arg1)
{
    return g_spu_manager_ptr->load_appldr(spu_id, arg0, arg1);
}

long svc_spu_load_lv2ldr(long spu_id, long arg0, long arg1, long arg2)
{
    return g_spu_manager_ptr->load_lv2ldr(spu_id, arg0, arg1, arg2);
}

long svc_spu_load_isoldr(unsigned long spu_id, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
    return g_spu_manager_ptr->load_isoldr(spu_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

long svc_spu_load_lv2ldr_mode4(long spu_id, long arg0)
{
    return g_spu_manager_ptr->load_lv2ldr_mode4(spu_id, arg0);
}

long svc_spu_exit_isolation(long spu_id)
{
    return g_spu_manager_ptr->exit_isolation(spu_id);
}

long svc_storage_open(long dev_index)
{
    return g_storage.open(dev_index);
}

long svc_storage_close(long dev_index)
{
    return g_storage.close(dev_index);
}

long svc_storage_lock(long dev_index)
{
    return g_storage.reserve(dev_index);
}

long svc_storage_read(unsigned long dev_index, unsigned long offset, unsigned long size, char *buf,
                      unsigned long *out_len)
{
    return g_storage.read(dev_index, offset, size, buf, out_len);
}

long svc_storage_write(unsigned long dev_index, unsigned long offset, unsigned long size, const char *buf,
                       unsigned long *out_len)
{
    return g_storage.write(dev_index, offset, size, buf, out_len);
}

long svc_storage_get_size(unsigned long dev_index, unsigned long *out_size)
{
    return g_storage.get_size(dev_index, out_size);
}

long svc_storage_get_count(void)
{
    return g_storage.get_device_count();
}

long svc_storage_get_block_size(long dev_index, unsigned long *out_size)
{
    return g_storage.get_block_size(dev_index, out_size);
}

long svc_spu_handle_class0_interrupt(long spu_id, long int_stat, long *out0, long *out1, long *out2)
{
    return g_spu_manager_ptr->handle_class0_interrupt(spu_id, int_stat, out0, out1, out2);
}

long svc_spu_handle_class2_interrupt(long spu_id, long int_stat, long *out0, long *out1, long *out2)
{
    return g_spu_manager_ptr->handle_class2_interrupt(spu_id, int_stat, out0, out1, out2);
}

long svc_spu_is_running(long spu_id, char *out_running)
{
    return g_spu_manager_ptr->is_running(spu_id, out_running);
}

long svc_entry19_unsupported(void)
{
    return -16;
}

long svc_spu_run(long spu_id)
{
    return g_spu_manager_ptr->run(spu_id);
}

long svc_transit_power_state(int state, unsigned int *params, int extra)
{
    return g_power_state_transit_ptr->transit_via_spu(state, params, extra);
}

long svc_sc_console_write(const unsigned char *data, unsigned int size)
{
    return sc_write_cmd32_chunked(data, size);
}

service_table g_service_table;
