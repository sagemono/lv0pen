#include "loader.h"
#include "mmio.h"
#include "util.h"

long ros_find_entry(u64 addr, u32 count, const char *name, ros_toc_entry *out)
{
    ros_toc_entry e[32];
    u64 at;
    u32 left;

    if (addr & 15)
        return -3;
    if (count == 0 || name[0] == '\0')
        return -2;
    at = addr;
    left = count;
    do {
        u32 n = left > 32 ? 32 : left;
        u32 i;

        copy_qwords_from_mmio(at, (char *)e, n * sizeof(ros_toc_entry));
        for (i = 0; i < n; i++) {
            if (strncmp(e[i].name, name, 32) == 0) {
                out->offset = e[i].offset;
                out->size = e[i].size;
                memcpy(out->name, e[i].name, 32);
                return 0;
            }
        }
        left -= n;
        at += n * sizeof(ros_toc_entry);
    } while (left);
    return -2;
}
