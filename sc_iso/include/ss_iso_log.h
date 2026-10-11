#ifndef SCISO_SS_ISO_LOG_H
#define SCISO_SS_ISO_LOG_H

#include "dma_log.h"

#define SS_ISO_LOG_RESET()      get_log()->reset()
#define SS_ISO_ERR(err, bits) \
    do { \
        (err) |= (bits); \
        get_log()->put((unsigned int)(err), 16); \
        get_log()->puts("\n"); \
    } while (0)
#define SS_ISO_ERR2(err, bits, a, b) \
    do { \
        (err) |= (bits); \
        get_log()->put((unsigned int)(err), 16); \
        get_log()->puts(", "); \
        get_log()->put((u64)(a), 10); \
        get_log()->puts(", "); \
        get_log()->put((u64)(b), 10); \
        get_log()->puts("\n"); \
    } while (0)

#endif
