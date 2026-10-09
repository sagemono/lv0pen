#define SPU_MANAGER_UNIT __attribute__((visibility("hidden")))

#include "intrinsics.h"
#include "memory.h"
#include "spu.h"
#include "config.h"
#include "component.h"
#include "storage.h"
#include "mmio.h"
#include "syscon.h"

extern spu_manager g_spu_manager;
extern char g_spu_ls_save_area[0x1000];
extern long g_qa_token[10];
extern long spu_ls_3e000_init_value[2];

extern componnet_manager *volatile g_componnet_manager_ptr;

spu_manager::spu_manager()
{
    lv0_memset(&qa, 0, sizeof qa);
    lv0_memset(ls_record, 0, sizeof ls_record);
    lock.release();
}

inline long spu_manager::is_loader_version_ok(unsigned long loader_addr)
{
    return *(unsigned long *)(loader_addr + 128) > 0x8FFFFFFFFUL;
}

int spu_manager::check_spu_idle(long spu_id)
{
    if ((unsigned long)spu_id > 7)
        return -3;
    unsigned int state = get_spu_slot_state(spu_id);
    return state != SPU_SLOT_IDLE ? -5 : 0;
}

long spu_manager::run(unsigned long spu_id)
{

    if (spu_id > 7)
        return -3;
    lock.acquire();
    if (get_spu_slot_state(spu_id) != SPU_SLOT_LOADED) {
        lock.release();
        return -30;
    }
    if (change_spu_slot_state(spu_id, SPU_SLOT_RUNNING)) {
        lock.release();
        return -99;
    }
    spuctl_set_master_run_control(spu_id);
    spuctl_run_request(spu_id);
    lock.release();
    return 0;
}

int spu_manager::start_isolation_load(long spu_id, unsigned long metldr_addr)
{
    if (change_spu_slot_state(spu_id, SPU_SLOT_LOADING) != 0)
        return -99;
    return start_spu_isolation_load(spu_id, metldr_addr);
}

void spu_manager::pack_class0_status(unsigned long int_stat, unsigned long *out)
{
    if (int_stat & 8)
        *out |= 1;
    *out <<= 4;
    if (int_stat & 4)
        *out |= 1;
    *out <<= 4;
    if (int_stat & 2)
        *out |= 1;
    *out <<= 4;
    if (int_stat & 1)
        *out |= 1;
}

void spu_manager::merge_class0_status(unsigned long int_stat, unsigned long *out, unsigned long *out_status)
{
    if (int_stat & 8)
        *out |= 8;
    if (int_stat & 4)
        *out |= 4;
    if (int_stat & 2)
        *out |= 2;
    if (int_stat & 1)
        *out |= 1;
}

long spu_manager::handle_class2_running(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status, long *out_events)
{
    long shifted;

    *out_ack = 0;
    *out_status = 0;
    *out_events = 0;
    if (int_stat & 0x10)
        *out_events = 1;
    else
        *out_events = int_stat & 0x10;

    shifted = *out_events << 4;
    *out_events = shifted;
    if (int_stat & 8)
        *out_events = shifted | 1;
    else
        *out_events = shifted;

    shifted = *out_events << 4;
    *out_events = shifted;
    if (int_stat & 4)
        *out_events = shifted | 1;
    else
        *out_events = shifted;

    shifted = *out_events << 4;
    *out_events = shifted;
    if (int_stat & 2) {
        *out_events = shifted | 3;
        change_spu_slot_state(spu_id, SPU_SLOT_STOPPED);
    } else {
        *out_events = shifted;
    }

    shifted = *out_events << 4;
    *out_events = shifted;
    if (int_stat & 1)
        *out_events = shifted | 1;
    else
        *out_events = shifted;

    return 0;
}

long spu_manager::handle_class2_stopped(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status, long *out_events)
{
    *out_ack = 0;
    *out_status = 0;
    *out_events = 0;
    if (int_stat & 0x10)
        *out_events = 1;
    else
        *out_events = 0;
    *out_events <<= 4;
    if (int_stat & 8)
        *out_events |= 1;
    else
        *out_events |= 0;
    *out_events <<= 4;
    if (int_stat & 4)
        *out_events |= 1;
    else
        *out_events |= 0;
    *out_events <<= 4;
    if (int_stat & 2)
        *out_events |= 3;
    else
        *out_events |= 0;
    *out_events <<= 4;
    if (int_stat & 1)
        *out_events |= 1;
    else
        *out_events |= 0;
    return 0;
}

