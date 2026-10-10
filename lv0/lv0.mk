include lv0/lv0.slots

LV0_OUT := $(BUILD_DIR)/lv0
LV0_OBJ_DIR := $(LV0_OUT)/obj
LV0_EMBED_DIR := $(LV0_OUT)/embedded
LV0_ELF := $(LV0_OUT)/lv0.elf
LV0_MAP := $(LV0_OUT)/lv0.map
LV0_LDSCRIPT := lv0/lv0.ld

LV0_IMAGE := lv0/lv0_ida/lv0.elf

LV0_START := lv0/src/boot/start.S
LV0_EMBEDDED := lv0/src/component/embedded_components.S

LV0_UNITS := \
	lv0/src/boot/c_entry.cpp \
	lv0/src/boot/run_global_constructors.cpp \
	lv0/src/boot/hang_forever.cpp \
	lv0/src/clock/read_timebase.cpp \
	lv0/src/clock/get_time_ms.cpp \
	lv0/src/clock/get_time_us.cpp \
	lv0/src/component/boot_order_component_loader.cpp \
	lv0/src/spu/spu_manager.cpp \
	lv0/src/storage/eeprom_nv1.cpp \
	lv0/src/component/component_loader.cpp \
	lv0/src/component/componnet_manager.cpp \
	lv0/src/component/component.cpp \
	lv0/src/component/loaded_component.cpp \
	lv0/src/component/component__base_dtor.c \
	lv0/src/platform/is_mambo.cpp \
	lv0/src/io/get_gbe_macaddr.cpp \
	lv0/src/platform/platform_config.cpp \
	lv0/src/platform/xio_memory.cpp \
	lv0/src/syscon/get_boot_gos.cpp \
	lv0/src/storage/eeprom_settings.cpp \
	lv0/src/storage/get_eeprom_dgbe_ip_gateway.cpp \
	lv0/src/storage/get_eeprom_dgbe_ip_netmask.cpp \
	lv0/src/storage/get_eeprom_dgbe_ip_address.cpp \
	lv0/src/storage/get_eeprom_sata_param.cpp \
	lv0/src/storage/get_eeprom_core_os_bank_indicator.cpp \
	lv0/src/storage/get_eeprom_cellos_spu_configure.cpp \
	lv0/src/storage/get_eeprom_flash_ext_format.cpp \
	lv0/src/storage/get_eeprom_cellos_flags.cpp \
	lv0/src/platform/is_qaf_enabled.cpp \
	lv0/src/storage/get_eeprom_select_dgbe_device.cpp \
	lv0/src/storage/get_eeprom_os_boot_order_flag.cpp \
	lv0/src/storage/set_release_mode.cpp \
	lv0/src/syscon/get_sc_version.cpp \
	lv0/src/platform/get_hw_config.cpp \
	lv0/src/platform/read_qa_token.cpp \
	lv0/src/platform/get_wake_source.cpp \
	lv0/src/platform/get_debug_interface_parm.cpp \
	lv0/src/component/file_hooks.cpp \
	lv0/src/util/cxa_pure_virtual.cpp \
	lv0/src/platform/debug_interface.cpp \
	lv0/src/storage/read_eeprom_select_net_device.cpp \
	lv0/src/storage/get_eeprom_select_net_device.cpp \
	lv0/src/boot/application.cpp \
	lv0/src/io/iopt_pack_entry_v1.cpp \
	lv0/src/io/iopt_unpack_entry_v1.cpp \
	lv0/src/io/iopt.cpp \
	lv0/src/io/iopt_unpack_entry_v2.cpp \
	lv0/src/component/flash_format_1_component_loader.cpp \
	lv0/src/io/sb_registers.cpp \
	lv0/src/io/init_sb_device.cpp \
	lv0/src/io/get_sb_device.cpp \
	lv0/src/component/host_get_file_size.cpp \
	lv0/src/component/host_put_file.cpp \
	lv0/src/component/host_get_file.cpp \
	lv0/src/log/default_print_hook.cpp \
	lv0/src/storage/storage_device__get_mapped_address.c \
	lv0/src/storage/storage.cpp \
	lv0/src/storage/storage_manager.cpp \
	lv0/src/io/iommu.cpp \
	lv0/src/io/get_iommu_context.cpp \
	lv0/src/spu/spuctl.cpp \
	lv0/src/boot/alt_sys_parm.cpp \
	lv0/src/boot/boot_parm.cpp \
	lv0/src/boot/boot_loader.cpp \
	lv0/src/boot/boot_cell_os.cpp \
	lv0/src/boot/component_info.cpp \
	lv0/src/util/lv0_memmove.cpp \
	lv0/src/platform/memory_budgets.cpp \
	lv0/src/log/log_ring.cpp \
	lv0/src/util/lv0_memset.cpp \
	lv0/src/console/init_debug_interface.cpp \
	lv0/src/console/finalize_debug_interface.cpp \
	lv0/src/platform/pio.cpp \
	lv0/src/platform/platform.cpp \
	lv0/src/storage/get_eeprom_bootrom_trace_leve.cpp \
	lv0/src/platform/platform_pwrmgr.cpp \
	lv0/src/util/delay.cpp \
	lv0/src/component/host_file_loader.cpp \
	lv0/src/flash/mapped_storage.cpp \
	lv0/src/syscon/syscon_clock.cpp \
	lv0/src/syscon/syscon_get_hw_config.cpp \
	lv0/src/syscon/syscon_get_platform_id.cpp \
	lv0/src/syscon/syscon_get_core_clock_multiplier.cpp \
	lv0/src/syscon/sc_console.cpp \
	lv0/src/util/copy_qwords_to_mmio.cpp \
	lv0/src/util/copy_qwords_from_mmio.cpp \
	lv0/src/syscon/syscon.cpp \
	lv0/src/syscon/get_syscon_device.cpp \
	lv0/src/syscon/sc_nv_storage.cpp \
	lv0/src/syscon/syscon_syspm_get_33.cpp \
	lv0/src/syscon/syscon_set_wake_source.cpp \
	lv0/src/syscon/syscon_power_off.cpp \
	lv0/src/syscon/syscon_reboot.cpp \
	lv0/src/syscon/syscon_shutdown.cpp \
	lv0/src/component/component_loaders.cpp \
	lv0/src/flash/flash_storage.cpp \
	lv0/src/storage/storage_manager__base_dtor.c \
	lv0/src/console/uart.cpp \
	lv0/src/flash/spansion_storage.cpp \
	lv0/src/spu/spu_mmio.cpp \
	lv0/src/flash/nand_flash_ctrl.cpp \
	lv0/src/flash/get_nand_flash_ctrl.cpp \
	lv0/src/flash/nand_flash.cpp \
	lv0/src/flash/nand_flash_abs.cpp \
	lv0/src/flash/nand_flash_transfer.cpp \
	lv0/src/util/lv0_strlen.cpp \
	lv0/src/util/lv0_strncmp.cpp \
	lv0/src/util/lv0_strncpy.cpp \
	lv0/src/util/lv0_strnlen.cpp \
	lv0/src/log/printf.cpp \
	lv0/src/storage/nv_storage.cpp \
	lv0/src/io/get_systemio_dmac_dx.cpp \
	lv0/src/io/systemio_dmac_dx.cpp \
	lv0/src/io/systemio_dmac_px.cpp \
	lv0/src/log/log.cpp \
	lv0/src/flash/flash_probe.cpp \
	lv0/src/console/cp_bridge.cpp \
	lv0/src/console/internal_console.cpp \
	lv0/src/console/log_cons_channel.cpp \
	lv0/src/io/gbe_work.cpp \
	lv0/src/io/pci_mini_driver.cpp \
	lv0/src/io/pci_bus_power.cpp \
	lv0/src/console/cp_ch4_channel.cpp \
	lv0/src/console/cp_channel.cpp \
	lv0/src/console/physical_console.cpp \
	lv0/src/console/file_transfer.cpp \
	lv0/src/console/ring_buffer.cpp \
	lv0/src/util/mmio_accessor.cpp \
	lv0/src/console/cp_link.cpp \
	lv0/src/io/pci_config.cpp \
	lv0/src/util/operator_delete.cpp \
	lv0/src/util/spin_lock.cpp \
	lv0/src/util/spin_lock_release.cpp \
	lv0/src/util/aes.c \
	lv0/src/boot/lv1ldr_key.c \
	$(LV0_EMBEDDED) \
	lv0/src/platform/memory_map.cpp \
	lv0/src/util/aes_tables.c

