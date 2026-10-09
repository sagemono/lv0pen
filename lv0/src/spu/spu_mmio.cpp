#include "mmio.h"
#include "spu.h"

struct spu_problem {
    char pad_0000[0x3004];
    volatile u32 mfc_lsa;
    volatile u64 mfc_ea;
    volatile u32 mfc_size_tag;
    volatile u32 mfc_class_cmd;
    char pad_3018[0x3204 - 0x3018];
    volatile u32 prxy_query_type;
    char pad_3208[0x321C - 0x3208];
    volatile u32 prxy_query_mask;
    char pad_3220[0x322C - 0x3220];
    volatile u32 prxy_tag_status;
    char pad_3230[0x4004 - 0x3230];
    volatile u32 spu_out_mbox;
    char pad_4008[0x400C - 0x4008];
    volatile u32 spu_in_mbox;
    char pad_4010[0x4014 - 0x4010];
    volatile u32 spu_mbox_stat;
    char pad_4018[0x401C - 0x4018];
    volatile u32 spu_runcntl;
    char pad_4020[0x4024 - 0x4020];
    volatile u32 spu_status;
    char pad_4028[0x4034 - 0x4028];
    u32 spu_npc;
    char pad_4038[0x1400C - 0x4038];
    volatile u32 spu_sig_notify_1;
    char pad_14010[0x1C00C - 0x14010];
    volatile u32 spu_sig_notify_2;
};

#define SPU_RUNCNTL_STOP            0
#define SPU_RUNCNTL_RUN             1
#define SPU_RUNCNTL_ISOLATE_EXIT    2
#define SPU_RUNCNTL_ISOLATE_LOAD    3

union spu_status {
    unsigned int raw;
    struct {
        unsigned int stop_code:16;
        unsigned int pad:9;
        unsigned int invalid_channel:1;
        unsigned int invalid_instr:1;
        unsigned int pad2:2;
        unsigned int halted:1;
        unsigned int stopped:1;
        unsigned int running:1;
    } f;
};

struct spu_priv2 {
    char pad_0000[0x3000];
    volatile u64 mfc_cntl;
    char pad_3008[0x4004 - 0x3008];
    volatile u32 spu_out_intr_mbox;
    char pad_4008[0x4040 - 0x4008];
    volatile u64 spu_privcntl;
    char pad_4048[0x4060 - 0x4048];
    volatile u64 spu_chnlcntptr;
    volatile u64 spu_chnlcnt;
};

#define MFC_CNTL_SUSPEND_DMA_QUEUE      0x1
#define MFC_CNTL_PURGE_DMA_REQUEST      0x8000
#define MFC_CNTL_RESTART_DMA_COMMAND    0x100000000UL
#define MFC_CNTL_SUSPEND_STATUS         0x300
#define MFC_CNTL_SUSPEND_COMPLETE       0x300
#define MFC_CNTL_PURGE_STATUS           0x3000000
#define MFC_CNTL_PURGE_COMPLETE         0x3000000

#define SPU_PRIVCNTL_LOAD_ENABLE        4

struct spu_priv1 {
    volatile u64 mfc_sr1;
    char pad_008[0x140 - 0x008];
    volatile u64 int_stat_class0;
    volatile u64 int_stat_class1;
    volatile u64 int_stat_class2;
    char pad_158[0x900 - 0x158];
    volatile u64 mfc_rm_boundary;
};

#define MFC_SR1_LS_DECODE               0x01UL
#define MFC_SR1_BUS_TLBIE               0x02UL
#define MFC_SR1_RELOCATE                0x10UL
#define MFC_SR1_MASTER_RUN              0x20UL

#define INT_CLASS2_MAILBOX              0x1
#define INT_CLASS2_STOP                 0x2
#define INT_CLASS2_HALT                 0x4
#define INT_CLASS2_TAG_GROUP            0x8

#define SPU_LS(spu)         ((char *)be_mmio_base + ((long)(spu) << 19))
#define SPU_PROBLEM(spu)    ((struct spu_problem *)(be_mmio_base + 0x40000 + ((long)(spu) << 19)))
#define SPU_PRIV2(spu)      ((struct spu_priv2 *)(be_mmio_base + 0x60000 + ((long)(spu) << 19)))
#define SPU_PRIV1(spu)      ((struct spu_priv1 *)(be_mmio_base + 0x400000 + ((long)(spu) << 13)))

#define SPU_AREA_FIELD(spu, shift, area_off, type, f) \
    (*(__typeof__(((struct type *)0)->f) *)(be_mmio_base + (area_off) + __builtin_offsetof(struct type, f) + ((long)(spu) << (shift))))
#define SPU_PROBLEM_FIELD(spu, f) SPU_AREA_FIELD(spu, 19, 0x40000, spu_problem, f)
#define SPU_PRIV2_FIELD(spu, f)   SPU_AREA_FIELD(spu, 19, 0x60000, spu_priv2, f)
#define SPU_PRIV1_FIELD(spu, f)   SPU_AREA_FIELD(spu, 13, 0x400000, spu_priv1, f)