long spu_manager::handle_class2_exiting(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status, long *out_events)
{
    int ret = 0;

    *out_ack = 0;
    *out_status = 0;
    *out_events = 0;
    if (int_stat & 8) {
        clear_spu_int_stat_class2_tag_group(spu_id);
        *out_ack |= 8;
    }
    if (int_stat & 4) {
        clear_spu_int_stat_class2_halt(spu_id);
        *out_ack |= 4;
    }
    if (int_stat & 1) {
        clear_spu_int_stat_class2_mailbox(spu_id);
        *out_ack |= 1;
    }
    if (int_stat & 2) {
        unsigned int spu_status = spuctl_get_status(spu_id);
        unsigned long stop_code = spu_status >> 16;
        if (spu_status & 2) {
            ret = -99;
            if (stop_code == 0) {
                ret = 0;
                spuctl_set_channel_count(spu_id, 21, 16);
                spuctl_restart_dma_queue(spu_id);
                change_spu_slot_state(spu_id, SPU_SLOT_IDLE);
                *out_status = 0x40;
            }
            spuctl_clear_stop_interrupt(spu_id);
            *out_ack |= 2;
        } else {
            *out_status = 1;
        }
    }
    return ret;
}

long spu_manager::ack_tag_group_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events)
{
    clear_spu_int_stat_class2_tag_group(spu_id);
    *out_ack |= 8;
    return 0;
}

long spu_manager::ack_halt_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events)
{
    clear_spu_int_stat_class2_halt(spu_id);
    *out_ack |= 4;
    return -31;
}

int spu_manager::handle_stop_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events)
{
    unsigned long saved_sr1;
    unsigned int code = (spuctl_get_status(spu_id) >> 16) & 0xFFFF;
    unsigned int code_hi = code >> 16;
    unsigned int value;
    int rc;

    switch (code) {
    case 11: {
        long dma_buf = (long)g_spu_ls_save_area;
        saved_sr1 = spuctl_get_mfc_sr1(spu_id);
        spuctl_clear_sr1_relocate(spu_id);
        __asm__ volatile("eieio" ::: "memory");
        spuctl_issue_proxy_dma(spu_id, SPU_LS_SAVE_OUT, dma_buf, sizeof g_spu_ls_save_area, 1, 0, MFC_PUT_CMD);
        while (!spuctl_is_proxy_dma_complete(spu_id, 1))
            ;
        spuctl_set_mfc_sr1(spu_id, saved_sr1);
        *out_status |= 0x30;
        return 0;
    }
    case 50:
        saved_sr1 = spuctl_get_mfc_sr1(spu_id);
        spuctl_clear_sr1_relocate(spu_id);
        __asm__ volatile("eieio" ::: "memory");
        spuctl_issue_proxy_dma(spu_id, SPU_LS_SAVE_OUT, (long)ls_record, sizeof ls_record, 1, 0, MFC_PUT_CMD);
        while (!spuctl_is_proxy_dma_complete(spu_id, 1))
            ;
        spuctl_set_mfc_sr1(spu_id, saved_sr1);
        *out_status |= 0x30;
        return 0;
    case 51:
        get_syscon_device()->initialize(sb_mmio_base, 1, 0);
        value = code_hi;
        if (read_eeprom_nv1_u32_at4(&value))
            goto fail;
        if (write_eeprom_nv1_u32_at4((value & 0xFFFFFF) | 0x5000000))
            goto fail;
        syscon_power_off_with_code(0, 0, 3);
        for (;;)
            ;
    case 52:
        value = code_hi;
        syscon_syspm_get_33(&value);
        rc = finish_nv1_request(this, 1, value);
        if (rc)
            goto fail;
        *out_status |= 0x30;
        return rc;
    case 53:
        value = code_hi;
        syscon_syspm_get_33(&value);
        rc = finish_nv1_request(this, 2, value);
        if (rc)
            goto fail;
        *out_status |= 0x30;
        return rc;
    case 44:
    case 60:
        *out_status |= 0x50;
        return 0;
    default:
        *out_status |= 0x30;
        return 0;
    }
fail:
    return -99;
}

