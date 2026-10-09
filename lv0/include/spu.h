#ifndef LV0_SPU_H
#define LV0_SPU_H

#include "lv0.h"

enum spu_slot_state {
    SPU_SLOT_IDLE    = 0,
    SPU_SLOT_LOADING = 1,
    SPU_SLOT_RUNNING = 2,
    SPU_SLOT_LOADED  = 3,
    SPU_SLOT_STOPPED = 4,
    SPU_SLOT_EXITING = 5,
    SPU_SLOT_PARAMS_SENT = 6,
};

#define MFC_PUT_CMD 0x20
#define MFC_GET_CMD 0x40

#define SPU_LS_SAVE_OUT  0x3E000
#define SPU_LS_EID0      0x3E400
#define SPU_LS_PARAMS    0x3E800
#define SPU_LS_QA_TOKEN  0x3EC00
#define SPU_LS_RECORD    0x3EE00
#define SPU_LS_SAVE_IN   0x3F000

struct spu_loader_params {
    long args[9];
    volatile long type;
    long args_93[8];
};

struct spu_slot {
    struct spu_loader_params params;
    enum spu_slot_state state;
    char pad_94[4];
    unsigned long saved_sr1;
    char params_sent;
    char pad_a1[0x5F];
} __attribute__((aligned(128)));

#ifndef SPU_MANAGER_UNIT
#define SPU_MANAGER_UNIT
#endif

#ifdef __cplusplus
#include "intrinsics.h"
#include "cxx.h"

class componnet_manager;
extern char loader_self_version_threshold[];

class spu_manager {
public:
    spu_manager();
    long is_loader_version_ok(unsigned long loader_addr);
    int check_spu_idle(long spu_id) SPU_MANAGER_UNIT;
    long run(unsigned long spu_id);
    int start_isolation_load(long spu_id, unsigned long metldr_addr) SPU_MANAGER_UNIT;
    void pack_class0_status(unsigned long int_stat, unsigned long *out);
    void merge_class0_status(unsigned long int_stat, unsigned long *out, unsigned long *out_status);
    long handle_class2_running(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                               long *out_events);
    long handle_class2_stopped(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                               long *out_events);
    long handle_class2_exiting(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                               long *out_events);
    long ack_tag_group_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events);
    long ack_halt_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events);
    int handle_stop_interrupt(unsigned long spu_id, long *out_ack, long *out_status, long *out_events);
    void restore_mfc_sr1(long spu_id);
    int handle_class0_interrupt(unsigned long spu_id, long int_stat, long *out_ack, long *out_status,
                                 long *out_events);
    int exit_isolation(unsigned long spu_id);
    long is_running(unsigned long spu_id, char *running);
    int send_loader_address(long spu_id, long loader_addr) SPU_MANAGER_UNIT;
    int load_qa_token();
    int send_ls_record(unsigned long spu_id);
    int send_loader_params(unsigned long spu_id);
    int handle_mailbox_interrupt(unsigned long spu_id, long *out_ack, long *out_status);
    int ack_mailbox_interrupt(unsigned long spu_id, long *out_ack, long *out_status);
    int start_loader(unsigned long spu_id, unsigned int id);
    int load_lv1ldr(unsigned long spu_id, const void *params);
    long load_appldr(unsigned long spu_id, long arg0, long arg1);
    long load_isoldr(unsigned long spu_id, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5,
                     long arg6);
    long load_lv2ldr(unsigned long spu_id, long arg0, long arg1, long arg2);
    long load_lv2ldr_mode4(unsigned long spu_id, long arg0);
    int handle_class2_loading(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                              long *out_events, int handle_mailbox);
    int handle_class2_loaded(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                             long *out_events);
    int handle_class2_interrupt(unsigned long spu_id, unsigned long int_stat, long *out_ack, long *out_status,
                                long *out_events);

    spin_lock_guard lock;
    struct {
        long flag;
        long reserved;
        unsigned char token[128];
    } qa __attribute__((aligned(16)));
    unsigned char ls_record[48];
};

extern spu_manager *g_spu_manager_ptr;
#endif

