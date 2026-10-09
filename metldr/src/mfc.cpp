#include "dma_stream.h"
#include "mailbox.h"
#include "mmio.h"

mfc_base::mfc_base()
{
}

unsigned int mbox_out_space(void)
{
    return spu_readchcnt(SPU_WrOutMbox);
}

long mfc_issue(unsigned int ls, u64 ea, unsigned int size, unsigned int tag,
               unsigned int rid, unsigned int cmd)
{
    spu_mfcdma64(ls, mfc_ea2h(ea), mfc_ea2l(ea), size, tag, MFC_CMD_WORD(0, rid, cmd));
    return 0;
}

bool mfc_tag_done(unsigned int tag)
{
    unsigned int n;

    mfc_write_tag_update_immediate();
    do
        __asm__ __volatile__ ("rchcnt %0,$ch23" : "=r" (n));
    while (n != 1);
    spu_readch(MFC_RdTagStat);
    qword mask = si_from_uint(1 << tag);
    si_wrch(MFC_WrTagMask, mask);
    mfc_write_tag_update_immediate();
    return mfc_read_tag_status() == si_to_uint(mask);
}

long mbox_out_write(unsigned int v)
{
    if (spu_readchcnt(SPU_WrOutMbox) == 0)
        return -1;
    spu_writech(SPU_WrOutMbox, v);
    return 0;
}

long mbox_in_read(unsigned int *v)
{
    unsigned int w;

    if (spu_readchcnt(SPU_RdInMbox) == 0)
        return -2;
    __asm__ __volatile__ ("rdch %0,$ch29" : "=r" (w));
    *v = w;
    return 0;
}

long mbox_out_intr_write(unsigned int v)
{
    if (spu_readchcnt(SPU_WrOutIntrMbox) == 0)
        return -1;
    spu_writech(SPU_WrOutIntrMbox, v);
    return 0;
}

long mbox_in_read64(u64 *v)
{
    unsigned int w;
    u64 hi;
    long rc;

    while ((rc = mbox_in_read(&w)) != 0)
        ;
    hi = (u64)w << 32;
    while ((rc = mbox_in_read(&w)) != 0)
        ;
    *v = hi | w;
    return rc;
}
