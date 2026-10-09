/* inffast.h -- header to use inffast.c
 * Copyright (C) 1995-2003 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
/* Altered from zlib 1.2.3: comments removed, changes for the loader. */

void inflate_fast OF((z_streamp strm, unsigned start));
