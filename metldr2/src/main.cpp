#include "loader.h"
#include "mailbox.h"
#include "util.h"

extern const unsigned char ls_bottom_0[32] = {
    0x37, 0xd5, 0xca, 0x34, 0x1a, 0x61, 0xe2, 0x29,
    0xb2, 0x7a, 0xb0, 0xf4, 0xb3, 0x0f, 0xd2, 0x94,
    0xb6, 0x79, 0x06, 0x06, 0xbd, 0xb9, 0x30, 0xd4,
    0x9a, 0x1c, 0xee, 0x47, 0xd8, 0xad, 0x3b, 0xbf,
};
extern const unsigned char ls_bottom_32[16] = {
    0xc3, 0x61, 0xb3, 0xa3, 0x20, 0x98, 0x09, 0x36,
    0xf9, 0x72, 0xe9, 0x51, 0x5c, 0x79, 0x82, 0xe2,
};
extern const u64 ls_bottom_48 = 0x0003006500000000ULL;

extern "C" void exit_stop(void);
extern "C" unsigned int g_exit_status;

int main(void)
{
    unsigned int entry;
    long rc;

    rc = g_loader.run(&entry);
    if (rc == 0) {
        while (mbox_out_space() == 0)
            ;
        memcpy((void *)0, ls_bottom_0, 32);
        memcpy((void *)32, ls_bottom_32, 16);
        memcpy((void *)48, &ls_bottom_48, 8);
        leave(entry);
    } else {
        memset((void *)0, 0, 0x400);
        g_exit_status = rc;
        leave((unsigned int)exit_stop);
        for (;;)
            ;
    }
    return 0;
}
