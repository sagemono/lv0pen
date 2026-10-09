#include "spansion_storage.h"
#include "memory.h"

mapped_storage::mapped_storage() : base(0)
{
}

void mapped_storage::set_base(unsigned long base)
{
    this->base = base;
}

int mapped_storage::open()
{
    return 0;
}

int mapped_storage::close()
{
    return 0;
}

long mapped_storage::get_mapped_address(unsigned long offset, unsigned long *out_addr)
{
    unsigned long size;
    int rc = get_size(&size);

    if (rc == 0) {
        if (size <= offset)
            rc = -14;
        else if (offset < FLASH_ROTATE)
            *out_addr = size + base + offset - FLASH_ROTATE;
        else
            *out_addr = offset + base - FLASH_ROTATE;
    }
    return rc;
}

int mapped_storage::read(unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read)
{
    unsigned long dev_size;
    int rc = get_size(&dev_size);

    if (rc == 0) {
        if (dev_size <= offset) {
            rc = -14;
        } else {
            unsigned long n = size;
            if (n > dev_size - offset)
                n = dev_size - offset;
            unsigned long end = offset + n;
            if (end < FLASH_ROTATE) {
                lv0_memmove(buf, (const char *)(dev_size + base + offset - FLASH_ROTATE), n);
            } else if (offset >= FLASH_ROTATE) {
                lv0_memmove(buf, (const char *)(offset + base - FLASH_ROTATE), n);
            } else {
                lv0_memmove(buf, (const char *)(dev_size + base + offset - FLASH_ROTATE), FLASH_ROTATE - offset);
                lv0_memmove(buf - offset + FLASH_ROTATE, (const char *)base, end - FLASH_ROTATE);
            }
            *bytes_read = n;
        }
    }
    return rc;
}
