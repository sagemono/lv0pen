#include "xdr.h"
#include "syscon.h"
#include "util.h"

void delay_ms(unsigned int ms);

struct xdr_cfg {
    unsigned char b[8];
    unsigned short h8;
    unsigned short h10;
    unsigned short h12;
    unsigned int x14 : 32 __attribute__((packed));
    unsigned short h18;
    unsigned int w20;
    unsigned long long a24 : 16;
    unsigned int b24 : 32 __attribute__((packed));
    unsigned short c24;
    unsigned long long a32 : 16;
    unsigned int b32 : 32 __attribute__((packed));
    unsigned short c32;
    unsigned int w40;
    unsigned short h44;
    unsigned short h46;
    unsigned int pad48[2];
    unsigned int w56;
    unsigned int w60;
    unsigned int w64;
    unsigned int w68;
    unsigned int w72;
    unsigned short h76;
    unsigned short h78;
    unsigned short h80;
    unsigned short h82;
    unsigned short h84;
    unsigned short h86;
    unsigned short h88;
    unsigned short pad90;
    unsigned short h92;
    unsigned short h94;
    unsigned short h96;
    unsigned short h98;
    unsigned short h100;
    unsigned short h102;
    unsigned short h104;
};

#define CFG ((xdr_cfg *)m_mcp->basic)

long xdr::write_mic_config()
{
    m_mic.write64(0x50A218, CFG->h44 << 16 | 0xC1800000, 0);
    m_mic.write64(0x50A0C0, CFG->w20, 0);
    m_mic.write64(0x50A180, CFG->w20, 0);
    m_mic.write64(0x50A0E8, CFG->w40, 0);
    m_mic.write64(0x50A1A8, CFG->w40, 0);
    m_mic.write64(0x50A0C8, CFG->a24 << 16, 0);
    m_mic.write64(0x50A188, CFG->a24 << 16, 0);
    m_mic.write64(0x50A0E0, CFG->b32, CFG->c32 << 16);
    m_mic.write64(0x50A1A0, CFG->b32, CFG->c32 << 16);
    m_mic.write64(0x50A0D8, CFG->a32 << 16, 0);
    m_mic.write64(0x50A198, CFG->a32 << 16, 0);
    m_mic.write64(0x50A0D0, CFG->b24, CFG->c24 << 16);
    m_mic.write64(0x50A190, CFG->b24, CFG->c24 << 16);
    m_mic.write64(0x50A210, CFG->h18 << 16, 0);
    m_mic.delay_ns(50);
    m_mic.write64(0x50A200, CFG->x14, 0);
    m_mic.write64(0x50A230, 0xFD40, 0);
    m_mic.write64(0x50A238, 640, 0);
    m_mic.write64(0x50A208, 0, 0);
    m_mic.write64(0x50A050, 0, 0);
    m_mic.write64(0x50A058, 15, 0xFFFFFF80);
    m_mic.write64(0x50A080, CFG->h8 << 16, 0);
    m_mic.write64(0x50A1C0, CFG->h8 << 16, 0);
    m_mic.write64(0x50A040, CFG->h12 << 16, 0);
    m_mic.write64(0x50A0B0, CFG->h10 << 16, 0);
    m_mic.write64(0x50A1F0, CFG->h10 << 16, 0);
    if (m_mcp->basic[7] == 1) {
        m_mic.write64(0x50A130, (m_mcp->f_84 + CFG->w72) >> 4, 0);
        m_mic.write64(0x50A170, (m_mcp->f_84 + CFG->w72) >> 4, 0);
        if (m_mcp->basic[107] == 1) {
            m_mic.write64(0x50A0A0, m_mcp->f_84 >> 5 | 0x80000000, (m_mcp->f_84 + 0x9000) >> 1);
            m_mic.write64(0x50A1E0, m_mcp->f_84 >> 5 | 0x80000000, (m_mcp->f_84 + 0x9000) >> 1);
        }
        u64 v = m_mic.read64(0x50A218);
        m_mic.write64(0x50A218, v >> 32 | 0xC000, 0);
        m_mic.write64(0x50A0F0, CFG->w64, CFG->w68);
        m_mic.write64(0x50A1B0, CFG->w64, CFG->w68);
        v = m_mic.read64(0x50A218);
        m_mic.write64(0x50A218, v >> 32 & ~0xC000, 0);
        m_mic.write64(0x50A0F0, CFG->w56, CFG->w60);
        m_mic.write64(0x50A1B0, CFG->w56, CFG->w60);
    }
    m_mic.poll64(0x50A110, 0x1000, 0, 0, 0, 10000);
    return 0;
}

