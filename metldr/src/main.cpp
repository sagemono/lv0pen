#include "loader.h"
#include "mailbox.h"
#include "util.h"
#include <spu_intrinsics.h>

extern const unsigned char ls_bottom_0[32];
extern const unsigned char ls_bottom_32[16];

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
        leave(entry);
    } else {
        switch (rc) {
        case 35:
            spu_stop(0x23);
            break;
        case 31:
            spu_stop(0x1F);
            break;
        case 32:
            spu_stop(0x20);
            break;
        case 34:
            spu_stop(0x22);
            break;
        case 33:
            spu_stop(0x21);
            break;
        case 30:
            spu_stop(0x1E);
            break;
        case 36:
            spu_stop(0x24);
            break;
        default:
            spu_stop(0x15);
            break;
        }
    }
    return 0;
}
