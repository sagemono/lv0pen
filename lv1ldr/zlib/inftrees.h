/* inftrees.h -- header to use inftrees.c
 * Copyright (C) 1995-2005 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
/* Altered from zlib 1.2.3: comments removed, changes for the loader. */

typedef struct {
    unsigned char op;
    unsigned char bits;
    unsigned short val;
} code;

#define ENOUGH 2048
#define MAXD 592

typedef enum {
    CODES,
    LENS,
    DISTS
} codetype;

extern int inflate_table OF((codetype type, unsigned short FAR *lens,
                             unsigned codes, code FAR * FAR *table,
                             unsigned FAR *bits, unsigned short FAR *work));
