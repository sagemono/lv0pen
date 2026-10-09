#include "xdr.h"
#include "syscon.h"
#include "util.h"

extern unsigned short xdr_rows_28870[];

#define COPY_ROWS(dst, table, n) do {                                   \
        int off = 0;                                                    \
        n = 0;                                                          \
        while ((unsigned char)(((table)[off] != 0xFFFF) & (n < 64))) {  \
            u64 j;                                                      \
            for (j = 0; j < 36; j++)                                    \
                (dst)[n * 36 + j] = (table)[off + (int)j];              \
            off += j;                                                   \
            n++;                                                        \
        }                                                               \
    } while (0)

void xdr::error(const char *msg)
{
    unsigned char level = m_mcp->basic[6];
    if (level == 0xFF)
        return;
    if (level != 0)
        syscon_write_string(msg);
}

void xdr::info(const char *msg)
{
    unsigned char level = m_mcp->basic[6];
    if (level == 0xFF)
        return;
    if (level > 1)
        syscon_write_string(msg);
}

void xdr::fill_stars(char *buf, int n)
{
    while (n-- > 0)
        *buf++ = '*';
    *buf = 0;
}

long xdr::wake_from_str()
{
    long rc = 0xB000200C;
    info("[INFO]: exercise wake-from-str appendant steps\n");
    info("[INFO]: SCK to 0.3V and CMD to 1.2V\n");
    m_mic.set_mic_timing(0, 513, 2);
    m_mic.set_mic_timing(1, 513, 2);
    info("[INFO]: ask syscon to disable SCK_FET\n");
    if (syscon_set_xdr_rail(1, 1) == 0) {
        info("[INFO]: SCK to 1.2V\n");
        m_mic.set_mic_timing(0, 513, 0);
        rc = 0;
        m_mic.set_mic_timing(1, 513, 0);
    }
    info("[INFO]: complete wake-from-str appendant steps\n");
    return rc;
}

long xdr::enable_scrubbing()
{
    m_mic.write64(0x50A208, 0x60000000, 0);
    return 0;
}

