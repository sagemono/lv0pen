#ifndef LV0_SVC_H
#define LV0_SVC_H

#include "lv0.h"

#ifdef __cplusplus
class svc_entry0 {
public:
    svc_entry0() {}
    int unsupported(int arg);
};

class power_state_transit {
public:
    power_state_transit() {}
    long transit_via_spu(int state, unsigned int *args, int arg3);
};

extern svc_entry0 *g_svc_entry0_target_ptr;
extern power_state_transit *g_power_state_transit_ptr;
#endif

int svc_entry0_unsupported(int arg);
long svc_spu_load_appldr(long spu_id, long arg0, long arg1);
long svc_spu_load_lv2ldr(long spu_id, long arg0, long arg1, long arg2);
long svc_spu_load_isoldr(unsigned long spu_id, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long svc_spu_load_lv2ldr_mode4(long spu_id, long arg0);
long svc_spu_exit_isolation(long spu_id);
long svc_storage_open(long dev_index);
long svc_storage_close(long dev_index);
long svc_storage_lock(long dev_index);
long svc_storage_read(unsigned long dev_index, unsigned long offset, unsigned long size, char *buf,
                      unsigned long *out_len);
long svc_storage_write(unsigned long dev_index, unsigned long offset, unsigned long size, const char *buf,
                       unsigned long *out_len);
long svc_storage_get_size(unsigned long dev_index, unsigned long *out_size);
long svc_storage_get_count(void);
long svc_storage_get_block_size(long dev_index, unsigned long *out_size);
long svc_spu_handle_class0_interrupt(long spu_id, long int_stat, long *out0, long *out1, long *out2);
long svc_spu_handle_class2_interrupt(long spu_id, long int_stat, long *out0, long *out1, long *out2);
long svc_spu_is_running(long spu_id, char *out_running);
long svc_entry19_unsupported(void);
long svc_spu_run(long spu_id);
long svc_transit_power_state(int state, unsigned int *params, int extra);
long svc_sc_console_write(const unsigned char *data, unsigned int size);

#endif
