#include "platform.h"
#include "config.h"
#include "iommu.h"
#include "memory.h"
#include "spu.h"
#include "component.h"
#include "log.h"
#include "mmio.h"
#include "aes.h"
#include "syscon.h"

extern componnet_manager *g_componnet_manager_ptr;
char g_sys_ac_sd;
long g_sys_ac_misc;
unsigned long g_sys_ac_misc2 = 1;
unsigned long g_sys_ac_product_code = 0xFFFFFFFF;

int run_lv1ldr(long spu_id, const void *ldr_parm, unsigned int *out_stop_code)
{
    unsigned int spu_status;
    long out1, out2, out0;
    long int_stat;

    if (__builtin_expect(g_spu_manager_ptr->load_lv1ldr(spu_id, ldr_parm) != 0, 0)) {
        log_message("[ERROR]: 0x%08x Boot CELL OS\n", LV0_ERR_BOOT_LV1);
        return -1;
    }
    out0 = 0;
    out1 = 0;
    out2 = 0;
    for (;;) {
        int_stat = get_spu_int_stat(spu_id, 2);
        if ((int_stat & 1) != 0)
            break;
        if (is_spu_stopped(spu_id, &spu_status)) {
            if (spu_status & 2)
                log_message("[ERROR]: 0x%08x lv1ldr stop with stopcode %d\n", LV0_ERR_BOOT_LV1,
                            (unsigned int)get_spu_stop_code(spu_id, spu_status));
            else
                log_message("[ERROR]: 0x%08x lv1ldr stop wo stopcode\n", LV0_ERR_BOOT_LV1);
            return -1;
        }
    }
    if (g_spu_manager_ptr->handle_class2_interrupt(spu_id, int_stat, &out0, &out1, &out2)) {
        log_message("[ERROR]: 0x%08x Boot CELL OS\n", LV0_ERR_INTERNAL);
        return -1;
    }
    while (!is_spu_stopped(spu_id, &spu_status))
        ;
    if (spu_status & 2) {
        unsigned int stop_code = get_spu_stop_code(spu_id, spu_status);
        if (stop_code == 10) {
            g_spu_manager_ptr->exit_isolation(spu_id);
            goto restart;
        }
        log_message("[ERROR]: 0x%08x Boot CELL OS : stopcode [%d]\n", LV0_ERR_BOOT_LV1, stop_code);
        if (out_stop_code)
            *out_stop_code = stop_code;
        return -1;
    }
    log_message("[ERROR]: 0x%08x Boot CELL OS : without stopcode %08x\n", LV0_ERR_BOOT_LV1, spu_status);
    return -1;

restart:
    while (!is_spu_stopped(spu_id, &spu_status))
        ;
    {
        long int_stat2 = get_spu_int_stat(spu_id, 2);
        int rc = g_spu_manager_ptr->handle_class2_interrupt(spu_id, int_stat2, &out0, &out1, &out2);
        if (__builtin_expect(rc == 0, 1))
            return 0;
        log_message("[ERROR]: 0x%08x Boot CELL OS : interrupt handler err [%d]\n", LV0_ERR_INTERNAL, rc);
    }
    return -1;
}

struct sel { u32 idx; unsigned char mask; };
extern const struct sel lv1ldr_out_sys_ac_sd_field = { 0x17, 0x01 };
extern const struct sel lv1ldr_out_sys_ac_misc_field = { 0x13, 0x01 };

struct lv1ldr_key {
    unsigned char b[16];
};

struct lv1ldr_parm {
    unsigned long lv1_addr;
    void *io_page;
    unsigned long io_addr;
    unsigned long iso_flags;
    unsigned long eid0_addr;
    unsigned long eid0_size;
    void *qa_token;
    unsigned long qa_len;
    unsigned char *out;
    unsigned long *misc2;
    unsigned long product_code;
    unsigned long key_len;
    struct lv1ldr_key key;
    char *lv0_base;
    void *report;
    unsigned long reserved[2];
};

