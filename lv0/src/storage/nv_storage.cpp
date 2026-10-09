#include "syscon.h"
#include "platform.h"
#include "log.h"
#include "storage.h"

unsigned long g_unused_41d40 = 0x0100000000000000;
unsigned char g_sc_nv_storage_version_initialized;
unsigned short g_sc_nv_storage_version;

static unsigned char nv_buf_48000[19];
static unsigned char nv_buf_48800[16];
static unsigned char nv_buf_48c00[32];
static unsigned char nv_buf_48c22[3];
static unsigned char nv_buf_48c30[13];
static unsigned char nv_buf_48c40[16];
static unsigned char nv_buf_48c80[16];
static unsigned char nv_buf_48c90[48];
static unsigned char nv_buf_48d00[12];
static unsigned char nv_buf_48d20[8];
static unsigned char nv_buf_48d3e[80];
static unsigned char nv_buf_48d28[24];

struct nv_entry g_nv_entry_table[12] = {
    { 0, 0x00, 0x12, 0x48000, { nv_buf_48000, 19, 0 } },
    { 1, 0x00, 0x0F, 0x48800, { nv_buf_48800, 16, 0 } },
    { 2, 0x00, 0x1F, 0x48C00, { nv_buf_48c00, 32, 0 } },
    { 2, 0x22, 0x24, 0x48C00, { nv_buf_48c22, 3, 0 } },
    { 2, 0x30, 0x3C, 0x48C00, { nv_buf_48c30, 13, 0 } },
    { 2, 0x40, 0x4F, 0x48C00, { nv_buf_48c40, 16, 0 } },
    { 2, 0x80, 0x8F, 0x48C00, { nv_buf_48c80, 16, 0 } },
    { 2, 0x90, 0xBF, 0x48C00, { nv_buf_48c90, 48, 0 } },
    { 3, 0x00, 0x0B, 0x48D00, { nv_buf_48d00, 12, 0 } },
    { 3, 0x20, 0x27, 0x48D00, { nv_buf_48d20, 8, 0 } },
    { 3, 0x3E, 0x8D, 0x48D00, { nv_buf_48d3e, 80, 0 } },
    { 3, 0x28, 0x3F, 0x48D00, { nv_buf_48d28, 24, 0 } },
};

bool is_sc_nv_storage_supported(void)
{
    if (g_sc_nv_storage_version_initialized == 0) {
        int ret = get_sc_version(SC_CMD_NV_STORAGE, &g_sc_nv_storage_version);
        if (ret != 0) {
            log_message("[WARN]: sc_version nvs fail %d\n", ret);
            return 0;
        }
        g_sc_nv_storage_version_initialized = 1;
    }
    return g_sc_nv_storage_version > 0x102;
}

int nv_storage::read(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base)
{
    unsigned int len;
    int ret = -8;

    if (is_sc_nv_storage_supported()) {
        ret = sc_nv_storage::read(block, offset, size, buf, &len);
        if (ret)
            log_message("[ERROR]: nv_storage::read block %d, offset %d, size %d fail %d\n", block, offset, size, ret);
    }
    return ret;
}

int load_nv_entry(unsigned int idx)
{
    nv_entry *entry = &g_nv_entry_table[(int)idx];
    nv_entry_buf *desc = &entry->data;
    int rc = 0;
    if (!desc->loaded) {
        rc = nv_storage::read(g_nv_entry_table[(int)idx].block, entry->offset, desc->size, desc->buf, entry->base);
        if (!rc)
            desc->loaded = 1;
    }
    return rc;
}

int nv_storage::write(unsigned int block, unsigned int offset, unsigned int size, void *buf, unsigned int base)
{
    int ret = -8;

    if (is_sc_nv_storage_supported()) {
        ret = sc_nv_storage::write(block, offset, size, buf);
        if (ret)
            log_message("[ERROR]: nv_storage::write block %d, offset %d, size %d fail %d\n", block, offset, size, ret);
    }
    return ret;
}
