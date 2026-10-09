#include "syscon.h"
#include "storage.h"
#include "iommu.h"
#include "errors.h"
#include "log.h"
#include "loader.h"
#include "mmio.h"
#include "xdr.h"

#define BE (0x20000000000ULL)

void loader_base::finalize(bool fatal)
{
    release_dma_io_address();
    write64(0x20000509C90ULL, 0);
    get_iommu_context()->disable();
    if (fatal)
        log_error("");
    log_message("[INFO]: Connecting to Debug Device (%s)\n", get_debug_device_name());
}

long loader_base::init_memory(void)
{
    xdr x;
    memory_config cfg;
    unsigned char mode;
    unsigned char setting;
    memory_config_query q;

    q.initialize();
    if (q.query_config(&cfg)) {
        log_message("[ERROR]: 0x%08x (FATAL) unable to set up memory configuration parameter.\n", LV0_ERROR_BASE);
        return -5;
    }
    if (get_bootrom_diag(&mode))
        cfg.basic[7] = 0;
    x.set_config(&cfg);
    long rc = x.initialize();
    if (rc) {
        log_message("[ERROR]: 0x%08x (FATAL) XDR Link not initilized.\n", rc);
        x.dump(0);
        x.dump(1);
        x.dump_regs();
        x.prepare_iow_dump();
        x.dump(2);
        get_eeprom_48c0e(&setting);
        if (setting)
            setting--;
        set_eeprom_48c0e(setting);
        return -5;
    }
    log_info("[INFO]: XDR Link successfully initilized.\n");
    if (get_bootrom_diag(&mode)) {
        u64 size = get_memory_size();
        log_message("[INFO]: memory diag (mode %d)\n", mode);
        if (!memory_diag(size, 0, 0, mode)) {
            log_message("[ERROR]: 0x%08x (FATAL) memory diag fail\n", LV0_ERROR_BASE | 7);
            get_eeprom_48c0d(&setting);
            setting &= ~1;
            set_eeprom_48c0d(setting);
            if (mode == 1)
                return -5;
        } else {
            get_eeprom_48c0d(&setting);
            setting |= 1;
            set_eeprom_48c0d(setting);
            get_eeprom_bootrom_diag(&setting);
            setting |= 1;
            set_eeprom_bootrom_diag(setting);
        }
        int code = 0;
        switch (mode) {
        case 1:
            code = 1;
            break;
        }
        syscon_power_off_with_code(0, 0, code);
    }
    get_iommu_context()->initialize(BE, 0x1C000, 1024, 0x1D000, 4096);
    write64(BE + 0x509C90, 0x8001);
    if (ata_activation(0x1E000, 4096)) {
        log_message("[ERROR]: 0x%08x ata_activation fail\n", LV0_ERROR_BASE | 3);
        return -5;
    }
    return cfg.str ? 1 : 0;
}
