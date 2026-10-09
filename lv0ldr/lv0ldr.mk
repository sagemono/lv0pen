LV0LDR_OUT := $(BUILD_DIR)/lv0ldr
LV0LDR_OBJ_DIR := $(LV0LDR_OUT)/obj
LV0LDR_ELF := $(LV0LDR_OUT)/lv0ldr.elf
LV0LDR_BIN := $(LV0LDR_OUT)/lv0ldr.bin
LV0LDR_MAP := $(LV0LDR_OUT)/lv0ldr.map
LV0LDR_LDSCRIPT := lv0ldr/lv0ldr.ld
LV0LDR_GLOBALS := lv0ldr/lv0ldr.globals
LV0LDR_WRITTEN := lv0ldr/lv0ldr.written

LV0LDR_IMAGE := lv0ldr/image/lv0ldr_1.0.0.bin
LV0LDR_LOAD_START := 0x400
LV0LDR_LOAD_START_BYTES := 1024
LV0LDR_LOAD_END := 0x2A6A0
LV0LDR_LOAD_BYTES := 172704

LV0LDR_UNITS := \
	lv0ldr/src/boot/crt0.S \
	lv0ldr/src/boot/main.cpp \
	lv0ldr/src/dma/dma.cpp \
	lv0ldr/src/boot/loader.cpp \
	lv0ldr/src/util/mmio_access.cpp \
	lv0ldr/src/auth/loader_auth.cpp \
	lv0ldr/src/platform/config_ring.cpp \
	lv0ldr/src/flash/ros_header.cpp \
	lv0ldr/src/util/strncmp.c \
	lv0ldr/src/util/memcpy.c \
	lv0ldr/src/flash/ros_entry.cpp \
	lv0ldr/src/auth/authenticator.cpp \
	lv0ldr/src/auth/auth_options.cpp \
	lv0ldr/src/util/memset.c \
	lv0ldr/src/auth/auth_segments.cpp \
	lv0ldr/src/auth/auth_transfer.cpp \
	lv0ldr/src/util/memcmp.c \
	lv0ldr/src/auth/auth_buffers.cpp \
	lv0ldr/src/auth/cipher_aes256_cbc.cpp \
	lv0ldr/src/auth/cipher_aes128_cbc.cpp \
	lv0ldr/src/auth/cipher_aes128_ctr.cpp \
	lv0ldr/src/auth/verifier_ecdsa.cpp \
	lv0ldr/src/auth/hmac_digest.cpp \
	lv0ldr/src/auth/elf_reader.cpp \
	lv0ldr/src/dma/dma_queue.cpp \
	lv0ldr/src/dma/dma_buffer.cpp \
	lv0ldr/src/dma/dma_buffer_io.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_transform.c \
	lv0ldr/crypto/ecdsa_id.c \
	lv0ldr/crypto/ec_curves.c \
	lv0ldr/crypto/ecdsa.c \
	lv0ldr/crypto/ec.c \
	lv0ldr/crypto/ec_mul2.c \
	lv0ldr/crypto/bn.c \
	lv0ldr/crypto/bn_range.c \
	lv0ldr/crypto/ec_curve_valid.c \
	lv0ldr/crypto/ec_curve_table.c \
	lv0ldr/src/dma/mfc.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	lv0ldr/src/xdr/xdr_config.cpp \
	lv0ldr/src/boot/loader_unit.cpp \
	lv0ldr/src/boot/memory_config.cpp \
	lv0ldr/src/boot/memory_diag.cpp \
	lv0ldr/src/platform/be_mmio.cpp \
	lv0ldr/src/xdr/xdr.cpp \
	lv0ldr/src/xdr/xdr_calib.cpp \
	lv0ldr/src/xdr/xdr_dump.cpp \
	lv0ldr/src/xdr/xdr_serial.cpp \
	lv0ldr/src/io/iommu.cpp \
	lv0ldr/src/flash/nand_flash_ctrl.cpp \
	lv0ldr/src/flash/nand_flash_transfer.cpp \
	lv0ldr/src/util/delay.cpp \
	lv0ldr/src/console/uart.cpp \
	lv0ldr/src/flash/flash_probe.cpp \
	lv0ldr/src/platform/pio.cpp \
	lv0ldr/src/io/sb_registers.cpp \
	lv0ldr/src/io/init_sb_device.cpp \
	lv0ldr/src/io/get_sb_device.cpp \
	lv0ldr/src/io/systemio_dmac_dx.cpp \
	lv0ldr/src/io/systemio_dmac_px.cpp \
	lv0ldr/src/io/iopt.cpp \
	lv0ldr/src/syscon/syscon_xdr.cpp \
	lv0ldr/src/syscon/syscon_config.cpp \
	lv0ldr/src/syscon/sc_console.cpp \
	lv0ldr/src/syscon/syscon.cpp \
	lv0ldr/src/syscon/syscon_power.cpp \
	lv0ldr/src/storage/nv_storage.cpp \
	lv0ldr/src/syscon/sc_dev_access.cpp \
	lv0ldr/src/syscon/sc_nv_storage.cpp \
	lv0ldr/src/log/log.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv0ldr/src/log/printf.cpp \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/src/util/shuffle_table.c

LV0LDR_LIBGCC_MEMBERS := divdi3 moddi3 udivdi3 umoddi3

LV0LDR_OBJS := \
	$(addprefix $(LV0LDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV0LDR_UNITS)))) \
	$(addprefix $(LV0LDR_OBJ_DIR)/lv0ldr/libgcc/,$(addsuffix .o,$(LV0LDR_LIBGCC_MEMBERS)))

