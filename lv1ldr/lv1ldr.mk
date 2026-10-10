LV1LDR_OUT := $(BUILD_DIR)/lv1ldr
LV1LDR_OBJ_DIR := $(LV1LDR_OUT)/obj
LV1LDR_ELF := $(LV1LDR_OUT)/lv1ldr.elf
LV1LDR_BIN := $(LV1LDR_OUT)/lv1ldr.bin
LV1LDR_STRIPPED := $(LV1LDR_OUT)/lv1ldr.stripped.elf
LV1LDR_COMMENT := $(LV1LDR_OUT)/comment.bin
LV1LDR_COMMENT_SIZE := 0x112F
LV1LDR_MAP := $(LV1LDR_OUT)/lv1ldr.map
LV1LDR_LDSCRIPT := lv1ldr/lv1ldr.ld
LV1LDR_GLOBALS := lv1ldr/lv1ldr.globals
LV1LDR_UNMATCHED := lv1ldr/lv1ldr.unmatched

LV1LDR_IMAGE := lv1ldr/image/lv1ldr.elf
LV1LDR_IMAGE_BIN := $(LV1LDR_OUT)/image.bin
LV1LDR_LOAD_START := 0x12C00
LV1LDR_LOAD_END := 0x37FE8

LV1LDR_UNITS := \
	isoldr/src/crt0.S \
	lv1ldr/src/loader.cpp \
	lv1ldr/src/loader_data.cpp \
	lv1ldr/src/loader_boot.cpp \
	lv0ldr/src/dma/dma.cpp \
	lv1ldr/src/mmio_access.cpp \
	lv1ldr/src/loader_global.cpp \
	lv1ldr/src/main.cpp \
	lv1ldr/src/qa_flag.cpp \
	isoldr/src/qa_flag_data.cpp \
	lv1ldr/src/sb_product.cpp \
	lv1ldr/src/init_device.cpp \
	lv1ldr/src/init_device_data.cpp \
	lv1ldr/src/setup.cpp \
	lv1ldr/src/storage.cpp \
	lv1ldr/encdec/encdec.cpp \
	lv1ldr/encdec/activate.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	lv0ldr/src/log/log.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv1ldr/src/abort.cpp \
	lv1ldr/src/printf.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	isoldr/src/memcpy.c \
	lv0ldr/src/util/shuffle_table.c \
	lv1ldr/src/uart.cpp \
	lv1ldr/src/wb_aes.cpp \
	lv1ldr/src/wb_aes_data.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/aes256_cbc_encryptor.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	lv1ldr/src/loader_params.cpp \
	lv1ldr/src/params_data.cpp \
	lv1ldr/src/authenticator.cpp \
	lv1ldr/src/auth_buffers.cpp \
	isoldr/src/hmac_digest.cpp \
	lv1ldr/src/auth_segments.cpp \
	lv1ldr/src/auth_transfer.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	lv1ldr/src/elf_reader.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_modes.c \
	isoldr/crypto/aes_cmac.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv1ldr/crypto/prng.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_blocks.c \
	lv0ldr/crypto/sha1_transform.c \
	lv0ldr/crypto/sha1.c \
	isoldr/crypto/ecdsa.c \
	lv0ldr/crypto/ec.c \
	lv0ldr/crypto/ec_mul2.c \
	lv0ldr/crypto/bn_core.c \
	lv0ldr/crypto/bn_in_range.c \
	lv0ldr/crypto/ec_curve_valid.c \
	lv0ldr/crypto/ecdsa_id.c \
	lv0ldr/crypto/ec_curves.c \
	lv0ldr/crypto/ec_curve_table.c \
	lv0ldr/crypto/bn_from_bytes_not.c \
	isoldr/src/mfc.cpp \
	isoldr/src/ch64.cpp \
	lv1ldr/src/ch72.cpp \
	lv1ldr/src/auth_header.cpp \
	lv1ldr/src/auth_check.cpp \
	lv1ldr/src/auth_data.cpp \
	lv1ldr/zlib/lv1ldr_inflate.c \
	lv1ldr/zlib/inflate.c \
	lv1ldr/zlib/inftrees.c \
	lv1ldr/zlib/adler32.c \
	lv1ldr/zlib/zutil.c \
	lv1ldr/zlib/inffast.c

