LV0LDR2_OUT := $(BUILD_DIR)/lv0ldr2
LV0LDR2_OBJ_DIR := $(LV0LDR2_OUT)/obj
LV0LDR2_ELF := $(LV0LDR2_OUT)/lv0ldr2.elf
LV0LDR2_BIN := $(LV0LDR2_OUT)/lv0ldr2.bin
LV0LDR2_MAP := $(LV0LDR2_OUT)/lv0ldr2.map
LV0LDR2_LDSCRIPT := lv0ldr2/lv0ldr2.ld
LV0LDR2_GLOBALS := lv0ldr2/lv0ldr2.globals
LV0LDR2_UNMATCHED := lv0ldr2/lv0ldr2.unmatched

LV0LDR2_IMAGE := lv0ldr2/image/lv0ldr2.bin
LV0LDR2_LOAD_START := 0x400
LV0LDR2_LOAD_END := 0x303B0

# the memory test's SPU program (spu_memtest.S): not in this tree, taken
# out of the image (0x2A280, 24721 bytes)
LV0LDR2_MEMTEST := $(LV0LDR2_OUT)/spu_memtest.elf
LV0LDR2_MEMTEST_AT := 0x2A280
LV0LDR2_MEMTEST_SIZE := 24721

LV0LDR2_UNITS := \
	isoldr/src/crt0.S \
	lv0ldr2/src/boot/main.cpp \
	lv0ldr2/src/boot/loader.cpp \
	lv0ldr2/src/dma/dma.cpp \
	lv0ldr2/src/auth/loader_auth.cpp \
	lv0ldr2/src/util/mmio_access.cpp \
	lv0ldr2/src/platform/config_ring.cpp \
	lv0ldr2/src/flash/ros_entry.cpp \
	lv0ldr2/src/flash/ros_header.cpp \
	lv2ldr/src/auth_checks.cpp \
	lv0ldr2/src/auth/authenticator.cpp \
	lv1ldr/src/auth_buffers.cpp \
	appldr/src/auth_app.cpp \
	isoldr/src/auth_segments.cpp \
	lv1ldr/src/auth_transfer.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	lv2ldr/src/hash_reader.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	lv1ldr/src/elf_reader.cpp \
	isoldr/src/hmac_digest.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_encrypt8.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_transform.c \
	lv0ldr/crypto/ecdsa_id.c \
	lv0ldr/crypto/ec_curves.c \
	isoldr/crypto/ecdsa.c \
	lv0ldr/crypto/ec.c \
	lv0ldr/crypto/ec_mul2.c \
	lv0ldr/crypto/bn_core.c \
	lv0ldr/crypto/bn_in_range.c \
	lv0ldr/crypto/ec_curve_valid.c \
	lv0ldr/crypto/bn_from_bytes_not.c \
	lv0ldr/crypto/ec_curve_table.c \
	isoldr/src/mfc.cpp \
	lv0ldr2/src/util/cxx_runtime.cpp \
	lv1ldr/src/auth_header.cpp \
	lv0ldr2/src/auth/auth_check.cpp \
	lv0ldr2/src/auth/auth_data.cpp \
	lv0ldr2/src/boot/loader_unit.cpp \
	lv0ldr2/src/xdr/xdr.cpp \
	lv0ldr2/src/boot/sc_config_info.cpp \
	lv0ldr2/src/boot/memory_diag.cpp \
	lv0ldr2/src/boot/spu_memtest.S \
	lv0ldr2/src/platform/be_mmio.cpp \
	lv0ldr2/src/xdr/xdr_calib.cpp \
	lv0ldr2/src/xdr/xdr_ptcal.cpp \
	lv0ldr2/src/xdr/xdr_dump.cpp \
	lv0ldr2/src/xdr/xdr_serial.cpp \
	lv0ldr2/src/io/iommu.cpp \
	lv0ldr2/src/flash/nand_flash_ctrl.cpp \
	lv0ldr2/src/flash/nand_flash_transfer.cpp \
	lv0ldr2/src/util/delay.cpp \
	lv1ldr/src/uart.cpp \
	lv0ldr2/src/flash/flash_probe.cpp \
	lv0ldr2/src/platform/pio.cpp \
	lv0ldr2/src/io/sb_registers.cpp \
	lv0ldr2/src/io/init_sb_device.cpp \
	lv0ldr2/src/io/get_sb_device.cpp \
	lv0ldr2/src/io/systemio_dmac_dx.cpp \
	lv0ldr2/src/io/systemio_dmac_px.cpp \
	lv0ldr2/src/io/iopt.cpp \
	lv0ldr2/src/syscon/syscon_config.cpp \
	lv0ldr2/src/syscon/syscon_xdr.cpp \
	lv0ldr2/src/syscon/sc_console.cpp \
	lv0ldr2/src/syscon/syscon.cpp \
	lv0ldr2/src/syscon/syscon_power.cpp \
	lv0ldr2/src/storage/nv_storage.cpp \
	lv0ldr2/src/syscon/sc_nv_storage.cpp \
	lv0ldr2/src/log/log.cpp \
	lv0ldr2/src/boot/exit.cpp \
	lv1ldr/src/printf.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	lv0ldr2/src/util/strncmp.c \
	isoldr/src/memcpy.c \
	lv0ldr2/src/util/shuffle_table.c