long xdr::print_banner()
{
    info("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    info("XDR Initialization (1.0.0 RTM)");
    info("\n");
    info("Revision: 1673");
    info("\n");
    info("Tool Chain Version: 4.0.2");
    info("\n");
    info("Built on ");
    info("Oct 30 2006");
    info(" at ");
    info("12:40:37");
    info("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}

long xdr::validate_config()
{
    if ((unsigned char)(m_mcp->basic[1] - 1) <= 1)
        info("[INFO]: m_mcp->basic_config.device_capacity: validated\n");
    else {
        error("[ERROR]: m_mcp->basic_config.device_capacity: illegal value\n");
        return 0xB000200B;
    }
    if ((unsigned char)(m_mcp->basic[110] - 1) <= 98)
        info("[INFO]: m_mcp->basic_config.min_window: validated\n");
    else {
        error("[ERROR]: m_mcp->basic_config.min_window: out of range\n");
        return 0xB000200F;
    }
    unsigned short devices = m_mcp->basic[0] * m_mcp->basic[1];
    unsigned short cfg = *(unsigned short *)&m_mcp->basic[24];
    unsigned int c = cfg;
    unsigned short need = (c >> 6) + 1;
    if (devices == need || devices * 8 == need * 9)
        info("[INFO]: m_mcp->mic_mem_cfg_n_H_,  num_dev_per_channel, device_capacity: validated\n");
    else {
        error("[ERROR]: m_mcp->mic_mem_cfg_n_H_,  num_dev_per_channel, device_capacity: conflicted\n");
        return 0xB0002010;
    }
    return 0;
}

long xdr::begin()
{
    long rc = print_banner();
    if (rc)
        return rc;
    return validate_config();
}

long xdr::load_rows(unsigned short *dst)
{
    long n = 0;
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    switch (ratio) {
    case 16:
        COPY_ROWS(dst, xdr_rows_28870, n);
        break;
    case 8:
        COPY_ROWS(dst, xdr_rows_28870, n);
        break;
    case 4:
        COPY_ROWS(dst, xdr_rows_28870, n);
        break;
    }
    return n;
}

unsigned short xdr::bank_map(int i, unsigned int ratio)
{
    unsigned short x16[16] = { 0x0F00, 0x0700, 0x0B00, 0x0300, 0x0D00, 0x0500, 0x0900, 0x0100, 0x0000, 0x0800, 0x0400, 0x0C00, 0x0200, 0x0A00, 0x0600, 0x0E00 };
    unsigned short x8[16] = { 0x0701, 0x0700, 0x0301, 0x0300, 0x0501, 0x0500, 0x0101, 0x0100, 0x0000, 0x0001, 0x0400, 0x0401, 0x0200, 0x0201, 0x0600, 0x0601 };
    unsigned short x4[16] = { 0x0303, 0x0301, 0x0302, 0x0300, 0x0103, 0x0101, 0x0102, 0x0100, 0x0000, 0x0002, 0x0001, 0x0003, 0x0200, 0x0202, 0x0201, 0x0203 };
    return ratio == 16 ? x16[i] : ratio == 8 ? x8[i] : ratio == 4 ? x4[i] : 0;
}

extern unsigned int xdr_t16_a[2][2][16], xdr_t16_b[2][2][16];
extern unsigned int xdr_t8x4_a[2][4][8], xdr_t8x4_b[2][4][8];
extern unsigned int xdr_t8x5_a[2][5][8], xdr_t8x5_b[2][5][8];
extern unsigned int xdr_t4_a[2][9][4], xdr_t4_b[2][9][4];

inline xdr_pair xdr::timing(int a, int b, int c)
{
    xdr_pair r;
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    unsigned char devices = m_mcp->basic[0];
    switch (ratio) {
    case 16:
        if (a < 2 && b < 2 && c < 16) {
            r.x = xdr_t16_b[a][b][c];
            r.y = xdr_t16_a[a][b][c];
            goto done;
        }
        break;
    case 8:
        switch (devices) {
        case 5:
            if (a < 2 && b < 5 && c < 8) {
                r.x = xdr_t8x5_b[a][b][c];
                r.y = xdr_t8x5_a[a][b][c];
                goto done;
            }
            break;
        case 4:
            if (a < 2 && b < 4 && c < 8) {
                r.x = xdr_t8x4_b[a][b][c];
                r.y = xdr_t8x4_a[a][b][c];
                goto done;
            }
            break;
        }
        break;
    case 4:
        if (a < 2 && b < 9 && c < 4) {
            r.x = xdr_t4_b[a][b][c];
            r.y = xdr_t4_a[a][b][c];
            goto done;
        }
        break;
    }
    r.x = 99;
    r.y = 99;
done:
    return r;
}

unsigned short xdr::permute(unsigned short x)
{
    unsigned short map[16] = { 15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0 };
    unsigned short r = 0;
    for (int i = 0; i < 16; i++)
        if ((x >> map[i]) & 1)
            r |= 0x8000 >> i;
    return r;
}

long xdr::store_zero_line_mode(unsigned int v)
{
    unsigned int w = (v & 64) ? 0x04010000 : 0x04000000;
    m_mic.write64(0x50A118, w, 0);
    m_mic.write64(0x50A158, w, 0);
    m_mic.write64(0x50A060, 0, v & ~127);
    m_mic.write64(0x50A068, 0, 0);
    for (int i = 0; i < 16; i++)
        m_mic.write64(0x50A070, 0, 0);
    return 0;
}

unsigned int xdr::row_address(unsigned int ratio, unsigned int v)
{
    unsigned short bank = ratio == 4 ? (v & 6) >> 1 : ratio == 8 ? (v & 2) >> 1 : 0;
    return bank << 13 | (v & 62) << 7 | (v & 1) << 6;
}

#define LO(h) ((unsigned char)(h))
#define HI(h) (((h) & 0xFF00) >> 8)

long xdr::pack(int idx, const unsigned short *rows, xdr_rec *out)
{
    int off = idx * 72;
    const unsigned short *a = rows + idx * 72;
    const unsigned short *p;
    int k;

    p = a + 8;
    for (k = 0; k < 4; k++) {
        const unsigned short *g = p + k * 9;
        out[k].b = LO(g[0]);
        out[k].w0 = LO(g[-1]) << 24 | LO(g[-2]) << 16 | LO(g[-3]) << 8 | LO(g[-4]);
        out[k].w1 = LO(g[-5]) << 24 | LO(g[-6]) << 16 | LO(g[-7]) << 8 | LO(g[-8]);
    }
    const unsigned short (*ga)[9] = (const unsigned short (*)[9])a;
    p = &ga[0][8];
    for (k = 0; k < 4; k++, p += 9) {
        out[4 + k].b = HI(p[0]);
        out[4 + k].w0 = HI(p[-1]) << 24 | HI(p[-2]) << 16 | HI(p[-3]) << 8 | HI(p[-4]);
        out[4 + k].w1 = HI(p[-5]) << 24 | HI(p[-6]) << 16 | HI(p[-7]) << 8 | HI(p[-8]);
    }
    off += 36;
    const unsigned short *b = rows + off;
    p = b + 8;
    for (k = 0; k < 4; k++, p += 9) {
        out[8 + k].b = LO(p[0]);
        out[8 + k].w0 = LO(p[-1]) << 24 | LO(p[-2]) << 16 | LO(p[-3]) << 8 | LO(p[-4]);
        out[8 + k].w1 = LO(p[-5]) << 24 | LO(p[-6]) << 16 | LO(p[-7]) << 8 | LO(p[-8]);
    }
    const unsigned short (*gb)[9] = (const unsigned short (*)[9])b;
    p = &gb[0][8];
    for (k = 0; k < 4; k++, p += 9) {
        out[12 + k].b = HI(p[0]);
        out[12 + k].w0 = HI(p[-1]) << 24 | HI(p[-2]) << 16 | HI(p[-3]) << 8 | HI(p[-4]);
        out[12 + k].w1 = HI(p[-5]) << 24 | HI(p[-6]) << 16 | HI(p[-7]) << 8 | HI(p[-8]);
    }
    return 0;
}

void xdr::write_rows(unsigned int count, const unsigned short *rows)
{
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    m_mic.write64(0x50A118, 0x01400000, 0);
    m_mic.delay_ns(100);
    m_mic.write64(0x50A158, 0x01400000, 0);
    m_mic.delay_ns(100);
    for (unsigned int i = 0; i < count; i++) {
        int k;
        pack(i, rows, m_recs);
        unsigned int addr = row_address(ratio, i * 2);
        addr = m_mcp->f_84 | addr;
        m_mic.write64(0x50A060, 0, addr);
        m_mic.write64(0x50A068, 0, 0x80000000);
        for (k = 0; k < 16; k++)
            m_mic.write64(0x50A070, m_recs[k].b << 24, 0);
        m_mic.write64(0x50A068, 0, 0);
        for (k = 0; k < 16; k++)
            m_mic.write64(0x50A070, m_recs[k].w0, m_recs[k].w1);
        m_mic.write64(0x50A060, 0, addr + 128);
        m_mic.write64(0x50A068, 0, 0x80000000);
        for (k = 0; k < 16; k++)
            m_mic.write64(0x50A070, m_recs[k].b << 24, 0);
        m_mic.write64(0x50A068, 0, 0);
        for (k = 0; k < 16; k++)
            m_mic.write64(0x50A070, m_recs[k].w0, m_recs[k].w1);
    }
}

extern unsigned short xdr_rows[];

long xdr::write_xdr_rows()
{
    write_rows(m_mcp->basic[3], xdr_rows);
    return 0;
}

void xdr::write_tx_tables()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            unsigned short base = (g << 8) + 0x800;
            unsigned int *p4 = tables[4][ch][g];
            unsigned int *p5 = tables[5][ch][g];
            for (unsigned int j = 0; j < 9; j++) {
                unsigned int v = *p4 / m_mcp->basic[4];
                unsigned short reg = base | j * 4 << 2;
                m_mic.set_mic_timing(ch, reg | 2, v);
                m_mic.set_mic_timing(ch, reg | 3, *p5 / m_mcp->basic[4]);
                m_mic.set_mic_timing(ch, reg | 1, (*p4 / m_mcp->basic[4] + *p5 / m_mcp->basic[4]) >> 1);
                p4++;
                p5++;
            }
        }
}

void xdr::write_rx_tables()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            unsigned short base = (g << 8) + 0x800;
            unsigned int *p1 = tables[1][ch][g];
            unsigned int *p2 = tables[2][ch][g];
            for (unsigned int j = 0; j < 9; j++) {
                unsigned int v = *p1 / m_mcp->basic[4];
                unsigned short reg = base | j * 4 << 2;
                m_mic.set_mic_timing(ch, reg | 6, v);
                m_mic.set_mic_timing(ch, reg | 7, *p2 / m_mcp->basic[4]);
                m_mic.set_mic_timing(ch, reg | 5, (*p1 / m_mcp->basic[4] + *p2 / m_mcp->basic[4]) >> 1);
                p1++;
                p2++;
            }
        }
}