LV0_O2_UNITS := \
	lv0/src/console/cp_bridge.cpp \
	lv0/src/io/gbe_work.cpp \
	lv0/src/io/pci_mini_driver.cpp \
	lv0/src/io/pci_bus_power.cpp \
	lv0/src/console/cp_ch4_channel.cpp \
	lv0/src/console/cp_channel.cpp \
	lv0/src/util/mmio_accessor.cpp \
	lv0/src/console/cp_link.cpp \
	lv0/src/io/pci_config.cpp \
	lv0/src/util/aes.c

LV0_START_OBJ := $(LV0_OBJ_DIR)/$(LV0_START:.S=.o)
LV0_OBJS := $(LV0_START_OBJ) $(addprefix $(LV0_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV0_UNITS))))
LV0_CUT_UNITS := $(filter-out %.S,$(LV0_UNITS))

LV0_CC := $(PPU_GCC411_SDK420_BIN)/ppu-lv2-gcc
LV0_LD := $(PPU_GCC411_SDK420_BIN)/../ppu-lv2/bin/ld
LV0_READELF := $(PPU_GCC411_SDK420_BIN)/ppu-lv2-readelf
LV0_SPU_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
LV0_SCETOOL = cd $(dir $(SCETOOL)) && ./$(notdir $(SCETOOL))