extern struct lv1ldr_key g_lv1ldr_key;

extern char lv0_image_start[];
extern char embedded_lv1ldr[], embedded_lv1ldr_size[];
extern char embedded_appldr[], embedded_appldr_size[];
extern char g_lv1ldr_report[];

static inline __attribute__((always_inline)) void store_dcache(unsigned long start, unsigned long size)
{
    for (unsigned long off = 0; off < ((size + 127) & ~127UL); off += 128) {
        __asm__ volatile ("dcbst 0,%0" :: "r"((start & ~127UL) + off) : "memory");
        __asm__ volatile ("sync" ::: "memory");
    }
}

static inline __attribute__((always_inline)) void store_dcache_range(unsigned long start, unsigned long end)
{
    store_dcache(start, end - start);
}

int boot_loader::boot_cell_os(unsigned long lv1_addr, unsigned int *out_stop_code)
{
    unsigned long lv1ldr_addr, appldr_addr, lv1ldr_size, appldr_size;
    unsigned long eid0_addr, eid0_size;
    unsigned long misc2;
    unsigned long spare;
    unsigned char ldr_out[32];
    unsigned char qa_token[128];
    struct lv1ldr_parm parm;
    bool found = false;
    long i;
    int rc;
    void *page;
    unsigned long rev, threshold;
    void *qa_ptr;
    unsigned long qa_len;
    unsigned long io_addr;
    unsigned long product_code;

    lv1ldr_addr = 0;
    lv1ldr_size = 0;
    appldr_addr = 0;
    appldr_size = 0;
    for (i = 0; i != 8; i++) {
        if (!(get_spu_fault_mask() & (0x80UL >> i))) {
            init_spu_mfc_sr1(i);
            set_spu_mfc_rm_boundary(i, 0);
            found = true;
            clear_spu_int_stat_class2_mailbox(i);
        }
    }
    if (!found) {
        log_message("[ERROR]: 0x%08x Boot CELL OS : valid spuid not found\n", LV0_ERR_NO_SPU);
        goto fail;
    }

    {
        page = g_memory_budget_high_ptr->allocate(0x1000, 0x1000);
        if (!page) {
            log_message("[ERROR]: 0x%08x : allocate io address\n", LV0_ERR_NO_SPU);
        fail:
            log_message("[ERROR]: 0x%08x Boot CELL OS. (init_load_elf)\n", LV0_ERR_BOOT_LV1);
            return -1;
        }
        io_addr = get_iommu_context()->allocate_io_address(0, (unsigned long)page, 0x1000, 1, 3, 1, 3, 1);
        rc = g_componnet_manager_ptr->get_component(COMPONENT_EID0, &eid0_addr, &eid0_size);
        if (rc) {
            log_message("[INFO]: eid_0 not found\n");
            eid0_addr = 0;
            eid0_size = 0;
        }
        rev = get_nv_entry2_byte8();
        if ((rev & 0xFF) == 0xFF)
            rev = 2;
        threshold = log_get_threshold();
        lv0_memset(ldr_out, 0, sizeof ldr_out);
        if (!is_qaf_enabled() || (qa_ptr = qa_token, qa_len = sizeof qa_token, read_qa_token(qa_token))) {
            qa_ptr = 0;
            qa_len = 0;
        }

        misc2 = 1;
        lv0_memset(&parm, 0, sizeof parm);
        product_code = (unsigned long)&g_sys_ac_product_code;
        parm.lv1_addr = lv1_addr;
        parm.io_page = page;
        parm.io_addr = io_addr;
        parm.key_len = 16;
        parm.eid0_addr = eid0_addr;
        parm.eid0_size = eid0_size;
        parm.qa_token = qa_ptr;
        parm.qa_len = qa_len;
        parm.out = ldr_out;
        parm.product_code = product_code;
        parm.lv0_base = lv0_image_start;
        parm.report = g_lv1ldr_report;
        parm.iso_flags = (threshold << 62) | (rev << 32) | 1;
        parm.misc2 = &misc2;
        parm.key = g_lv1ldr_key;

        if (get_boot_gos() != 0 && get_boot_gos() != 1) {
            unsigned long mask = (unsigned long)calc_spu_faultbm_hi() >> 32;
            for (unsigned int spu = 0; spu < 8; spu++) {
                if (!(mask & (0x80 >> spu))) {
                    rc = run_lv1ldr(spu, &parm, out_stop_code);
                    if (rc)
                        goto wipe;
                    break;
                }
            }
        } else {
            bool first = true;
            for (long spu = 0; spu != 8; spu++) {
                if (get_spu_fault_mask() & (0x80UL >> spu))
                    continue;
                if (first)
                    first = false;
                else
                    parm.iso_flags |= 2;
                rc = run_lv1ldr(spu, &parm, out_stop_code);
                if (rc)
                    goto wipe;
            }
        }

        if ((ldr_out[lv1ldr_out_sys_ac_sd_field.idx] & lv1ldr_out_sys_ac_sd_field.mask) != 0)
            g_sys_ac_sd = 1;
        g_sys_ac_misc = ldr_out[lv1ldr_out_sys_ac_misc_field.idx];
        g_sys_ac_misc2 = misc2;
        g_componnet_manager_ptr->get_component(COMPONENT_LV1LDR, &lv1ldr_addr, &lv1ldr_size);
        g_componnet_manager_ptr->get_component(COMPONENT_APPLDR, &appldr_addr, &appldr_size);

    wipe:
        lv0_memset(&parm.key, 0, sizeof parm.key);
        lv0_memset(&g_lv1ldr_key, 0, sizeof g_lv1ldr_key);
        if (lv1ldr_addr)
            lv0_memset((void *)lv1ldr_addr, 0, lv1ldr_size);
        lv0_memset(embedded_lv1ldr, 0, (unsigned long)embedded_lv1ldr_size);
        if (get_boot_gos() != 0 && get_boot_gos() != 1) {
            if (appldr_addr)
                lv0_memset((void *)appldr_addr, 0, appldr_size);
            lv0_memset(embedded_appldr, 0, (unsigned long)embedded_appldr_size);
        }
        {
            unsigned long key_start = (unsigned long)g_lv1ldr_key.b;
            unsigned long key_size = (unsigned long)(g_lv1ldr_key.b + sizeof g_lv1ldr_key.b) - key_start;
            store_dcache((unsigned long)&parm.key, key_size);
            store_dcache((unsigned long)&g_lv1ldr_key, key_size);
        }
        if (lv1ldr_addr)
            store_dcache_range(lv1ldr_addr, lv1ldr_addr + lv1ldr_size);
        store_dcache_range((unsigned long)embedded_lv1ldr, (unsigned long)embedded_lv1ldr + (unsigned long)embedded_lv1ldr_size);
        if (get_boot_gos() != 0 && get_boot_gos() != 1) {
            if (appldr_addr)
                store_dcache_range(appldr_addr, appldr_addr + appldr_size);
            store_dcache_range((unsigned long)embedded_appldr, (unsigned long)embedded_appldr + (unsigned long)embedded_appldr_size);
        }
        get_iommu_context()->free_io_address(io_addr);
    }
    __asm__ volatile ("" :: "m"(spare));
    return rc ? -1 : 0;
}

void decrypt_lv1ldr(void *data, unsigned int size)
{
    unsigned long ctr[2];

    ctr[0] = ((const unsigned long *)lv1ldr_aes_iv)[0];
    ctr[1] = ((const unsigned long *)lv1ldr_aes_iv)[1];
    for (unsigned int i = 0; i < size; i += 16) {
        aes_ctr_crypt(lv1ldr_aes_rk, (const unsigned char *)ctr, 16, (unsigned char *)data + i,
                      (unsigned char *)data + i);
        for (int j = 1; j >= 0; j--)
            if (++ctr[j] != 0)
                break;
    }
}
