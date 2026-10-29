#include "loader.h"
#include "mmio.h"

long ros_read_header(u64 addr, ros_toc_header *hdr)
{
    ros_toc_header h;

    if (addr & 15)
        return -3;
    copy_qwords_from_mmio(addr, (char *)&h, sizeof(h));
    if (h.version != 1)
        return -1;
    *hdr = h;
    return 0;
}
