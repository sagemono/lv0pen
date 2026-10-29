#include "xdr.h"
#include "syscon.h"
#include "util.h"

extern unsigned short xdr_rows_28870[];

#define COPY_ROWS(dst, table, n) do {                                   \
        int off = 0;                                                    \
        n = 0;                                                          \
        while ((unsigned char)(((table)[off] != 0xFFFF) & (n < 64))) {  \
            int j;                                                      \
            for (j = 0; j < 36; j++)                                    \
                (dst)[n * 36 + j] = (table)[off + (int)j];              \
            off += j;                                                   \
            n++;                                                        \
        }                                                               \
    } while (0)

void xdr::info(const char *msg)
{
    unsigned char level = m_mcp->basic[6];
    if (level == 0xFF)
        return;
    if (level > 1)
        syscon_write_string(msg);
}

void xdr::error(const char *msg)
{
    unsigned char level = m_mcp->basic[6];
    if (level == 0xFF)
        return;
    if (level != 0)
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

long xdr::validate_config()
{
    switch (m_mcp->basic[1]) {
    case 1:
    case 2:
    case 4:
        info("[INFO]: m_mcp->basic_config.device_capacity: validated\n");
        break;
    default:
        error("[ERROR]: m_mcp->basic_config.device_capacity: illegal value\n");
        return 0xB000200B;
    }
    if ((unsigned char)(m_mcp->basic[110] - 1) <= 98)
        info("[INFO]: m_mcp->basic_config.min_window: validated\n");
    else {
        error("[ERROR]: m_mcp->basic_config.min_window: out of range\n");
        return 0xB000200F;
    }
    return 0;
}

long xdr::begin()
{
    return validate_config();
}

long xdr::load_rows(unsigned short *dst)
{
    long n = 0;
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    switch (ratio) {
    case 32:
        COPY_ROWS(dst, xdr_rows_28870, n);
        break;
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
    unsigned short x32[32] = {
        0x1F00, 0x0F00, 0x1700, 0x0700, 0x1B00, 0x0B00, 0x1300, 0x0300,
        0x1D00, 0x0D00, 0x1500, 0x0500, 0x1900, 0x0900, 0x1100, 0x0100,
        0x0000, 0x1000, 0x0800, 0x1800, 0x0400, 0x1400, 0x0C00, 0x1C00,
        0x0200, 0x1200, 0x0A00, 0x1A00, 0x0600, 0x1600, 0x0E00, 0x1E00,
    };
    unsigned short x16[16] = { 0x0F00, 0x0700, 0x0B00, 0x0300, 0x0D00, 0x0500, 0x0900, 0x0100, 0x0000, 0x0800, 0x0400, 0x0C00, 0x0200, 0x0A00, 0x0600, 0x0E00 };
    unsigned short x8[16] = { 0x0701, 0x0700, 0x0301, 0x0300, 0x0501, 0x0500, 0x0101, 0x0100, 0x0000, 0x0001, 0x0400, 0x0401, 0x0200, 0x0201, 0x0600, 0x0601 };
    unsigned short x4[16] = { 0x0303, 0x0301, 0x0302, 0x0300, 0x0103, 0x0101, 0x0102, 0x0100, 0x0000, 0x0002, 0x0001, 0x0003, 0x0200, 0x0202, 0x0201, 0x0203 };
    return ratio == 32 ? x32[i] : ratio == 16 ? x16[i] : ratio == 8 ? x8[i] : ratio == 4 ? x4[i] : 0;
}

extern unsigned char xdr_t45_x[2][1][32], xdr_t45_y[2][1][32];
extern unsigned char xdr_t4_x[2][4][8], xdr_t4_y[2][4][8];
extern unsigned char xdr_t7_x[2][4][8], xdr_t7_y[2][4][8];
extern unsigned char xdr_t32_x[2][2][16], xdr_t32_y[2][2][16];
extern unsigned char xdr_t33_x[2][2][16], xdr_t33_y[2][2][16];
extern unsigned char xdr_t41_x[2][5][8], xdr_t41_y[2][5][8];
extern unsigned char xdr_t0_x[2][2][16], xdr_t0_y[2][2][16];
extern unsigned char xdr_r4_x[2][9][4], xdr_r4_y[2][9][4];
extern unsigned char xdr_r8x5_x[2][5][8], xdr_r8x5_y[2][5][8];
extern unsigned char xdr_r8x4_x[2][4][8], xdr_r8x4_y[2][4][8];
extern unsigned char xdr_r16_x[2][2][16], xdr_r16_y[2][2][16];
extern unsigned char xdr_r4_k11_x[2][9][4], xdr_r4_k11_y[2][9][4];
extern unsigned char xdr_r8x5_k11_x[2][5][8], xdr_r8x5_k11_y[2][5][8];
extern unsigned char xdr_r8x4_k11_x[2][4][8], xdr_r8x4_k11_y[2][4][8];
extern unsigned char xdr_r16_k11_x[2][2][16], xdr_r16_k11_y[2][2][16];

enum xdr_board {
    XDR_BOARD_0 = 0,
    XDR_BOARD_4 = 4,
    XDR_BOARD_7 = 7,
    XDR_BOARD_32 = 32,
    XDR_BOARD_33 = 33,
    XDR_BOARD_41 = 41,
    XDR_BOARD_45 = 45,
};

#define BOARD_PAIR(t, na, nb, nc)               \
    if (a < na && b < nb && c < nc) {           \
        r.x = t##_x[a][b][c];                   \
        r.y = t##_y[a][b][c];                   \
    } else {                                    \
        r.x = 99;                               \
        r.y = 99;                               \
    }

#define RATIO_PAIR(t, na, nb, nc)               \
    if (a >= na || b >= nb || c >= nc)          \
        break;                                  \
    if (kind11) {                               \
        r.x = t##_k11_x[a][b][c];               \
        r.y = t##_k11_y[a][b][c];               \
    } else {                                    \
        r.x = t##_x[a][b][c];                   \
        r.y = t##_y[a][b][c];                   \
    }                                           \
    goto done;

inline xdr_pair xdr::timing(int a, int b, int c)
{
    xdr_pair r;
    xdr_board board = (xdr_board)m_mcp->basic[112];
    unsigned char devices = m_mcp->basic[0];
    unsigned int ratio = (*(unsigned int *)&m_mcp->basic[20] >> 24) & 63;
    bool kind11 = m_mcp->cbe_kind == 11;
    switch (board) {
    case XDR_BOARD_45:
        BOARD_PAIR(xdr_t45, 2, 1, 32)
        return r;
    case XDR_BOARD_4:
        BOARD_PAIR(xdr_t4, 2, 4, 8)
        return r;
    case XDR_BOARD_7:
        BOARD_PAIR(xdr_t7, 2, 4, 8)
        return r;
    case XDR_BOARD_32:
        BOARD_PAIR(xdr_t32, 2, 2, 16)
        return r;
    case XDR_BOARD_33:
        BOARD_PAIR(xdr_t33, 2, 2, 16)
        return r;
    case XDR_BOARD_41:
        BOARD_PAIR(xdr_t41, 2, 5, 8)
        return r;
    case XDR_BOARD_0:
        BOARD_PAIR(xdr_t0, 2, 2, 16)
        return r;
    }
    switch (ratio) {
    case 16:
        RATIO_PAIR(xdr_r16, 2, 2, 16)
        break;
    case 8:
        switch (devices) {
        case 5:
            RATIO_PAIR(xdr_r8x5, 2, 5, 8)
            break;
        case 4:
            RATIO_PAIR(xdr_r8x4, 2, 4, 8)
            break;
        }
        break;
    case 4:
        RATIO_PAIR(xdr_r4, 2, 9, 4)
        break;
    }
    r.x = 99;
    r.y = 99;
done:
    return r;
}

unsigned short xdr::permute(unsigned short x)
{
    int i;
    unsigned short r = 0;
    unsigned short map[16] = { 15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0 };
    for (i = 0; i < 16; i++)
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
    int j, k = 0;

    for (j = off + 8; k != 4; k++, j += 9) {
        out[k].b = LO(rows[j]);
        out[k].w0 = LO(rows[j - 1]) << 24 | LO(rows[j - 2]) << 16 | LO(rows[j - 3]) << 8 | LO(rows[j - 4]);
        out[k].w1 = LO(rows[j - 5]) << 24 | LO(rows[j - 6]) << 16 | LO(rows[j - 7]) << 8 | LO(rows[j - 8]);
    }
    for (j = off + 8; k != 8; k++, j += 9) {
        out[k].b = HI(rows[j]);
        out[k].w0 = HI(rows[j - 1]) << 24 | HI(rows[j - 2]) << 16 | HI(rows[j - 3]) << 8 | HI(rows[j - 4]);
        out[k].w1 = HI(rows[j - 5]) << 24 | HI(rows[j - 6]) << 16 | HI(rows[j - 7]) << 8 | HI(rows[j - 8]);
    }
    for (j = off + 44; k != 12; k++, j += 9) {
        out[k].b = LO(rows[j]);
        out[k].w0 = LO(rows[j - 1]) << 24 | LO(rows[j - 2]) << 16 | LO(rows[j - 3]) << 8 | LO(rows[j - 4]);
        out[k].w1 = LO(rows[j - 5]) << 24 | LO(rows[j - 6]) << 16 | LO(rows[j - 7]) << 8 | LO(rows[j - 8]);
    }
    for (j = off + 44; k != 16; k++, j += 9) {
        out[k].b = HI(rows[j]);
        out[k].w0 = HI(rows[j - 1]) << 24 | HI(rows[j - 2]) << 16 | HI(rows[j - 3]) << 8 | HI(rows[j - 4]);
        out[k].w1 = HI(rows[j - 5]) << 24 | HI(rows[j - 6]) << 16 | HI(rows[j - 7]) << 8 | HI(rows[j - 8]);
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
            for (unsigned int j = 0; j < 9; j++) {
                unsigned short base = (g << 8) + 0x800;
                unsigned int v = m_tx_left[ch][g][j] / m_mcp->basic[4];
                unsigned short reg = base | j * 4 << 2;
                m_mic.set_mic_timing(ch, reg | 2, v);
                m_mic.set_mic_timing(ch, reg | 3, m_tx_right[ch][g][j] / m_mcp->basic[4]);
                m_mic.set_mic_timing(ch, reg | 1, (m_tx_left[ch][g][j] / m_mcp->basic[4] + m_tx_right[ch][g][j] / m_mcp->basic[4]) >> 1);
            }
        }
}

void xdr::write_rx_tables()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            for (unsigned int j = 0; j < 9; j++) {
                unsigned short base = (g << 8) + 0x800;
                unsigned int v = m_rx_left[ch][g][j] / m_mcp->basic[4];
                unsigned short reg = base | j * 4 << 2;
                m_mic.set_mic_timing(ch, reg | 6, v);
                m_mic.set_mic_timing(ch, reg | 7, m_rx_right[ch][g][j] / m_mcp->basic[4]);
                m_mic.set_mic_timing(ch, reg | 5, (m_rx_left[ch][g][j] / m_mcp->basic[4] + m_rx_right[ch][g][j] / m_mcp->basic[4]) >> 1);
            }
        }
}