static const unsigned int mic_read_reg[2] = { 0x50A100, 0x50A140 };

bool be_mmio::poll_mic_timing(unsigned int ch, unsigned short a, unsigned short mask,
                              unsigned short val, unsigned int tries)
{
    unsigned int reg = mic_read_reg[ch];
    write64(reg, ((a & 0xFFF) | 0x1000) << 16, 0);
    unsigned int i = 0;
    for (;;) {
        unsigned short v = read64(reg) >> 32;
        if ((v & mask) == val)
            return 1;
        delay_us(1);
        i++;
        if (i == tries)
            return 0;
    }
}

static const unsigned short xio_pll_set[5] = { 0xC438, 0xC474, 0xC4B0, 0xC50C, 0xC40C };

long xdr::init_xio_link()
{
    unsigned short pll[8] __attribute__((aligned(16)));
    unsigned short diff[8] __attribute__((aligned(16)));
    unsigned short sel[8] __attribute__((aligned(16)));
    int i;

    m_mic.set_mic_timing(0, 521, CFG->h76);
    m_mic.set_mic_timing(1, 521, CFG->h76);
    m_mic.set_mic_timing(0, 6, 12);
    m_mic.delay_us(10);
    m_mic.set_mic_timing(0, 6, 15);
    m_mic.delay_us(10);
    m_mic.set_mic_timing(1, 6, 12);
    m_mic.delay_us(10);
    m_mic.set_mic_timing(1, 6, 15);
    m_mic.poll_mic_timing(0, 0, 2, 2, 10000000);
    m_mic.poll_mic_timing(1, 0, 2, 2, 10000000);
    m_mic.set_mic_timing(0, 7, 15);
    m_mic.set_mic_timing(1, 7, 15);
    m_mic.poll_mic_timing(0, 0, 6, 6, 10000000);
    m_mic.poll_mic_timing(1, 0, 6, 6, 10000000);
    unsigned short yrac = m_mcp->basic[109];
    if (m_mic.get_mic_timing(0, 3069) != (yrac << 8 | 57)) {
        info("[ERROR]: Abort XDR Init due to incorrect YRAC PLL Config setting.\n");
        return 0xB000200D;
    }
    m_mic.set_mic_timing(0, 1013, 0x8000);
    m_mic.set_mic_timing(1, 1013, 0x8000);
    m_mic.set_mic_timing(0, 1012, xio_pll_set[0]);
    m_mic.set_mic_timing(1, 1012, xio_pll_set[0]);
    for (unsigned short k = 1; k <= ((7 - yrac) & 63); k++) {
        m_mic.set_mic_timing(0, 1011, ((7 - k) & 63) | 64);
        m_mic.set_mic_timing(1, 1011, ((7 - k) & 63) | 64);
    }
    m_mic.set_mic_timing(0, 1013, 0);
    m_mic.set_mic_timing(1, 1013, 0);
    m_mic.delay_ns(500);
    m_mic.set_mic_timing(0, 1013, 0x8000);
    m_mic.set_mic_timing(1, 1013, 0x8000);
    for (unsigned short ch = 0; ch < 2; ch++) {
        unsigned short *p = &pll[ch * 4];
        for (unsigned short a = 0x8F5; a != 0xCF5; a += 0x100) {
            unsigned short v = m_mic.get_mic_timing(ch, a);
            *p = v;
            unsigned char c = (ch == 1) & (m_mcp->basic[0] == 9);
            if (c)
                *p = v - 128;
            p++;
        }
    }
    qword q = *(qword *)pll;
    if (!si_to_ushort(si_ceqhi(si_gbh(si_rothi(q, 1)), 255)))
        return 0xB0002013;
    *(qword *)diff = si_sfh(si_ilh(-28544), q);
    for (i = 0; i < 8; i++) {
        if ((unsigned short)(pll[i] + 0x6F80) > 0x2FF) {
            if (m_mcp->basic[0] == 9 && i / 4 != 0)
                sel[i] = 3;
            else
                sel[i] = (pll[i] >> 7) <= 287 || (pll[i] >> 7) == 295 ? 4 : 3;
        } else
            sel[i] = 2 - (diff[i] >> 8);
    }
    for (int ch = 0; ch < 2; ch++) {
        unsigned short *s = &sel[ch * 4];
        for (unsigned short a = 0x8F4; a != 0xCF4; a += 0x100)
            m_mic.set_mic_timing(ch, a, xio_pll_set[*s++]);
    }
    m_mic.poll_mic_timing(0, 0, 7, 7, 10000000);
    m_mic.poll_mic_timing(1, 0, 7, 7, 10000000);
    m_mic.set_mic_timing(0, 514, 9);
    m_mic.set_mic_timing(1, 514, 9);
    m_mic.poll_mic_timing(0, 0, 7, 7, 10000000);
    m_mic.poll_mic_timing(1, 0, 7, 7, 10000000);
    m_mic.set_mic_timing(0, 514, 11);
    m_mic.set_mic_timing(1, 514, 11);
    m_mic.write64(0x50A120, CFG->h46 << 16, 0);
    m_mic.write64(0x50A160, CFG->h46 << 16, 0);
    m_mic.set_mic_timing(0, 519, CFG->h78);
    m_mic.set_mic_timing(1, 519, CFG->h78);
    for (i = 0; i < 128; i++) {
        m_mic.set_mic_timing(0, 16, 17);
        m_mic.set_mic_timing(1, 16, 17);
        m_mic.poll_mic_timing(0, 16, 32, 32, 10000000);
        m_mic.poll_mic_timing(1, 16, 32, 32, 10000000);
        m_mic.set_mic_timing(0, 16, 0);
        m_mic.set_mic_timing(1, 16, 0);
    }
    for (i = 0; i < 128; i++) {
        m_mic.set_mic_timing(0, 16, 16);
        m_mic.set_mic_timing(1, 16, 16);
        m_mic.poll_mic_timing(0, 16, 32, 32, 10000000);
        m_mic.poll_mic_timing(1, 16, 32, 32, 10000000);
        m_mic.set_mic_timing(0, 16, 0);
        m_mic.set_mic_timing(1, 16, 0);
    }
    m_mic.set_mic_timing(0, 1009, CFG->h88);
    m_mic.set_mic_timing(1, 1009, CFG->h88);
    if (m_mcp->basic[5] == 1) {
        m_mic.set_mic_timing(0, 1010, 1);
        m_mic.set_mic_timing(1, 1010, 1);
    } else {
        m_mic.set_mic_timing(0, 1010, 0);
        m_mic.set_mic_timing(1, 1010, 0);
    }
    return 0;
}

