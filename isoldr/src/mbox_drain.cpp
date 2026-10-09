#include "mailbox.h"

void mbox_in_drain(void)
{
    unsigned int v;

    while (mbox_in_count())
        mbox_in_read(&v);
}
