#include "syscon.h"
#include "storage.h"
#include "util.h"

unsigned char g_sc_nv_storage_version_initialized;
unsigned short g_sc_nv_storage_version;

static unsigned char nv_buf_48000[19];
static unsigned char nv_buf_48800[1];
static unsigned char nv_buf_48c00[19];
static unsigned char nv_buf_48c22[2];
static unsigned char nv_buf_48c30[1];
static unsigned char nv_buf_48c40[16];
static unsigned char nv_buf_48c80[64];

struct nv_entry g_nv_entry_table[7] = {
    { 0, 0x00, 0x12, 0x48000, { nv_buf_48000, 19, 0 } },
    { 1, 0x00, 0x00, 0x48800, { nv_buf_48800, 1, 0 } },
    { 2, 0x00, 0x12, 0x48C00, { nv_buf_48c00, 19, 0 } },
    { 2, 0x22, 0x23, 0x48C00, { nv_buf_48c22, 2, 0 } },
    { 2, 0x30, 0x30, 0x48C00, { nv_buf_48c30, 1, 0 } },
    { 2, 0x40, 0x4F, 0x48C00, { nv_buf_48c40, 16, 0 } },
    { 2, 0x80, 0xBF, 0x48C00, { nv_buf_48c80, 64, 0 } },
};

bool is_sc_nv_storage_supported(void)
{
    if (g_sc_nv_storage_version_initialized == 0) {
        long ret = get_sc_version(20, &g_sc_nv_storage_version);
        if (ret != 0) {
            log_message("[WARN]: sc_version nvs fail %d\n", ret);
            return 0;
        }
        g_sc_nv_storage_version_initialized = 1;
    }
    if (g_sc_nv_storage_version > 0x102)
        return true;
    return false;
}

long nv_storage::read(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base)
{
    unsigned int len;
    long ret;
    if (is_sc_nv_storage_supported()) {
        ret = sc_nv_storage::read(block, offset, size, buf, &len);
        if (ret) {
            log_message("[ERROR]: nv_storage::read block %d, offset %d, size %d fail %d\n", block, offset, size, ret);
            return ret;
        }
    } else {
        unsigned int addr = base + offset;
        ret = sc_dev_access::read(addr, buf, size);
        if (ret) {
            log_message("[ERROR]: sc_dev_access::read 0x%08x fail %d\n", addr, ret);
            return ret;
        }
    }
    return 0;
}

long load_nv_entry(unsigned int idx)
{
    long rc = 0;
    if (!g_nv_entry_table[idx].data.loaded) {
        rc = nv_storage::read(g_nv_entry_table[idx].block, g_nv_entry_table[idx].offset,
                              g_nv_entry_table[idx].data.size, g_nv_entry_table[idx].data.buf,
                              g_nv_entry_table[idx].base);
        if (!rc)
            g_nv_entry_table[idx].data.loaded = true;
    }
    return rc;
}

long nv_storage::write(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base)
{
    long ret;
    if (is_sc_nv_storage_supported()) {
        ret = sc_nv_storage::write(block, offset, size, buf);
        if (ret) {
            log_message("[ERROR]: nv_storage::write block %d, offset %d, size %d fail %d\n", block, offset, size, ret);
            return ret;
        }
    } else {
        unsigned int addr = base + offset;
        ret = sc_dev_access::write(addr, buf, size);
        if (ret) {
            log_message("[ERROR]: sc_dev_access::write 0x%08x fail %d\n", addr, ret);
            return ret;
        }
    }
    return 0;
}
