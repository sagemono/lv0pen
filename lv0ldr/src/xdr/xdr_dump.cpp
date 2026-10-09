#include "xdr.h"
#include "syscon.h"
#include "util.h"

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

CXX_DROPPED void xdr_dump_xig314()
{
    syscon_printf("\nXIG314");
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
