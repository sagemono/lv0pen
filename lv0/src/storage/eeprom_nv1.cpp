#include "platform.h"
#include "storage.h"
#include "syscon.h"
#include "mmio.h"

int read_eeprom_nv1_u32_at4(unsigned int *out)
{
    long rc = load_nv_entry(1);
    if (!rc) *out = *(unsigned int *)(g_nv_entry_table[1].data.buf + 4);
    return rc;
}

int read_eeprom_nv1_u16_at2(unsigned short *out)
{
    long rc = load_nv_entry(1);
    if (!rc) *out = *(unsigned short *)(g_nv_entry_table[1].data.buf + 2);
    return rc;
}

int write_eeprom_nv1_u32_at12(unsigned int value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 12, 4, &value, entry->base);
    if (!rc)
        *(unsigned int *)(entry->data.buf + 12) = value;
    return rc;
}

int write_eeprom_nv1_u32_at4(unsigned int value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 4, 4, &value, entry->base);
    if (!rc)
        *(unsigned int *)(entry->data.buf + 4) = value;
    return rc;
}

int write_eeprom_nv1_u16_at2(unsigned short value)
{
    struct nv_entry *entry = &g_nv_entry_table[1];
    int rc = nv_storage::write(entry->block, entry->offset + 2, 2, &value, entry->base);
    if (!rc)
        *(unsigned short *)(entry->data.buf + 2) = value;
    return rc;
}

int finish_nv1_request(void *ctx, unsigned short done_bits, unsigned int result)
{
    unsigned short flags = 0;
    int rc = -99;

    get_syscon_device()->initialize(sb_mmio_base, 1, 0);
    if (read_eeprom_nv1_u16_at2(&flags) == 0
        && write_eeprom_nv1_u16_at2(flags & ~done_bits) == 0) {
        if (write_eeprom_nv1_u32_at12(result))
            return rc;
        return 0;
    }
    return rc;
}
