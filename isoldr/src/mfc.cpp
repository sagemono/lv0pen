#include "dma_stream.h"
#include "mailbox.h"
#include "mmio.h"

mfc_base::mfc_base()
{
}

unsigned int mbox_in_count(void)
{
    return spu_readchcnt(SPU_RdInMbox);
}

#define WRCH(ch, v)     __asm__ __volatile__ ("wrch $ch" #ch ",%0" : : "r" (v))
#define RDCH(ch, v)     __asm__ __volatile__ ("rdch %0,$ch" #ch : "=r" (v))
#define RCHCNT(ch, v)   __asm__ __volatile__ ("rchcnt %0,$ch" #ch : "=r" (v))

long mfc_issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
               unsigned int rid, unsigned int cmd)
{
    WRCH(16, ls);
    WRCH(17, (unsigned int)(ea >> 32));
    WRCH(18, (unsigned int)ea);
    WRCH(19, size);
    WRCH(20, tag);
    WRCH(21, rid << 16 | cmd);
    return 0;
}

bool mfc_tag_done(unsigned int tag)
{
    unsigned int n, s, mask;

    WRCH(23, 0);
    do
        RCHCNT(23, n);
    while (n != 1);
    RDCH(24, s);
    mask = 1 << tag;
    WRCH(22, mask);
    WRCH(23, 0);
    RDCH(24, s);
    return s == mask;
}

long mbox_out_intr_write(unsigned int v)
{
    unsigned int n;
    long rc;

    RCHCNT(30, n);
    rc = -1;
    if (n) {
        WRCH(30, v);
        rc = 0;
    }
    return rc;
}

long mbox_out_write(unsigned int v)
{
    unsigned int n;
    long rc;

    RCHCNT(28, n);
    rc = -1;
    if (n) {
        WRCH(28, v);
        rc = 0;
    }
    return rc;
}

long mbox_in_read(unsigned int *v)
{
    unsigned int n, w;
    long rc;

    RCHCNT(29, n);
    rc = -2;
    if (n) {
        RDCH(29, w);
        rc = 0;
        *v = w;
    }
    return rc;
}