LV0LDR2_LIBGCC_MEMBERS := divdi3 moddi3 udivdi3 umoddi3

LV0LDR2_OBJS := \
	$(addprefix $(LV0LDR2_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV0LDR2_UNITS)))) \
	$(addprefix $(LV0LDR2_OBJ_DIR)/lv0ldr/libgcc/,$(addsuffix .o,$(LV0LDR2_LIBGCC_MEMBERS)))

LV0LDR2_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
LV0LDR2_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
LV0LDR2_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
LV0LDR2_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
LV0LDR2_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
LV0LDR2_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc
# libgcc's members are the SDK 3.50/3.70 prebuilt libgcc.a's: SDK350's
# compiler, with SDK420's spu_intrinsics.h
LV0LDR2_LIBGCC_CC := $(SPU_GCC411_SDK350_BIN)/spu-lv2-gcc
LV0LDR2_LIBGCC_INCLUDES := -idirafter $(SPU_GCC411_SDK420_BIN)/../../../target/spu/include

LV0LDR2_CFLAGS := -Os -ffunction-sections -fdata-sections
LV0LDR2_CRYPTO_CFLAGS := -O3
LV0LDR2_LIBGCC_CFLAGS := -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions
LV0LDR2_DEPFLAGS := -MMD -MP
LV0LDR2_INCLUDES := -Ilv0ldr2/include -Ilv0ldr/include
LV0LDR2_LV0LDR_INCLUDES := -Ilv0ldr/include
LV0LDR2_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
LV0LDR2_LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
LV0LDR2_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
LV0LDR2_APPLDR_INCLUDES := -Iappldr/include -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include

# the compiler's code for libgcc's members is not always the same run to
# run (libgcc2.c says why): each is compiled again, an unused define
# longer each time, until its .text is the image's
LV0LDR2_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
LV0LDR2_LIBGCC_TEXT_MD5_divdi3 := 034ae84398f1c5acf600d932849ee96b
LV0LDR2_LIBGCC_TEXT_MD5_moddi3 := ccd53ab4c077ba70532993dd852b6800
LV0LDR2_LIBGCC_TEXT_MD5_udivdi3 := f8c44df433ffc1a66dbd5ca4a0a63475
LV0LDR2_LIBGCC_TEXT_MD5_umoddi3 := ffb89e36b4a3cf9a128c7fa2aa72e055

