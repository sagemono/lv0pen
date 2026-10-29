#include "syscon.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "loader.h"
#include "mmio.h"
#include "xdr.h"
#include "../../../lv1ldr/include/elf.h"

#define BE (0x20000000000ULL)

bool is_spu_enabled(u64 spu);

struct spu_memtest_param {
    u64 area_address;
    unsigned int area_size;
    unsigned int dma_size;
    unsigned int is_infinite;
    unsigned int loop_times;
    unsigned int run_on_error;
    unsigned int read_retry;
    unsigned int spu_id;
    unsigned int pad;
    int error;
    unsigned int flag;
};

extern unsigned char spu_memtest[];

void spu_run(u64 spe)
{
    write32(BE + 0x4401C + (spe << 19), 1);
}

u32 spu_read_status(u64 spe)
{
    return read32(BE + 0x44024 + (spe << 19));
}

u64 get_memory_size(void)
{
    u64 mb0 = ((read64(BE + 0x50A0C8) >> 54) + 1) * 32;
    u64 mb1 = ((read64(BE + 0x50A188) >> 54) + 1) * 32;
    return (mb0 + mb1) << 20;
}

void spu_write_npc(u64 spe, u32 npc)
{
    write32(BE + 0x44034 + (spe << 19), npc);
}

CXX_DROPPED u64 spe_problem_area(u64 spe)
{
    return BE + 0x40000 + (spe << 19);
}

void spe_priv1_write64(u64 spe, u64 v)
{
    write64(BE + 0x400000 + (spe << 13), v);
}

bool memory_diag(u64 size, unsigned int unused1, u64 unused2, unsigned char mode)
{
    spu_memtest_param p __attribute__((aligned(128)));
    u64 spus[8];
    spu_memtest_param st;
    char text[976];
    int n = 0;

    memset(&p, 0, sizeof(p));
    p.dma_size = 0x4000;
    p.loop_times = 1;
    p.run_on_error = mode;
    spus[0] = -1; spus[1] = -1; spus[2] = -1; spus[3] = -1;
    spus[4] = -1; spus[5] = -1; spus[6] = -1; spus[7] = -1;
    for (u64 spe = 0; spe < 8; spe++)
        if (is_spu_enabled(spe) && !(spu_read_status(spe) & 1))
            spus[n++] = spe;
    if (n == 0) {
        log_message("[ERROR]: 0x%08x available spu not found\n", 0xB0000009);
        return false;
invalid:
        log_message("[ERROR]: 0x%08x invalid spu_memtest\n", 0xB0000004);
        return false;
    }
    unsigned int chunk = (size / n + 0x7FFF) & ~0x7FFF;
    for (int i = 0; i < n; i++) {
        u64 spe = spus[i];
        spe_priv1_write64(spe, 0x21);
        Elf32_Ehdr *eh = (Elf32_Ehdr *)spu_memtest;
        if (*(unsigned int *)eh->e_ident != 0x7F454C46)
            goto invalid;
        if (eh->e_type != 2)
            goto invalid;
        u64 ls = spe << 19;
        for (unsigned int k = 0; k < eh->e_phnum; k++) {
            unsigned char *elf = spu_memtest;
            Elf32_Phdr *ph = (Elf32_Phdr *)((si_to_uint(si_from_uint(k)) << 5) + ((unsigned int)elf + eh->e_phoff));
            if (ph->p_type != 1)
                continue;
            unsigned int left = ph->p_filesz;
            unsigned int off = 0;
            while (left) {
                unsigned int len = left > 0x2000 ? 0x2000 : left;
                u64 dst = BE + ph->p_vaddr;
                copy_qwords_to_mmio(dst + ((long long)ls + off),
                                    (char *)(off + ((unsigned int)elf + ph->p_offset)), (len + 15) & ~15);
                left -= len;
                off += len;
            }
        }
        spu_write_npc(spe, 0x400);
        u64 rest = size - p.area_address;
        p.area_size = rest > chunk ? chunk : rest;
        p.spu_id = spe;
        log_message("areaAddress 0x%08llx, areaSize 0x%08x, dmaSize 0x%08x, isInfinite %d, loopTimes %d, runOnError %d, readRetry %d, spuID %d, flag 0x%08x\n",
                    p.area_address, p.area_size, p.dma_size, p.is_infinite, p.loop_times,
                    p.run_on_error, p.read_retry, p.spu_id, p.flag);
        copy_qwords_to_mmio(BE + ls, (char *)&p, sizeof(p));
        p.area_address += p.area_size;
    }
    for (int i = 0; i < n; i++)
        spu_run(spus[i]);
    int errors = 0;
    int stopped;
    do {
        stopped = 0;
        for (int i = 0; i < n; i++) {
            u64 spe = spus[i];
            if (spe != (u64)-1) {
            unsigned int status = spu_read_status(spe);
            u64 ls = BE + (spe << 19);
            copy_qwords_from_mmio(ls, (char *)&st, sizeof(st));
            if (st.flag) {
                copy_qwords_from_mmio(ls + 0x30, text, sizeof(text));
                text[975] = 0;
                log_message("(%lld) %s", spe, text);
                st.flag = 0;
                copy_qwords_to_mmio(ls, (char *)&st, sizeof(st));
            }
            if (status & 2) {
                log_message("(%lld) stop. error=%d\n", spe, st.error);
                spus[i] = -1;
                if (st.error)
                    errors++;
            }
            } else
                stopped++;
        }
    } while (stopped != n);
    return errors == 0;
}
