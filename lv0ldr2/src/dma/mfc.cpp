#include "dma_stream.h"
#include "mmio.h"

mfc_base::mfc_base()
{
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
