#include "component.h"
#include "memory.h"

componnet_manager::componnet_manager(component_record *records, int count, component_alt_record *alt_records,
                                     int alt_count)
    : records(records), count(count), alt_records(alt_records), alt_count(alt_count), loader(0),
      initialized(false)
{
}

componnet_manager::~componnet_manager()
{
}

int componnet_manager::find_record(unsigned int id, component_record **out)
{
    if (!initialized)
        return 1;
    for (unsigned int i = 0; i < count; i++) {
        if (records[i].id == id) {
            *out = &records[i];
            return 0;
        }
    }
    return -55;
}

int componnet_manager::load_record(component_record *rec)
{
    memory_budget *budget;

    if (rec->region == 0)
        budget = g_memory_budget_low_ptr;
    else if (rec->region == 2 || rec->region == 3)
        budget = g_memory_budget_high_ptr;
    else
        return 0;
    return loader->load_component(rec->id, rec->name, budget, rec->region, rec->max_size, rec->align,
                                  &rec->component->address, &rec->component->size);
}

int componnet_manager::load_components()
{
    for (unsigned int i = 0; i < count; ++i) {
        int rc = load_record(&records[i]);
        if (rc != -55 && rc != -10 && rc != -51 && rc)
            return rc;
    }
    return 0;
}

int componnet_manager::overwrite_2nd_component()
{
    unsigned long addr, size;

    if (__builtin_expect(!alt_records, 0) || !alt_count)
        return 5;
    for (unsigned int i = 0; i < alt_count; i++) {
        if (alt_records[i].flag != 0)
            continue;
        int rc = get_component(alt_records[i].pair_id, &addr, &size);
        if (rc)
            return rc;
        rc = loader->load_component_to(alt_records[i].rec.id, alt_records[i].rec.name, (void *)addr, size);
        if (rc)
            return rc;
    }
    return 0;
}

int componnet_manager::load_2nd_component()
{
    unsigned long addr, size;
    component_record *rec;

    if (__builtin_expect(!alt_records, 0) || !alt_count)
        return 5;
    for (unsigned int i = 0; i < alt_count; i++) {
        if (alt_records[i].flag != 1)
            continue;
        if (__builtin_expect(get_component(alt_records[i].pair_id, &addr, &size) == 0, 0))
            return 6;
        if (__builtin_expect(find_record(alt_records[i].pair_id, &rec) != 0, 0))
            return 5;
        loaded_component *slot = rec->component;
        if (__builtin_expect(!slot, 0))
            return 5;
        rec->id = alt_records[i].rec.id;
        rec->name = alt_records[i].rec.name;
        rec->max_size = alt_records[i].rec.max_size;
        rec->region = alt_records[i].rec.region;
        rec->align = alt_records[i].rec.align;
        if (__builtin_expect(slot->initialize(rec->id, 0) != 0, 0))
            return 5;
        if (__builtin_expect(load_record(rec) != 0, 0))
            return 6;
    }
    return 0;
}

int componnet_manager::initialize(component_loader *loader)
{
    int rc = 4;
    unsigned int i;

    if (count > 19)
        return rc;

    for (i = 0; i < count; i++) {
        loaded_component *slot = &components[i];
        component_record *rec = &records[i];
        slot->initialize(rec->id, 0);
        rec->component = slot;
    }

    for (i = 0; i < count; i++) {
        component_record *rec = &records[i];
        if (rec->region == 4)
            *(unsigned long *)&rec->component = 0;
    }

    this->loader = loader;
    rc = load_components();
    if (!rc)
        initialized = true;
    return rc;
}

int componnet_manager::get_component_name(unsigned int id, unsigned long *out_addr, const char **out_name)
{
    unsigned int i;

    if (!initialized)
        return 1;

    for (i = 0; i < count; i++) {
        component_record *rec = &records[i];
        if (rec->id == id) {
            loaded_component *comp = rec->component;
            if (!comp)
                return 4;
            *out_addr = comp->address;
            *out_name = rec->name;
            return 0;
        }
    }
    return -55;
}

int componnet_manager::get_component(unsigned int id, unsigned long *out_addr, unsigned long *out_size)
{
    unsigned int i, count;

    if (!initialized)
        return 1;

    count = this->count;
    for (i = 0; i < count; i++) {
        component_record *rec = records + i;
        if (rec->id == id) {
            loaded_component *comp = rec->component;
            if (!comp)
                return 4;
            return comp->get(id, out_addr, out_size);
        }
    }
    return -55;
}