LV0_SCETOOL_FLAGS := -0 SELF -2 0000 -4 FF000000 -5 LDR -A 0004009300000000 -1 FALSE -s TRUE -7 TRUE \
	-8 0000000000000000000000000000000000000000000000000000000000000000 \
	-9 0000000000000000000000000000000000000000000000780000000000000000

LV0_CFLAGS := -mlp64 -Os -mtraceback=none -msoft-float -mno-altivec -fno-inline-functions -fno-common -fdata-sections -mcellos-kernel -Ilv0/include
LV0_SECTION_FLAGS := -ffunction-sections -fdata-sections
LV0_CXXFLAGS := -fno-exceptions -fno-rtti -fpermissive

$(patsubst %,$(LV0_OBJ_DIR)/%.s,$(basename $(LV0_O2_UNITS))): LV0_UNIT_FLAGS := -O2

LV0_CUT = \
	sed -e 's/^\(\t\.section\t[^,]*,"[a-z]*\)G\([a-z]*"\),\(@[a-z]*\),[^,]*,comdat$$/\1\2,\3/' \
		-e '/^\t\.section\t"\.opd","aw"$$/{N;N;s/^\t\.section\t"\.opd","aw"\n\(\t\.align 3\)\n\([^\n]*\):$$/\t.section\t".opd.\2","aw"\n\1\n\2:/}' $(1) | \
	sed -e '/^\.LC[0-9]*:$$/{N;s/^\(\.LC\([0-9]*\)\):\n\t\.tc/\t.section\t".toc.LC\2","aw"\n\1:\n\t.tc/;t;P;D}' | \
	sed -e '/^\t\.section\t/h' -e '/^\t\.text$$/h' \
		-e '/^\t\.align [0-9]*$$/{x;/^\t\.section\t\.rodata\.str/!{x;b};x;N;s/^\(\t\.align [0-9]*\)\n\(\.LC\([0-9]*\)\):$$/\t.section\t".rodata.str.LC\3","a",@progbits\n\1\n\2:/;t;P;D}' \
		-e '/^\.LC[0-9]*:$$/{x;/^\t\.section\t\.rodata\.str/!{x;b};x;s/^\(\.LC\([0-9]*\)\):$$/\t.section\t".rodata.str.LC\2","a",@progbits\n\1:/}'

