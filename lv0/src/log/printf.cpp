#include "lv0.h"
#include "printf.h"

typedef int (*putc_fn)(long ctx_a, long *state, long ctx_b, int ch);

#define F_MINUS 0x001
#define F_PLUS  0x002
#define F_ZERO  0x004
#define F_SPACE 0x008
#define F_ALT   0x010
#define F_PREC  0x020
#define F_LONG  0x040
#define F_SHORT 0x080
#define F_CHAR  0x100

int lv0_vfprintf_engine(putc_fn put, long ctx_a, unsigned int ctx_b,
                        const u8 *format, u8 **ap)
{
    int count = 0;
    unsigned int ctxv = ctx_b;
    const u8 *fmt = format;
    char nbuf[32];
    char pfx[32];
    u8 **t;
    unsigned long *q;
    const char *s;
    int flags, width;
    const char *start = 0, *end;
    int prec;
    char *p;
    int pfxlen, len;
    int c, w, n;
    const char *digits;
    unsigned long u;
    long v;
    int zpad, fpad;
    char padc;
    long i;
    char *bufend;
    int j, k;

    while (*fmt) {
        flags = 0;
        width = 0;
        prec = 0;
        if (*fmt != '%')
            goto literal;
parse:
        c = *++fmt;
        switch (c) {
        case '-':
            flags |= F_MINUS;
            goto parse;
        case '+':
            flags |= F_PLUS;
            goto parse;
        case '0':
            flags |= F_ZERO;
            goto parse;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            w = 0;
            do {
                w = w * 10 + *fmt - '0';
            } while ((u8)(*++fmt - '0') <= 9);
            if (*fmt == 0)
                continue;
            fmt--;
            width = w;
            goto parse;
        case '*':
            t = ap;
            ap = t + 1;
            width = ((int *)t)[1] < 0 ? -1 : ((int *)t)[1];
            goto parse;
        case '.':
            flags |= F_PREC;
            w = 0;
            if (*++fmt == '*') {
                t = ap;
                ap = t + 1;
                prec = ((int *)t)[1] < 0 ? -1 : ((int *)t)[1];
                goto parse;
            }
            while ((u8)(*fmt - '0') <= 9) {
                w = w * 10 + *fmt - '0';
                fmt++;
            }
            if (*fmt == 0)
                continue;
            prec = w < 0 ? -1 : w;
            fmt--;
            goto parse;
        case ' ':
            flags |= F_SPACE;
            goto parse;
        case '#':
            flags |= F_ALT;
            goto parse;
        case 'l':
            flags |= F_LONG;
            goto parse;
        case 'h':
            flags |= F_SHORT;
            goto parse;
        case 'o':
            if (flags & F_LONG) {
                t = ap + 1;
                u = *(unsigned long *)ap;
            } else if (flags & F_SHORT) {
                t = ap + 1;
                u = (unsigned short)((unsigned int *)ap)[1];
            } else {
                t = ap + 1;
                u = ((unsigned int *)ap)[1];
            }
            p = &nbuf[31];
            if (u == 0) {
                if (!(flags & F_PREC) || prec != 0)
                    *p-- = '0';
            } else {
                while (u) {
                    *p-- = '0' + (u & 7);
                    u >>= 3;
                }
                if (flags & F_ALT)
                    *p-- = '0';
            }
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            pfxlen = 0;
            ap = t;
            goto emit;
        case 'x':
            digits = "0123456789abcdef";
            if (flags & F_ALT) {
                pfx[0] = '0';
                pfx[1] = 'x';
                pfxlen = 2;
            } else
                pfxlen = 0;
            goto hex;
        case 'X':
            digits = "0123456789ABCDEF";
            if (flags & F_ALT) {
                pfx[0] = '0';
                pfx[1] = 'X';
                pfxlen = 2;
            } else
                pfxlen = 0;
hex:
            if (flags & F_LONG) {
                u = *(unsigned long *)ap;
                ap++;
            } else if (flags & F_SHORT) {
                u = (unsigned short)((unsigned int *)ap)[1];
                ap++;
            } else {
                u = ((unsigned int *)ap)[1];
                ap++;
            }
            p = &nbuf[31];
            if (u == 0) {
                pfxlen = 0;
                if (!(flags & F_PREC) || prec != 0)
                    *p-- = '0';
            } else {
                do {
                    *p-- = digits[u & 15];
                    u >>= 4;
                } while (u);
            }
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            goto emit;
        case 'd':
        case 'i':
            if (flags & F_LONG) {
                t = ap + 1;
                v = *(long *)ap;
            } else if (flags & F_SHORT) {
                t = ap + 1;
                v = (short)((int *)ap)[1];
            } else {
                t = ap + 1;
                v = ((int *)ap)[1];
            }
            if (v >= 0) {
                if (flags & F_PLUS) {
                    pfxlen = 1;
                    pfx[0] = '+';
                } else if (flags & F_SPACE) {
                    pfxlen = 1;
                    pfx[0] = ' ';
                } else
                    pfxlen = 0;
            } else {
                pfx[0] = '-';
                pfxlen = 1;
            }
            p = &nbuf[31];
            if (v == 0) {
                if (!(flags & F_PREC) || prec != 0)
                    *p-- = '0';
            } else {
                if (v == (long)0x8000000000000000UL) {
                    *p-- = '8';
                    v = 922337203685477580L;
                } else if (v < 0)
                    v = -v;
                while (v) {
                    *p-- = '0' + v % 10;
                    v /= 10;
                }
            }
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            ap = t;
            goto emit;
        case 'u':
            q = (unsigned long *)ap;
            if (flags & F_LONG) {
                ap = (u8 **)(q + 1);
                u = *(unsigned long *)q;
            } else if (flags & F_SHORT) {
                ap = (u8 **)(q + 1);
                u = (unsigned short)((unsigned int *)q)[1];
            } else {
                ap = (u8 **)(q + 1);
                u = ((unsigned int *)q)[1];
            }
            p = &nbuf[31];
            if (u == 0) {
                if (!(flags & F_PREC) || prec != 0)
                    *p-- = '0';
            } else {
                do {
                    *p-- = '0' + u % 10;
                    u /= 10;
                } while (u);
            }
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            pfxlen = 0;
            goto emit;
        case 's':
            t = ap;
            ap = t + 1;
            s = *(const char **)t;
            if (s == 0) {
                put(ctx_a, (long *)&ctx_b, ctxv, '(');
                put(ctx_a, (long *)&ctx_b, ctxv, 'n');
                put(ctx_a, (long *)&ctx_b, ctxv, 'u');
                put(ctx_a, (long *)&ctx_b, ctxv, 'l');
                put(ctx_a, (long *)&ctx_b, ctxv, 'l');
                put(ctx_a, (long *)&ctx_b, ctxv, ')');
                goto advance;
            }
            if (flags & F_PREC) {
                const char *q = s;
                for (n = 0; n < prec; n++)
                    if (*q++ == 0)
                        break;
                len = n;
            } else {
                const char *e = s;
                while (*e)
                    e++;
                len = e - s;
            }
            start = s;
            pfxlen = 0;
            goto emit;
        case 'c':
            flags |= F_CHAR;
            p = &nbuf[31];
            t = ap;
            ap = t + 1;
            *p-- = ((u8 *)t)[7];
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            prec = 1;
            pfxlen = 0;
            if (flags & F_ZERO)
                flags = (flags & ~F_ZERO) | F_SPACE;
            goto emit;
        case 'p':
            q = (unsigned long *)ap;
            if (flags & F_LONG) {
                ap = (u8 **)(q + 1);
                u = *(unsigned long *)q;
            } else if (flags & F_SHORT) {
                ap = (u8 **)(q + 1);
                u = (unsigned short)((unsigned int *)q)[1];
            } else {
                ap = (u8 **)(q + 1);
                u = ((unsigned int *)q)[1];
            }
            p = &nbuf[31];
            if (u == 0) {
                if (!(flags & F_PREC) || prec != 0)
                    *p-- = '0';
            } else {
                digits = "0123456789abcdef";
                do {
                    *p-- = digits[u & 15];
                    u >>= 4;
                } while (u);
            }
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            pfxlen = 2;
            pfx[0] = '0';
            pfx[1] = 'x';
            goto emit;
        default:
            if (c == 0)
                continue;
            flags |= F_CHAR;
            p = &nbuf[31];
            *p-- = *fmt;
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            prec = 1;
            pfxlen = 0;
            goto emit;
        }
emit:
        end = start + len;
        zpad = 0;
        if (prec > len)
            zpad = prec - len;
        fpad = width - len - pfxlen - zpad;
        if (flags & F_CHAR) {
            if ((flags & F_ZERO) && fpad > 0)
                padc = '0';
            else
                padc = ' ';
        } else {
            if ((flags & (F_ZERO | F_PREC)) == F_ZERO && fpad > 0)
                padc = '0';
            else
                padc = ' ';
        }
        i = 0;
        if (flags & F_MINUS) {
            while ((int)i < pfxlen) {
                i++;
                count += put(ctx_a, (long *)&ctx_b, ctxv, pfx[i - 1]);
            }
            for (j = 0; j < zpad; j++)
                count += put(ctx_a, (long *)&ctx_b, ctxv, '0');
            while (start < end)
                count += put(ctx_a, (long *)&ctx_b, ctxv, *start++);
            for (k = 0; k < fpad; k++)
                count += put(ctx_a, (long *)&ctx_b, ctxv, ' ');
        } else if (flags & F_ZERO) {
            if (padc == '0') {
                while ((int)i < pfxlen) {
                    i++;
                    count += put(ctx_a, (long *)&ctx_b, ctxv, pfx[i - 1]);
                }
                for (j = 0; j < fpad; j++)
                    count += put(ctx_a, (long *)&ctx_b, ctxv, '0');
            } else {
                for (k = 0; k < fpad; k++)
                    count += put(ctx_a, (long *)&ctx_b, ctxv, padc);
                i = 0;
                while ((int)i < pfxlen) {
                    i++;
                    count += put(ctx_a, (long *)&ctx_b, ctxv, pfx[i - 1]);
                }
            }
            for (j = 0; j < zpad; j++)
                count += put(ctx_a, (long *)&ctx_b, ctxv, '0');
            while (start < end)
                count += put(ctx_a, (long *)&ctx_b, ctxv, *start++);
        } else {
            for (j = 0; j < fpad; j++)
                count += put(ctx_a, (long *)&ctx_b, ctxv, padc);
            i = 0;
            while ((int)i < pfxlen) {
                i++;
                count += put(ctx_a, (long *)&ctx_b, ctxv, pfx[i - 1]);
            }
            for (j = 0; j < zpad; j++)
                count += put(ctx_a, (long *)&ctx_b, ctxv, '0');
            while (start < end)
                count += put(ctx_a, (long *)&ctx_b, ctxv, *start++);
        }
        goto advance;
literal:
        count += put(ctx_a, (long *)&ctx_b, ctxv, *fmt);
advance:
        if (*fmt)
            fmt++;
    }
    put(ctx_a, (long *)&ctx_b, ctxv, 0);
    return count;
}

int snprintf_put_char(char **pbuf, unsigned int *prem, long ctxv, int c)
{
    int ret = 1;
    if (*prem != 0) {
        if (c <= 255) {
            if (*prem == 1)
                c = 0;
            **pbuf = c;
            (*pbuf)++;
            (*prem)--;
        } else {
            **pbuf = 0;
            ret = 0;
        }
    }
    return ret;
}

long lv0_snprintf(char *buf, unsigned long size, const char *format, ...)
{
    __builtin_va_list ap;
    char *cursor = buf;

    __builtin_va_start(ap, format);
    return lv0_vfprintf_engine((putc_fn)snprintf_put_char, (long)&cursor, size,
                               (const u8 *)format, (u8 **)ap);
}

long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, void *ap)
{
    char *cursor = buf;
    return lv0_vfprintf_engine((putc_fn)snprintf_put_char, (long)&cursor, size,
                               (const u8 *)fmt, (u8 **)ap);
}
