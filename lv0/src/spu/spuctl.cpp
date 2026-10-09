#include "spu.h"

struct spu_slot g_spu_slots[8] __attribute__((aligned(128)));

long spuctl_write_signal_notify64(long spu_id, unsigned long value)
{
    write_spu_sig_notify1(spu_id, value >> 32);
    return write_spu_sig_notify2(spu_id, (unsigned int)value);
}

unsigned int spuctl_get_status(long spu_id)
{
    return get_spu_status(spu_id);
}

void spuctl_clear_stop_interrupt(long spu_id)
{
    clear_spu_int_stat_class2_stop(spu_id);
}

void spuctl_clear_class0_interrupts(long spu_id, unsigned long mask)
{
    clear_spu_int_stat_class0(spu_id, mask);
}

void spuctl_run_request(long spu_id)
{
    request_spu_run(spu_id);
}

void spuctl_stop_request(long spu_id)
{
    request_spu_stop(spu_id);
}

void spuctl_clear_sr1_relocate(long spu_id)
{
    clear_spu_mfc_sr1_relocate(spu_id);
}

void spuctl_set_master_run_control(long spu_id)
{
    set_spu_mfc_sr1_master_run(spu_id);
}

void spuctl_clear_master_run_control(long spu_id)
{
    clear_spu_mfc_sr1_master_run(spu_id);
}

unsigned long spuctl_get_mfc_sr1(long spu_id)
{
    return get_spu_mfc_sr1(spu_id);
}

void save_spu_slot_sr1(long spu_id)
{
    g_spu_slots[spu_id].saved_sr1 = spuctl_get_mfc_sr1(spu_id);
}

int spuctl_set_mfc_sr1(long spu_id, unsigned long sr1)
{
    return set_spu_mfc_sr1(spu_id, sr1);
}

int spuctl_request_isolation_load(long spu_id, unsigned long ldr_addr, void *scratch)
{
    spuctl_write_signal_notify64(spu_id, ldr_addr);
    set_spu_privcntl_load_enable(spu_id);
    __asm__ volatile ("eieio" ::: "memory");
    request_spu_isolate_load(spu_id);
    __asm__ volatile ("eieio" ::: "memory");
    return 0;
}

int start_spu_isolation_load(long spu_id, unsigned long ldr_addr)
{
    char buf[16];

    clear_spu_mfc_sr1_relocate(spu_id);
    return spuctl_request_isolation_load(spu_id, ldr_addr, buf) ? -99 : 0;
}

int spuctl_write_in_mbox64(long spu_id, unsigned long data)
{
    return write_spu_in_mbox64(spu_id, data);
}

int spuctl_issue_proxy_dma(long spu_id, unsigned int lsa, long ea, unsigned int size, unsigned int tag, unsigned int class_id, unsigned int cmd)
{
    return enqueue_spu_proxy_dma(spu_id, lsa, ea, size, tag, class_id, cmd);
}

unsigned char spuctl_is_proxy_dma_complete(long unit, char tag)
{
    return is_spu_proxy_tag_complete(unit, tag);
}

int spuctl_read_out_intr_mbox(long spu_id, unsigned int *out)
{
    return read_spu_out_intr_mbox(spu_id, out);
}

void spuctl_clear_mailbox_interrupt(long spu_id)
{
    clear_spu_int_stat_class2_mailbox(spu_id);
}

int spuctl_read_out_mbox(long spu_id, int *out)
{
    return read_spu_out_mbox(spu_id, out);
}

unsigned int get_spu_slot_state(long spu_id)
{
    return g_spu_slots[spu_id].state;
}

struct spu_slot *get_spu_slot(long spu_id)
{
    return &g_spu_slots[spu_id];
}

long get_spu_slot_saved_sr1(long spu_id)
{
    return g_spu_slots[spu_id].saved_sr1;
}

int change_spu_slot_state(long spu_id, enum spu_slot_state new_state)
{
    int ret = -33;
    if ((unsigned int)new_state <= SPU_SLOT_PARAMS_SENT) {
        switch (new_state) {
        case SPU_SLOT_LOADING:
            if (g_spu_slots[spu_id].state != SPU_SLOT_IDLE)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_PARAMS_SENT:
            if (g_spu_slots[spu_id].state != SPU_SLOT_LOADING)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_RUNNING:
            if (g_spu_slots[spu_id].state != SPU_SLOT_LOADED)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_STOPPED:
            if (g_spu_slots[spu_id].state != SPU_SLOT_RUNNING)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_EXITING:
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_IDLE:
            if (g_spu_slots[spu_id].state != SPU_SLOT_EXITING)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        case SPU_SLOT_LOADED:
            if (g_spu_slots[spu_id].state != SPU_SLOT_LOADING)
                break;
            g_spu_slots[spu_id].state = new_state;
            ret = 0;
            break;
        }
    }
    return ret;
}

long spuctl_set_channel_count(long spu_id, long channel, long count)
{
    set_spu_channel_index(spu_id, channel);
    return set_spu_channel_count(spu_id, count);
}

void spuctl_restart_dma_queue(unsigned long spu_id)
{
    restart_spu_mfc_dma_queue(spu_id);
    while (!is_spu_mfc_dma_queue_running(spu_id))
        ;
}

void spuctl_purge_dma_queue(unsigned long spu_id)
{
    purge_spu_mfc_dma_queue(spu_id);
    while (!is_spu_mfc_purge_complete(spu_id))
        ;
}

void spuctl_suspend_dma_queue(unsigned long spu_id)
{
    suspend_spu_mfc_dma_queue(spu_id);
    while (!is_spu_mfc_suspend_complete(spu_id))
        ;
}

int spuctl_request_isolation_exit(long spu_id)
{
    set_spu_mfc_sr1_master_run(spu_id);
    request_spu_stop(spu_id);
    spuctl_suspend_dma_queue(spu_id);
    spuctl_purge_dma_queue(spu_id);
    get_spu_status(spu_id);
    __asm__ volatile ("eieio" ::: "memory");
    request_spu_isolate_exit(spu_id);
    return 0;
}

void set_spu_slot_params_sent(long spu_id)
{
    g_spu_slots[spu_id].params_sent = 1;
}

unsigned char get_spu_slot_params_sent(unsigned long spu_id)
{
    return g_spu_slots[spu_id].params_sent;
}