void spu_manager::restore_mfc_sr1(long spu_id)
{
    long saved_sr1 = get_spu_slot_saved_sr1(spu_id);
    if (saved_sr1) {
        spuctl_set_mfc_sr1(spu_id, saved_sr1);
        __asm__ volatile ("eieio" ::: "memory");
    }
}

int spu_manager::handle_class0_interrupt(unsigned long spu_id, long int_stat, long *out_ack, long *out_status, long *out_events)
{
    int ret;

    *out_ack = 0;
    *out_status = 0;
    *out_events = 0;
    lock.acquire();
    if (spu_id > 7) {
        lock.release();
        return -3;
    }
    if (int_stat == 0) {
        restore_mfc_sr1(spu_id);
        lock.release();
        return -32;
    }
    ret = -99;
    switch (get_spu_slot_state(spu_id)) {
    case SPU_SLOT_IDLE:
        ret = -30;
        break;
    case SPU_SLOT_LOADING:
    case SPU_SLOT_LOADED:
    case SPU_SLOT_PARAMS_SENT:
        pack_class0_status(int_stat, (unsigned long *)out_status);
        ret = 0;
        break;
    case SPU_SLOT_RUNNING:
    case SPU_SLOT_STOPPED:
        pack_class0_status(int_stat, (unsigned long *)out_events);
        ret = 0;
        break;
    case SPU_SLOT_EXITING:
        ret = -30;
        merge_class0_status(int_stat, (unsigned long *)out_ack, (unsigned long *)out_status);
        spuctl_clear_class0_interrupts(spu_id, int_stat);
        break;
    }
    restore_mfc_sr1(spu_id);
    lock.release();
    return ret;
}

int spu_manager::start_loader(unsigned long spu_id, unsigned int id)
{
    unsigned long metldr_addr = 0, ldr_addr = 0;
    int rc, err = -99;

    if (get_component_info_table()->get(COMPONENT_METLDR, &metldr_addr, 0) != 0)
        goto fail;
    if (get_component_info_table()->get(id, &ldr_addr, 0) != 0)
        goto fail;
    if (!is_loader_version_ok(ldr_addr))
        return -21;
    if (send_loader_address(spu_id, ldr_addr))
        goto fail;
    save_spu_slot_sr1(spu_id);
    rc = start_isolation_load(spu_id, metldr_addr);
    if (rc)
        rc = err;
    goto out;
fail:
    rc = err;
out:
    return rc;
}

int spu_manager::load_lv1ldr(unsigned long spu_id, const void *params)
{
    int rc;

    lock.acquire();
    rc = check_spu_idle(spu_id);
    if (rc)
        goto out;
    {
        struct spu_slot *slot = get_spu_slot(spu_id);
        lv0_memmove((char *)&slot->params, (const char *)params, sizeof slot->params);
        slot->params.args_93[7] = 0;
    }
    rc = start_loader(spu_id, COMPONENT_LV1LDR);
    if (rc != 0) {
        lock.release();
        return -99;
    }
    lock.release();
    return 0;
out:
    lock.release();
    return rc;
}

int spu_manager::exit_isolation(unsigned long spu_id)
{
    lock.acquire();
    if (spu_id > 7) {
        lock.release();
        return -3;
    }
    if (get_spu_slot_state(spu_id) == SPU_SLOT_IDLE) {
        lock.release();
        return -6;
    }
    if (spuctl_request_isolation_exit(spu_id) != 0) {
        lock.release();
        return -99;
    }
    if (change_spu_slot_state(spu_id, SPU_SLOT_EXITING) != 0) {
        lock.release();
        return -99;
    }
    restore_mfc_sr1(spu_id);
    lock.release();
    return 0;
}

long spu_manager::is_running(unsigned long spu_id, char *running)
{
    if (spu_id > 7)
        return -3;
    int state = get_spu_slot_state(spu_id);
    if (state == SPU_SLOT_RUNNING || state == SPU_SLOT_STOPPED)
        *running = 1;
    else
        *running = 0;
    return 0;
}

int spu_manager::send_loader_address(long spu_id, long loader_addr)
{
    return spuctl_write_in_mbox64(spu_id, loader_addr) ? -99 : 0;
}

static bool s_qa_token_loaded;

