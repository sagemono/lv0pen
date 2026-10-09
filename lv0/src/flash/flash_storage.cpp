#include "iommu.h"
#include "nand_flash.h"
#include "sb.h"
#include "storage.h"
#include "log.h"
#include "mmio.h"
#include "spansion_storage.h"

class flash_storage_manager : public storage_manager {
public:
    flash_storage_manager() {}
    ~flash_storage_manager();
    storage_device **get_device_table();
    unsigned long get_device_count();
};

extern storage_device *g_flash_storage_device_table[2];
extern nand_flash_abs g_nand_flash_abs;
extern spansion_storage_plain g_spansion_storage;
extern flash_storage_manager g_flash_storage_manager;

void init_nand_flash_storage(char *dma_buf, unsigned long dma_io_addr, unsigned long dma_buf_size)
{
    long entry[7];
    iopt_codec codec;
    long base = sb_mmio_base;
    int rc;

    codec.unpack_entry(base, 0, entry);
    switch (detect_flash_type(base + entry[0])) {
    case 0: {
        long flash_base = base + entry[0];
        get_nand_flash_ctrl()->set_base(flash_base);
        nand_flash_ready_fn ready_cb;

        if ((mmio_sb_product_code & 0x7F000000) == 0x1000000) {
            get_systemio_dmac_dx()->configure(base + 0xFF0000, (unsigned long)(dma_buf + dma_buf_size - 128),
                                              dma_io_addr + dma_buf_size - 128, 0x500000000L, 0x200000000L);
            ready_cb = poll_ss2_interrupt;
        } else
            ready_cb = 0;
        if (!ready_cb && (mmio_sb_product_code & 0x7F000000) == 0x4000000) {
            get_systemio_dmac_px()->configure(base + 0xFF0000, flash_base + 0x40000,
                                              (unsigned long)(dma_buf + dma_buf_size - 128),
                                              dma_io_addr + dma_buf_size - 128);
            ready_cb = poll_emmcbridge_interrupt;
        }
        if (!ready_cb) {
            rc = get_systemio_dmac_px()->configure(base + 0xFF0000, flash_base + 0x44000,
                                                   (unsigned long)(dma_buf + dma_buf_size - 128),
                                                   dma_io_addr + dma_buf_size - 128);
            if (rc)
                log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x systemio_dmac_px::configure fail %d, ", LV0_ERR_INTERNAL, rc);
            ready_cb = (nand_flash_ready_fn)poll_ebus_interrupt;
        }
        set_nand_flash_ready_callback(ready_cb);

        g_nand_flash_abs.configure_dma(flash_base, 0, true, dma_buf, dma_io_addr, dma_buf_size - 128);
        g_flash_storage_device_table[0] = &g_nand_flash_abs;
        break;
    }
    case 256:
        rc = g_spansion_storage.init(base + 0x1F000000, 0, 5, 4);
        if (rc)
            log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x spansion_storage_plain::init fail %d, ", LV0_ERR_INTERNAL, rc);
        g_flash_storage_device_table[0] = &g_spansion_storage;
        break;
    case 272:
    case 273:
    case 288:
    case 400:
    case 416:
        rc = g_spansion_storage.init(base + 0x1F000000, 0, -1, 4);
        if (rc)
            log_error(LV0_ERR_INTERNAL, "[ERROR]: 0x%08x spansion_storage_plain::init fail %d, ", LV0_ERR_INTERNAL, rc);
        g_spansion_storage.protect_range(base + 0x1FFC0000, 0x40000);
        g_flash_storage_device_table[0] = &g_spansion_storage;
        break;
    }
}

flash_storage_manager::~flash_storage_manager()
{
}

storage_device **flash_storage_manager::get_device_table()
{
    return g_flash_storage_device_table;
}

unsigned long flash_storage_manager::get_device_count()
{
    long i;
    for (i = 0; i < 2; i++)
        if (g_flash_storage_device_table[i] == 0)
            break;
    return i;
}

storage_device *g_flash_storage_device_table[2];
nand_flash_abs g_nand_flash_abs;
spansion_storage_plain g_spansion_storage;
flash_storage_manager g_flash_storage_manager;

storage_manager *g_storage_manager_ptr = &g_flash_storage_manager;
