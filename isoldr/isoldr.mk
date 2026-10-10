ISOLDR_OUT := $(BUILD_DIR)/isoldr
ISOLDR_OBJ_DIR := $(ISOLDR_OUT)/obj
ISOLDR_ELF := $(ISOLDR_OUT)/isoldr.elf
ISOLDR_BIN := $(ISOLDR_OUT)/isoldr.bin
ISOLDR_STRIPPED := $(ISOLDR_OUT)/isoldr.stripped.elf
ISOLDR_COMMENT := $(ISOLDR_OUT)/comment.bin
ISOLDR_COMMENT_SIZE := 0xD24
ISOLDR_MAP := $(ISOLDR_OUT)/isoldr.map
ISOLDR_LDSCRIPT := isoldr/isoldr.ld
ISOLDR_GLOBALS := isoldr/isoldr.globals

ISOLDR_IMAGE := isoldr/image/isoldr.elf
ISOLDR_IMAGE_BIN := $(ISOLDR_OUT)/image.bin
ISOLDR_LOAD_START := 0x25800
ISOLDR_LOAD_END := 0x38B30

ISOLDR_UNITS := \
	isoldr/src/exit_stop.c \
	isoldr/src/scrub.S \
	isoldr/src/crt0.S \
	isoldr/src/g_loader.cpp \
	isoldr/src/main.cpp \
	isoldr/src/loader.cpp \
	isoldr/src/loader_data.cpp \
	isoldr/src/encryptor_aes256_cbc.cpp \
	isoldr/src/local_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/qa_flag.cpp \
	isoldr/src/qa_flag_data.cpp \
	isoldr/src/hmac_digest.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	lv2ldr/src/hash_reader.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	lv1ldr/src/authenticator.cpp \
	lv2ldr/src/revoke_list.cpp \
	lv2ldr/src/auth_checks.cpp \
	isoldr/src/auth_data.cpp \
	isoldr/src/auth_state_data.cpp \
	lv1ldr/src/loader_params.cpp \
	lv1ldr/src/auth_buffers.cpp \
	isoldr/src/elf_reader32.cpp \
	lv2ldr/src/revoke_check.cpp \
	isoldr/src/auth_segments.cpp \
	lv1ldr/src/auth_transfer.cpp \
	lv2ldr/src/revoke_scan.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_modes.c \
	isoldr/crypto/aes_cmac.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_blocks.c \
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
	isoldr/src/mbox_drain.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	isoldr/src/ch64.cpp \
	lv2ldr/src/ch72.cpp \
	lv1ldr/src/auth_header.cpp \
	lv1ldr/src/auth_check.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	isoldr/src/memcpy.c \
	lv1ldr/src/abort.cpp \
	lv1ldr/src/elf_reader.cpp \
	lv0ldr/src/dma/dma.cpp \
	lv0ldr/src/io/get_sb_device.cpp \
	lv0ldr/src/log/log.cpp \
	lv0ldr/src/util/mmio_access.cpp \
	lv0ldr/src/log/printf.cpp \
	lv0ldr/src/io/sb_product.cpp \
	lv0ldr/src/console/uart.cpp \
	lv0ldr/src/util/shuffle_table.c

ISOLDR_OBJS := $(addprefix $(ISOLDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(ISOLDR_UNITS))))

ISOLDR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
ISOLDR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
ISOLDR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
ISOLDR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
ISOLDR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
ISOLDR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
ISOLDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

ISOLDR_CFLAGS := -Os -ffunction-sections -fdata-sections
ISOLDR_CRYPTO_CFLAGS := -O3
ISOLDR_DEPFLAGS := -MMD -MP
ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
ISOLDR_LV0LDR_INCLUDES := -Ilv0ldr/include
ISOLDR_LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
ISOLDR_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include