int spu_manager::load_qa_token()
{
    unsigned long qa_flag, token;
    int rc;

    if (s_qa_token_loaded)
        return 0;
    lv0_memset(&qa, 0, sizeof qa);
    rc = get_component_info_table()->get(COMPONENT_LV1LDR_2, &qa_flag, 0);
    if (rc || !*(unsigned long *)qa_flag) {
        qa.flag = -1;
        s_qa_token_loaded = true;
        return 0;
    }
    if (get_component_info_table()->get(COMPONENT_LV2LDR_2, &token, 0)) {
        qa.flag = -1;
        return -99;
    }
    lv0_memmove((char *)qa.token, (const char *)token, sizeof qa.token);
    qa.flag = 0;
    s_qa_token_loaded = true;
    return 0;
}

int spu_manager::send_ls_record(unsigned long spu_id)
{
    struct spu_slot *slot = get_spu_slot(spu_id);
    long type = slot->params.type;

    if (type == 2 || type == 5) {
        spuctl_issue_proxy_dma(spu_id, SPU_LS_RECORD, (long)ls_record, sizeof ls_record, 1, 0, MFC_GET_CMD);
        while (!spuctl_is_proxy_dma_complete(spu_id, 1))
            ;
    }
    return 0;
}

int spu_manager::send_loader_params(unsigned long spu_id)
{
    unsigned long eid0_addr, eid0_size;
    struct spu_slot *slot = get_spu_slot(spu_id);
    long type = slot->params.type;

    if (type == 2) goto blk;
    if (type == 3) goto blk;
    if (type != 5) goto skip;
blk:
    if (!get_component_info_table()->get(COMPONENT_EID0, &eid0_addr, &eid0_size))
        spuctl_issue_proxy_dma(spu_id, SPU_LS_EID0, eid0_addr, 0x400, 1, 0, MFC_GET_CMD);
    if (load_qa_token())
        goto fail;
    spuctl_issue_proxy_dma(spu_id, SPU_LS_QA_TOKEN, (long)&qa, sizeof qa, 1, 0, MFC_GET_CMD);
skip:
    spuctl_issue_proxy_dma(spu_id, SPU_LS_PARAMS, (long)&slot->params, sizeof slot->params, 1, 0, MFC_GET_CMD);
    spuctl_issue_proxy_dma(spu_id, SPU_LS_SAVE_IN, (long)g_spu_ls_save_area, sizeof g_spu_ls_save_area, 1, 0,
                           MFC_GET_CMD);
    while (!spuctl_is_proxy_dma_complete(spu_id, 1))
        ;
    if (send_ls_record(spu_id))
        goto fail;
    restore_mfc_sr1(spu_id);
    set_spu_slot_params_sent(spu_id);
    {
        long *dst = (long *)(get_spu_ls_addr(spu_id) + SPU_LS_SAVE_OUT);
        dst[0] = spu_ls_3e000_init_value[0];
        dst[1] = spu_ls_3e000_init_value[1];
    }
    return 0;
fail:
    return -99;
}

int spu_manager::handle_mailbox_interrupt(unsigned long spu_id, long *out_ack, long *out_status)
{
    int reply;
    unsigned int request;
    int rc;

    rc = spuctl_read_out_intr_mbox(spu_id, &request);
    if (rc)
        return -31;
    switch (request) {
    case 1: {
        spuctl_clear_mailbox_interrupt(spu_id);
        rc = send_loader_params(spu_id);
        if (__builtin_expect(rc == 0, 0) && !spuctl_read_out_mbox(spu_id, &reply) && reply == 1) {
            *out_status = 0;
            *out_ack |= 1;
            long type = get_spu_slot(spu_id)->params.type;
            if (type == 2 || type == 5)
                rc = change_spu_slot_state(spu_id, SPU_SLOT_PARAMS_SENT);
            return rc;
        }
        return -99;
    }
    case 2:
        spuctl_clear_mailbox_interrupt(spu_id);
        spuctl_stop_request(spu_id);
        change_spu_slot_state(spu_id, SPU_SLOT_LOADED);
        if (!spuctl_read_out_mbox(spu_id, &reply) && reply == 2) {
            *out_status = 2;
            *out_ack |= 1;
            restore_mfc_sr1(spu_id);
            spuctl_clear_master_run_control(spu_id);
            return rc;
        }
        return -99;
    default:
        spuctl_clear_mailbox_interrupt(spu_id);
        restore_mfc_sr1(spu_id);
        return -31;
    }
}

int spu_manager::ack_mailbox_interrupt(unsigned long spu_id, long *out_ack, long *out_status)
{
    *out_status = 1;
    restore_mfc_sr1(spu_id);
    return 0;
}