xdr::xdr()
{
    m_mcp = 0;
    memset(&m_cfg, 0, sizeof m_cfg);
    memset(m_rx_mid, 0, sizeof m_rx_mid);
    memset(m_rx_left, 0, sizeof m_rx_left);
    memset(m_rx_right, 0, sizeof m_rx_right);
    memset(m_tx_mid, 0, sizeof m_tx_mid);
    memset(m_tx_left, 0, sizeof m_tx_left);
    memset(m_tx_right, 0, sizeof m_tx_right);
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
                    syscon_printf("%04x", *(m_tx_left[ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(m_tx_right[ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(m_rx_left[ch][g] + j) / m_mcp->basic[4]);
                    syscon_printf("%04x", *(m_rx_right[ch][g] + j) / m_mcp->basic[4]);
                }
        break;
    case 1:
        if (m_mcp->basic[7] != 1) {
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
    if (m_mcp->basic[0] == 1)
        n = 8;
    else if (m_mcp->basic[0] == 2)
        n = 8;
    else if (m_mcp->basic[0] == 4)
        n = 8;
    for (unsigned short g = 0; g < 4; g++) {
        for (unsigned short j = 0; j < n; j++) {
            unsigned short base = g * 0x100 + 0x800;
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
            for (unsigned int j = 0; j < 9; j++) {
                unsigned short base = (g << 8) + 0x800;
                unsigned short reg = base | j * 4 << 2;
                m_rx_left[ch][g][j] += m_mic.get_mic_timing(ch, reg | 6);
                m_rx_right[ch][g][j] += m_mic.get_mic_timing(ch, reg | 7);
                m_rx_mid[ch][g][j] += m_mic.get_mic_timing(ch, reg | 5);
            }
        }
}

void xdr::add_tx_readings()
{
    for (unsigned int ch = 0; ch < 2; ch++)
        for (unsigned int g = 0; g < 4; g++) {
            for (unsigned int j = 0; j < 9; j++) {
                unsigned short base = (g << 8) + 0x800;
                unsigned short reg = base | j * 4 << 2;
                m_tx_left[ch][g][j] += m_mic.get_mic_timing(ch, reg | 2);
                m_tx_right[ch][g][j] += m_mic.get_mic_timing(ch, reg | 3);
                m_tx_mid[ch][g][j] += m_mic.get_mic_timing(ch, reg | 1);
            }
        }
}

bool be_mmio::poll64(unsigned int off, unsigned int mask_hi, unsigned int mask_lo,
                     unsigned int val_hi, unsigned int val_lo, unsigned int tries)
{
    u64 mask = (u64)mask_hi << 32 | mask_lo;
    u64 val = ((u64)val_hi << 32 | val_lo) & mask;
    for (;;) {
        if ((read64(off) & mask) == val)
            return 1;
        delay_us(1);
        if (--tries == 0)
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
            for (unsigned int b = 0; b < nb; b++) {
                unsigned short row = (i & 62) * cmds;
                unsigned int reg = 0x40004 | a << 8;
                unsigned short bm = bank_map(b, ratio);
                int col = (bm & 0xFF00) >> 8;
                unsigned short r = row + (bm & 0xFF) * 2 + (i & 1);
                xdr_pair t0 = timing(0, a, col);
                xdr_pair t1 = timing(1, a, col);
                unsigned int v0 = xdr_rows[r * 36 + t0.x * 9 + t0.y];
                unsigned int v1 = xdr_rows[r * 36 + t1.x * 9 + t1.y];
                unsigned short p0 = permute(v0);
                unsigned short p1 = permute(v1);
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

void post_code_01(void);
void post_code_02(void);
void post_code_03(void);
void post_code_05(void);
void post_code_06(void);
void post_code_07(bool f80, bool f40);

long xdr::initialize()
{
    long rc;

    if (m_mcp->str) {
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
    if (m_mcp->str)
        error("[INFO]: Wake from STR\n");
    error("\n[INFO]: >>> XDR Init: OK <<<\n");
    return rc;

fail:
    error("[ERROR]: >>> XDR Init: NG <<<\n");
    post_code_06();
    return rc;
}
