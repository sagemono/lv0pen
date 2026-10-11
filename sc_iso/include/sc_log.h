#ifndef SCISO_SC_LOG_H
#define SCISO_SC_LOG_H

#include "dma_log.h"

#define SC_ERR(err, bits) \
    do { \
        (err) |= (bits); \
        get_log()->put((unsigned int)(err), 16); \
        get_log()->puts("\n"); \
    } while (0)

#define SC_ERR_RET(err, bits, r) \
    do { \
        (err) |= (bits); \
        get_log()->put((unsigned int)(err), 16); \
        get_log()->puts(", "); \
        get_log()->put((int)(r), 10); \
        get_log()->puts("\n"); \
    } while (0)

#define SC_ERR_LIB(err, bits, r) \
    do { \
        (err) |= (bits); \
        get_log()->put((unsigned int)(err), 16); \
        get_log()->puts(", "); \
        get_log()->put((int)(r), 10); \
        get_log()->puts(", "); \
        get_log()->put(sc_lib_error(), 16); \
        get_log()->puts("\n"); \
    } while (0)

#endif