lv0_toc_global = -e 's/^\.$(1):$$/\t.globl $(2)\n$(2):\n.$(1):/'
lv0_toc_alias = -e '/^\.$(1):$$/{N;s/^\.$(1):\n\t\.tc .*$$/\t.set .$(1),$(2)/}'
lv0_toc_clone = -e '/^\.$(1):$$/{N;s/^\.$(1):\n\(\t\.tc .*\)$$/&\n\t.section\t".toc.$(2)","aw"\n$(3).$(2):\n\1/}'
lv0_toc_use = -e '/^\t\.section\t\.text\.$(1),/,/^\t\.size\t\.$(1),/s/\.$(2)@toc/.$(3)@toc/g'
lv0_string_at = -e '/^\t\.section\t"\.rodata\.str\.$(1)",/{$(3)s/^.*$$/\t.set .$(1),$(2)/}'
lv0_drop = -e '/^\t\.section\t\.text\.$(1),/,/^\t\.size\t\.$(1),/d'
lv0_same_as = $(call lv0_drop,$(1)) -e '$$s/$$/\n\n\t.globl .$(1)\n\t.set .$(1),.$(2)\n\t.globl $(1)\n\t.set $(1),$(2)\n/'

LV0_FB_LOAD := _ZN27flash_bank_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_

$(LV0_OBJ_DIR)/lv0/src/spu/spu_manager.s: LV0_EDIT = \
	$(call lv0_toc_global,LC5,__toc_slot_6)
$(LV0_OBJ_DIR)/lv0/src/storage/eeprom_nv1.s: LV0_EDIT = \
	$(call lv0_toc_alias,LC4,__toc_slot_6)
$(LV0_OBJ_DIR)/lv0/src/component/flash_format_1_component_loader.s: LV0_EDIT = \
	$(call lv0_toc_clone,LC5,LC5_s85) \
	$(call lv0_toc_clone,LC16,LC16_s86) \
	$(call lv0_toc_use,$(LV0_FB_LOAD),LC14,LC5_s85) \
	$(call lv0_toc_use,$(LV0_FB_LOAD),LC16,LC16_s86) \
	$(call lv0_string_at,LC0,0x8017aa8,N;N;N;N;)
$(LV0_OBJ_DIR)/lv0/src/component/component.s: LV0_EDIT = \
	$(call lv0_drop,_ZN9componentD2Ev)
$(LV0_OBJ_DIR)/lv0/src/storage/storage.s: LV0_EDIT = \
	$(call lv0_drop,_Z41__static_initialization_and_destruction_0ii)
$(LV0_OBJ_DIR)/lv0/src/storage/storage_manager.s: LV0_EDIT = \
	$(call lv0_drop,_ZN15storage_managerD2Ev)
$(LV0_OBJ_DIR)/lv0/src/boot/boot_loader.s: LV0_EDIT = \
	$(call lv0_toc_clone,LC14,LC14_s177) \
	$(call lv0_toc_clone,LC16,LC16_s193,\t.globl __toc_slot_193\n__toc_slot_193:\n) \
	$(call lv0_toc_use,_ZN11boot_loader17print_boot_bannerEv,LC14,LC14_s177) \
	$(call lv0_toc_use,_ZN11boot_loader14component_initEv,LC24,LC16_s193)
$(LV0_OBJ_DIR)/lv0/src/boot/boot_cell_os.s: LV0_EDIT = \
	$(call lv0_toc_alias,LC22,__toc_slot_193)
$(LV0_OBJ_DIR)/lv0/src/platform/platform.s: LV0_EDIT = \
	$(call lv0_toc_global,LC25,__toc_slot_251) \
	$(call lv0_string_at,LC38,0x8018c48,N;N;N;)