$(ISOLDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: isoldr check-isoldr clean-isoldr

isoldr: $(ISOLDR_ELF) $(ISOLDR_BIN) $(ISOLDR_STRIPPED)

$(ISOLDR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(ISOLDR_CRYPTO_CC) $(ISOLDR_CRYPTO_CFLAGS) $(ISOLDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(ISOLDR_CC) $(ISOLDR_CFLAGS) $(ISOLDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(ISOLDR_CXX) $(ISOLDR_CFLAGS) $(ISOLDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(ISOLDR_CXX) $(ISOLDR_CFLAGS) $(ISOLDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(ISOLDR_CRYPTO_CC) $(ISOLDR_CRYPTO_CFLAGS) $(ISOLDR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(ISOLDR_CC) $(ISOLDR_CFLAGS) $(ISOLDR_LV0LDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(ISOLDR_CXX) $(ISOLDR_CFLAGS) $(ISOLDR_LV0LDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(ISOLDR_CXX) $(ISOLDR_CFLAGS) $(ISOLDR_LV1LDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(ISOLDR_CXX) $(ISOLDR_CFLAGS) $(ISOLDR_LV2LDR_INCLUDES) $(ISOLDR_DEPFLAGS) -c $< -o $@

$(ISOLDR_ELF): $(ISOLDR_OBJS) $(ISOLDR_LDSCRIPT)
	$(ISOLDR_LD) -T $(ISOLDR_LDSCRIPT) --gc-sections -Map $(ISOLDR_MAP) -o $@ $(ISOLDR_OBJS)

$(ISOLDR_BIN): $(ISOLDR_ELF)
	$(ISOLDR_OBJCOPY) -O binary $< $@

$(ISOLDR_COMMENT):
	@mkdir -p $(@D)
	truncate -s $$(( $(ISOLDR_COMMENT_SIZE) )) $@

$(ISOLDR_STRIPPED): $(ISOLDR_ELF) $(ISOLDR_COMMENT)
	$(ISOLDR_OBJCOPY) --strip-all --add-section .comment=$(ISOLDR_COMMENT) $< $@

$(ISOLDR_IMAGE_BIN): $(ISOLDR_IMAGE)
	@mkdir -p $(@D)
	$(ISOLDR_OBJCOPY) -O binary $< $@

check-isoldr: $(ISOLDR_BIN) $(ISOLDR_IMAGE_BIN) $(ISOLDR_ELF) $(ISOLDR_STRIPPED)
	@$(ISOLDR_READELF) -lW $(ISOLDR_IMAGE) | grep LOAD > $(ISOLDR_OUT)/segments.image
	@$(ISOLDR_READELF) -lW $(ISOLDR_ELF) | grep LOAD > $(ISOLDR_OUT)/segments.built
	@if cmp -s $(ISOLDR_OUT)/segments.image $(ISOLDR_OUT)/segments.built; then \
		echo "isoldr: program headers identical to $(ISOLDR_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "isoldr: program headers DIFFER from $(ISOLDR_IMAGE) (image, then build):"; \
		cat $(ISOLDR_OUT)/segments.image $(ISOLDR_OUT)/segments.built; \
		exit 1; \
	fi
	@if cmp -s $(ISOLDR_IMAGE_BIN) $(ISOLDR_BIN); then \
		echo "isoldr: $(ISOLDR_LOAD_START)-$(ISOLDR_LOAD_END) identical to $(ISOLDR_IMAGE)"; \
	else \
		echo "isoldr: $(ISOLDR_LOAD_START)-$(ISOLDR_LOAD_END) DIFFERS from $(ISOLDR_IMAGE) (offset from $(ISOLDR_LOAD_START) + 1, image byte, built byte):"; \
		cmp -l $(ISOLDR_IMAGE_BIN) $(ISOLDR_BIN) | head -20; \
		echo "$$(cmp -l $(ISOLDR_IMAGE_BIN) $(ISOLDR_BIN) 2>/dev/null | wc -l) bytes differ"; \
		exit 1; \
	fi
	@$(ISOLDR_NM) $(ISOLDR_ELF) | cut -d' ' -f1,3 > $(ISOLDR_OUT)/globals.built
	@if grep -Fxv -f $(ISOLDR_OUT)/globals.built $(ISOLDR_GLOBALS); then \
		echo "isoldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "isoldr: all $$(wc -l < $(ISOLDR_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi
	@phoff=$$(od -An -tu4 --endian=big -j28 -N4 $(ISOLDR_IMAGE)); \
	shoff=$$(od -An -tu4 --endian=big -j32 -N4 $(ISOLDR_IMAGE)); \
	phnum=$$(od -An -tu2 --endian=big -j44 -N2 $(ISOLDR_IMAGE)); \
	shnum=$$(od -An -tu2 --endian=big -j48 -N2 $(ISOLDR_IMAGE)); \
	for w in image built; do \
		if [ $$w = image ]; then f=$(ISOLDR_IMAGE); else f=$(ISOLDR_STRIPPED); fi; \
		{ dd if=$$f bs=52 count=1 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((phoff)) count=$$((phnum * 32)) iflag=skip_bytes,count_bytes 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((shoff)) count=$$((shnum * 40)) iflag=skip_bytes,count_bytes 2>/dev/null; } > $(ISOLDR_OUT)/headers.$$w || exit 1; \
	done; \
	if cmp -s $(ISOLDR_OUT)/headers.image $(ISOLDR_OUT)/headers.built; then \
		echo "isoldr: $(ISOLDR_STRIPPED): ELF header, $$((phnum)) program headers and $$((shnum)) section headers identical to $(ISOLDR_IMAGE)"; \
	else \
		echo "isoldr: $(ISOLDR_STRIPPED): ELF header, program headers or section headers DIFFER from $(ISOLDR_IMAGE) (offset in the three + 1, image byte, built byte):"; \
		cmp -l $(ISOLDR_OUT)/headers.image $(ISOLDR_OUT)/headers.built | head -20; \
		exit 1; \
	fi

clean-isoldr:
	rm -rf $(ISOLDR_OUT)

-include $(ISOLDR_OBJS:.o=.d)