int read_eeprom_restrict_spu_flag(unsigned char *out);
int write_eeprom_restrict_spu_flag(unsigned char flag);
long get_spu_fault_mask(void);
long get_enabled_spu_count(void);
long spuctl_write_signal_notify64(long spu_id, unsigned long value);
unsigned int spuctl_get_status(long spu_id);
void spuctl_clear_stop_interrupt(long spu_id);
void spuctl_clear_class0_interrupts(long spu_id, unsigned long mask);
void spuctl_run_request(long spu_id);
void spuctl_stop_request(long spu_id);
void spuctl_clear_sr1_relocate(long spu_id);
void spuctl_set_master_run_control(long spu_id);
void spuctl_clear_master_run_control(long spu_id);
unsigned long spuctl_get_mfc_sr1(long spu_id);
int spuctl_set_mfc_sr1(long spu_id, unsigned long sr1);
int spuctl_request_isolation_load(long spu_id, unsigned long ldr_addr, void *scratch);
int start_spu_isolation_load(long spu_id, unsigned long ldr_addr);
void save_spu_slot_sr1(long spu_id);
int spuctl_write_in_mbox64(long spu_id, unsigned long data);
int spuctl_issue_proxy_dma(long spu_id, unsigned int lsa, long ea, unsigned int size, unsigned int tag, unsigned int class_id, unsigned int cmd);
unsigned char spuctl_is_proxy_dma_complete(long unit, char tag);
int spuctl_read_out_intr_mbox(long spu_id, unsigned int *out);
void spuctl_clear_mailbox_interrupt(long spu_id);
int spuctl_read_out_mbox(long spu_id, int *out);
unsigned int get_spu_slot_state(long spu_id);
long get_spu_slot_saved_sr1(long spu_id);
int change_spu_slot_state(long spu_id, enum spu_slot_state new_state);
long spuctl_set_channel_count(long spu_id, long channel, long count);
void spuctl_restart_dma_queue(unsigned long spu_id);
void spuctl_purge_dma_queue(unsigned long spu_id);
void spuctl_suspend_dma_queue(unsigned long spu_id);
int spuctl_request_isolation_exit(long spu_id);
void set_spu_slot_params_sent(long spu_id);
unsigned char get_spu_slot_params_sent(unsigned long spu_id);
long first_usable_spu(void);
long calc_spu_faultbm_hi(void);
long calc_spu_faultbm_lo(void);

char *get_spu_ls_addr(long spu_id);
unsigned long get_spu_stop_code(long spu_id, unsigned long status);
void clear_spu_int_stat_class2_mailbox(long spu_id);
void clear_spu_int_stat_class2_stop(long spu_id);
void clear_spu_int_stat_class2_tag_group(long spu_id);
void clear_spu_int_stat_class2_halt(long spu_id);
unsigned long get_spu_mfc_sr1(long spu_id);
int set_spu_mfc_sr1(long spu_id, unsigned long sr1);
long clear_spu_mfc_sr1_relocate(long spu_id);
long set_spu_mfc_sr1_ls_decode(long spu_id);
long clear_spu_mfc_sr1_master_run(long spu_id);
long set_spu_mfc_sr1_master_run(long spu_id);
long set_spu_mfc_sr1_bus_tlbie(unsigned long spu_id);
long init_spu_mfc_sr1(long spu_id);
long set_spu_mfc_rm_boundary(long spu_id, unsigned long boundary);
long get_spu_int_stat(long spu_id, int int_class);
void clear_spu_int_stat_class0(long spu_id, unsigned long mask);
void set_spu_npc(long spu_id, long npc);
unsigned int get_spu_mbox_stat(long spu_id);
void write_spu_sig_notify1(long spu_id, unsigned int val);
long write_spu_sig_notify2(long spu_id, unsigned int val);
void request_spu_run(long spu_id);
void request_spu_stop(long spu_id);
void request_spu_isolate_load(long spu_id);
long request_spu_isolate_exit(long spu_id);
unsigned int get_spu_status(long spu_id);
int enqueue_spu_proxy_dma(long spu_id, unsigned int lsa, long ea, unsigned int size, unsigned int tag, unsigned int class_id, unsigned int cmd);
unsigned long is_spu_proxy_tag_complete(long unit, char tag);
void set_spu_privcntl_load_enable(long spu_id);
void suspend_spu_mfc_dma_queue(unsigned long spu_id);
void purge_spu_mfc_dma_queue(unsigned long spu_id);
void restart_spu_mfc_dma_queue(unsigned long spu_id);
bool is_spu_mfc_suspend_complete(unsigned long spu_id);
bool is_spu_mfc_purge_complete(unsigned long spu_id);
bool is_spu_mfc_dma_queue_running(unsigned long spu_id);
void set_spu_channel_index(long spu_id, long channel);
long set_spu_channel_count(long spu_id, long count);
int get_spu_in_mbox_free_count(long spu_id);
int write_spu_in_mbox(long spu_id, unsigned int data);
int write_spu_in_mbox64(long spu_id, unsigned long data);
int get_spu_out_mbox_count(long spu_id);
int read_spu_out_mbox(long spu_id, int *out);
int get_spu_out_intr_mbox_count(long spu_id);
int read_spu_out_intr_mbox(long spu_id, unsigned int *out);
bool is_spu_stopped(long spu_id, unsigned int *status_out);
extern char power_transit_spu_image_start[];
extern char power_transit_spu_image_end[];

struct spu_slot *get_spu_slot(long spu_id);
struct spu_problem *get_spu_problem_area_addr(long spu_id);

#endif
