#include "xdr.h"

static long serial_bit(unsigned char *p, unsigned char bit)
{
    if (bit) {
        p[0] = 4;
        p[1] = 6;
    } else {
        p[0] = 0;
        p[1] = 2;
    }
    return 0;
}

static long serial_byte(unsigned char *p, unsigned char v)
{
    unsigned char *q = p;
    for (unsigned char i = 0; i < 8; i++) {
        serial_bit(q, v & 0x80);
        q += 2;
        v <<= 1;
    }
    return 0;
}

long xdr::serial_read(unsigned int ch, unsigned short dev, unsigned char reg,
                      unsigned short *out)
{
    unsigned char seq[64];
    unsigned short v = 0;
    unsigned char *p;

    serial_bit(&seq[0], 1);
    serial_bit(&seq[2], 1);
    serial_bit(&seq[4], 0);
    serial_bit(&seq[6], 0);
    if (dev == 0xFFFF) {
        serial_bit(&seq[8], 1);
        dev = 0;
        serial_bit(&seq[10], 1);
    } else {
        serial_bit(&seq[8], 1);
        serial_bit(&seq[10], 0);
    }
    serial_byte(&seq[12], dev & 63);
    serial_byte(&seq[28], reg);
    for (p = &seq[44]; p != &seq[60]; p += 2) {
        serial_bit(p, 0);
        p[0] |= 16;
        p[1] |= 16;
    }
    serial_bit(&seq[60], 0);
    serial_bit(&seq[62], 0);
    for (p = seq; ; p += 2) {
        m_mic.set_mic_timing(ch, 1025, p[0] & 7);
        m_mic.set_mic_timing(ch, 1025, p[1] & 7);
        if (p[1] & 16) {
            v <<= 1;
            if (m_mic.get_mic_timing(ch, 1025) & 8)
                v |= 1;
        }
        if (p == &seq[62])
            break;
    }
    m_mic.set_mic_timing(ch, 1025, 0);
    *out = v;
    return 0;
}
