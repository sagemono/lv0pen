#ifndef SB_SS_ISO_LOG_H
#define SB_SS_ISO_LOG_H

#define SS_ISO_LOG_RESET()      do { } while (0)
#define SS_ISO_ERR(err, bits)   do { (err) |= (bits); } while (0)
#define SS_ISO_ERR2(err, bits, a, b) do { (err) |= (bits); } while (0)

#endif