$(LV0_OBJ_DIR)/lv0/src/storage/get_eeprom_bootrom_trace_leve.s: LV0_EDIT = \
	$(call lv0_toc_alias,LC0,__toc_slot_251)
$(LV0_OBJ_DIR)/lv0/src/flash/mapped_storage.s: LV0_EDIT = \
	$(call lv0_same_as,_ZN14mapped_storageC2Ev,_ZN14mapped_storageC1Ev)
$(LV0_OBJ_DIR)/lv0/src/component/component_loaders.s: LV0_EDIT = \
	$(call lv0_drop,_ZN27flash_bank_component_loaderD1Ev)
$(LV0_OBJ_DIR)/lv0/src/flash/spansion_storage.s: LV0_EDIT = \
	$(call lv0_same_as,_ZN16spansion_storageC2Ev,_ZN16spansion_storageC1Ev)

LV0_SELF_SLOTS := lv2ldr isoldr appldr
LV0_ZERO_SLOTS := $(filter-out $(LV0_SELF_SLOTS),$(LV0_SLOTS))
LV0_SELF_DIR := $(LV0_OUT)/self

LV0_SELF_ELF_lv2ldr := $(LV2LDR_STRIPPED)
LV0_SELF_ELF_isoldr := $(ISOLDR_STRIPPED)
LV0_SELF_ELF_appldr := $(APPLDR_STRIPPED)
LV0_SELF_AUTH_ID_lv2ldr := 1FF0000009000001
LV0_SELF_AUTH_ID_isoldr := 1FF000000A000001
LV0_SELF_AUTH_ID_appldr := 1FF000000C000001
LV0_SELF_UNMATCHED_lv2ldr := $(LV2LDR_UNMATCHED)
LV0_SLOT_BINS := $(addprefix $(LV0_EMBED_DIR)/,$(addsuffix .bin,$(LV0_SLOTS)))

.PHONY: lv0 check-lv0 clean-lv0

lv0: $(LV0_ELF)

$(LV0_OBJ_DIR)/%.s: %.cpp
	@mkdir -p $(@D)
	$(LV0_CC) $(LV0_CFLAGS) $(LV0_SECTION_FLAGS) $(LV0_CXXFLAGS) $(LV0_UNIT_FLAGS) -MMD -MP -MT $@ -MF $(@:.s=.d) -S $< -o $@.raw
	$(call LV0_CUT,$@.raw) $(if $(LV0_EDIT),| sed $(LV0_EDIT)) > $@

$(LV0_OBJ_DIR)/%.s: %.c
	@mkdir -p $(@D)
	$(LV0_CC) $(LV0_CFLAGS) $(LV0_SECTION_FLAGS) $(LV0_UNIT_FLAGS) -MMD -MP -MT $@ -MF $(@:.s=.d) -S $< -o $@.raw
	$(call LV0_CUT,$@.raw) $(if $(LV0_EDIT),| sed $(LV0_EDIT)) > $@

$(addprefix $(LV0_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV0_CUT_UNITS)))): %.o: %.s
	$(LV0_CC) $(LV0_CFLAGS) -x assembler -c $< -o $@

$(LV0_START_OBJ): $(LV0_START)
	@mkdir -p $(@D)
	$(LV0_CC) $(LV0_CFLAGS) -c $< -o $@

$(LV0_OBJ_DIR)/$(LV0_EMBEDDED:.S=.o): $(LV0_EMBEDDED) $(LV0_SLOT_BINS)
	@mkdir -p $(@D)
	$(LV0_CC) $(LV0_CFLAGS) -Wa,-I$(LV0_OUT) -c $< -o $@

$(LV0_EMBED_DIR)/lv2ldr.self: $(LV0_SELF_ELF_lv2ldr)
$(LV0_EMBED_DIR)/isoldr.self: $(LV0_SELF_ELF_isoldr)
$(LV0_EMBED_DIR)/appldr.self: $(LV0_SELF_ELF_appldr)