xdr::xdr()
{
    m_mcp = 0;
    memset(tables[0], 0, sizeof tables[0]);
    memset(tables[1], 0, sizeof tables[1]);
    memset(tables[2], 0, sizeof tables[2]);
    memset(tables[3], 0, sizeof tables[3]);
    memset(tables[4], 0, sizeof tables[4]);
    memset(tables[5], 0, sizeof tables[5]);
}

u64 be_mmio::read64(unsigned int off)
{
    return ::read64(BE_MMIO_BASE | off);
}

unsigned short be_mmio::get_mic_timing(unsigned int ch, unsigned short a)
{
    unsigned int reg = mic_timing_reg[ch];
    write64(reg, ((a & 0xFFF) | 0x1000) << 16, 0);
    return read64(reg) >> 32;
}

void xdr::dump(int which)
{
    switch (which) {
    case 2:
        syscon_printf("IOW_DUMP");
        goto regs;
    case 0:
        syscon_printf("ITC_DUMP");
        for (int ch = 0; ch < 2; ch++)
            for (int g = 0; g < 4; g++)
                for (int j = 0; j < 9; j++) {
                    syscon_printf("%04x", *(tables[4][ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(tables[5][ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(tables[1][ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(tables[2][ch][g] + j) / m_mcp->basic[4]);
                }
        break;
    case 1:
        if (__builtin_expect(m_mcp->basic[7] != 1, 0)) {
            syscon_printf("PTC is not enabled.");
            break;
        }
        syscon_printf("PTC_DUMP");
    regs:
        for (int ch = 0; ch < 2; ch++)
            for (unsigned short base = 0x800; base != 0xC00; base += 0x100)
                for (unsigned int j = 0; j < 9; j++) {
                    unsigned short reg = base | j << 4;
                    syscon_printf("%04x", m_mic.get_mic_timing(ch, reg | 2));
                    syscon_printf("%04x", m_mic.get_mic_timing(ch, reg | 3));
                    syscon_printf("%04x", m_mic.get_mic_timing(ch, reg | 6));
                    syscon_printf("%04x", m_mic.get_mic_timing(ch, reg | 7));
                }
        break;
    default:
        syscon_printf("DUMP_PREFIX UNKNOWN");
        break;
    }
    syscon_printf("\n");
}

long xdr::check_channel_windows(int ch)
{
    m_win[ch].tx_dq = 0;
    m_win[ch].tx_pin = 0;
    m_win[ch].tx_ui = 100;
    m_win[ch].rx_dq = 0;
    m_win[ch].rx_pin = 0;
    m_win[ch].rx_ui = 100;
    unsigned short n = 9;
    if (m_mcp->basic[0] == 2)
        n = 8;
    else if (m_mcp->basic[0] == 4)
        n = 8;
    for (unsigned int g = 0; g < 4; g++) {
        unsigned short base = (g << 8) + 0x800;
        for (unsigned short j = 0; j < n; j++) {
            unsigned short reg = base | j << 4;
            unsigned short a = m_mic.get_mic_timing(ch, reg | 2);
            unsigned short b = m_mic.get_mic_timing(ch, reg | 3);
            unsigned short c = m_mic.get_mic_timing(ch, reg | 6);
            unsigned short d = m_mic.get_mic_timing(ch, reg | 7);
            unsigned short tx = a <= b ? (b - a) * 100 / 128 : 0;
            unsigned short rx = c <= d ? (d - c) * 100 / 128 : 0;
            if (m_win[ch].tx_ui > tx) {
                m_win[ch].tx_dq = g;
                m_win[ch].tx_pin = j;
                m_win[ch].tx_ui = tx;
            }
            if (m_win[ch].rx_ui > rx) {
                m_win[ch].rx_dq = g;
                m_win[ch].rx_pin = j;
                m_win[ch].rx_ui = rx;
            }
            if ((b - a) * 100 < m_mcp->basic[110] * 128)
                return 0xB0002006;
            if ((d - c) * 100 < m_mcp->basic[110] * 128)
                return 0xB0002007;
        }
    }
    return 0;
}

void xdr::add_rx_readings()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            unsigned short base = (g << 8) + 0x800;
            unsigned int *p1 = tables[1][ch][g];
            unsigned int *p2 = tables[2][ch][g];
            unsigned int *p0 = tables[0][ch][g];
            for (unsigned int j = 0; j < 9; j++) {
                *p1 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 6);
                *p2 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 7);
                *p0 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 5);
                p1++;
                p2++;
                p0++;
            }
        }
}

void xdr::add_tx_readings()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            unsigned short base = (g << 8) + 0x800;
            unsigned int *p4 = tables[4][ch][g];
            unsigned int *p5 = tables[5][ch][g];
            unsigned int *p3 = tables[3][ch][g];
            for (unsigned int j = 0; j < 9; j++) {
                *p4 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 2);
                *p5 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 3);
                *p3 += m_mic.get_mic_timing(ch, base | j * 4 << 2 | 1);
                p4++;
                p5++;
                p3++;
            }
        }
}