long xdr::init_devices()
{
    int i;

    if (!m_mcp->str) {
        syscon_write_xdr_config(0, 64);
        m_mic.set_mic_timing(0, 513, 1);
        m_mic.set_mic_timing(1, 513, 1);
        for (i = 0; i < 4; i++) {
            m_mic.set_mic_timing(0, 513, 3);
            m_mic.set_mic_timing(1, 513, 3);
            m_mic.set_mic_timing(0, 513, 1);
            m_mic.set_mic_timing(1, 513, 1);
        }
        m_mic.set_mic_timing(0, 513, 0);
        m_mic.set_mic_timing(1, 513, 0);
        syscon_write_xdr_config(128, 64);
        int n = m_mcp->basic[0];
        for (i = 0; i < n; i++) {
            m_mic.set_mic_timing(0, 513, 2);
            m_mic.set_mic_timing(1, 513, 2);
            m_mic.set_mic_timing(0, 513, 0);
            m_mic.set_mic_timing(1, 513, 0);
        }
        m_mic.set_mic_timing_b(0, 0x140002, m_mcp->f_ac);
        m_mic.set_mic_timing_b(1, 0x140002, m_mcp->f_ac);
        m_mic.set_mic_timing_b(0, 0x14001F, m_mcp->basic[106]);
        m_mic.set_mic_timing_b(1, 0x14001F, m_mcp->basic[106]);
    }
    m_mic.set_mic_timing_b(0, 0x140003, 1);
    m_mic.set_mic_timing_b(1, 0x140003, 1);
    m_mic.read64(0x50A110);
    m_mic.read64(0x50A150);
    m_mic.delay_us(13);
    for (i = 0; i < 8; i++) {
        m_mic.write64(0x50A208, 0x06000000, 0);
        m_mic.poll64(0x50A208, 0x04000000, 0, 0, 0, 10000);
    }
    m_mic.write64(0x50A208, 0x40000000, 0);
    for (i = 0; i < 128; i++) {
        m_mic.set_mic_timing(0, 16, 18);
        m_mic.set_mic_timing(1, 16, 18);
        m_mic.poll_mic_timing(0, 16, 32, 32, 10000000);
        m_mic.poll_mic_timing(1, 16, 32, 32, 10000000);
        m_mic.set_mic_timing(0, 16, 0);
        m_mic.set_mic_timing(1, 16, 0);
    }
    for (i = 0; i < 128; i++) {
        m_mic.set_mic_timing(0, 16, 19);
        m_mic.set_mic_timing(1, 16, 19);
        m_mic.poll_mic_timing(0, 16, 32, 32, 10000000);
        m_mic.poll_mic_timing(1, 16, 32, 32, 10000000);
        m_mic.set_mic_timing(0, 16, 0);
        m_mic.set_mic_timing(1, 16, 0);
    }
    return 0;
}

