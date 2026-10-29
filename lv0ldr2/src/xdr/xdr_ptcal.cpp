#include "xdr.h"
#include "syscon.h"
#include "util.h"

void delay_ms(unsigned int ms);

struct xdr_cfg {
    unsigned char b[8];
    unsigned short h8;
    unsigned short h10;
    unsigned short h12;
    unsigned short h14;
    unsigned short h16;
    unsigned short h18;
    unsigned int w20;
    unsigned short h24;
    unsigned short h26;
    unsigned short h28;
    unsigned short h30;
    unsigned short h32;
    unsigned short h34;
    unsigned short h36;
    unsigned short h38;
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
    unsigned char pad106[6];
    unsigned char board;
    unsigned short h113 __attribute__((packed));
};

#define CFG ((xdr_cfg *)m_mcp->basic)

long xdr::calibrate_tx()
{
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
        if (m_mic.get_mic_timing(0, 37) & 0x100)
            return 0xB0002003;
        m_mic.set_mic_timing(0, 37, 0);
        if (m_mic.get_mic_timing(1, 37) & 0x100)
            return 0xB0002004;
        m_mic.set_mic_timing(1, 37, 0);
        add_tx_readings();
    }
    write_tx_tables();
    m_mic.delay_ns(3450);
    m_mic.write64(0x50A118, 0x00200000, 0);
    m_mic.delay_ns(100);
    m_mic.write64(0x50A158, 0x00200000, 0);
    m_mic.delay_ns(100);
    m_mic.delay_ns(360);
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
    if (m_mcp->ecc_off_ch0 == 1) {
        w218 |= 0x40000000;
        info("[INFO]: ECC OFF for XIO Ch0\n");
    } else {
        w218 &= ~0x40000000;
        info("[INFO]: ECC ON for XIO Ch0\n");
    }
    if (m_mcp->ecc_off_ch1 == 1) {
        w218 |= 0x00800000;
        info("[INFO]: ECC OFF for XIO Ch1\n");
    } else {
        w218 &= ~0x00800000;
        info("[INFO]: ECC ON for XIO Ch1\n");
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
    m_mic.write64(0x50A230, 0xFD7E, 0);
    m_mic.write64(0x50A238, 0x280, 0);
    w218 &= ~0x80000000;
    info("[INFO]: SRAM Parity ON for XIO Ch0\n");
    w218 &= ~0x01000000;
    info("[INFO]: SRAM Parity ON for XIO Ch1\n");
    m_mic.write64(0x50A218, w218, 0);
    info("[INFO]: MIC Data Flow is now configured.\n");
    info("[INFO]: XIO Ch0 is available.\n");
    info("[INFO]: XIO Ch1 is available.\n");
    return 0;
}

long xdr::read_ptcal_start()
{
    for (int ch = 0; ch < 2; ch++) {
        int g = 0;
        for (unsigned short base = 0x805; base != 0xC05; base += 0x100, g++) {
            for (int j = 0; j < 9; j++) {
                unsigned short reg = base + j * 16;
                m_rx_mid[ch][g][j] = m_mic.get_mic_timing(ch, reg);
                m_tx_mid[ch][g][j] = m_mic.get_mic_timing(ch, reg - 4);
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
        int g = 0;
        for (unsigned short base = 0x805; base != 0xC05; base += 0x100, g++) {
            for (int j = 0; j < 9; j++) {
                unsigned short reg = base + j * 16;
                m_mic.set_mic_timing(ch, reg, m_rx_mid[ch][g][j]);
                m_mic.set_mic_timing(ch, reg - 4, m_tx_mid[ch][g][j]);
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