bool be_mmio::poll64(unsigned int off, unsigned int mask_hi, unsigned int mask_lo,
                     unsigned int val_hi, unsigned int val_lo, unsigned int tries)
{
    u64 mask = (u64)mask_hi << 32 | mask_lo;
    u64 val = ((u64)val_hi << 32 | val_lo) & mask;
    unsigned int i = 0;
    for (;;) {
        i++;
        if ((read64(off) & mask) == val)
            return 1;
        delay_us(1);
        if (i == tries)
            return 0;
    }
}

void xdr::fill_hex2(char *out, unsigned int v)
{
    char hex[] = "0123456789ABCDEF";

    out[0] = hex[(v & 0xF0) >> 4];
    out[1] = hex[v & 15];
}

void xdr::fill_dec2(char *out, unsigned int v)
{
    char dec[] = "0123456789";

    if (v > 99) {
        out[0] = 'x';
        out[1] = 'x';
    } else {
        out[0] = dec[v / 10];
        out[1] = dec[v % 10];
    }
}

long xdr::print_window_summary()
{
    char buf[4];

    fill_stars(buf, 2);
    info("[INFO]: XIO CH0 Summary: \n");
    info("[INFO]: Worst TX UI: 0.");
    fill_dec2(buf, m_win[0].tx_ui);
    info(buf);
    info(" (DQ: ");
    fill_hex2(buf, m_win[0].tx_dq);
    info(buf);
    info(" PIN: ");
    fill_hex2(buf, m_win[0].tx_pin);
    info(buf);
    info(")\n");
    info("[INFO]: Worst RX UI: 0.");
    fill_dec2(buf, m_win[0].rx_ui);
    info(buf);
    info(" (DQ: ");
    fill_hex2(buf, m_win[0].rx_dq);
    info(buf);
    info(" PIN: ");
    fill_hex2(buf, m_win[0].rx_pin);
    info(buf);
    info(")\n");
    if (!m_mcp->xio_ch0_on)
        info("[ERROR]: => dynamically disabled due to poor UI value.\n");
    info("[INFO]: XIO CH1 Summary: \n");
    info("[INFO]: Worst TX UI: 0.");
    fill_dec2(buf, m_win[1].tx_ui);
    info(buf);
    info(" (DQ: ");
    fill_hex2(buf, m_win[1].tx_dq);
    info(buf);
    info(" PIN: ");
    fill_hex2(buf, m_win[1].tx_pin);
    info(buf);
    info(")\n");
    info("[INFO]: Worst RX UI: 0.");
    fill_dec2(buf, m_win[1].rx_ui);
    info(buf);
    info(" (DQ: ");
    fill_hex2(buf, m_win[1].rx_dq);
    info(buf);
    info(" PIN: ");
    fill_hex2(buf, m_win[1].rx_pin);
    info(buf);
    info(")\n");
    if (!m_mcp->xio_ch1_on)
        info("[ERROR]: => dynamically disabled due to poor UI value.\n");
    return 0;
}