$(addprefix $(LV0_EMBED_DIR)/,$(addsuffix .self,$(LV0_SELF_SLOTS))):
	$(if $(SCETOOL),,$(error SCETOOL is not set: give scetool's path in mk/local.mk))
	@mkdir -p $(@D)
	rm -f $@
	( $(LV0_SCETOOL) $(LV0_SCETOOL_FLAGS) -3 $(LV0_SELF_AUTH_ID_$(basename $(@F))) --encrypt $(abspath $<) $(abspath $@) ) > $@.log
	@if [ ! -s $@ ]; then cat $@.log; echo "lv0: scetool made no $@"; exit 1; fi

$(addprefix $(LV0_EMBED_DIR)/,$(addsuffix .bin,$(LV0_SELF_SLOTS))): %.bin: %.self
	@size=$$(wc -c < $<); slot=$$(( $(LV0_SLOT_SIZE_$(basename $(@F))) )); \
	if [ $$size -gt $$slot ]; then \
		echo "lv0: $< is $$size bytes, larger than the $(basename $(@F)) slot ($$slot bytes)"; \
		exit 1; \
	fi
	cp $< $@
	truncate -s $$(( $(LV0_SLOT_SIZE_$(basename $(@F))) )) $@

$(addprefix $(LV0_EMBED_DIR)/,$(addsuffix .bin,$(LV0_ZERO_SLOTS))):
	@mkdir -p $(@D)
	rm -f $@
	truncate -s $$(( $(LV0_SLOT_SIZE_$(basename $(@F))) )) $@

$(LV0_ELF): $(LV0_OBJS) $(LV0_LDSCRIPT)
	$(LV0_LD) -T $(LV0_LDSCRIPT) -o $@ -EB -e _start --no-check-sections -Map $(LV0_MAP) $(LV0_OBJS)

check-lv0: $(LV0_ELF) $(LV0_IMAGE) $(addprefix $(LV0_EMBED_DIR)/,$(addsuffix .self,$(LV0_SELF_SLOTS)))
	@$(foreach s,$(LV0_SLOTS),printf 'lv0: slot %-8s %s %7d bytes  %s\n' $(s) $(LV0_SLOT_ADDR_$(s)) $$(( $(LV0_SLOT_SIZE_$(s)) )) "$(if $(filter $(s),$(LV0_SELF_SLOTS)),$(LV0_EMBED_DIR)/$(s).self ($$(wc -c < $(LV0_EMBED_DIR)/$(s).self) bytes) and zeros,zeros)";)
	@$(LV0_READELF) -h $(LV0_IMAGE) | grep -E 'Class|Data|OS/ABI|Type|Machine|Entry' > $(LV0_OUT)/header.image
	@$(LV0_READELF) -h $(LV0_ELF) | grep -E 'Class|Data|OS/ABI|Type|Machine|Entry' > $(LV0_OUT)/header.built
	@if cmp -s $(LV0_OUT)/header.image $(LV0_OUT)/header.built; then \
		echo "lv0: ELF header identical to $(LV0_IMAGE) (class, data, OS/ABI, type, machine, entry point)"; \
	else \
		echo "lv0: ELF header DIFFERS from $(LV0_IMAGE) (image, then build):"; \
		cat $(LV0_OUT)/header.image $(LV0_OUT)/header.built; \
		exit 1; \
	fi
	@$(LV0_READELF) -lW $(LV0_IMAGE) | grep LOAD > $(LV0_OUT)/segments.image
	@$(LV0_READELF) -lW $(LV0_ELF) | grep LOAD > $(LV0_OUT)/segments.built
	@if cmp -s $(LV0_OUT)/segments.image $(LV0_OUT)/segments.built; then \
		echo "lv0: program headers identical to $(LV0_IMAGE) ($$(wc -l < $(LV0_OUT)/segments.image) PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "lv0: program headers DIFFER from $(LV0_IMAGE) (image, then build):"; \
		cat $(LV0_OUT)/segments.image $(LV0_OUT)/segments.built; \
		exit 1; \
	fi
	@masked=0; n=0; \
	while read type off va pa fsz msz rest; do \
		for w in image built; do \
			if [ $$w = image ]; then f=$(LV0_IMAGE); else f=$(LV0_ELF); fi; \
			dd if=$$f of=$(LV0_OUT)/$$w.seg$$n bs=65536 skip=$$((off)) count=$$((fsz)) iflag=skip_bytes,count_bytes 2>/dev/null || exit 1; \
			for s in $(foreach s,$(LV0_SLOTS),$(LV0_SLOT_ADDR_$(s)):$(LV0_SLOT_SIZE_$(s))); do \
				a=$${s%:*}; z=$${s#*:}; \
				if [ $$((a)) -ge $$((va)) ] && [ $$((a + z)) -le $$((va + fsz)) ]; then \
					dd if=/dev/zero of=$(LV0_OUT)/$$w.seg$$n bs=65536 seek=$$((a - va)) count=$$((z)) oflag=seek_bytes iflag=count_bytes conv=notrunc 2>/dev/null || exit 1; \
					[ $$w = image ] && masked=$$((masked + 1)); \
				fi; \
			done; \
		done; \
		if cmp -s $(LV0_OUT)/image.seg$$n $(LV0_OUT)/built.seg$$n; then \
			echo "lv0: segment $$n ($$va, $$((fsz)) bytes) identical to $(LV0_IMAGE) outside the slots"; \
		else \
			echo "lv0: segment $$n ($$va) DIFFERS from $(LV0_IMAGE) outside the slots (offset in the segment + 1, image byte, built byte):"; \
			cmp -l $(LV0_OUT)/image.seg$$n $(LV0_OUT)/built.seg$$n | head -20; \
			echo "$$(cmp -l $(LV0_OUT)/image.seg$$n $(LV0_OUT)/built.seg$$n 2>/dev/null | wc -l) bytes differ"; \
			exit 1; \
		fi; \
		n=$$((n + 1)); \
	done < $(LV0_OUT)/segments.image; \
	if [ $$masked -ne $(words $(LV0_SLOTS)) ]; then \
		echo "lv0: $$masked of the $(words $(LV0_SLOTS)) slots in lv0/lv0.slots lie inside a segment"; \
		exit 1; \
	fi; \
	echo "lv0: every loaded byte identical but the $$masked slots of lv0/lv0.slots"
	@mkdir -p $(LV0_SELF_DIR)
	@for e in $(foreach s,$(LV0_SELF_SLOTS),$(s):$(LV0_SLOT_ADDR_$(s)):$(LV0_SLOT_SIZE_$(s)):$(or $(LV0_SELF_UNMATCHED_$(s)),-):$(LV0_SELF_ELF_$(s))); do \
		s=$${e%%:*}; e=$${e#*:}; a=$${e%%:*}; e=$${e#*:}; z=$${e%%:*}; e=$${e#*:}; u=$${e%%:*}; elf=$${e#*:}; \
		d=$(LV0_SELF_DIR)/$$s; \
		rm -f $$d.carried $$d.ours.elf $$d.stock.self $$d.stock.elf; \
		truncate -s $$(wc -c < $$elf) $$d.carried || exit 1; \
		phoff=$$(od -An -tu4 --endian=big -j28 -N4 $$elf); shoff=$$(od -An -tu4 --endian=big -j32 -N4 $$elf); \
		phnum=$$(od -An -tu2 --endian=big -j44 -N2 $$elf); shnum=$$(od -An -tu2 --endian=big -j48 -N2 $$elf); \
		dd if=$$elf of=$$d.carried bs=65536 count=$$((phoff + phnum * 32)) iflag=count_bytes conv=notrunc 2>/dev/null || exit 1; \
		dd if=$$elf of=$$d.carried bs=65536 skip=$$((shoff)) seek=$$((shoff)) count=$$((shnum * 40)) iflag=skip_bytes,count_bytes oflag=seek_bytes conv=notrunc 2>/dev/null || exit 1; \
		first=; \
		for l in $$($(LV0_SPU_READELF) -lW $$elf | awk '$$1 == "LOAD" { print $$2 ":" $$5 }'); do \
			off=$$(($${l%:*})); fsz=$$(($${l#*:})); \
			dd if=$$elf of=$$d.carried bs=65536 skip=$$off seek=$$off count=$$fsz iflag=skip_bytes,count_bytes oflag=seek_bytes conv=notrunc 2>/dev/null || exit 1; \
			if [ -z "$$first" ] && [ $$fsz -gt 0 ]; then first=$$off; fi; \
		done; \
		fo=; \
		while read type off va pa fsz rest; do \
			if [ $$((a)) -ge $$((va)) ] && [ $$((a + z)) -le $$((va + fsz)) ]; then fo=$$((off + a - va)); fi; \
		done < $(LV0_OUT)/segments.image; \
		dd if=$(LV0_IMAGE) of=$$d.stock.self bs=65536 skip=$$fo count=$$((z)) iflag=skip_bytes,count_bytes 2>/dev/null || exit 1; \
		( $(LV0_SCETOOL) --decrypt $(abspath $(LV0_EMBED_DIR))/$$s.self $(abspath $(LV0_SELF_DIR))/$$s.ours.elf ) > $$d.ours.log; \
		( $(LV0_SCETOOL) --decrypt $(abspath $(LV0_SELF_DIR))/$$s.stock.self $(abspath $(LV0_SELF_DIR))/$$s.stock.elf ) > $$d.stock.log; \
		for w in ours stock; do \
			if [ ! -s $$d.$$w.elf ]; then cat $$d.$$w.log; echo "lv0: slot $$s: scetool could not decrypt $$d.$$w.self"; exit 1; fi; \
		done; \
		if ! cmp -s $$d.ours.elf $$d.carried; then \
			echo "lv0: slot $$s: $(LV0_EMBED_DIR)/$$s.self decrypts to something else than $$elf's ELF header, program headers, segments and section headers (offset + 1, decrypted byte, built byte):"; \
			cmp -l $$d.ours.elf $$d.carried | head -20; \
			exit 1; \
		fi; \
		if [ "$$u" = - ]; then : > $$d.expected; else awk -v k=$$first '{ print $$1 + k, $$2, $$3 }' $$u > $$d.expected; fi; \
		cmp -l $$d.stock.elf $$d.carried 2>&1 | tr -s ' ' | sed 's/^ //' > $$d.stock.diff; \
		if [ $$(wc -c < $$d.stock.elf) -ne $$(wc -c < $$d.carried) ] || ! cmp -s $$d.stock.diff $$d.expected; then \
			echo "lv0: slot $$s: the stock SELF in $(LV0_IMAGE) decrypts to something else than $$elf's ELF header, program headers, segments and section headers (offset + 1, stock byte, built byte):"; \
			diff $$d.expected $$d.stock.diff | grep '^[<>]' | head -20; \
			exit 1; \
		fi; \
		printf 'lv0: slot %-6s %s (%d of %d bytes) and the stock SELF in %s both decrypt to the ELF header, program headers, segments and section headers of %s%s\n' \
			$$s $(LV0_EMBED_DIR)/$$s.self $$(wc -c < $(LV0_EMBED_DIR)/$$s.self) $$((z)) $(LV0_IMAGE) $$elf \
			"$$(if [ -s $$d.expected ]; then echo ", the stock one but for the $$(wc -l < $$d.expected) bytes of $$u"; fi)"; \
	done

clean-lv0:
	rm -rf $(LV0_OUT)

-include $(addprefix $(LV0_OBJ_DIR)/,$(addsuffix .d,$(basename $(LV0_CUT_UNITS))))
