#include <spu_intrinsics.h>
#include "aim.h"
#include "util.h"

#define TAG_LOG         7
#define LS_BOUNCE       0x3E000

static dma_log g_log;
dma_log *g_log_ptr = &g_log;

dma_log *get_log()
{
    return g_log_ptr;
}

dma_log::dma_log()
{
    m_ea = 0;
    m_size = 0;
    m_pos = 0;
}

dma_log::~dma_log()
{
}

unsigned int dma_log::puts(const char *s)
{
    unsigned char save[16] __attribute__((aligned(16)));
    unsigned int n, i, off;

    if (m_pos >= m_size)
        return 0;
    n = strlen(s) + 1;
    if (m_pos + n > m_size)
        n = m_size - m_pos;
    memcpy(save, (void *)LS_BOUNCE, 16);
    off = m_pos & 15;
    for (i = 0; i != n; i++) {
        unsigned int ls = LS_BOUNCE + ((off + i) & 15);

        *(char *)ls = s[i];
        if (m_dma.issue(ls, m_ea + m_pos + i, 1, TAG_LOG, 0, MFC_PUT))
            return 0;
        m_dma.wait(TAG_LOG);
    }
    m_pos += n - 1;
    memcpy((void *)LS_BOUNCE, save, 16);
    return n - 1;
}

void dma_log::reset()
{
    m_pos = 0;
    puts("");
    m_pos = 0;
}

void dma_log::set_buffer(const quad_u64 &ea, const quad_uint &size)
{
    m_ea = ea;
    m_size = size;
    reset();
}

void dma_log::init(const quad_u64 &ea, const quad_uint &size)
{
    u64 buf_ea __attribute__((aligned(16))) = 0;
    u64 buf_size __attribute__((aligned(16))) = 0;
    unsigned int n = 16;
    unsigned char save[n];

    if (ea == 0 || size < 16)
        return;
    memcpy(save, (void *)LS_BOUNCE, 16);
    if (m_dma.issue(LS_BOUNCE, ea, 16, TAG_LOG, 0, MFC_GET))
        return;
    m_dma.wait(TAG_LOG);
    memcpy(&buf_ea, (void *)LS_BOUNCE, 8);
    memcpy(&buf_size, (void *)(LS_BOUNCE + 8), 8);
    memcpy((void *)LS_BOUNCE, save, 16);
    set_buffer(buf_ea, buf_size);
}

u64 dma_log::top_power(u64 v)
{
    u64 d = 1;
    int i;

    for (i = 0; i < 19; i++) {
        if (v <= d)
            return d > 9 ? d / 10 : d;
        d *= 10;
    }
    return d;
}

void dma_log::put_digit(unsigned int d)
{
    switch (d) {
    case 0: puts("0"); break;
    case 1: puts("1"); break;
    case 2: puts("2"); break;
    case 3: puts("3"); break;
    case 4: puts("4"); break;
    case 5: puts("5"); break;
    case 6: puts("6"); break;
    case 7: puts("7"); break;
    case 8: puts("8"); break;
    case 9: puts("9"); break;
    case 10: puts("a"); break;
    case 11: puts("b"); break;
    case 12: puts("c"); break;
    case 13: puts("d"); break;
    case 14: puts("e"); break;
    case 15: puts("f"); break;
    default: puts("?"); break;
    }
}

int dma_log::put_number(const void *p, int size, unsigned int base, int neg)
{
    const unsigned char *b = (const unsigned char *)p;
    int n;

    if (base != 10 && base != 16)
        return -1;
    n = 0;
    if (neg) {
        puts("-");
        n = 1;
    }
    if (base == 16) {
        int lead = 1;
        int i;

        puts("0");
        puts("x");
        for (i = 0; i < size; i++) {
            unsigned int c = b[i];
            unsigned int hi = c >> 4;
            unsigned int lo = c & 15;

            if (!lead || hi != 0) {
                put_digit(hi);
                put_digit(lo);
                lead = 0;
            } else if (!lead || lo != 0 || i == size - 1) {
                put_digit(lo);
                lead = 0;
            }
        }
        n += i;
        n += 2;
    } else if (base == 10) {
        u64 v, d;

        switch (size) {
        case 1:
            v = *(const u8 *)p;
            break;
        case 2:
            v = *(const u16 *)p;
            break;
        case 4:
            v = *(const u32 *)p;
            break;
        case 8:
            v = *(const u64 *)p;
            break;
        default:
            v = 0;
            break;
        }
        for (d = top_power(v); d != 0; d /= 10) {
            n++;
            put_digit(v / d);
            v %= d;
        }
    }
    return n;
}

int dma_log::put(s64 v, unsigned int base)
{
    bool neg = false;

    if (base == 10 && v < 0) {
        v = -v;
        neg = true;
    }
    return put_number(&v, 8, base, neg);
}

int dma_log::put(u64 v, unsigned int base)
{
    return put_number(&v, 8, base, false);
}

int dma_log::put(unsigned int v, unsigned int base)
{
    return put_number(&v, 4, base, false);
}

int dma_log::put(int v, unsigned int base)
{
    bool neg = false;

    if (base == 10) {
        if (v < 0) {
            v = -v;
            neg = true;
        }
    }
    return put_number(&v, 4, base, neg);
}

int dma_log::put(u8 v, unsigned int base)
{
    return put_number(&v, 1, base, false);
}