long xdr::check_windows()
{
    long rc;

    if (m_mcp->xio_ch0_on == 1) {
        rc = check_channel_windows(0);
        if (rc != 0)
            return rc;
    }
    if (m_mcp->xio_ch1_on == 1) {
        rc = check_channel_windows(1);
        if (rc != 0)
            return rc;
    }
    return print_window_summary();
}

long xdr::load_rows_to_devices()
{
    load_rows(xdr_rows);
    m_mic.set_mic_timing_b(0, 0x140002, m_mcp->f_ac | 0x10);
    m_mic.set_mic_timing_b(1, 0x140002, m_mcp->f_ac | 0x10);
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    unsigned int na = m_mcp->basic[0];
    unsigned int rows = m_mcp->f_b8;
    unsigned int nb = m_mcp->basic[2];
    unsigned int cmds = m_mcp->f_b4;
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int a = 0; a < na; a++) {
            unsigned short row = (i & 62) * cmds;
            for (unsigned int b = 0; b < nb; b++) {
                unsigned short bm = bank_map(b, ratio);
                int col = (bm & 0xF00) >> 8;
                unsigned short r = row + (bm & 0xFF) * 2 + (i & 1);
                xdr_pair t0 = timing(0, a, col);
                xdr_pair t1 = timing(1, a, col);
                unsigned int v0 = xdr_rows[r * 36 + t0.x * 9 + t0.y];
                unsigned int v1 = xdr_rows[r * 36 + t1.x * 9 + t1.y];
                unsigned short p0 = permute(v0);
                unsigned short p1 = permute(v1);
                unsigned int reg = 0x40004 | a << 8;
                m_mic.set_mic_timing_b(0, reg, (p0 & 0xFF00) >> 8);
                m_mic.set_mic_timing_b(1, reg, (p1 & 0xFF00) >> 8);
                m_mic.set_mic_timing_b(0, reg, p0 & 0xFF);
                m_mic.set_mic_timing_b(1, reg, p1 & 0xFF);
            }
        }
        m_mic.read64(0x50A110);
        m_mic.read64(0x50A150);
        unsigned short row = (i & 62) * cmds;
        for (unsigned int k = 0; k < cmds; k++) {
            unsigned int addr = row_address(ratio, (unsigned short)(row + k * 2 + (i & 1))) | m_mcp->f_84;
            store_zero_line_mode(addr);
            store_zero_line_mode(addr + 128);
        }
    }
    m_mic.poll64(0x50A110, 0x400, 0, 0x400, 0, 10000);
    m_mic.poll64(0x50A150, 0x400, 0, 0x400, 0, 10000);
    m_mic.set_mic_timing_b(0, 0x140002, m_mcp->f_ac);
    m_mic.set_mic_timing_b(1, 0x140002, m_mcp->f_ac);
    m_mic.read64(0x50A110);
    m_mic.read64(0x50A150);
    return 0;
}

