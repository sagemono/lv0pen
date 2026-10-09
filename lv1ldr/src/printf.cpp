#include "types.h"

typedef int (*putc_fn)(long ctx_a, long *state, long ctx_b, long ch);

#define F_MINUS 0x001
#define F_PLUS  0x002
#define F_ZERO  0x004
#define F_SPACE 0x008
#define F_ALT   0x010
#define F_PREC  0x020
#define F_LONG  0x040
#define F_SHORT 0x080
#define F_CHAR  0x100

long lv0_vfprintf_engine(putc_fn put, long ctx_a, unsigned int ctx_b,
                         const u8 *format, __builtin_va_list ap)
{
    int count = 0;
    long ctxv = ctx_b;
    const u8 *fmt = format;
    char nbuf[32];
    char pfx[32];
    const char *s;
    const char *start = 0, *end;
    char *p;
    int len;
    int c, w, n;
    const char *digits;
    u64 u;
    int pfxlen, prec, width, flags;
    s64 v;
    int zpad, fpad;
    char padc;
    int i;
    char *bufend;
    int j, k;

    while (*fmt) {
        flags = 0;
        width = 0;
        prec = 0;
        if (*fmt != '%')
            goto literal;
parse:
        c = (char)*++fmt;
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
                w = w * 10 + (char)*fmt - '0';
            } while ((u8)(*++fmt - '0') <= 9);
            if (*fmt == 0)
                continue;
            fmt--;
            width = w;
            goto parse;
        case '*':
            w = __builtin_va_arg(ap, int);
            width = w < 0 ? -1 : w;
            goto parse;
        case '.':
            flags |= F_PREC;
            w = 0;
            if (*++fmt == '*') {
                w = __builtin_va_arg(ap, int);
                prec = w < 0 ? -1 : w;
                goto parse;
            }
            while ((u8)(*fmt - '0') <= 9) {
                w = w * 10 + (char)*fmt - '0';
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
            if (flags & F_LONG)
                u = __builtin_va_arg(ap, u64);
            else if (flags & F_SHORT)
                u = (unsigned short)__builtin_va_arg(ap, unsigned int);
            else
                u = __builtin_va_arg(ap, unsigned int);
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
            if (flags & F_LONG)
                u = __builtin_va_arg(ap, u64);
            else if (flags & F_SHORT)
                u = (unsigned short)__builtin_va_arg(ap, unsigned int);
            else
                u = __builtin_va_arg(ap, unsigned int);
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
            if (flags & F_LONG)
                v = __builtin_va_arg(ap, s64);
            else if (flags & F_SHORT)
                v = (short)__builtin_va_arg(ap, int);
            else
                v = __builtin_va_arg(ap, int);
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
                if (v == (s64)0x8000000000000000ULL) {
                    *p-- = '8';
                    v = 922337203685477580LL;
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
            goto emit;
        case 'u':
            if (flags & F_LONG)
                u = __builtin_va_arg(ap, u64);
            else if (flags & F_SHORT)
                u = (unsigned short)__builtin_va_arg(ap, unsigned int);
            else
                u = __builtin_va_arg(ap, unsigned int);
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
            s = __builtin_va_arg(ap, const char *);
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
                for (n = 0; n < prec; n++)
                    if (s[n] == 0)
                        break;
                len = n;
            } else {
                for (len = 0; s[len]; len++)
                    ;
            }
            start = s;
            pfxlen = 0;
            goto emit;
        case 'c':
            flags |= F_CHAR;
            p = &nbuf[31];
            u = __builtin_va_arg(ap, unsigned int);
            c = (s64)(u << 56) >> 56;
            *p-- = c;
            bufend = &nbuf[31];
            start = p + 1;
            len = bufend - p;
            prec = 1;
            pfxlen = 0;
            if (flags & F_ZERO)
                flags = (flags & ~F_ZERO) | F_SPACE;
            goto emit;
        case 'p':
            if (flags & F_LONG)
                u = __builtin_va_arg(ap, u64);
            else if (flags & F_SHORT)
                u = (unsigned short)__builtin_va_arg(ap, unsigned int);
            else
                u = __builtin_va_arg(ap, unsigned int);
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
            while (i < pfxlen) {
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
                while (i < pfxlen) {
                    i++;
                    count += put(ctx_a, (long *)&ctx_b, ctxv, pfx[i - 1]);
                }
                for (j = 0; j < fpad; j++)
                    count += put(ctx_a, (long *)&ctx_b, ctxv, '0');
            } else {
                for (k = 0; k < fpad; k++)
                    count += put(ctx_a, (long *)&ctx_b, ctxv, padc);
                i = 0;
                while (i < pfxlen) {
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
            while (i < pfxlen) {
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
        count += put(ctx_a, (long *)&ctx_b, ctxv, (char)*fmt);
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

long lv0_vsnprintf(char *buf, unsigned long size, const char *fmt, __builtin_va_list ap)
{
    char *cursor = buf;
    return lv0_vfprintf_engine((putc_fn)snprintf_put_char, (long)&cursor, size,
                               (const u8 *)fmt, ap);
}