spu_manager g_spu_manager;

char g_spu_ls_save_area[0x1000];
long g_qa_token_unused[4];
long g_qa_token[10];
spu_manager *g_spu_manager_ptr = &g_spu_manager;
long spu_ls_3e000_init_value[2] = { 0xFF00000000, 0 };

long spu_manager::load_appldr(unsigned long spu_id, long arg0, long arg1)
{
    unsigned long metldr_addr, appldr_addr;
    long ret;
    volatile struct spu_slot *slot;
    componnet_manager *components;

    lock.acquire();
    ret = check_spu_idle(spu_id);
    if (ret != 0)
        goto out;
    slot = get_spu_slot(spu_id);
    components = g_componnet_manager_ptr;
    slot->params.type = 5;
    slot->params.args[0] = arg0;
    slot->params.args[1] = arg1;
    if (components->get_component(COMPONENT_METLDR, &metldr_addr, 0)) {
        ret = -99;
        lock.release();
        return ret;
    }
    if (g_componnet_manager_ptr->get_component(COMPONENT_APPLDR, &appldr_addr, 0))
        goto fail;
    if (!is_loader_version_ok(appldr_addr))
        goto small;
    if (send_loader_address(spu_id, appldr_addr) || start_isolation_load(spu_id, metldr_addr))
        goto fail;
    goto out;
fail:
    ret = -99;
    lock.release();
    return ret;
out:
    lock.release();
    return ret;
small:
    ret = -21;
    lock.release();
    return ret;
}

long spu_manager::load_isoldr(unsigned long spu_id, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5,
                              long arg6)
{
    unsigned long metldr_addr, isoldr_addr;
    long ret;
    volatile struct spu_slot *slot;
    componnet_manager *components;

    lock.acquire();
    ret = check_spu_idle(spu_id);
    if (ret != 0)
        goto out;
    slot = get_spu_slot(spu_id);
    components = g_componnet_manager_ptr;
    slot->params.type = 3;
    slot->params.args[0] = arg0;
    slot->params.args[1] = arg1;
    slot->params.args[2] = arg2;
    slot->params.args[3] = arg3;
    slot->params.args[4] = arg4;
    slot->params.args[5] = arg5;
    slot->params.args[6] = arg6;
    if (components->get_component(COMPONENT_METLDR, &metldr_addr, 0)) {
        ret = -99;
        lock.release();
        return ret;
    }
    if (g_componnet_manager_ptr->get_component(COMPONENT_ISOLDR, &isoldr_addr, 0))
        goto fail;
    if (!is_loader_version_ok(isoldr_addr))
        goto small;
    if (send_loader_address(spu_id, isoldr_addr) || start_isolation_load(spu_id, metldr_addr))
        goto fail;
    goto out;
fail:
    ret = -99;
    lock.release();
    return ret;
out:
    lock.release();
    return ret;
small:
    ret = -21;
    lock.release();
    return ret;
}

long spu_manager::load_lv2ldr(unsigned long spu_id, long arg0, long arg1, long arg2)
{
    unsigned long metldr_addr, lv2ldr_addr;
    long ret;
    volatile struct spu_slot *slot;
    componnet_manager *components;

    lock.acquire();
    ret = check_spu_idle(spu_id);
    if (ret != 0)
        goto out;
    slot = get_spu_slot(spu_id);
    components = g_componnet_manager_ptr;
    slot->params.type = 1;
    slot->params.args[0] = arg0;
    slot->params.args[1] = arg1;
    slot->params.args[2] = arg2;
    slot->params.args[3] = -1;
    if (components->get_component(COMPONENT_METLDR, &metldr_addr, 0)) {
        ret = -99;
        lock.release();
        return ret;
    }
    if (g_componnet_manager_ptr->get_component(COMPONENT_LV2LDR, &lv2ldr_addr, 0))
        goto fail;
    if (!is_loader_version_ok(lv2ldr_addr))
        goto small;
    if (send_loader_address(spu_id, lv2ldr_addr) || start_isolation_load(spu_id, metldr_addr))
        goto fail;
    goto out;
fail:
    ret = -99;
    lock.release();
    return ret;
out:
    lock.release();
    return ret;
small:
    ret = -21;
    lock.release();
    return ret;
}

