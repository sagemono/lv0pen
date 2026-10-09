#ifndef MLDR_MAILBOX_H
#define MLDR_MAILBOX_H

#include "types.h"

unsigned int mbox_in_count(void);

void mbox_in_drain(void);

long mbox_out_write(unsigned int v);
long mbox_out_intr_write(unsigned int v);

long mbox_in_read(unsigned int *v);

#endif
