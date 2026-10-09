#include "loader.h"
#include "mailbox.h"
#include "util.h"
#include <spu_intrinsics.h>

extern const unsigned char ls_bottom_0[32] = {
    0xf7, 0x93, 0xd8, 0xb1, 0xd4, 0x12, 0x9a, 0xd9,
    0x0c, 0x20, 0x75, 0xc5, 0x20, 0x14, 0x3f, 0x27,
    0xc2, 0x8a, 0x2d, 0xe1, 0x3a, 0x93, 0xb4, 0xc6,
    0xea, 0x9a, 0x05, 0x30, 0xb7, 0xd3, 0x5d, 0x0a,
};
extern const unsigned char ls_bottom_32[16] = {
    0xdd, 0x8e, 0x36, 0xef, 0x54, 0x25, 0x64, 0x31,
    0x81, 0xc2, 0x64, 0x66, 0xea, 0x66, 0xd9, 0x6b,
};

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