long xdr::calibrate_rx()
{
    m_mic.set_mic_timing(0, 38, CFG->h80);
    m_mic.set_mic_timing(1, 38, CFG->h80);
    m_mic.set_mic_timing(0, 39, CFG->h82);
    m_mic.set_mic_timing(1, 39, CFG->h82);
    m_mic.set_mic_timing(0, 42, CFG->h92);
    m_mic.set_mic_timing(1, 42, CFG->h92);
    for (unsigned short k = 0; k < m_mcp->basic[4]; k++) {
        m_mic.set_mic_timing(0, 36, 7);
        m_mic.set_mic_timing(1, 36, 7);
        m_mic.poll_mic_timing(0, 36, 8, 8, 10000000);
        m_mic.poll_mic_timing(1, 36, 8, 8, 10000000);
        if (m_mic.get_mic_timing(0, 36) & 0x100)
            return 0xB0002001;
        m_mic.set_mic_timing(0, 36, 0);
        if (m_mic.get_mic_timing(1, 36) & 0x100)
            return 0xB0002002;
        m_mic.set_mic_timing(1, 36, 0);
        add_rx_readings();
    }
    write_rx_tables();
    return 0;
}

long xdr::store_zero_line(unsigned int v)
{
    m_mic.write64(0x50A060, 0, v & ~127);
    m_mic.write64(0x50A068, 0, 0);
    for (int i = 0; i < 16; i++)
        m_mic.write64(0x50A070, 0, 0);
    return 0;
}

long xdr::run_mic_cmd_c0()
{
    m_mic.write64(0x50A208, 0xC0000000, 0);
    m_mic.delay_us(1000);
    return m_mic.poll64(0x50A208, 0x80000000, 0, 0, 0, 10000000) == 1 ? 0 : 0xB000200A;
}