char *get_spu_ls_addr(long spu_id) {
    return SPU_LS(spu_id);
}

unsigned long get_spu_stop_code(long spu_id, unsigned long status) {
    return status >> 16;
}

void clear_spu_int_stat_class2_mailbox(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, int_stat_class2) = INT_CLASS2_MAILBOX;
}

void clear_spu_int_stat_class2_stop(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, int_stat_class2) = INT_CLASS2_STOP;
}

void clear_spu_int_stat_class2_tag_group(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, int_stat_class2) = INT_CLASS2_TAG_GROUP;
}

void clear_spu_int_stat_class2_halt(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, int_stat_class2) = INT_CLASS2_HALT;
}

unsigned long get_spu_mfc_sr1(long spu_id) {
    return SPU_PRIV1_FIELD(spu_id, mfc_sr1);
}

int set_spu_mfc_sr1(long spu_id, unsigned long sr1) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) = sr1;
    return 0;
}

long clear_spu_mfc_sr1_relocate(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) &= ~MFC_SR1_RELOCATE;
    return 0;
}

long set_spu_mfc_sr1_ls_decode(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) |= MFC_SR1_LS_DECODE;
    return 0;
}

long clear_spu_mfc_sr1_master_run(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) &= ~MFC_SR1_MASTER_RUN;
    return 0;
}

long set_spu_mfc_sr1_master_run(long spu_id) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) |= MFC_SR1_MASTER_RUN;
    return 0;
}

long set_spu_mfc_sr1_bus_tlbie(unsigned long spu_id) {
    SPU_PRIV1_FIELD(spu_id, mfc_sr1) |= MFC_SR1_BUS_TLBIE;
    return 0;
}

long init_spu_mfc_sr1(long spu_id) {
    clear_spu_mfc_sr1_relocate(spu_id);
    set_spu_mfc_sr1_ls_decode(spu_id);
    set_spu_mfc_sr1_master_run(spu_id);
    return set_spu_mfc_sr1_bus_tlbie(spu_id);
}

long set_spu_mfc_rm_boundary(long spu_id, unsigned long boundary) {
    SPU_PRIV1_FIELD(spu_id, mfc_rm_boundary) = boundary;
    return 0;
}

long get_spu_int_stat(long spu_id, int int_class) {
    struct spu_priv1 *priv1 = SPU_PRIV1(spu_id);
    if (int_class == 0)
        return priv1->int_stat_class0;
    if (__builtin_expect(int_class == 1, 0))
        return priv1->int_stat_class1;
    if (int_class == 2)
        return priv1->int_stat_class2;
    return -99;
}

void clear_spu_int_stat_class0(long spu_id, unsigned long mask) {
    SPU_PRIV1_FIELD(spu_id, int_stat_class0) = mask;
}

struct spu_problem *get_spu_problem_area_addr(long spu_id) {
    return SPU_PROBLEM(spu_id);
}

unsigned int get_spu_mbox_stat(long spu_id) {
    return SPU_PROBLEM_FIELD(spu_id, spu_mbox_stat);
}

void set_spu_npc(long spu_id, long npc) {
    SPU_PROBLEM_FIELD(spu_id, spu_npc) = npc;
}

void write_spu_sig_notify1(long spu_id, unsigned int val) {
    SPU_PROBLEM_FIELD(spu_id, spu_sig_notify_1) = val;
}

long write_spu_sig_notify2(long spu_id, unsigned int val) {
    SPU_PROBLEM_FIELD(spu_id, spu_sig_notify_2) = val;
}

void request_spu_run(long spu_id) {
    SPU_PROBLEM_FIELD(spu_id, spu_runcntl) = SPU_RUNCNTL_RUN;
}

void request_spu_stop(long spu_id) {
    SPU_PROBLEM_FIELD(spu_id, spu_runcntl) = SPU_RUNCNTL_STOP;
}

void request_spu_isolate_load(long spu_id) {
    SPU_PROBLEM_FIELD(spu_id, spu_runcntl) = SPU_RUNCNTL_ISOLATE_LOAD;
}

long request_spu_isolate_exit(long spu_id) {
    SPU_PROBLEM_FIELD(spu_id, spu_runcntl) = SPU_RUNCNTL_ISOLATE_EXIT;
}

unsigned int get_spu_status(long spu_id) {
    __asm__ volatile ("eieio" ::: "memory");
    return SPU_PROBLEM_FIELD(spu_id, spu_status);
}

