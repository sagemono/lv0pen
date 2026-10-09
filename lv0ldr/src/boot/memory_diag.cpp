#include "syscon.h"
#include "errors.h"
#include "log.h"
#include "util.h"
#include "loader.h"
#include "dma_stream.h"
#include "xdr.h"
#include "aes.h"

unsigned int g_read_buffers[4] = { 0x3E000, 0x3E800, 0x3F000, 0x3F800 };
unsigned int g_write_buffer = 0x3E000;

#define BE (0x20000000000ULL)

class memory_tester {
public:
    memory_tester();
    long comp_memory(u64 ea, u64 size, const qword *expect);
    long fill(u64 ea, u64 size);

    tagged_dma_buffer buf0, buf1, buf2, buf3, buf4;
    dma_buffer *bufs[5];
    dma_queue queue;
};

memory_tester::memory_tester()
    : buf0(0), buf1(1), buf2(2), buf3(3), buf4(4)
{
    tagged_dma_buffer *readers[4] = { &buf1, &buf2, &buf3, &buf4 };
    bufs[0] = &buf0;
    buf0.set_buffer(g_write_buffer, 0x2000);
    for (int i = 0; i < 4; i++) {
        bufs[i + 1] = readers[i];
        readers[i]->set_buffer(g_read_buffers[i], 0x800);
    }
}

long memory_tester::comp_memory(u64 ea, u64 size, const qword *expect)
{
    FUNCTION_NAME("comp_memory");
    dma_buffer *b;
    unsigned char last = 0;

    queue.open(ea, size, ea, size);
    for (int i = 0; i < 4; i++)
        queue.enqueue_read(bufs[i + 1]);
    for (u64 addr = ea; addr < ea + size; addr += 0x2000) {
        const qword *e = expect;
        for (int n = 0; n < 4; n++) {
            long rc = queue.dequeue_read(&b);
            if (rc == 1)
                last = 1;
            else if (rc) {
                log_message("[ERROR]: %s dequeue_read %d\n", function_name, rc);
                return -2;
            }
            const qword *a = (const qword *)b->ls;
            for (u64 off = 0; off < b->size; off += 16, e++, a++) {
                if (si_to_uint(si_gbb(si_ceqb(*e, *a))) != 0xFFFF) {
                    qword x = *(const volatile qword *)a, y = *(const volatile qword *)e;
                    log_message("[cmp fail address 0x%08llx, actual(%016llx_%016llx), expect(%016llx_%016llx)]\n",
                                addr + off + 0x2000,
                                si_to_ullong(x), si_to_ullong(si_rotqbyi(x, 8)),
                                si_to_ullong(y), si_to_ullong(si_rotqbyi(y, 8)));
                    return -1;
                }
            }
            if (last == 1)
                break;
            rc = queue.enqueue_read(b);
            if (rc) {
                log_message("[ERROR]: %s enqueue_write %d\n", function_name, rc);
                return -2;
            }
            last = 0;
        }
    }
    queue.close();
    return 0;
}

u64 get_memory_size(void)
{
    u64 mb0 = ((read64(BE + 0x50A0C8) >> 54) + 1) * 32;
    u64 mb1 = ((read64(BE + 0x50A188) >> 54) + 1) * 32;
    return (mb0 + mb1) << 20;
}

long memory_tester::fill(u64 ea, u64 size)
{
    qword tag = si_from_uint(0);
    dma_buffer *b = bufs[0];
    unsigned int ls = b->ls;
    unsigned int n = b->size;
    for (u64 addr = ea; addr < ea + size; addr += 0x2000) {
        spu_writech(MFC_LSA, ls);
        spu_writech(MFC_EAH, addr >> 32);
        spu_writech(MFC_EAL, addr);
        spu_writech(MFC_Size, n);
        si_wrch(MFC_TagID, tag);
        spu_writech(MFC_Cmd, MFC_PUT_CMD);
    }
    mfc_write_tag_update_immediate();
    while (spu_readchcnt(MFC_WrTagUpdate) != 1)
        ;
    spu_readch(MFC_RdTagStat);
    mfc_write_tag_mask(1 << 0);
    mfc_write_tag_update_all();
    mfc_read_tag_status();
    return 0;
}

bool memory_diag(u64 size, unsigned int unused1, u64 unused2, unsigned char mode)
{
    qword expect[512];
    memory_tester t;
    qword *buf = (qword *)g_write_buffer;
    char line[] = "=====================================\n";

    log_message("%s", line);
    vec_uchar16 key = { 0 };
    vec_uchar16 iv = { 0 };
    log_message("[begin: cmp random data]\n");
    for (u64 addr = 0; addr < size; addr += 0x2000000) {
        memset(buf, 0, 0x2000);
        aes_cbc_encrypt((vec_uchar16 *)buf, (vec_uchar16 *)buf, 0x2000, (unsigned char *)&key, 128, &iv);
        t.fill(addr, 0x2000000);
        log_message("w");
    }
    log_message("\n");
    int fails = 0;
    for (u64 addr = 0; addr < size; addr += 0x2000000) {
        memset(expect, 0, 0x2000);
        aes_cbc_encrypt((vec_uchar16 *)expect, (vec_uchar16 *)expect, 0x2000, (unsigned char *)&key, 128, &iv);
        long rc = t.comp_memory(addr, 0x2000000, expect);
        if (rc == -1) {
            fails++;
            if (!mode)
                return false;
        } else if (rc)
            return false;
        log_message("r");
    }
    log_message("\n[end: cmp random data]\n");
    unsigned char patterns[8] = { 0x00, 0xFF, 0x55, 0xAA, 0xCC, 0x33, 0x99, 0x66 };
    for (u64 i = 0; i < 8; i++) {
        log_message("%s", line);
        unsigned char p = patterns[i];
        log_message("[begin: cmp fix data(%02x)]\n", p);
        for (u64 addr = 0; addr < size; addr += 0x2000000) {
            for (u64 j = 0; j < 0x2000; j += 0x100)
                memset(&buf[j / 16], patterns[(i + j / 0x100) % 8], 0x100);
            t.fill(addr, 0x2000000);
            log_message("w");
        }
        log_message("\n");
        for (u64 addr = 0; addr < size; addr += 0x2000000) {
            for (u64 j = 0; j < 0x2000; j += 0x100)
                memset(&expect[j / 16], patterns[(i + j / 0x100) % 8], 0x100);
            long rc = t.comp_memory(addr, 0x2000000, expect);
            if (rc == -1) {
                fails++;
                if (!mode)
                    return false;
            } else if (rc)
                return false;
            log_message("r");
        }
        log_message("\n[end: cmp fix data(%02x)]\n", p);
    }
    return fails == 0;
}