long xdr::calibrate_tx()
{
    unsigned short err0 = 0;
    unsigned short err1 = 0;

    m_mic.set_mic_timing(0, 40, CFG->h84);
    m_mic.set_mic_timing(1, 40, CFG->h84);
    m_mic.set_mic_timing(0, 41, CFG->h86);
    m_mic.set_mic_timing(1, 41, CFG->h86);
    m_mic.set_mic_timing(0, 42, CFG->h92);
    m_mic.set_mic_timing(1, 42, CFG->h92);
    for (unsigned short k = 0; k < m_mcp->basic[4]; k++) {
        m_mic.set_mic_timing(0, 37, 7);
        m_mic.set_mic_timing(1, 37, 7);
        m_mic.poll_mic_timing(0, 37, 8, 8, 10000000);
        m_mic.poll_mic_timing(1, 37, 8, 8, 10000000);
        if (m_mic.get_mic_timing(0, 37) & 0x100) {
            err0 = m_mic.get_mic_timing(0, 0x8F6);
            err0 |= m_mic.get_mic_timing(0, 0x9F6);
            err0 |= m_mic.get_mic_timing(0, 0xAF6);
            err0 |= m_mic.get_mic_timing(0, 0xBF6);
        }
        if (m_mic.get_mic_timing(1, 37) & 0x100) {
            err1 = m_mic.get_mic_timing(1, 0x8F6);
            err1 |= m_mic.get_mic_timing(1, 0x9F6);
            err1 |= m_mic.get_mic_timing(1, 0xAF6);
            err1 |= m_mic.get_mic_timing(1, 0xBF6);
        }
        add_tx_readings();
    }
    write_tx_tables();
    m_mic.set_mic_timing(0, 37, 0);
    m_mic.set_mic_timing(1, 37, 0);
    m_mic.delay_ns(3450);
    m_mic.write64(0x50A118, 0x00200000, 0);
    m_mic.delay_ns(100);
    m_mic.write64(0x50A158, 0x00200000, 0);
    m_mic.delay_ns(100);
    m_mic.delay_ns(360);
    unsigned short e0 = err0 & 0xFF, e1 = err1 & 0xFF;
    if (e0 == 0 && e1 != 0) {
        m_mcp->xio_ch0_on = true;
        m_mcp->xio_ch1_on = false;
        return 0xB0002004;
    }
    if (e0 != 0 && e1 == 0) {
        m_mcp->xio_ch0_on = false;
        m_mcp->xio_ch1_on = true;
        return 0xB0002003;
    }
    if (e0 != 0 && e1 != 0) {
        m_mcp->xio_ch0_on = false;
        m_mcp->xio_ch1_on = false;
        return 0xB0002003;
    }
    m_mcp->xio_ch0_on = true;
    m_mcp->xio_ch1_on = true;
    u64 v0 = m_mic.read64(0x50A0A8);
    u64 v1 = m_mic.read64(0x50A1E8);
    m_mic.write64(0x50A0A8, v0 >> 32 | 2, v0);
    m_mic.write64(0x50A1E8, v1 >> 32 | 2, v1);
    info("[INFO]: Store 128 Bytes to each channel\n");
    m_mic.delay_ns(200);
    store_zero_line(m_mcp->f_84);
    m_mic.delay_ns(200);
    store_zero_line(m_mcp->f_84 + 128);
    m_mic.delay_ns(1000);
    unsigned int w218 = m_mic.read64(0x50A218) >> 32;
    unsigned int w210 = m_mic.read64(0x50A210) >> 32;
    if (m_mcp->xio_ch0_on == 1) {
        if (m_mcp->ecc_off_ch0 == 1) {
            w218 |= 0x40000000;
            info("[INFO]: ECC OFF for XIO Ch0\n");
        } else {
            w218 &= ~0x40000000;
            info("[INFO]: ECC ON for XIO Ch0\n");
        }
    } else {
        w218 |= 0xC0000000;
        w210 &= ~0x00020000;
    }
    if (m_mcp->xio_ch1_on == 1) {
        if (m_mcp->ecc_off_ch1 == 1) {
            w218 |= 0x00800000;
            info("[INFO]: ECC OFF for XIO Ch1\n");
        } else {
            w218 &= ~0x00800000;
            info("[INFO]: ECC ON for XIO Ch1\n");
        }
    } else {
        w218 |= 0x01800000;
        w210 &= ~0x00040000;
    }
    m_mic.write64(0x50A208, 0x50000000, 0);
    m_mic.poll64(0x50A208, 0x50000000, 0, 0x50000000, 0, 10000);
    m_mic.write64(0x50A210, w210, 0);
    m_mic.write64(0x50A208, 0x40000000, 0);
    m_mic.write64(0x50A218, w218, 0);
    m_mic.read64(0x50A0F8);
    m_mic.read64(0x50A1B8);
    if (m_mcp->ecc_on == 1 && m_mcp->str == 0) {
        long rc = run_mic_cmd_c0();
        if (rc != 0)
            return rc;
    }
    if (m_mcp->xio_ch0_on == 1 && m_mcp->xio_ch1_on == 1) {
        m_mic.write64(0x50A230, 0xFD7E, 0);
        m_mic.write64(0x50A238, 0x280, 0);
    }
    if (m_mcp->xio_ch0_on == 0 && m_mcp->xio_ch1_on == 1) {
        m_mic.write64(0x50A230, 0xAD3E, 0);
        m_mic.write64(0x50A238, 0x50000280, 0);
    }
    if (m_mcp->xio_ch0_on == 1 && m_mcp->xio_ch1_on == 0) {
        m_mic.write64(0x50A230, 0x5C7E, 0);
        m_mic.write64(0x50A238, 0xA3000280, 0);
    }
    if (m_mcp->xio_ch0_on == 1) {
        w218 &= ~0x80000000;
        info("[INFO]: SRAM Parity ON for XIO Ch0\n");
    }
    if (m_mcp->xio_ch1_on == 1) {
        w218 &= ~0x01000000;
        info("[INFO]: SRAM Parity ON for XIO Ch1\n");
    }
    m_mic.write64(0x50A218, w218, 0);
    info("[INFO]: MIC Data Flow is now configured.\n");
    if (m_mcp->xio_ch0_on == 1)
        info("[INFO]: XIO Ch0 is available.\n");
    if (m_mcp->xio_ch1_on == 1)
        info("[INFO]: XIO Ch1 is available.\n");
    return 0;
}

