#include "config.h"
#include "memory.h"
#include "platform.h"
#include "spu.h"
#include "svc.h"

long power_state_transit::transit_via_spu(int state, unsigned int *args, int arg3)
{
    long i = 0;
    long spu_id;

    while (1) {
        long fault_mask = get_spu_fault_mask();
        unsigned int idx = i;
        i = i + 1;
        if ((fault_mask & (0x80UL >> idx)) == 0) {
            spu_id = idx;
            goto LABEL_6;
        }
        if (i == 8) {
            spu_id = 255;
LABEL_6:
            {
                const char *src, *src_end;
                char *prob_area;
                long mem_size;
                int *hdr;
                unsigned int hdr1, hdr2;

                init_spu_mfc_sr1(spu_id);
                src = power_transit_spu_image_start;
                src_end = power_transit_spu_image_end;
                lv0_memset(get_spu_ls_addr(spu_id), 0, 0x40000);
                lv0_memmove(get_spu_ls_addr(spu_id) + 1024, src, src_end - src);

                hdr = (int *)get_spu_ls_addr(spu_id);
                hdr1 = *args;
                hdr2 = args[1];
                hdr[0] = state;
                hdr[1] = hdr1;
                hdr[2] = hdr2;
                hdr[3] = arg3;

                set_spu_npc(spu_id, 1024);
                prob_area = (char *)get_spu_problem_area_addr(spu_id);
                mem_size = get_total_memory_size();
                flush_dcache_signal_and_halt(mem_size, (int *)(prob_area + 16412));
            }
            return -99;
        }
    }
}

power_state_transit g_power_state_transit;
power_state_transit *g_power_state_transit_ptr = &g_power_state_transit;
