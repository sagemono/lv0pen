#include "storage.h"
#include "intrinsics.h"

storage g_storage;

int storage::get_size(unsigned long dev_index, unsigned long *out_size)
{
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();
    int rc = -11;

    if (count > dev_index) {
        storage_device *dev = table[dev_index];
        rc = -12;
        if (dev->state != DEVICE_RESERVED)
            return dev->get_size(out_size);
    }
    return rc;
}

unsigned long storage::get_device_count()
{
    return g_storage_manager_ptr->get_device_count();
}

int storage::get_mapped_address(unsigned long dev_index, unsigned long offset, unsigned long *out_addr)
{
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    if (count > dev_index) {
        return table[dev_index]->get_mapped_address(offset, out_addr);
    }
    return -11;
}

int storage::get_block_size(long dev_index, unsigned long *block_size)
{
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    if (count > dev_index) {
        return table[dev_index]->get_block_size(block_size);
    }
    return -11;
}

long storage::reserve(long dev_index)
{
    long rc = -11;
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    spin_lock_guard lock;
    if (count > dev_index) {
        unsigned int state = table[dev_index]->state;
        rc = 0;
        if (state != 2) {
            rc = -13;
            if (state != 1) {
                table[dev_index]->state = 2;
                lock.release();
                rc = 0;
            }
        }
    }
    return rc;
}

int storage::read(unsigned long dev_index, unsigned long offset, unsigned long size, char *buf, unsigned long *bytes_read)
{
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    if (count > dev_index) {
        storage_device *dev = table[dev_index];
        if (dev->state == DEVICE_OPEN) {
            if (!size) {
                *bytes_read = size;
                return 0;
            }
            return dev->read(offset, size, buf, bytes_read);
        }
        return -15;
    }
    return -11;
}

long storage::write(unsigned long dev_index, unsigned long offset, unsigned long size, const char *src,
                    unsigned long *bytes_written)
{
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    if (count > dev_index) {
        storage_device *dev = table[dev_index];
        if (dev->state == 1) {
            if (!size) {
                *bytes_written = size;
                return 0;
            }
            return dev->write(offset, size, src, bytes_written);
        }
        return -15;
    }
    return -11;
}

int storage::open(unsigned long dev_index)
{
    int rc = -11;
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    spin_lock_guard lock;
    if (count > dev_index) {
        unsigned int state = table[dev_index]->state;
        rc = -13;
        if (state != DEVICE_OPEN) {
            rc = -12;
            if (state != DEVICE_RESERVED) {
                rc = 0;
                table[dev_index]->state = DEVICE_OPEN;
                lock.release();
                if (table[dev_index]->open()) {
                    lock.acquire();
                    rc = -99;
                    table[dev_index]->state = DEVICE_CLOSED;
                }
            }
        }
    }
    return rc;
}

int storage::close(unsigned long dev_index)
{
    int rc = -11;
    unsigned long count = g_storage_manager_ptr->get_device_count();
    storage_device **table = g_storage_manager_ptr->get_device_table();

    spin_lock_guard lock;
    if (count > dev_index) {
        unsigned int state = table[dev_index]->state;
        rc = -12;
        if (state == DEVICE_CLOSED)
            goto zero;
        if (state != DEVICE_RESERVED) {
            rc = -99;
            table[dev_index]->state = DEVICE_CLOSED;
            lock.release();
            if (table[dev_index]->close()) {
                lock.acquire();
                table[dev_index]->state = DEVICE_OPEN;
                goto out;
            }
zero:
            rc = 0;
        }
    }
out:
    return rc;
}
