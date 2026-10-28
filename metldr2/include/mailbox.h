#ifndef MLDR_MAILBOX_H
#define MLDR_MAILBOX_H

#include "types.h"

unsigned int mbox_out_space(void);

long mbox_out_write(unsigned int v);
long mbox_out_intr_write(unsigned int v);

long mbox_in_read(unsigned int *v);

long mbox_in_read64(u64 *v);

#endif