long xdr::read_ptcal_start()
{
    for (int ch = 0; ch < 2; ch++) {
        unsigned short base = 0x805;
        unsigned short &b = base;

        for (int g = 0; g < 4; g++, base += 0x100) {
            unsigned short reg = b;

            for (int j = 0; j < 9; j++) {
                *(tables[0][ch][g] + j) = m_mic.get_mic_timing(ch, reg);
                *(tables[3][ch][g] + j) = m_mic.get_mic_timing(ch, reg - 4);
                reg += 16;
            }
        }
    }
    return 0;
}

long xdr::write_ptcal_settings()
{
    m_mic.set_mic_timing(0, 18, CFG->h94);
    m_mic.set_mic_timing(1, 18, CFG->h94);
    m_mic.set_mic_timing(0, 20, CFG->h98);
    m_mic.set_mic_timing(1, 20, CFG->h98);
    m_mic.set_mic_timing(0, 19, CFG->h96);
    m_mic.set_mic_timing(1, 19, CFG->h96);
    return 0;
}

void be_mmio::delay_ms(unsigned int ms)
{
    ::delay_ms(ms);
}

long xdr::enable_ptcal()
{
    unsigned int n;
    unsigned int s;

    m_mic.write64(0x50A118, 0x00200000, 0);
    m_mic.write64(0x50A158, 0x00200000, 0);
    m_mic.set_mic_timing(0, 17, 0x8000);
    m_mic.set_mic_timing(1, 17, 0x8000);
    if (m_mcp->basic[111] == 0xFF)
        m_mic.delay_ms(40);
    else
        m_mic.delay_ms(m_mcp->basic[111]);
    for (n = 0xA00000; ; ) {
        n--;
        s = (m_mic.read64(0x50A110) >> 32) & 0x010E0000;
        if (s == 0x01020000 || s == 0x01040000)
            break;
        if (n == 0)
            goto timeout0;
    }
    for (n = 0xA00000; ; ) {
        n--;
        s = m_mic.read64(0x50A110) >> 32;
        if (!(s & 0x01000000))
            break;
        if (n == 0)
            goto timeout0;
    }
    m_mic.set_mic_timing(0, 17, 0);
    for (n = 0xA00000; ; ) {
        n--;
        s = (m_mic.read64(0x50A150) >> 32) & 0x010E0000;
        if (s == 0x01020000 || s == 0x01040000)
            break;
        if (n == 0)
            goto timeout1;
    }
    for (n = 0xA00000; ; ) {
        n--;
        s = m_mic.read64(0x50A150) >> 32;
        if (!(s & 0x01000000))
            break;
        if (n == 0)
            goto timeout1;
    }
    m_mic.set_mic_timing(1, 17, 0);
    for (int ch = 0; ch < 2; ch++) {
        unsigned short base = 0x805;
        unsigned short &b = base;

        for (int g = 0; g < 4; g++, base += 0x100) {
            unsigned short reg = b;

            for (int j = 0; j < 9; j++) {
                m_mic.set_mic_timing(ch, reg, *(tables[0][ch][g] + j));
                m_mic.set_mic_timing(ch, reg - 4, *(tables[3][ch][g] + j));
                reg += 16;
            }
        }
    }
    m_mic.set_mic_timing(0, 18, CFG->h100);
    m_mic.set_mic_timing(1, 18, CFG->h100);
    m_mic.set_mic_timing(0, 20, CFG->h104);
    m_mic.set_mic_timing(1, 20, CFG->h104);
    m_mic.set_mic_timing(0, 19, CFG->h102);
    m_mic.set_mic_timing(1, 19, CFG->h102);
    m_mic.set_mic_timing(0, 17, 0x8000);
    m_mic.set_mic_timing(1, 17, 0x8000);
    return 0;

timeout0:
    return 0xB0002011;
timeout1:
    return 0xB0002012;
}

