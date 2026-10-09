#ifndef PT_MMIO_H
#define PT_MMIO_H

#include <spu_mfcio.h>
#include "types.h"

#undef spu_mfcdma64
#define spu_mfcdma64(ls, h, l, sz, tag, cmd) {                  \
        spu_writech(MFC_LSA, (unsigned int)(ls));               \
        spu_writech(MFC_EAH, (unsigned int)(h));                \
        spu_writech(MFC_EAL, (unsigned int)(l));                \
        spu_writech(MFC_Size, (unsigned int)(sz));              \
        spu_writech(MFC_TagID, (unsigned int)(tag));            \
        spu_writech(MFC_Cmd, (unsigned int)(cmd));              \
    }

#define DMA_BUF 0x3E000
#define DMA_TAG 2

void dma_wait(void);
u16 read16(u64 ea);
u32 read32(u64 ea);
u64 read64(u64 ea);
void write32(u64 ea, u32 val);
void write64(u64 ea, u64 val);

class mmio_device {
public:
    mmio_device() : base(0) {}

    u64 base;
};

unsigned long copy_qwords_to_mmio(u64 dest, char *src, unsigned int count);
unsigned long copy_qwords_from_mmio(u64 src, char *dst, unsigned int n);

#endif