long xdr::initialize()
{
    long rc;

    if (m_mcp->str == 1) {
        rc = wake_from_str();
        if (rc != 0)
            goto fail;
    }
    rc = begin();
    if (rc != 0)
        goto fail;
    rc = write_mic_config();
    if (rc != 0)
        goto fail;
    rc = init_xio_link();
    if (rc != 0)
        goto fail;
    rc = init_devices();
    if (rc != 0)
        goto fail;
    rc = load_rows_to_devices();
    if (rc != 0)
        goto fail;
    rc = write_xdr_rows();
    if (rc != 0)
        goto fail;
    error("\n[INFO]: XIO Calibration start \n");
    error("[INFO]: Rx Calibration being processed...\n");
    post_code_01();
    post_code_02();
    rc = calibrate_rx();
    if (rc != 0)
        goto fail;
    error("[INFO]: Tx Calibration being processed...\n");
    post_code_03();
    post_code_05();
    rc = calibrate_tx();
    if (rc != 0)
        goto fail;
    error("[INFO]: Do post process...\n");
    rc = check_windows();
    if (rc != 0)
        goto fail;
    if (m_mcp->basic[7] == 1) {
        error("[INFO]: Enable PTCal...\n");
        rc = read_ptcal_start();
        if (rc != 0)
            goto fail;
        rc = write_ptcal_settings();
        if (rc != 0)
            goto fail;
        rc = enable_ptcal();
        if (rc != 0)
            goto fail;
    }
    if (m_mcp->basic[107] == 1) {
        error("[INFO]: Enable Scrubbing...\n");
        rc = enable_scrubbing();
        if (rc != 0)
            goto fail;
    }
    post_code_07(m_mcp->xio_ch0_on, m_mcp->xio_ch1_on);
    if (m_mcp->str == 1)
        error("[INFO]: Wake from STR\n");
    error("\n[INFO]: >>> XDR Init: OK <<<\n");
    return rc;

fail:
    error("[ERROR]: >>> XDR Init: NG <<<\n");
    post_code_06();
    return rc;
}