long spu_manager::load_lv2ldr_mode4(unsigned long spu_id, long arg0)
{
    unsigned long metldr_addr, lv2ldr_addr;
    long ret;
    volatile struct spu_slot *slot;
    componnet_manager *components;

    lock.acquire();
    ret = check_spu_idle(spu_id);
    if (ret != 0)
        goto out;
    slot = get_spu_slot(spu_id);
    components = g_componnet_manager_ptr;
    slot->params.type = 4;
    slot->params.args[0] = arg0;
    slot->params.args[1] = 0;
    slot->params.args[2] = 0;
    slot->params.args[3] = 0;
    if (components->get_component(COMPONENT_METLDR, &metldr_addr, 0)) {
        ret = -99;
        lock.release();
        return ret;
    }
    if (g_componnet_manager_ptr->get_component(COMPONENT_LV2LDR, &lv2ldr_addr, 0))
        goto fail;
    if (!is_loader_version_ok(lv2ldr_addr))
        goto small;
    if (send_loader_address(spu_id, lv2ldr_addr) || start_isolation_load(spu_id, metldr_addr))
        goto fail;
    goto out;
fail:
    ret = -99;
    lock.release();
    return ret;
out:
    lock.release();
    return ret;
small:
    ret = -21;
    lock.release();
    return ret;
}

int spu_manager::handle_class2_loading(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                                       long *out_events, int handle_mailbox)
{
    int rc;

    *out_ack = 0;
    *out_status = 0;
    *out_events = 0;
    if (int_stat & 1) {
        if (handle_mailbox)
            rc = handle_mailbox_interrupt(spu_id, out_ack, out_status);
        else
            rc = ack_mailbox_interrupt(spu_id, out_ack, out_status);
        if (rc)
            return rc;
    }
    if (int_stat & 8) {
        rc = ack_tag_group_interrupt(spu_id, out_ack, out_status, out_events);
        if (rc)
            return rc;
    }
    if (int_stat & 4) {
        rc = ack_halt_interrupt(spu_id, out_ack, out_status, out_events);
        if (rc)
            return rc;
    }
    if (int_stat & 2) {
        rc = handle_stop_interrupt(spu_id, out_ack, out_status, out_events);
        if (rc)
            return rc;
    }
    if (get_spu_slot_params_sent(spu_id) && get_spu_slot_state(spu_id) != SPU_SLOT_LOADED)
        restore_mfc_sr1(spu_id);
    return 0;
}

int spu_manager::handle_class2_loaded(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                                      long *out_events)
{
    return handle_class2_loading(spu_id, int_stat, out_ack, out_status, out_events, 1);
}

int spu_manager::handle_class2_interrupt(unsigned long spu_id, unsigned long int_stat, long *out_ack,
                                         long *out_status, long *out_events)
{
    int ret;

    lock.acquire();
    if (spu_id > 7) {
        lock.release();
        return -3;
    }
    if (int_stat == 0) {
        restore_mfc_sr1(spu_id);
        lock.release();
        return -32;
    }
    switch (get_spu_slot_state(spu_id)) {
    case SPU_SLOT_IDLE:
        ret = -30;
        restore_mfc_sr1(spu_id);
        break;
    case SPU_SLOT_LOADING:
        ret = handle_class2_loading(spu_id, int_stat, out_ack, out_status, out_events, 1);
        restore_mfc_sr1(spu_id);
        break;
    case SPU_SLOT_PARAMS_SENT:
        ret = handle_class2_loading(spu_id, int_stat, out_ack, out_status, out_events, 0);
        restore_mfc_sr1(spu_id);
        break;
    case SPU_SLOT_LOADED:
        ret = handle_class2_loaded(spu_id, int_stat, out_ack, out_status, out_events);
        break;
    case SPU_SLOT_RUNNING:
        ret = handle_class2_running(spu_id, int_stat, out_ack, out_status, out_events);
        restore_mfc_sr1(spu_id);
        break;
    case SPU_SLOT_STOPPED:
        ret = handle_class2_stopped(spu_id, int_stat, out_ack, out_status, out_events);
        break;
    case SPU_SLOT_EXITING:
        ret = handle_class2_exiting(spu_id, int_stat, out_ack, out_status, out_events);
        restore_mfc_sr1(spu_id);
        break;
    default:
        ret = -99;
        restore_mfc_sr1(spu_id);
        break;
    }
    lock.release();
    return ret;
}