void xdr::dump_xdr()
{
    unsigned short v = 0;

    syscon_printf("XDR_DUMP");
    unsigned int regs[26] = {
        0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0F, 0x10, 0x11,
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F,
    };
    for (int ch = 0; ch < 2; ch++)
        for (int i = 0; i < 26; i++)
            for (int dev = 0; dev < 9; dev++) {
                serial_read(ch, dev, regs[i], &v);
                syscon_printf("%02x", v);
            }
    syscon_printf("\n");
}

void xdr::dump_xio()
{
    syscon_printf("XIO_DUMP");
    unsigned short a[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 17, 18, 19 };
    unsigned short b[25] = {
        0x000, 0x001, 0x002, 0x005, 0x006, 0x007, 0x010, 0x011, 0x012, 0x013, 0x014, 0x024, 0x025,
        0x026, 0x027, 0x028, 0x029, 0x02A, 0x02B, 0x040, 0x041, 0x042, 0x043, 0x044, 0x045,
    };
    unsigned short c[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 13 };
    unsigned short d[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int i;

    for (int ch = 0; ch < 2; ch++) {
        for (i = 0; i < 14; i++)
            syscon_printf("%04x", m_mic.get_mic_timing(ch, a[i] + 0x400));
        for (i = 0; i < 25; i++)
            syscon_printf("%04x", m_mic.get_mic_timing(ch, b[i]));
        for (i = 0; i < 12; i++)
            for (unsigned short g = 0; g < 4; g++)
                syscon_printf("%04x", m_mic.get_mic_timing(ch, c[i] + 0x8F0 + g * 0x100));
        for (unsigned short base = 0x800; base != 0xC00; base += 0x100)
            for (i = 0; i < 16; i++)
                for (unsigned int j = 0; j < 9; j++)
                    syscon_printf("%04x", m_mic.get_mic_timing(ch, base | j << 4 | d[i]));
    }
    syscon_printf("\n");
}

void xdr::dump_mic()
{
    syscon_printf("MIC_DUMP");
    u64 regs[70] = {
        0x50A000, 0x50A008, 0x50A010, 0x50A040, 0x50A048, 0x50A050, 0x50A058, 0x50A060,
        0x50A068, 0x50A080, 0x50A088, 0x50A090, 0x50A098, 0x50A0A0, 0x50A0A8, 0x50A0B0,
        0x50A0C0, 0x50A0C8, 0x50A0D0, 0x50A0D8, 0x50A0E0, 0x50A0E8, 0x50A0F0, 0x50A0F8,
        0x50A100, 0x50A108, 0x50A110, 0x50A118, 0x50A120, 0x50A128, 0x50A130, 0x50A138,
        0x50A140, 0x50A148, 0x50A150, 0x50A158, 0x50A160, 0x50A168, 0x50A170, 0x50A178,
        0x50A180, 0x50A188, 0x50A190, 0x50A198, 0x50A1A0, 0x50A1A8, 0x50A1B0, 0x50A1B8,
        0x50A1C0, 0x50A1C8, 0x50A1D0, 0x50A1D8, 0x50A1E0, 0x50A1E8, 0x50A1F0, 0x50A200,
        0x50A208, 0x50A210, 0x50A218, 0x50A220, 0x50A228, 0x50A230, 0x50A238, 0x50AFC0,
        0x50AFC8, 0x50AFD0, 0x50AFD8, 0x50AFE0, 0x50AFE8, 0x50AFF0,
    };
    for (int i = 0; i < 70; i++)
        syscon_printf("%016llx", ::read64(BE_MMIO_BASE + regs[i]));
    syscon_printf("\n");
}

void xdr::dump_regs()
{
    dump_mic();
    dump_xio();
    dump_xdr();
}

long xdr::prepare_iow_dump()
{
    if (m_mcp->basic[7] == 1) {
        m_mic.poll64(0x50A110, 0x01000000, 0, 0x01000000, 0, 10000);
        m_mic.poll64(0x50A110, 0x01000000, 0, 0, 0, 10000);
        m_mic.set_mic_timing(0, 17, 0);
        m_mic.poll64(0x50A150, 0x01000000, 0, 0x01000000, 0, 10000);
        m_mic.poll64(0x50A150, 0x01000000, 0, 0, 0, 10000);
        m_mic.set_mic_timing(1, 17, 0);
    }
    m_mic.set_mic_timing(0, 1005, 226);
    m_mic.set_mic_timing(1, 1005, 226);
    m_mic.set_mic_timing(0, 993, 5632);
    m_mic.set_mic_timing(1, 993, 5632);
    m_mic.set_mic_timing(0, 997, 0);
    m_mic.set_mic_timing(1, 997, 0);
    m_mic.set_mic_timing(0, 998, 0);
    m_mic.set_mic_timing(1, 998, 0);
    m_mic.set_mic_timing(0, 999, 255);
    m_mic.set_mic_timing(1, 999, 255);
    m_mic.set_mic_timing(0, 38, 512);
    m_mic.set_mic_timing(1, 38, 512);
    m_mic.set_mic_timing(0, 39, 7936);
    m_mic.set_mic_timing(1, 39, 7936);
    m_mic.set_mic_timing(0, 64, 16);
    m_mic.set_mic_timing(1, 64, 16);
    m_mic.delay_ms(500);
    m_mic.set_mic_timing(0, 64, 0);
    m_mic.set_mic_timing(1, 64, 0);
    m_mic.set_mic_timing(0, 993, 0);
    m_mic.set_mic_timing(1, 993, 0);
    m_mic.set_mic_timing(0, 994, 0);
    m_mic.set_mic_timing(1, 994, 0);
    m_mic.set_mic_timing(0, 995, 255);
    m_mic.set_mic_timing(1, 995, 255);
    m_mic.set_mic_timing(0, 40, 5120);
    m_mic.set_mic_timing(1, 40, 5120);
    m_mic.set_mic_timing(0, 41, 7936);
    m_mic.set_mic_timing(1, 41, 7936);
    m_mic.set_mic_timing(0, 64, 17);
    m_mic.set_mic_timing(1, 64, 17);
    m_mic.delay_ms(500);
    m_mic.set_mic_timing(0, 64, 1);
    m_mic.set_mic_timing(1, 64, 1);
    return 0;
}
