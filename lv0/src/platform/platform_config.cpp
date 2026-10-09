#include "clock.h"
#include "config.h"
#include "syscon.h"
#include "spu.h"
#include "platform.h"
#include "memory.h"
#include "iommu.h"
#include "nand_flash.h"
#include "storage.h"
#include "log.h"
#include "sb.h"
#include "mmio.h"
#include "component.h"

long g_platform_id_cache[1] = { -1 };
long g_spu_fault_mask_cache = -1;
unsigned int g_sb_revision_cache[1] = { 0xFFFFFFFF };
volatile unsigned int g_be_revision_cache = 0xFFFFFFFF;

unsigned int get_be_revision(void)
{
    unsigned int rev = g_be_revision_cache;
    if (rev != -1)
        return rev;

    unsigned long bpvr;
    __asm__ volatile ("mfspr %0, 1022" : "=r"(bpvr));

    switch (bpvr) {
    case 0x000:  g_be_revision_cache = 0x10;  break;
    case 0x001:  g_be_revision_cache = 0x11;  break;
    case 0x100:  g_be_revision_cache = 0x20;  break;
    case 0x200:  g_be_revision_cache = 0x30;  break;
    case 0x201:  g_be_revision_cache = 0x31;  break;
    case 0x202:  g_be_revision_cache = 0x32;  break;
    case 0x1000: g_be_revision_cache = 0x110; break;
    case 0x2000: g_be_revision_cache = 0x210; break;
    case 0x2100: g_be_revision_cache = 0x220; break;
    default:     g_be_revision_cache = -1;    break;
    }
    return g_be_revision_cache;
}

long get_core_clock_multiplier(void)
{
    long mult;
    unsigned char flag;

    if (syscon_get_core_clock_multiplier(&mult) == 0)
        return mult;
    long rc = read_eeprom_core_clock_multiplier(&flag);
    if (rc == 0 && flag != 0xFF)
        return flag;
    return get_be_revision() <= 0x1F ? 4 : 6;
}

long get_reference_clock(void)
{
    unsigned int clock_hz;
    unsigned char flag;

    if (syscon_get_ref_clock(&clock_hz, 1) == 0)
        return clock_hz;
    long rc = read_eeprom_ref_clock(&flag);
    if (rc == 0 && flag != 0xFF)
        return flag * 10000000;
    return get_be_revision() <= 0x1F ? 600000000 : 400000000;
}

unsigned int get_sb_revision(void)
{
    if (g_sb_revision_cache[0] != -1)
        return g_sb_revision_cache[0];

    switch (*(unsigned int *)(sb_mmio_base + 0x87000) & 0x7FFFFFFF) {
    case 0x1000101: g_sb_revision_cache[0] = 0x10;  break;
    case 0x1000102: g_sb_revision_cache[0] = 0x11;  break;
    case 0x1000201: g_sb_revision_cache[0] = 0x20;  break;
    case 0x1000301: g_sb_revision_cache[0] = 0x30;  break;
    case 0x1000302: g_sb_revision_cache[0] = 0x31;  break;
    case 0x1000303: g_sb_revision_cache[0] = 0x32;  break;
    case 0x2000101: g_sb_revision_cache[0] = 0x110; break;
    case 0x2000102: g_sb_revision_cache[0] = 0x120; break;
    case 0x3000101: g_sb_revision_cache[0] = 0x210; break;
    case 0x3000102: g_sb_revision_cache[0] = 0x220; break;
    case 0x3000103: g_sb_revision_cache[0] = 0x230; break;
    case 0x4000100:
    case 0x4000101: g_sb_revision_cache[0] = 0x310; break;
    case 0x4000102: g_sb_revision_cache[0] = 0x320; break;
    case 0x4000103: g_sb_revision_cache[0] = 0x330; break;
    default:        g_sb_revision_cache[0] = -1;    break;
    }
    return g_sb_revision_cache[0];
}

long get_spu_fault_mask(void)
{
    long mask;

    if (get_be_revision() - 0x10 <= 2) {
        mask = g_spu_fault_mask_cache;
        if (mask == -1) {
            mask = (unsigned char)*(volatile long *)(be_mmio_base + 0x50AFD0);
            g_spu_fault_mask_cache = mask;
        }
        return mask;
    }
    return (unsigned char)~*(unsigned int *)(be_mmio_base + 0x509C38);
}

