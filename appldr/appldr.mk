APPLDR_OUT := $(BUILD_DIR)/appldr
APPLDR_OBJ_DIR := $(APPLDR_OUT)/obj
APPLDR_ELF := $(APPLDR_OUT)/appldr.elf
APPLDR_BIN := $(APPLDR_OUT)/appldr.bin
APPLDR_STRIPPED := $(APPLDR_OUT)/appldr.stripped.elf
APPLDR_COMMENT := $(APPLDR_OUT)/comment.bin
APPLDR_COMMENT_SIZE := 0x11F2
APPLDR_MAP := $(APPLDR_OUT)/appldr.map
APPLDR_LDSCRIPT := appldr/appldr.ld
APPLDR_GLOBALS := appldr/appldr.globals

APPLDR_IMAGE := appldr/image/appldr.elf
APPLDR_IMAGE_BIN := $(APPLDR_OUT)/image.bin
APPLDR_LOAD_START := 0x12C00
APPLDR_LOAD_END := 0x38CA0

APPLDR_UNITS := \
	isoldr/src/crt0.S \
	appldr/src/loader_global.cpp \
	appldr/src/main.cpp \
	appldr/src/auth_flags.cpp \
	appldr/src/loader.cpp \
	appldr/src/loader_data.cpp \
	appldr/src/segment_loader.cpp \
	isoldr/src/qa_flag.cpp \
	isoldr/src/qa_flag_data.cpp \
	appldr/src/hash_verify.cpp \
	lv2ldr/src/hash_reader.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/aes256_cbc_encryptor.cpp \
	lv2ldr/src/auth_checks.cpp \
	appldr/src/auth_app.cpp \
	appldr/src/auth_app_checks.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	lv1ldr/src/loader_params.cpp \
	lv2ldr/src/revoke_list.cpp \
	lv1ldr/src/authenticator.cpp \
	isoldr/src/elf_reader32.cpp \
	lv1ldr/src/auth_buffers.cpp \
	isoldr/src/hmac_digest.cpp \
	lv2ldr/src/revoke_check.cpp \
	appldr/src/auth_app_transfer.cpp \
	lv1ldr/src/auth_transfer.cpp \
	appldr/src/data_decryptor.cpp \
	appldr/src/decryptor_data.cpp \
	appldr/src/mac_aes_cmac.cpp \
	appldr/src/mac_hmac_sha1.cpp \
	appldr/src/mac_type1.cpp \
	appldr/src/auth_app_control.cpp \
	appldr/src/cipher_aes128_cbc_chain.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	appldr/src/cipher_plain.cpp \
	lv1ldr/src/elf_reader.cpp \
	lv2ldr/src/revoke_scan.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_modes.c \
	isoldr/crypto/aes_cmac.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv0ldr/crypto/aes_encrypt8.c \
	lv0ldr/crypto/aes_decrypt8.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_transform.c \
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
	appldr/src/mfc_ext.cpp \
	appldr/src/mfc_atomic.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	isoldr/src/ch64.cpp \
	lv2ldr/src/ch72.cpp \
	appldr/src/auth_data.cpp \
	lv1ldr/src/auth_header.cpp \
	lv1ldr/src/auth_check.cpp \
	lv1ldr/zlib/lv1ldr_inflate.c \
	lv1ldr/zlib/inflate.c \
	lv1ldr/zlib/inftrees.c \
	lv1ldr/zlib/adler32.c \
	lv1ldr/zlib/zutil.c \
	lv1ldr/zlib/inffast.c \
	lv0ldr/src/boot/exit.cpp \
	lv1ldr/src/abort.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	isoldr/src/memcpy.c \
	lv0ldr/src/util/shuffle_table.c

APPLDR_LIBGCC_MEMBERS := muldi3 umoddi3

APPLDR_OBJS := \
	$(addprefix $(APPLDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(APPLDR_UNITS)))) \
	$(addprefix $(APPLDR_OBJ_DIR)/lv0ldr/libgcc/,$(addsuffix .o,$(APPLDR_LIBGCC_MEMBERS)))

