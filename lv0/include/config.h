#ifndef LV0_CONFIG_H
#define LV0_CONFIG_H

#include "lv0.h"

long read_eeprom_os_boot_order_flag(unsigned char *out);
int read_eeprom_48c08(unsigned char *out);
int read_eeprom_qa_flag(unsigned char *out);
int read_eeprom_core_clock_multiplier(unsigned char *out);
int read_eeprom_ref_clock(unsigned char *out);
int read_eeprom_rsx_rdcy_1(unsigned long *out);
int read_eeprom_rsx_rdcy_2(unsigned long *out);
int read_eeprom_rsx_rdcy_3(unsigned long *out);
int read_eeprom_rsx_rdcy_4(unsigned long *out);
int read_eeprom_rsx_rdcy_5(unsigned long *out);
int read_eeprom_rsx_rdcy_6(unsigned long *out);
int read_eeprom_rsx_rdcy_7(unsigned long *out);
int read_eeprom_rsx_rdcy_8(unsigned long *out);
int read_eeprom_gbe_macaddr(unsigned long *out);
int read_eeprom_nv4_u64_at5(unsigned long *out);
int read_eeprom_qa_token(void *dst);
int get_eeprom_os_boot_order_flag(void);
bool is_mambo(void);
long get_eeprom_select_dgbe_device(void);
unsigned int get_eeprom_dgbe_ip_address(void);
unsigned int get_eeprom_dgbe_ip_netmask(void);
unsigned int get_eeprom_dgbe_ip_gateway(void);
unsigned long get_gbe_macaddr(unsigned int id);
long get_mambo_version(void);
int get_flash_format(void);
long get_flash_boot(void);
unsigned long get_nv_entry2_byte8(void);
int read_sdk_version(char *buf, unsigned long size);
long get_wake_source(void);
bool is_qaf_enabled(void);
unsigned long get_hw_config(int which);
unsigned short get_eeprom_cellos_flags(void);
unsigned long get_total_memory_size(void);
long get_debug_interface(void);
int get_eeprom_select_net_device(void);
void build_alt_sys_parm(struct boot_parm *parm);
int get_eeprom_bootrom_trace_leve(unsigned char *out_level);
int read_eeprom_select_dgbe_device(unsigned char *out);
int read_eeprom_cellos_flags(unsigned short *out);
int read_eeprom_dgbe_ip_address(unsigned int *out);
int read_eeprom_dgbe_ip_netmask(unsigned int *out);
int read_eeprom_dgbe_ip_gateway(unsigned int *out);
int read_eeprom_sata_param(unsigned int *out);
int read_eeprom_core_os_bank_indicator(unsigned char *out);
int read_eeprom_cellos_spu_configure(unsigned int *out);
int read_eeprom_flash_ext_format(unsigned char *out);
int read_eeprom_nv1_u32_at4(unsigned int *out);
int read_eeprom_nv1_u16_at2(unsigned short *out);
int write_eeprom_nv1_u32_at12(unsigned int value);
int write_eeprom_nv1_u32_at4(unsigned int value);
int write_eeprom_nv1_u32_at8(unsigned int value);
int write_eeprom_nv1_u16_at2(unsigned short value);
int finish_nv1_request(void *ctx, unsigned short done_bits, unsigned int result);
int read_eeprom_release_mode(unsigned char *out);
int write_eeprom_release_mode(unsigned char mode);
void set_release_mode(unsigned char release);
unsigned int get_eeprom_sata_param(void);
unsigned char get_eeprom_core_os_bank_indicator(void);
unsigned int get_eeprom_cellos_spu_configure(void);
unsigned char get_eeprom_flash_ext_format(void);
int get_debug_interface_parm(void);
int read_rsx_rdcy(unsigned long *out, unsigned int size);
int read_qa_token(void *dst);
int read_eeprom_select_net_device(unsigned char *out);

extern int g_flash_format_cache;
extern long g_flash_type;
typedef int rsx_rdcy_reader_fn(unsigned long *out);
extern rsx_rdcy_reader_fn *const rsx_rdcy_reader_table[8];

#endif
