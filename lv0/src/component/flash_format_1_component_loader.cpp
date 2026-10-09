#include "component.h"
#include "config.h"
#include "memory.h"
#include "lv0.h"
#include "storage.h"
#include "log.h"

struct ros_region_header {
    unsigned long table;
    char pad_8[24];
};

struct composite_region_header {
    unsigned int count;
    char pad_4[12];
};

struct composite_region_entry {
    unsigned int offset;
    unsigned int size;
    unsigned long id;
};

flash_format_1_component_loader::flash_format_1_component_loader(long dev_index, const char *ros_name,
                                                                 const char *asecure_loader_name,
                                                                 const embedded_component *embedded,
                                                                 unsigned int embedded_count,
                                                                 unsigned long flash_base)
    : dev_index(dev_index), ros_name(ros_name), asecure_loader_name(asecure_loader_name),
      embedded(embedded), embedded_count(embedded_count), flash_base(flash_base)
{
}

flash_format_1_component_loader::~flash_format_1_component_loader()
{
}

int flash_format_1_component_loader::read_component_data(long dev_index, long offset, unsigned long len,
                                                          void *buf, unsigned long cap)
{
    unsigned long nread;

    if (cap < len)
        return -10;
    if (g_storage.read(dev_index, offset, len, (char *)buf, &nread))
        return 3;
    if (nread == len)
        return 0;
    return 3;
}

int flash_format_1_component_loader::read_region_table_header(long dev_index, long offset,
                                                               region_table_header *hdr)
{
    unsigned long nread;

    if (g_storage.read(dev_index, offset, sizeof *hdr, (char *)hdr, &nread) != 0)
        return 3;
    if (nread != sizeof *hdr)
        return 3;
    return (hdr->version != 1) ? -51 : 0;
}

int flash_format_1_component_loader::read_composite_region_header(long dev_index, long offset,
                                                                   composite_region_header *hdr)
{
    unsigned long nread;
    long rc = g_storage.read(dev_index, offset, sizeof *hdr, (char *)hdr, &nread);

    if (rc != 0 || nread != sizeof *hdr) {
        log_message("read_composite_region_header: result %d\n", rc);
        log_message("read_composite_region_header: read_size %lld\n", nread);
        return 3;
    }
    return 0;
}

int flash_format_1_component_loader::find_composite_region_entry(long dev_index, long addr, unsigned int count,
                                                                  long key, struct composite_region_entry *ent)
{
    unsigned long nread;
    unsigned int i;

    for (i = 0; i < count; i++) {
        long read = g_storage.read(dev_index, addr, sizeof *ent, (char *)ent, &nread);
        if (read != 0 || nread != sizeof *ent)
            return 3;
        if (ent->id == key)
            return 0;
        addr += sizeof *ent;
    }
    return -55;
}

int flash_format_1_component_loader::find_region_table_entry(long dev_index, long addr, unsigned int count,
                                                              const char *name, region_table_entry *entry)
{
    unsigned long nread;
    unsigned int i = 0;

    while (i < count) {
        long read = g_storage.read(dev_index, addr, sizeof *entry, (char *)entry, &nread);
        if (read != 0 || nread != sizeof *entry)
            return 3;
        if (lv0_strncmp(entry->name, name, sizeof entry->name) == 0)
            return 0;
        addr += sizeof *entry;
        i++;
    }
    return -55;
}

int flash_format_1_component_loader::load_component_from_rom_region(const char *region_name, long id,
                                                                    memory_budget *budget, unsigned long max_size,
                                                                    unsigned long align, unsigned long *out_addr,
                                                                    unsigned long *out_size)
{
    struct region_table_header table;
    struct composite_region_header header;
    struct region_table_entry region;
    struct composite_region_entry entry;
    int rc = 2;
    long region_off;
    void *buf;
    unsigned long base = flash_base;
    long used, free;

    if (!g_storage.open(dev_index)) {
        rc = read_region_table_header(dev_index, base + 0x200, &table);
        if (!rc) {
            rc = find_region_table_entry(dev_index, base + 0x210, table.count, region_name,
                                         &region);
            if (!rc) {
                region_off = base + 0x200 + region.offset;
                rc = read_composite_region_header(dev_index, region_off, &header);
                if (!rc) {
                    rc = find_composite_region_entry(dev_index, region_off + sizeof header,
                                                     header.count, id, &entry);
                    if (!rc) {
                        buf = budget->allocate(entry.size, align);
                        if (!buf) {
                            rc = -10;
                            budget->get_used_size(&used);
                            budget->get_free_size(&free);
                            log_message("%s(%d): allocation failure. size 0x%x, align 0x%x, alloc 0x%x, free 0x%x\n",
                                        __FUNCTION__, 512, entry.size, align, used, free);
                        } else {
                            rc = read_component_data(dev_index, region_off + entry.offset,
                                                     entry.size, buf, max_size);
                            if (rc) {
                                if (rc == -10)
                                    log_message("load_components %lld too large.\n", id);
                            } else {
                                *out_addr = (unsigned long)buf;
                                *out_size = entry.size;
                                g_storage.close(dev_index);
                                return rc;
                            }
                        }
                    }
                }
            }
        }
        g_storage.close(dev_index);
    }
    return rc;
}