LV1LDR_LIBGCC_MEMBERS := divdi3 moddi3 udivdi3 umoddi3

LV1LDR_OBJS := \
	$(addprefix $(LV1LDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV1LDR_UNITS)))) \
	$(addprefix $(LV1LDR_OBJ_DIR)/lv0ldr/libgcc/,$(addsuffix .o,$(LV1LDR_LIBGCC_MEMBERS)))

LV1LDR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
LV1LDR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
LV1LDR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
LV1LDR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
LV1LDR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
LV1LDR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
LV1LDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc
LV1LDR_ENCDEC_CXX := $(SPU_GCC411_BETA_BIN)/spu-lv2-g++

LV1LDR_CFLAGS := -Os -ffunction-sections -fdata-sections
LV1LDR_CRYPTO_CFLAGS := -O3
LV1LDR_ZLIB_CFLAGS := -O3 -DNO_GZIP -ffunction-sections -fdata-sections
LV1LDR_ENCDEC_CFLAGS := -Os -ffunction-sections -fdata-sections
LV1LDR_LIBGCC_CFLAGS := -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions
LV1LDR_DEPFLAGS := -MMD -MP
LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
LV1LDR_LV0LDR_INCLUDES := -Ilv0ldr/include
LV1LDR_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
LV1LDR_ZLIB_INCLUDES := -Ilv1ldr/zlib/sys -Ilv1ldr/zlib
LV1LDR_ENCDEC_INCLUDE_DIR := $(LV1LDR_OBJ_DIR)/encdec_include
LV1LDR_ENCDEC_GCC_HEADERS := $(wildcard $(SPU_GCC411_BETA_BIN)/../lib/gcc/spu-lv2/4.1.1/include/*.h)
LV1LDR_ENCDEC_TARGET_HEADERS := $(wildcard $(SPU_GCC411_BETA_BIN)/../../../target/spu/include/*.h)
LV1LDR_ENCDEC_HEADERS := \
	$(LV1LDR_ENCDEC_GCC_HEADERS) \
	$(LV1LDR_ENCDEC_TARGET_HEADERS) \
	$(wildcard lv0ldr/include/*.h) \
	$(wildcard isoldr/include/*.h) \
	$(wildcard lv1ldr/include/*.h) \
	$(wildcard lv1ldr/encdec/sys/*.h)

LV1LDR_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
LV1LDR_LIBGCC_TEXT_MD5_divdi3 := 034ae84398f1c5acf600d932849ee96b
LV1LDR_LIBGCC_TEXT_MD5_moddi3 := 59a6f5252631e1ad6c51387514fd7001
LV1LDR_LIBGCC_TEXT_MD5_udivdi3 := f8c44df433ffc1a66dbd5ca4a0a63475
LV1LDR_LIBGCC_TEXT_MD5_umoddi3 := 8711d716069e15311f73103fe9d98eb2

$(LV1LDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov
$(LV1LDR_OBJ_DIR)/lv1ldr/src/wb_aes.o: UNIT_FLAGS := -O2

.PHONY: lv1ldr check-lv1ldr clean-lv1ldr

lv1ldr: $(LV1LDR_ELF) $(LV1LDR_BIN) $(LV1LDR_STRIPPED)

$(LV1LDR_OBJ_DIR)/lv1ldr/crypto/%.o: lv1ldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CRYPTO_CC) $(LV1LDR_CRYPTO_CFLAGS) $(LV1LDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/lv1ldr/zlib/%.o: lv1ldr/zlib/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CC) $(LV1LDR_ZLIB_CFLAGS) $(LV1LDR_ZLIB_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_ENCDEC_INCLUDE_DIR): $(LV1LDR_ENCDEC_HEADERS)
	rm -rf $@
	mkdir -p $@
	cp $(LV1LDR_ENCDEC_GCC_HEADERS) $@
	cp $(LV1LDR_ENCDEC_TARGET_HEADERS) $@
	cp $(wildcard lv0ldr/include/*.h) $@
	cp $(wildcard isoldr/include/*.h) $@
	cp $(wildcard lv1ldr/include/*.h) $@
	cp $(wildcard lv1ldr/encdec/sys/*.h) $@

$(LV1LDR_OBJ_DIR)/lv1ldr/encdec/%.o: lv1ldr/encdec/%.cpp $(LV1LDR_ENCDEC_INCLUDE_DIR)
	@mkdir -p $(@D)
	$(LV1LDR_ENCDEC_CXX) $(LV1LDR_ENCDEC_CFLAGS) -nostdinc -I$(LV1LDR_ENCDEC_INCLUDE_DIR) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(LV1LDR_CXX) $(LV1LDR_CFLAGS) $(LV1LDR_INCLUDES) $(UNIT_FLAGS) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CRYPTO_CC) $(LV1LDR_CRYPTO_CFLAGS) $(LV1LDR_ISOLDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CC) $(LV1LDR_CFLAGS) $(LV1LDR_ISOLDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(LV1LDR_CXX) $(LV1LDR_CFLAGS) $(LV1LDR_ISOLDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(LV1LDR_CXX) $(LV1LDR_CFLAGS) $(LV1LDR_ISOLDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CRYPTO_CC) $(LV1LDR_CRYPTO_CFLAGS) $(LV1LDR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/lv0ldr/libgcc/%.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(LV1LDR_LIBGCC_ATTEMPTS); do \
		echo "$(LV1LDR_CC) $(LV1LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@"; \
		$(LV1LDR_CC) $(LV1LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@ || exit 1; \
		$(LV1LDR_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(LV1LDR_LIBGCC_TEXT_MD5_$*)" ]; then \
			rm -f $@.text; \
			exit 0; \
		fi; \
		pad=$${pad}x; \
	done; \
	echo "lv1ldr: __$* never came out as the image's code in $(words $(LV1LDR_LIBGCC_ATTEMPTS)) compiles"; \
	rm -f $@ $@.text; \
	exit 1

$(LV1LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(LV1LDR_CC) $(LV1LDR_CFLAGS) $(LV1LDR_LV0LDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(LV1LDR_CXX) $(LV1LDR_CFLAGS) $(LV1LDR_LV0LDR_INCLUDES) $(LV1LDR_DEPFLAGS) -c $< -o $@

$(LV1LDR_ELF): $(LV1LDR_OBJS) $(LV1LDR_LDSCRIPT)
	$(LV1LDR_LD) -T $(LV1LDR_LDSCRIPT) --gc-sections -Map $(LV1LDR_MAP) -o $@ $(LV1LDR_OBJS)

$(LV1LDR_BIN): $(LV1LDR_ELF)
	$(LV1LDR_OBJCOPY) -O binary $< $@

$(LV1LDR_COMMENT):
	@mkdir -p $(@D)
	truncate -s $$(( $(LV1LDR_COMMENT_SIZE) )) $@

$(LV1LDR_STRIPPED): $(LV1LDR_ELF) $(LV1LDR_COMMENT)
	$(LV1LDR_OBJCOPY) --strip-all --add-section .comment=$(LV1LDR_COMMENT) $< $@

$(LV1LDR_IMAGE_BIN): $(LV1LDR_IMAGE)
	@mkdir -p $(@D)
	$(LV1LDR_OBJCOPY) -O binary $< $@

check-lv1ldr: $(LV1LDR_BIN) $(LV1LDR_IMAGE_BIN) $(LV1LDR_ELF) $(LV1LDR_STRIPPED)
	@$(LV1LDR_READELF) -lW $(LV1LDR_IMAGE) | grep LOAD > $(LV1LDR_OUT)/segments.image
	@$(LV1LDR_READELF) -lW $(LV1LDR_ELF) | grep LOAD > $(LV1LDR_OUT)/segments.built
	@if cmp -s $(LV1LDR_OUT)/segments.image $(LV1LDR_OUT)/segments.built; then \
		echo "lv1ldr: program headers identical to $(LV1LDR_IMAGE) (3 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "lv1ldr: program headers DIFFER from $(LV1LDR_IMAGE) (image, then build):"; \
		cat $(LV1LDR_OUT)/segments.image $(LV1LDR_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(LV1LDR_IMAGE_BIN) $(LV1LDR_BIN) > $(LV1LDR_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(LV1LDR_OUT)/differing.raw | sed 's/^ //' > $(LV1LDR_OUT)/differing.built; \
	if [ ! -s $(LV1LDR_OUT)/differing.built ]; then \
		echo "lv1ldr: $(LV1LDR_LOAD_START)-$(LV1LDR_LOAD_END) identical to $(LV1LDR_IMAGE)"; \
	elif cmp -s $(LV1LDR_OUT)/differing.built $(LV1LDR_UNMATCHED); then \
		echo "lv1ldr: $(LV1LDR_LOAD_START)-$(LV1LDR_LOAD_END) identical to $(LV1LDR_IMAGE) but for the $$(wc -l < $(LV1LDR_UNMATCHED)) bytes of the six functions that do not match yet: io_map::put_key, io_map::put_key2, kgen::gen, encdec::activate, lv0_vfprintf_engine and ch72_put_request ($(LV1LDR_UNMATCHED))"; \
	else \
		echo "lv1ldr: $(LV1LDR_LOAD_START)-$(LV1LDR_LOAD_END) DIFFERS from $(LV1LDR_IMAGE) (offset from $(LV1LDR_LOAD_START) + 1, image byte, built byte):"; \
		diff $(LV1LDR_UNMATCHED) $(LV1LDR_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(LV1LDR_OUT)/differing.built) bytes differ, $$(wc -l < $(LV1LDR_UNMATCHED)) of them expected ($(LV1LDR_UNMATCHED))"; \
		exit 1; \
	fi
	@$(LV1LDR_NM) $(LV1LDR_ELF) | cut -d' ' -f1,3 > $(LV1LDR_OUT)/globals.built
	@if grep -Fxv -f $(LV1LDR_OUT)/globals.built $(LV1LDR_GLOBALS); then \
		echo "lv1ldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "lv1ldr: all $$(wc -l < $(LV1LDR_GLOBALS)) globals at the image's addresses (.bss, .rodata, .data)"; \
	fi
	@phoff=$$(od -An -tu4 --endian=big -j28 -N4 $(LV1LDR_IMAGE)); \
	shoff=$$(od -An -tu4 --endian=big -j32 -N4 $(LV1LDR_IMAGE)); \
	phnum=$$(od -An -tu2 --endian=big -j44 -N2 $(LV1LDR_IMAGE)); \
	shnum=$$(od -An -tu2 --endian=big -j48 -N2 $(LV1LDR_IMAGE)); \
	for w in image built; do \
		if [ $$w = image ]; then f=$(LV1LDR_IMAGE); else f=$(LV1LDR_STRIPPED); fi; \
		{ dd if=$$f bs=52 count=1 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((phoff)) count=$$((phnum * 32)) iflag=skip_bytes,count_bytes 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((shoff)) count=$$((shnum * 40)) iflag=skip_bytes,count_bytes 2>/dev/null; } > $(LV1LDR_OUT)/headers.$$w || exit 1; \
	done; \
	if cmp -s $(LV1LDR_OUT)/headers.image $(LV1LDR_OUT)/headers.built; then \
		echo "lv1ldr: $(LV1LDR_STRIPPED): ELF header, $$((phnum)) program headers and $$((shnum)) section headers identical to $(LV1LDR_IMAGE)"; \
	else \
		echo "lv1ldr: $(LV1LDR_STRIPPED): ELF header, program headers or section headers DIFFER from $(LV1LDR_IMAGE) (offset in the three + 1, image byte, built byte):"; \
		cmp -l $(LV1LDR_OUT)/headers.image $(LV1LDR_OUT)/headers.built | head -20; \
		exit 1; \
	fi

clean-lv1ldr:
	rm -rf $(LV1LDR_OUT)

-include $(addprefix $(LV1LDR_OBJ_DIR)/,$(addsuffix .d,$(basename $(LV1LDR_UNITS))))