long get_enabled_spu_count(void)
{
    unsigned char restrict_spu[16];
    long fault_mask = get_spu_fault_mask();
    unsigned long n_good = 0;
    unsigned long i;
    long rc;

    for (i = 0; i < 8; i++)
        n_good += ((0x80UL >> i) & fault_mask) == 0;

    if (n_good <= 7)
        return 6;

    rc = read_eeprom_restrict_spu_flag(restrict_spu);
    if (rc != 0) {
        log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x get_eeprom_restrict_spu_flag %d\n", LV0_ERR_CONFIG, rc);
    } else if (restrict_spu[0] == 0xFF) {
        restrict_spu[0] = 6;
        rc = write_eeprom_restrict_spu_flag(6);
        if (rc != 0)
            log_error(LV0_ERR_CONFIG, "[ERROR]: 0x%08x set_eeprom_restrict_spu_flag %d\n", LV0_ERR_CONFIG, rc);
    }
    if (restrict_spu[0] > 8)
        return 8;
    return restrict_spu[0];
}

long get_platform_id(void)
{
    if (g_platform_id_cache[0] == -1 && syscon_get_platform_id((unsigned char *)g_platform_id_cache))
        g_platform_id_cache[0] = 0x756E6B6E6F776E00;
    return g_platform_id_cache[0];
}

long get_mambo_version(void)
{
    return 0;
}

#define ROR8(mask, n) (((unsigned long)(mask) >> (n)) | ((unsigned long)(mask) << (64 - (n))))

long first_usable_spu(void)
{
    long i;

    for (i = 0; i != 8; i++)
        if ((0x80UL >> i) & get_spu_fault_mask())
            break;
    return i;
}

long calc_spu_faultbm_hi(void)
{
    long start;
    if (first_usable_spu() == 8) {
        start = 4;
    } else {
        start = first_usable_spu() & 7;
        start = 7 - start;
    }
    long i = start;

    do {
        long cur = i;
        if (((0x80UL >> cur) & get_spu_fault_mask()) == 0)
            return ROR8(~(0x8000000000UL >> cur) << 24, 24) & 0xFFFFFFFF00000000UL;
        i = (cur + 1) & 7;
    } while (start != i);
    return 0xFF00000000UL;
}

long calc_spu_faultbm_lo(void)
{
    long mask = 0xFF;
    long limit = get_enabled_spu_count();

    if (limit) {
        long start = first_usable_spu() == 8 ? 5 : (8 - first_usable_spu()) & 7;
        long i = start;
        unsigned int count = 0;

        do {
            unsigned long bit = 0x80UL >> i;
            long sample = get_spu_fault_mask();
            i = (i + 1) & 7;
            if ((bit & sample) == 0) {
                ++count;
                mask &= ~bit;
                if (count > (unsigned long)(limit - 1))
                    break;
            }
        } while (start != i);
    }
    return mask;
}

int read_rsx_rdcy(unsigned long *out, unsigned int size)
{
    rsx_rdcy_reader_fn *handlers[8];
    int i;

    handlers[0] = rsx_rdcy_reader_table[0];
    handlers[1] = rsx_rdcy_reader_table[1];
    handlers[2] = rsx_rdcy_reader_table[2];
    handlers[3] = rsx_rdcy_reader_table[3];
    handlers[4] = rsx_rdcy_reader_table[4];
    handlers[5] = rsx_rdcy_reader_table[5];
    handlers[6] = rsx_rdcy_reader_table[6];
    handlers[7] = rsx_rdcy_reader_table[7];

    for (i = 0; i != size >> 3; i++) {
        if (handlers[i](out + i))
            return -1;
    }
    return 0;
}

static inline void get_flash_format_version(long addr)
{
    struct {
        unsigned int magic;
        unsigned int version;
        unsigned int format_version;
        unsigned int reserved;
    } hdr;

    __builtin_memcpy(&hdr, (const void *)addr, sizeof hdr);
    if (hdr.magic == 0x49464900) {
        if (hdr.version == 1 && hdr.format_version == 2) {
            g_flash_format_cache = 3;
            return;
        }
        log_message("[WARN]: %s: unknown flash_format_version version %04x, format_version %04x\n", __FUNCTION__,
                    hdr.version, hdr.format_version);
        g_flash_format_cache = -1;
        return;
    }
    g_flash_format_cache = 2;
}