int flash_format_1_component_loader::load_component_from_bank(const char *region_name, const char *name,
                                                               memory_budget *budget, unsigned int region,
                                                               unsigned long max_size, unsigned long align,
                                                               unsigned long *out_addr, unsigned long *out_size)
{
    struct region_table_header table;
    struct ros_region_header ros;
    struct region_table_entry entry;
    int rc = 2;
    long region_off, table_off;
    unsigned long base = flash_base;
    void *buf;
    unsigned long nread;
    long used;
    long free;

    if (!g_storage.open(dev_index)) {
        rc = read_region_table_header(dev_index, base + 0x200, &table);
        if (!rc) {
            rc = find_region_table_entry(dev_index, base + 0x210, table.count, region_name,
                                         &entry);
            if (!rc) {
                region_off = base + 0x200 + entry.offset;
                long read = g_storage.read(dev_index, region_off, sizeof ros, (char *)&ros, &nread);
                if (read || nread != sizeof ros) {
                    rc = 3;
                } else {
                    table_off = region_off + 16;
                    table_off += ros.table;
                    rc = read_region_table_header(dev_index, table_off, &table);
                    if (!rc) {
                        rc = find_region_table_entry(dev_index, table_off + sizeof table,
                                                     table.count, name, &entry);
                        if (!rc) {
                            buf = budget->allocate(entry.size, align);
                            if (!buf) {
                                rc = -10;
                                budget->get_used_size(&used);
                                budget->get_free_size(&free);
                                log_message("%s(%d): allocation failure. size 0x%x, align 0x%x, alloc 0x%x, free 0x%x\n",
                                            __FUNCTION__, 397, entry.size, align, used, free);
                            } else {
                                rc = read_component_data(dev_index,
                                                         table_off + entry.offset, entry.size,
                                                         buf, max_size);
                                if (rc) {
                                    if (rc == -10)
                                        log_message("load_components %s too large.\n", name);
                                } else {
                                    *out_addr = (unsigned long)buf;
                                    *out_size = entry.size;
                                    g_storage.close(dev_index);
                                    return rc;
                                }
                            }
                        }
                    }
                }
            }
        }
        g_storage.close(dev_index);
    }
    return rc;
}

int flash_format_1_component_loader::load_component_from_region(const char *region_name, const char *name,
                                                                 memory_budget *budget, unsigned long max_size,
                                                                 unsigned long align, unsigned long *out_addr,
                                                                 unsigned long *out_size)
{
    struct region_table_header table;
    struct region_table_entry entry;
    int rc = 2;
    long region_off;
    void *buf;
    unsigned long base = flash_base;
    long used, free;

    if (!g_storage.open(dev_index)) {
        rc = read_region_table_header(dev_index, base + 0x200, &table);
        if (!rc) {
            rc = find_region_table_entry(dev_index, base + 0x210, table.count, region_name,
                                         &entry);
            if (!rc) {
                region_off = base + 0x200 + entry.offset;
                rc = read_region_table_header(dev_index, region_off, &table);
                if (!rc) {
                    rc = find_region_table_entry(dev_index, region_off + sizeof table,
                                                 table.count, name, &entry);
                    if (!rc) {
                        buf = budget->allocate(entry.size, align);
                        if (!buf) {
                            rc = -10;
                            budget->get_used_size(&used);
                            budget->get_free_size(&free);
                            log_message("%s(%d): allocation failure. size 0x%x, align 0x%x, alloc 0x%x, free 0x%x\n",
                                        __FUNCTION__, 252, entry.size, align, used, free);
                        } else {
                            rc = read_component_data(dev_index, region_off + entry.offset,
                                                     entry.size, buf, max_size);
                            if (rc) {
                                if (rc == -10)
                                    log_message("load_components %s too large.\n", name);
                            } else {
                                *out_addr = (unsigned long)buf;
                                *out_size = entry.size;
                                g_storage.close(dev_index);
                                return rc;
                            }
                        }
                    }
                }
            }
        }
        g_storage.close(dev_index);
    }
    return rc;
}

int flash_format_1_component_loader::load_component(unsigned int component_id, const char *name,
                                                    memory_budget *budget, unsigned int region, unsigned long max_size,
                                                    unsigned long align, unsigned long *out_addr,
                                                    unsigned long *out_size)
{
    switch (component_id) {
    case COMPONENT_LV1LDR: case COMPONENT_LV2LDR: case COMPONENT_APPLDR: case COMPONENT_ISOLDR:
        return load_component_from_memory(component_id, budget, max_size, align, out_addr, out_size);
    case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 13:
        return load_component_from_bank(ros_name, name, budget, region, max_size, align, out_addr, out_size);
    case COMPONENT_METLDR:
    case COMPONENT_METLDR_2:
        return load_component_from_region(asecure_loader_name, name, budget, max_size, align, out_addr,
                                          out_size);
    case COMPONENT_EID0:
        return load_component_from_rom_region("eEID", 0, budget, max_size, align, out_addr, out_size);
    default:
        log_message("flash_format_1_component_loader: unknown component_id=%d\n", component_id);
        return -55;
    }
}