LV0LDR_CC := $(SPU_GCC402_BIN)/spu-lv2-gcc
LV0LDR_CXX := $(SPU_GCC402_BIN)/spu-lv2-g++
LV0LDR_LD := $(SPU_GCC402_BIN)/spu-lv2-ld
LV0LDR_OBJCOPY := $(SPU_GCC402_BIN)/spu-lv2-objcopy
LV0LDR_NM := $(SPU_GCC402_BIN)/spu-lv2-nm
LV0LDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

LV0LDR_CFLAGS := -Os -ffunction-sections -fdata-sections
LV0LDR_CRYPTO_CFLAGS := -O3
LV0LDR_LIBGCC_CFLAGS := -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions
LV0LDR_DEPFLAGS := -MMD -MP
LV0LDR_INCLUDES := -Ilv0ldr/include

LV0LDR_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
LV0LDR_LIBGCC_TEXT_MD5_divdi3 := 1462204cf5229afe35e5d11166fd72a0
LV0LDR_LIBGCC_TEXT_MD5_moddi3 := 82b687cff67409fa346b987c5d8ff95e
LV0LDR_LIBGCC_TEXT_MD5_udivdi3 := 0922b0e3e6f0f875ba188ddf164e053b
LV0LDR_LIBGCC_TEXT_MD5_umoddi3 := 8829b3f3e565a78aed40468d0bdc8f96

$(LV0LDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: lv0ldr check-lv0ldr clean-lv0ldr

lv0ldr: $(LV0LDR_ELF) $(LV0LDR_BIN)

$(LV0LDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV0LDR_CRYPTO_CC) $(LV0LDR_CRYPTO_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV0LDR_DEPFLAGS) -c $< -o $@

$(LV0LDR_OBJ_DIR)/lv0ldr/libgcc/%.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(LV0LDR_LIBGCC_ATTEMPTS); do \
		echo "$(LV0LDR_CC) $(LV0LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@"; \
		$(LV0LDR_CC) $(LV0LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@ || exit 1; \
		$(LV0LDR_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(LV0LDR_LIBGCC_TEXT_MD5_$*)" ]; then \
			rm -f $@.text; \
			exit 0; \
		fi; \
		pad=$${pad}x; \
	done; \
	echo "lv0ldr: __$* never came out as the image's code in $(words $(LV0LDR_LIBGCC_ATTEMPTS)) compiles"; \
	rm -f $@ $@.text; \
	exit 1

$(LV0LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(LV0LDR_CC) $(LV0LDR_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV0LDR_DEPFLAGS) -c $< -o $@

$(LV0LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR_CXX) $(LV0LDR_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV0LDR_DEPFLAGS) -c $< -o $@

$(LV0LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.S
	@mkdir -p $(@D)
	$(LV0LDR_CXX) $(LV0LDR_CFLAGS) $(LV0LDR_INCLUDES) $(LV0LDR_DEPFLAGS) -c $< -o $@

$(LV0LDR_ELF): $(LV0LDR_OBJS) $(LV0LDR_LDSCRIPT)
	$(LV0LDR_LD) -T $(LV0LDR_LDSCRIPT) --gc-sections -Map $(LV0LDR_MAP) -o $@ $(LV0LDR_OBJS)

$(LV0LDR_BIN): $(LV0LDR_ELF)
	$(LV0LDR_OBJCOPY) -O binary -j .text -j .rodata -j .data -j .ctors -j .dtors --gap-fill 0 --pad-to $(LV0LDR_LOAD_END) $< $@
	truncate -s $$(( $(LV0LDR_LOAD_END) - $(LV0LDR_LOAD_START) )) $@

check-lv0ldr: $(LV0LDR_BIN) $(LV0LDR_ELF)
	@cmp -l -n $(LV0LDR_LOAD_BYTES) -i $(LV0LDR_LOAD_START_BYTES):0 $(LV0LDR_IMAGE) $(LV0LDR_BIN) > $(LV0LDR_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(LV0LDR_OUT)/differing.raw | sed 's/^ //' > $(LV0LDR_OUT)/differing.built; \
	if [ ! -s $(LV0LDR_OUT)/differing.built ]; then \
		echo "lv0ldr: $(LV0LDR_LOAD_START)-$(LV0LDR_LOAD_END) identical to $(LV0LDR_IMAGE)"; \
	elif cmp -s $(LV0LDR_OUT)/differing.built $(LV0LDR_WRITTEN); then \
		echo "lv0ldr: $(LV0LDR_LOAD_START)-$(LV0LDR_LOAD_END) identical to $(LV0LDR_IMAGE) but for the $$(wc -l < $(LV0LDR_WRITTEN)) bytes of .data the loader wrote as it ran ($(LV0LDR_WRITTEN))"; \
	else \
		echo "lv0ldr: $(LV0LDR_LOAD_START)-$(LV0LDR_LOAD_END) DIFFERS from $(LV0LDR_IMAGE) (offset from $(LV0LDR_LOAD_START) + 1, image byte, built byte):"; \
		diff $(LV0LDR_WRITTEN) $(LV0LDR_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(LV0LDR_OUT)/differing.built) bytes differ, $$(wc -l < $(LV0LDR_WRITTEN)) of them expected ($(LV0LDR_WRITTEN))"; \
		exit 1; \
	fi
	@$(LV0LDR_NM) $(LV0LDR_ELF) | cut -d' ' -f1,3 > $(LV0LDR_OUT)/globals.built
	@if grep -Fxv -f $(LV0LDR_OUT)/globals.built $(LV0LDR_GLOBALS); then \
		echo "lv0ldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "lv0ldr: all $$(wc -l < $(LV0LDR_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-lv0ldr:
	rm -rf $(LV0LDR_OUT)

-include $(addprefix $(LV0LDR_OBJ_DIR)/,$(addsuffix .d,$(basename $(LV0LDR_UNITS))))