int enqueue_spu_proxy_dma(long spu_id, unsigned int lsa, long ea, unsigned int size, unsigned int tag, unsigned int class_id, unsigned int cmd) {
    struct spu_problem *problem = SPU_PROBLEM(spu_id);
    problem->mfc_lsa = lsa;
    problem->mfc_ea = ea;
    problem->mfc_size_tag = (size << 16) | tag;
    problem->mfc_class_cmd = (class_id << 16) | cmd;
    return problem->mfc_class_cmd;
}

unsigned long is_spu_proxy_tag_complete(long unit, char tag) {
    struct spu_problem *problem = SPU_PROBLEM(unit);
    unsigned int bit = 1 << tag;
    problem->prxy_query_type = 0;
    problem->prxy_query_mask = bit;
    __asm__ volatile ("eieio" ::: "memory");
    return problem->prxy_tag_status != 0;
}

void set_spu_privcntl_load_enable(long spu_id) {
    SPU_PRIV2_FIELD(spu_id, spu_privcntl) = SPU_PRIVCNTL_LOAD_ENABLE;
}

void suspend_spu_mfc_dma_queue(unsigned long spu_id) {
    SPU_PRIV2_FIELD(spu_id, mfc_cntl) = MFC_CNTL_SUSPEND_DMA_QUEUE;
    __asm__ volatile ("eieio" ::: "memory");
}

void purge_spu_mfc_dma_queue(unsigned long spu_id) {
    SPU_PRIV2_FIELD(spu_id, mfc_cntl) = MFC_CNTL_PURGE_DMA_REQUEST;
    __asm__ volatile ("eieio" ::: "memory");
}

void restart_spu_mfc_dma_queue(unsigned long spu_id) {
    SPU_PRIV2_FIELD(spu_id, mfc_cntl) = MFC_CNTL_RESTART_DMA_COMMAND;
    __asm__ volatile ("eieio" ::: "memory");
}

bool is_spu_mfc_suspend_complete(unsigned long spu_id) {
    return (SPU_PRIV2_FIELD(spu_id, mfc_cntl) & MFC_CNTL_SUSPEND_STATUS) == MFC_CNTL_SUSPEND_COMPLETE;
}

bool is_spu_mfc_purge_complete(unsigned long spu_id) {
    return (SPU_PRIV2_FIELD(spu_id, mfc_cntl) & MFC_CNTL_PURGE_STATUS) == MFC_CNTL_PURGE_COMPLETE;
}

bool is_spu_mfc_dma_queue_running(unsigned long spu_id) {
    return (SPU_PRIV2_FIELD(spu_id, mfc_cntl) & MFC_CNTL_SUSPEND_STATUS) == 0;
}

void set_spu_channel_index(long spu_id, long channel) {
    SPU_PRIV2_FIELD(spu_id, spu_chnlcntptr) = channel;
    __asm__ volatile ("eieio" ::: "memory");
}

long set_spu_channel_count(long spu_id, long count) {
    SPU_PRIV2_FIELD(spu_id, spu_chnlcnt) = count;
    __asm__ volatile ("eieio" ::: "memory");
}

int get_spu_in_mbox_free_count(long spu_id) {
    return (get_spu_mbox_stat(spu_id) >> 8) & 0x7;
}

int write_spu_in_mbox(long spu_id, unsigned int data) {
    int free_count = get_spu_in_mbox_free_count(spu_id);
    if (free_count < 0)
        return -99;
    if (!free_count)
        return -1;
    SPU_PROBLEM_FIELD(spu_id, spu_in_mbox) = data;
    return 0;
}

int write_spu_in_mbox64(long spu_id, unsigned long data) {
    long rc = write_spu_in_mbox(spu_id, data >> 32);
    if (!rc)
        return write_spu_in_mbox(spu_id, (unsigned int)data);
    return rc;
}

int get_spu_out_mbox_count(long spu_id) {
    return get_spu_mbox_stat(spu_id) & 0x1;
}

int read_spu_out_mbox(long spu_id, int *out) {
    int count = get_spu_out_mbox_count(spu_id);
    if (count < 0)
        return -99;
    if (!count)
        return -2;
    *out = SPU_PROBLEM_FIELD(spu_id, spu_out_mbox);
    return 0;
}

int get_spu_out_intr_mbox_count(long spu_id) {
    return (get_spu_mbox_stat(spu_id) >> 16) & 0x1;
}

int read_spu_out_intr_mbox(long spu_id, unsigned int *out) {
    int count = get_spu_out_intr_mbox_count(spu_id);
    if (count < 0)
        return -99;
    if (!count)
        return -2;
    *out = SPU_PRIV2_FIELD(spu_id, spu_out_intr_mbox);
    return 0;
}

bool is_spu_stopped(long spu_id, unsigned int *status_out) {
    union spu_status st;
    st.raw = get_spu_status(spu_id);
    *status_out = st.raw;
    if (st.f.invalid_channel)
        return 1;
    if (st.f.invalid_instr)
        return 1;
    if (st.f.halted)
        return 1;
    if (st.f.stopped)
        return 1;
    return 0;
}
