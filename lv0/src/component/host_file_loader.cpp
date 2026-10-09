#include "component.h"
#include "console.h"
#include "memory.h"
#include "log.h"

host_file_loader::host_file_loader()
{
}

host_file_loader::~host_file_loader()
{
}

int host_file_loader::load_component(unsigned int id, const char *name, memory_budget *budget,
                                     unsigned int region, unsigned long max_size, unsigned long align,
                                     unsigned long *out_addr, unsigned long *out_size)
{
    unsigned long file_size;
    long used, free;
    void *buf;

    int rc = -55;
    if (id != COMPONENT_EID0 && id != COMPONENT_SDK_VERSION) {
        if (!call_get_file_size_hook(name, &file_size)) {
            if (max_size < file_size) {
                log_message("error : %s size is too large.\n", name);
                return -10;
            }
            buf = budget->allocate(file_size, align);
            if (!buf) {
                budget->get_used_size(&used);
                budget->get_free_size(&free);
                log_message("%s(%d): allocation failure. size 0x%x, align 0x%x, alloc 0x%x, free 0x%x\n",
                            __FUNCTION__, 93, file_size, align, used, free);
                return -10;
            }
            if (!call_get_file_hook(name, 0, buf, file_size, out_size)) {
                *out_addr = (unsigned long)buf;
                rc = 0;
            }
        }
    }
    return rc;
}

int host_file_loader::load_component_to(unsigned int id, const char *name, void *dst, unsigned long size)
{
    unsigned long file_size, got;

    int rc = -55;
    switch (id) {
    case COMPONENT_LV1LDR_2:
    case COMPONENT_LV2LDR_2:
    case COMPONENT_APPLDR_2:
    case COMPONENT_ISOLDR_2:
        if (!call_get_file_size_hook(name, &file_size)) {
            if (size < file_size) {
                log_message("error : %s size is too large.\n", name);
                return -10;
            }
            if (!call_get_file_hook(name, 0, dst, file_size, &got))
                rc = 0;
        }
        break;
    }
    return rc;
}