$(LV0LDR2_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: lv0ldr2 check-lv0ldr2 clean-lv0ldr2

lv0ldr2: $(LV0LDR2_ELF) $(LV0LDR2_BIN)

$(LV0LDR2_MEMTEST): $(LV0LDR2_IMAGE)
	@mkdir -p $(@D)
	dd if=$< of=$@ bs=65536 skip=$$(( $(LV0LDR2_MEMTEST_AT) - $(LV0LDR2_LOAD_START) )) count=$(LV0LDR2_MEMTEST_SIZE) iflag=skip_bytes,count_bytes 2>/dev/null
	@if [ "$$(wc -c < $@)" != "$(LV0LDR2_MEMTEST_SIZE)" ]; then echo "lv0ldr2: no memory test program in $<"; rm -f $@; exit 1; fi

$(LV0LDR2_OBJ_DIR)/lv0ldr2/src/boot/spu_memtest.o: lv0ldr2/src/boot/spu_memtest.S $(LV0LDR2_MEMTEST)
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) -Wa,-I$(LV0LDR2_OUT) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv0ldr2/%.o: lv0ldr2/%.c
	@mkdir -p $(@D)
	$(LV0LDR2_CC) $(LV0LDR2_CFLAGS) $(LV0LDR2_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv0ldr2/%.o: lv0ldr2/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV0LDR2_CRYPTO_CC) $(LV0LDR2_CRYPTO_CFLAGS) $(LV0LDR2_ISOLDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(LV0LDR2_CC) $(LV0LDR2_CFLAGS) $(LV0LDR2_ISOLDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_ISOLDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_ISOLDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV0LDR2_CRYPTO_CC) $(LV0LDR2_CRYPTO_CFLAGS) $(LV0LDR2_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv0ldr/libgcc/%.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(LV0LDR2_LIBGCC_ATTEMPTS); do \
		echo "$(LV0LDR2_LIBGCC_CC) $(LV0LDR2_LIBGCC_CFLAGS) $(LV0LDR2_LIBGCC_INCLUDES) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@"; \
		$(LV0LDR2_LIBGCC_CC) $(LV0LDR2_LIBGCC_CFLAGS) $(LV0LDR2_LIBGCC_INCLUDES) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@ || exit 1; \
		$(LV0LDR2_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(LV0LDR2_LIBGCC_TEXT_MD5_$*)" ]; then \
			rm -f $@.text; \
			exit 0; \
		fi; \
		pad=$${pad}x; \
	done; \
	echo "lv0ldr2: __$* never came out as the image's code in $(words $(LV0LDR2_LIBGCC_ATTEMPTS)) compiles"; \
	rm -f $@ $@.text; \
	exit 1

$(LV0LDR2_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(LV0LDR2_CC) $(LV0LDR2_CFLAGS) $(LV0LDR2_LV0LDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_LV0LDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_LV1LDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_LV2LDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_OBJ_DIR)/appldr/%.o: appldr/%.cpp
	@mkdir -p $(@D)
	$(LV0LDR2_CXX) $(LV0LDR2_CFLAGS) $(LV0LDR2_APPLDR_INCLUDES) $(LV0LDR2_DEPFLAGS) -c $< -o $@

$(LV0LDR2_ELF): $(LV0LDR2_OBJS) $(LV0LDR2_LDSCRIPT)
	$(LV0LDR2_LD) -T $(LV0LDR2_LDSCRIPT) --gc-sections -Map $(LV0LDR2_MAP) -o $@ $(LV0LDR2_OBJS)

$(LV0LDR2_BIN): $(LV0LDR2_ELF)
	$(LV0LDR2_OBJCOPY) -O binary -j .text -j .rodata -j .data -j .ctors -j .dtors --gap-fill 0 --pad-to $(LV0LDR2_LOAD_END) $< $@
	truncate -s $$(( $(LV0LDR2_LOAD_END) - $(LV0LDR2_LOAD_START) )) $@

check-lv0ldr2: $(LV0LDR2_BIN) $(LV0LDR2_ELF)
	@cmp -l $(LV0LDR2_IMAGE) $(LV0LDR2_BIN) > $(LV0LDR2_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(LV0LDR2_OUT)/differing.raw | sed 's/^ //' > $(LV0LDR2_OUT)/differing.built; \
	if [ ! -s $(LV0LDR2_OUT)/differing.built ]; then \
		echo "lv0ldr2: $(LV0LDR2_LOAD_START)-$(LV0LDR2_LOAD_END) identical to $(LV0LDR2_IMAGE)"; \
	elif cmp -s $(LV0LDR2_OUT)/differing.built $(LV0LDR2_UNMATCHED); then \
		echo "lv0ldr2: $(LV0LDR2_LOAD_START)-$(LV0LDR2_LOAD_END) identical to $(LV0LDR2_IMAGE) but for the $$(wc -l < $(LV0LDR2_UNMATCHED)) bytes of strncmp and lv0_vfprintf_engine, the two functions that do not match yet, and of xdr_dump.cpp's messages, where the image has \"\nXIG314\" between dump_mic's and dump_xio's ($(LV0LDR2_UNMATCHED))"; \
	else \
		echo "lv0ldr2: $(LV0LDR2_LOAD_START)-$(LV0LDR2_LOAD_END) DIFFERS from $(LV0LDR2_IMAGE) (offset from $(LV0LDR2_LOAD_START) + 1, image byte, built byte):"; \
		diff $(LV0LDR2_UNMATCHED) $(LV0LDR2_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(LV0LDR2_OUT)/differing.built) bytes differ, $$(wc -l < $(LV0LDR2_UNMATCHED)) of them expected ($(LV0LDR2_UNMATCHED))"; \
		exit 1; \
	fi
	@$(LV0LDR2_NM) $(LV0LDR2_ELF) | cut -d' ' -f1,3 > $(LV0LDR2_OUT)/globals.built
	@if grep -Fxv -f $(LV0LDR2_OUT)/globals.built $(LV0LDR2_GLOBALS); then \
		echo "lv0ldr2: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "lv0ldr2: all $$(wc -l < $(LV0LDR2_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-lv0ldr2:
	rm -rf $(LV0LDR2_OUT)

-include $(addprefix $(LV0LDR2_OBJ_DIR)/,$(addsuffix .d,$(basename $(LV0LDR2_UNITS))))
