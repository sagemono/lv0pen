#include "loader.h"
#include "mailbox.h"
#include "util.h"

extern unsigned int g_exit_status;
extern "C" void exit_stop(void);
extern "C" void scrub_and_jump(u64 a0, u64 a1, u64 a2, u64 a3,
                               spu_reg r0, spu_reg r1, spu_reg r2, spu_reg r3,
                               spu_reg r4, spu_reg r5, spu_reg r6, spu_reg r7,
                               spu_reg r8, spu_reg r9, spu_reg r10, spu_reg r11,
                               spu_reg r12, spu_reg r13, spu_reg r14, spu_reg r15,
                               void (*entry)(void), u64 a4);

int main(void)
{
    u64 a0, a1, a2, a3;
    spu_reg r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
    void (*entry)(void);
    u64 a4;
    long rc;

    a0 = 0;
    a1 = 0;
    a2 = 0;
    a3 = 0;
    a4 = 0;
    memset(&r0, 0, sizeof(r0));
    memset(&r1, 0, sizeof(r1));
    memset(&r2, 0, sizeof(r2));
    memset(&r3, 0, sizeof(r3));
    memset(&r4, 0, sizeof(r4));
    memset(&r5, 0, sizeof(r5));
    memset(&r6, 0, sizeof(r6));
    memset(&r7, 0, sizeof(r7));
    memset(&r8, 0, sizeof(r8));
    memset(&r9, 0, sizeof(r9));
    memset(&r10, 0, sizeof(r10));
    memset(&r11, 0, sizeof(r11));
    memset(&r12, 0, sizeof(r12));
    memset(&r13, 0, sizeof(r13));
    memset(&r14, 0, sizeof(r14));
    memset(&r15, 0, sizeof(r15));
    mbox_in_drain();
    rc = g_loader.run(&a0, &a1, &a2, &a3, &r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7,
                      &r8, &r9, &r10, &r11, &r12, &r13, &r14, &r15, &entry, &a4);
    if (!rc) {
        scrub_and_jump(a0, a1, a2, a3, r0, r1, r2, r3, r4, r5, r6, r7,
                       r8, r9, r10, r11, r12, r13, r14, r15, entry, a4);
        return 0;
    }
    memset(0, 0, 0x400);
    g_exit_status = rc;
    scrub_and_jump(a0, a1, a2, a3, r0, r1, r2, r3, r4, r5, r6, r7,
                   r8, r9, r10, r11, r12, r13, r14, r15, exit_stop, a4);
    for (;;)
        ;
}