int get_flash_format(void)
{
    if (g_flash_format_cache == -1) {
        iopt_codec codec;
        long entry[7];
        unsigned long base = sb_mmio_base;

        codec.unpack_entry(base, 0, entry);
        int type = detect_flash_type(base + entry[0]);
        g_flash_type = type;
        switch (type) {
        case 0:
            g_flash_format_cache = 1;
            break;
        case 0x100:
        case 0x110:
        case 0x111:
        case 0x120:
        case 0x190:
        case 0x1A0:
            {
                long hdr = base + 0x200;
                hdr += entry[0];
                get_flash_format_version(hdr);
            }
            break;
        default:
            g_flash_format_cache = 0;
            break;
        }
    }
    return g_flash_format_cache;
}

unsigned long get_nv_entry2_byte8(void)
{
    unsigned char buf;
    if (read_eeprom_48c08(&buf) != 0)
        return 0;
    return buf;
}

unsigned long get_syscon_protocol_version(void)
{
    return get_syscon_device()->get_protocol_version() == 1;
}

long find_flash_table_entry(unsigned long dev_index, unsigned long offset, const char *name,
                            struct region_table_entry *entry)
{
    long hdr_nread;
    long entry_nread;
    struct region_table_header hdr;

    if (g_storage.read(dev_index, offset, 16, (char *)&hdr, (unsigned long *)&hdr_nread) != 0)
        return 0;
    if (hdr_nread != 16)
        return 0;
    if (hdr.version != 1)
        return 0;

    {
        long entry_off = offset + 16;
        unsigned int i = 0;
        unsigned int cnt = hdr.count;
        while (i < cnt) {
            if (g_storage.read(dev_index, entry_off, 48, (char *)entry, (unsigned long *)&entry_nread) != 0)
                return 0;
            if (entry_nread != 48)
                return 0;
            if (lv0_strncmp(entry->name, name, 32) == 0)
                return 1;
            entry_off += 48;
            i++;
        }
    }
    return 0;
}

extern componnet_manager *g_componnet_manager_ptr;

int read_sdk_version(char *buf, unsigned long size)
{
    unsigned long addr, len;
    int rc;

    rc = g_componnet_manager_ptr->get_component(COMPONENT_SDK_VERSION, &addr, &len);
    if (rc)
        return -1;
    if (size < len)
        return -1;
    lv0_memmove(buf, (const char *)addr, len);
    buf[len - 1] = 0;
    return 0;
}

long locate_os_image(unsigned long *out_addr, unsigned long *out_size)
{
    long rc = g_storage.open(0);
    long ret = 0;
    struct region_table_entry os_ent;
    unsigned long os_off;
    struct region_table_entry img_ent;

    if ( (rc && (int)rc != -13)
       || !(unsigned)find_flash_table_entry(0, 0, "os", &os_ent)
       || (os_off = os_ent.offset, !(unsigned)find_flash_table_entry(0, os_off, "image.bin", &img_ent))
       || (*out_size = img_ent.size, g_storage.get_mapped_address(0, os_off + img_ent.offset, out_addr)) )
        ret = 1;

    g_storage.close(0);
    return ret;
}

long get_os_image_size(void) { unsigned long addr, size; if (locate_os_image(&addr, &size) != 0) return 0; return size; }

long get_os_image_address(void) {
    unsigned long addr, size;
    if (locate_os_image(&addr, &size) == 0) return addr;
    return 0;
}

long get_flash_boot(void)
{
    if (g_flash_type == 0 && is_nand_flash_boot())
        return 1;
    return g_flash_type;
}

DEAD_STRING(platform_name_gno, "Gno");
DEAD_STRING(platform_name_cok, "Cok");
DEAD_STRING(platform_name_cyt2, "Cyt2");
DEAD_STRING(platform_name_cyt3, "Cyt3");
DEAD_STRING(platform_name_shr, "Shr");
DEAD_STRING(platform_name_cyt1, "Cyt1");