APPLDR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
APPLDR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
APPLDR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
APPLDR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
APPLDR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
APPLDR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
APPLDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

APPLDR_CFLAGS := -Os -ffunction-sections -fdata-sections
APPLDR_CRYPTO_CFLAGS := -O3
APPLDR_ZLIB_CFLAGS := -O3 -DNO_GZIP -ffunction-sections -fdata-sections
APPLDR_LIBGCC_CFLAGS := -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions
APPLDR_DEPFLAGS := -MMD -MP
APPLDR_INCLUDES := -Iappldr/include -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
APPLDR_LV0LDR_INCLUDES := -Ilv0ldr/include
APPLDR_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
APPLDR_LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
APPLDR_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
APPLDR_ZLIB_INCLUDES := -Ilv1ldr/zlib/sys -Ilv1ldr/zlib

APPLDR_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
APPLDR_LIBGCC_TEXT_MD5_muldi3 := 25a961649015c079fe263056ba569136
APPLDR_LIBGCC_TEXT_MD5_umoddi3 := 8711d716069e15311f73103fe9d98eb2

$(APPLDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: appldr check-appldr clean-appldr

appldr: $(APPLDR_ELF) $(APPLDR_BIN) $(APPLDR_STRIPPED)

$(APPLDR_OBJ_DIR)/appldr/%.o: appldr/%.cpp
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_LV2LDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(APPLDR_CRYPTO_CC) $(APPLDR_CRYPTO_CFLAGS) $(APPLDR_ISOLDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(APPLDR_CC) $(APPLDR_CFLAGS) $(APPLDR_ISOLDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_ISOLDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_ISOLDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv1ldr/zlib/%.o: lv1ldr/zlib/%.c
	@mkdir -p $(@D)
	$(APPLDR_CC) $(APPLDR_ZLIB_CFLAGS) $(APPLDR_ZLIB_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_LV1LDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(APPLDR_CRYPTO_CC) $(APPLDR_CRYPTO_CFLAGS) $(APPLDR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv0ldr/libgcc/%.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(APPLDR_LIBGCC_ATTEMPTS); do \
		echo "$(APPLDR_CC) $(APPLDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@"; \
		$(APPLDR_CC) $(APPLDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@ || exit 1; \
		$(APPLDR_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(APPLDR_LIBGCC_TEXT_MD5_$*)" ]; then \
			rm -f $@.text; \
			exit 0; \
		fi; \
		pad=$${pad}x; \
	done; \
	echo "appldr: __$* never came out as the image's code in $(words $(APPLDR_LIBGCC_ATTEMPTS)) compiles"; \
	rm -f $@ $@.text; \
	exit 1

$(APPLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(APPLDR_CC) $(APPLDR_CFLAGS) $(APPLDR_LV0LDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(APPLDR_CXX) $(APPLDR_CFLAGS) $(APPLDR_LV0LDR_INCLUDES) $(APPLDR_DEPFLAGS) -c $< -o $@

$(APPLDR_ELF): $(APPLDR_OBJS) $(APPLDR_LDSCRIPT)
	$(APPLDR_LD) -T $(APPLDR_LDSCRIPT) --gc-sections -Map $(APPLDR_MAP) -o $@ $(APPLDR_OBJS)

$(APPLDR_BIN): $(APPLDR_ELF)
	$(APPLDR_OBJCOPY) -O binary $< $@

$(APPLDR_COMMENT):
	@mkdir -p $(@D)
	truncate -s $$(( $(APPLDR_COMMENT_SIZE) )) $@

$(APPLDR_STRIPPED): $(APPLDR_ELF) $(APPLDR_COMMENT)
	$(APPLDR_OBJCOPY) --strip-all --add-section .comment=$(APPLDR_COMMENT) $< $@

$(APPLDR_IMAGE_BIN): $(APPLDR_IMAGE)
	@mkdir -p $(@D)
	$(APPLDR_OBJCOPY) -O binary $< $@

check-appldr: $(APPLDR_BIN) $(APPLDR_IMAGE_BIN) $(APPLDR_ELF) $(APPLDR_STRIPPED)
	@$(APPLDR_READELF) -lW $(APPLDR_IMAGE) | grep LOAD > $(APPLDR_OUT)/segments.image
	@$(APPLDR_READELF) -lW $(APPLDR_ELF) | grep LOAD > $(APPLDR_OUT)/segments.built
	@if cmp -s $(APPLDR_OUT)/segments.image $(APPLDR_OUT)/segments.built; then \
		echo "appldr: program headers identical to $(APPLDR_IMAGE) (3 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "appldr: program headers DIFFER from $(APPLDR_IMAGE) (image, then build):"; \
		cat $(APPLDR_OUT)/segments.image $(APPLDR_OUT)/segments.built; \
		exit 1; \
	fi
	@if cmp -s $(APPLDR_IMAGE_BIN) $(APPLDR_BIN); then \
		echo "appldr: $(APPLDR_LOAD_START)-$(APPLDR_LOAD_END) identical to $(APPLDR_IMAGE)"; \
	else \
		echo "appldr: $(APPLDR_LOAD_START)-$(APPLDR_LOAD_END) DIFFERS from $(APPLDR_IMAGE) (offset from $(APPLDR_LOAD_START) + 1, image byte, built byte):"; \
		cmp -l $(APPLDR_IMAGE_BIN) $(APPLDR_BIN) | head -20; \
		echo "$$(cmp -l $(APPLDR_IMAGE_BIN) $(APPLDR_BIN) 2>/dev/null | wc -l) bytes differ"; \
		exit 1; \
	fi
	@$(APPLDR_NM) $(APPLDR_ELF) | cut -d' ' -f1,3 > $(APPLDR_OUT)/globals.built
	@if grep -Fxv -f $(APPLDR_OUT)/globals.built $(APPLDR_GLOBALS); then \
		echo "appldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "appldr: all $$(wc -l < $(APPLDR_GLOBALS)) globals at the image's addresses (.bss, .rodata, .data)"; \
	fi
	@phoff=$$(od -An -tu4 --endian=big -j28 -N4 $(APPLDR_IMAGE)); \
	shoff=$$(od -An -tu4 --endian=big -j32 -N4 $(APPLDR_IMAGE)); \
	phnum=$$(od -An -tu2 --endian=big -j44 -N2 $(APPLDR_IMAGE)); \
	shnum=$$(od -An -tu2 --endian=big -j48 -N2 $(APPLDR_IMAGE)); \
	for w in image built; do \
		if [ $$w = image ]; then f=$(APPLDR_IMAGE); else f=$(APPLDR_STRIPPED); fi; \
		{ dd if=$$f bs=52 count=1 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((phoff)) count=$$((phnum * 32)) iflag=skip_bytes,count_bytes 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((shoff)) count=$$((shnum * 40)) iflag=skip_bytes,count_bytes 2>/dev/null; } > $(APPLDR_OUT)/headers.$$w || exit 1; \
	done; \
	if cmp -s $(APPLDR_OUT)/headers.image $(APPLDR_OUT)/headers.built; then \
		echo "appldr: $(APPLDR_STRIPPED): ELF header, $$((phnum)) program headers and $$((shnum)) section headers identical to $(APPLDR_IMAGE)"; \
	else \
		echo "appldr: $(APPLDR_STRIPPED): ELF header, program headers or section headers DIFFER from $(APPLDR_IMAGE) (offset in the three + 1, image byte, built byte):"; \
		cmp -l $(APPLDR_OUT)/headers.image $(APPLDR_OUT)/headers.built | head -20; \
		exit 1; \
	fi

clean-appldr:
	rm -rf $(APPLDR_OUT)

-include $(addprefix $(APPLDR_OBJ_DIR)/,$(addsuffix .d,$(basename $(APPLDR_UNITS))))