int flash_format_1_component_loader::find_embedded_component(unsigned int id, const embedded_component **out)
{
    if (embedded) {
        for (unsigned int i = 0; i < embedded_count; i++) {
            if (embedded[i].id == id) {
                *out = &embedded[i];
                return 0;
            }
        }
    }
    return -55;
}

int flash_format_1_component_loader::load_2nd_component(unsigned int id, void *dst, unsigned long size)
{
    const embedded_component *ec;

    int rc = find_embedded_component(id, &ec);
    if (rc == 0) {
        if (ec->size > size)
            rc = -10;
        else
            lv0_memmove((char *)dst, (const char *)ec->start, ec->size);
    }
    return rc;
}

int flash_format_1_component_loader::load_component_from_memory(unsigned int id, memory_budget *budget,
                                                                unsigned long max_size, unsigned long align,
                                                                unsigned long *out_addr, unsigned long *out_size)
{
    const embedded_component *ec;
    long used, free;

    int rc = find_embedded_component(id, &ec);
    if (rc == 0) {
        if (ec->size > max_size) {
            rc = -10;
        } else {
            void *buf = budget->allocate(ec->size, align);
            if (!buf) {
                rc = -10;
                budget->get_used_size(&used);
                budget->get_free_size(&free);
                log_message("%s(%d): allocation failure. size 0x%x, align 0x%x, alloc 0x%x, free 0x%x\n",
                            __FUNCTION__, 736, ec->size, align, used, free);
            } else {
                lv0_memmove((char *)buf, (const char *)ec->start, ec->size);
                *out_addr = (unsigned long)buf;
                *out_size = ec->size;
            }
        }
    }
    return rc;
}

int flash_format_1_component_loader::load_component_to(unsigned int id, const char *name, void *dst,
                                                       unsigned long size)
{
    if (id - COMPONENT_LV1LDR_2 <= COMPONENT_ISOLDR_2 - COMPONENT_LV1LDR_2)
        return load_2nd_component(id, dst, size);
    log_message("flash_format_1_component_loader: unsupported component_id=%d\n", id);
    return -55;
}

int flash_bank_component_loader::load_component_from_bank(const char *region_name, const char *name,
                                                          memory_budget *budget, unsigned int region,
                                                          unsigned long max_size, unsigned long align,
                                                          unsigned long *out_addr, unsigned long *out_size)
{
    unsigned long addr;
    struct region_table_header table;
    struct region_table_entry entry;
    int ret = 2;
    long region_off;
    void *buf;

    if (!g_storage.open(bank_dev_index)) {
        int rc = read_region_table_header(bank_dev_index, 0x400, &table);
        if (!rc) {
            const char *bank;
            if (get_eeprom_core_os_bank_indicator() == 0xFF)
                bank = bank0_name;
            else
                bank = bank1_name;
            rc = find_region_table_entry(bank_dev_index, 0x410, table.count, bank, &entry);
            if (!rc) {
                region_off = entry.offset + 0x410;
                rc = read_region_table_header(bank_dev_index, region_off, &table);
                if (!rc) {
                    ret = find_region_table_entry(bank_dev_index, region_off + sizeof table, table.count, name, &entry);
                    if (!ret) {
                        addr = 0;
                        if (region == 3) {
                            if (g_storage.get_mapped_address(bank_dev_index, region_off + entry.offset, &addr))
                                addr = 0;
                            else if (addr && !(addr & (align - 1)))
                                goto found;
                        }
                        buf = budget->allocate(entry.size, align);
                        if (buf) {
                            addr = (unsigned long)buf;
                            rc = read_component_data(bank_dev_index, region_off + entry.offset, entry.size,
                                                     buf, max_size);
                            if (rc) {
                                if (rc == -10)
                                    log_message("load_components %s too large.\n", name);
                            } else {
                            found:
                                *out_addr = addr;
                                *out_size = entry.size;
                                g_storage.close(bank_dev_index);
                                return ret;
                            }
                        } else {
                            rc = -10;
                        }
                    } else {
                        rc = ret;
                    }
                }
            }
        }
        ret = rc;
        g_storage.close(bank_dev_index);
    }
    return ret;
}

flash_bank_component_loader::flash_bank_component_loader(long dev_index, const char *ros_name,
                                                         const char *asecure_loader_name,
                                                         const embedded_component *embedded,
                                                         unsigned int embedded_count, const char *bank0_name,
                                                         const char *bank1_name)
    : flash_format_1_component_loader(dev_index, ros_name, asecure_loader_name, embedded, embedded_count, 0x200),
      bank_dev_index(dev_index), bank0_name(bank0_name), bank1_name(bank1_name),
      bank_asecure_loader_name(asecure_loader_name)
{
}
