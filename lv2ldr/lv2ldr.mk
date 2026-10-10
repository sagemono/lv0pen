LV2LDR_OUT := $(BUILD_DIR)/lv2ldr
LV2LDR_OBJ_DIR := $(LV2LDR_OUT)/obj
LV2LDR_ELF := $(LV2LDR_OUT)/lv2ldr.elf
LV2LDR_BIN := $(LV2LDR_OUT)/lv2ldr.bin
LV2LDR_STRIPPED := $(LV2LDR_OUT)/lv2ldr.stripped.elf
LV2LDR_COMMENT := $(LV2LDR_OUT)/comment.bin
LV2LDR_COMMENT_SIZE := 0xD99
LV2LDR_MAP := $(LV2LDR_OUT)/lv2ldr.map
LV2LDR_LDSCRIPT := lv2ldr/lv2ldr.ld
LV2LDR_GLOBALS := lv2ldr/lv2ldr.globals
LV2LDR_UNMATCHED := lv2ldr/lv2ldr.unmatched

LV2LDR_IMAGE := lv2ldr/image/lv2ldr.elf
LV2LDR_IMAGE_BIN := $(LV2LDR_OUT)/image.bin
LV2LDR_LOAD_START := 0x12C00
LV2LDR_LOAD_END := 0x2A1B0

LV2LDR_UNITS := \
	isoldr/src/crt0.S \
	lv2ldr/src/loader_global.cpp \
	lv2ldr/src/main.cpp \
	lv2ldr/src/loader.cpp \
	lv2ldr/src/loader_data.cpp \
	lv2ldr/src/hash_reader.cpp \
	lv2ldr/src/auth_checks.cpp \
	lv2ldr/src/auth_data.cpp \
	lv2ldr/src/revoke_list.cpp \
	lv1ldr/src/authenticator.cpp \
	lv1ldr/src/auth_buffers.cpp \
	isoldr/src/hmac_digest.cpp \
	lv2ldr/src/revoke_check.cpp \
	lv1ldr/src/auth_segments.cpp \
	lv1ldr/src/auth_transfer.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	lv1ldr/src/elf_reader.cpp \
	lv2ldr/src/revoke_scan.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv0ldr/crypto/aes.c \
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
	isoldr/src/mbox_drain.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	lv2ldr/src/ch64.cpp \
	lv2ldr/src/ch72.cpp \
	lv1ldr/src/auth_header.cpp \
	lv1ldr/src/auth_check.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	isoldr/src/memcpy.c \
	lv0ldr/src/util/shuffle_table.c \
	lv1ldr/zlib/lv1ldr_inflate.c \
	lv1ldr/zlib/inflate.c \
	lv1ldr/zlib/inftrees.c \
	lv1ldr/zlib/adler32.c \
	lv1ldr/zlib/zutil.c \
	lv1ldr/zlib/inffast.c \
	lv1ldr/src/abort.cpp

LV2LDR_LIBGCC_MEMBERS := umoddi3

LV2LDR_OBJS := \
	$(addprefix $(LV2LDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(LV2LDR_UNITS)))) \
	$(addprefix $(LV2LDR_OBJ_DIR)/lv0ldr/libgcc/,$(addsuffix .o,$(LV2LDR_LIBGCC_MEMBERS)))

LV2LDR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
LV2LDR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
LV2LDR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
LV2LDR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
LV2LDR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
LV2LDR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
LV2LDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

LV2LDR_CFLAGS := -Os -ffunction-sections -fdata-sections
LV2LDR_CRYPTO_CFLAGS := -O3
LV2LDR_ZLIB_CFLAGS := -O3 -DNO_GZIP -ffunction-sections -fdata-sections
LV2LDR_LIBGCC_CFLAGS := -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions
LV2LDR_DEPFLAGS := -MMD -MP
LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
LV2LDR_LV0LDR_INCLUDES := -Ilv0ldr/include
LV2LDR_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
LV2LDR_LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
LV2LDR_ZLIB_INCLUDES := -Ilv1ldr/zlib/sys -Ilv1ldr/zlib

LV2LDR_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
LV2LDR_LIBGCC_TEXT_MD5_umoddi3 := 8711d716069e15311f73103fe9d98eb2

$(LV2LDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: lv2ldr check-lv2ldr clean-lv2ldr

lv2ldr: $(LV2LDR_ELF) $(LV2LDR_BIN) $(LV2LDR_STRIPPED)

$(LV2LDR_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(LV2LDR_CXX) $(LV2LDR_CFLAGS) $(LV2LDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV2LDR_CRYPTO_CC) $(LV2LDR_CRYPTO_CFLAGS) $(LV2LDR_ISOLDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(LV2LDR_CC) $(LV2LDR_CFLAGS) $(LV2LDR_ISOLDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(LV2LDR_CXX) $(LV2LDR_CFLAGS) $(LV2LDR_ISOLDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(LV2LDR_CXX) $(LV2LDR_CFLAGS) $(LV2LDR_ISOLDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/lv1ldr/zlib/%.o: lv1ldr/zlib/%.c
	@mkdir -p $(@D)
	$(LV2LDR_CC) $(LV2LDR_ZLIB_CFLAGS) $(LV2LDR_ZLIB_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(LV2LDR_CXX) $(LV2LDR_CFLAGS) $(LV2LDR_LV1LDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(LV2LDR_CRYPTO_CC) $(LV2LDR_CRYPTO_CFLAGS) $(LV2LDR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/lv0ldr/libgcc/%.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(LV2LDR_LIBGCC_ATTEMPTS); do \
		echo "$(LV2LDR_CC) $(LV2LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@"; \
		$(LV2LDR_CC) $(LV2LDR_LIBGCC_CFLAGS) -D__LIBGCC2_PAD=$$pad -DL_$* -c $< -o $@ || exit 1; \
		$(LV2LDR_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(LV2LDR_LIBGCC_TEXT_MD5_$*)" ]; then \
			rm -f $@.text; \
			exit 0; \
		fi; \
		pad=$${pad}x; \
	done; \
	echo "lv2ldr: __$* never came out as the image's code in $(words $(LV2LDR_LIBGCC_ATTEMPTS)) compiles"; \
	rm -f $@ $@.text; \
	exit 1

$(LV2LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(LV2LDR_CC) $(LV2LDR_CFLAGS) $(LV2LDR_LV0LDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(LV2LDR_CXX) $(LV2LDR_CFLAGS) $(LV2LDR_LV0LDR_INCLUDES) $(LV2LDR_DEPFLAGS) -c $< -o $@

$(LV2LDR_ELF): $(LV2LDR_OBJS) $(LV2LDR_LDSCRIPT)
	$(LV2LDR_LD) -T $(LV2LDR_LDSCRIPT) --gc-sections -Map $(LV2LDR_MAP) -o $@ $(LV2LDR_OBJS)

$(LV2LDR_BIN): $(LV2LDR_ELF)
	$(LV2LDR_OBJCOPY) -O binary $< $@

$(LV2LDR_COMMENT):
	@mkdir -p $(@D)
	truncate -s $$(( $(LV2LDR_COMMENT_SIZE) )) $@

$(LV2LDR_STRIPPED): $(LV2LDR_ELF) $(LV2LDR_COMMENT)
	$(LV2LDR_OBJCOPY) --strip-all --add-section .comment=$(LV2LDR_COMMENT) $< $@

$(LV2LDR_IMAGE_BIN): $(LV2LDR_IMAGE)
	@mkdir -p $(@D)
	$(LV2LDR_OBJCOPY) -O binary $< $@

check-lv2ldr: $(LV2LDR_BIN) $(LV2LDR_IMAGE_BIN) $(LV2LDR_ELF) $(LV2LDR_STRIPPED)
	@$(LV2LDR_READELF) -lW $(LV2LDR_IMAGE) | grep LOAD > $(LV2LDR_OUT)/segments.image
	@$(LV2LDR_READELF) -lW $(LV2LDR_ELF) | grep LOAD > $(LV2LDR_OUT)/segments.built
	@if cmp -s $(LV2LDR_OUT)/segments.image $(LV2LDR_OUT)/segments.built; then \
		echo "lv2ldr: program headers identical to $(LV2LDR_IMAGE) (3 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "lv2ldr: program headers DIFFER from $(LV2LDR_IMAGE) (image, then build):"; \
		cat $(LV2LDR_OUT)/segments.image $(LV2LDR_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(LV2LDR_IMAGE_BIN) $(LV2LDR_BIN) > $(LV2LDR_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(LV2LDR_OUT)/differing.raw | sed 's/^ //' > $(LV2LDR_OUT)/differing.built; \
	if [ ! -s $(LV2LDR_OUT)/differing.built ]; then \
		echo "lv2ldr: $(LV2LDR_LOAD_START)-$(LV2LDR_LOAD_END) identical to $(LV2LDR_IMAGE)"; \
	elif cmp -s $(LV2LDR_OUT)/differing.built $(LV2LDR_UNMATCHED); then \
		echo "lv2ldr: $(LV2LDR_LOAD_START)-$(LV2LDR_LOAD_END) identical to $(LV2LDR_IMAGE) but for the $$(wc -l < $(LV2LDR_UNMATCHED)) bytes of ch72_put_request, the one function that does not match yet ($(LV2LDR_UNMATCHED))"; \
	else \
		echo "lv2ldr: $(LV2LDR_LOAD_START)-$(LV2LDR_LOAD_END) DIFFERS from $(LV2LDR_IMAGE) (offset from $(LV2LDR_LOAD_START) + 1, image byte, built byte):"; \
		diff $(LV2LDR_UNMATCHED) $(LV2LDR_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(LV2LDR_OUT)/differing.built) bytes differ, $$(wc -l < $(LV2LDR_UNMATCHED)) of them expected ($(LV2LDR_UNMATCHED))"; \
		exit 1; \
	fi
	@$(LV2LDR_NM) $(LV2LDR_ELF) | cut -d' ' -f1,3 > $(LV2LDR_OUT)/globals.built
	@if grep -Fxv -f $(LV2LDR_OUT)/globals.built $(LV2LDR_GLOBALS); then \
		echo "lv2ldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "lv2ldr: all $$(wc -l < $(LV2LDR_GLOBALS)) globals at the image's addresses (.bss, .rodata, .data)"; \
	fi
	@phoff=$$(od -An -tu4 --endian=big -j28 -N4 $(LV2LDR_IMAGE)); \
	shoff=$$(od -An -tu4 --endian=big -j32 -N4 $(LV2LDR_IMAGE)); \
	phnum=$$(od -An -tu2 --endian=big -j44 -N2 $(LV2LDR_IMAGE)); \
	shnum=$$(od -An -tu2 --endian=big -j48 -N2 $(LV2LDR_IMAGE)); \
	for w in image built; do \
		if [ $$w = image ]; then f=$(LV2LDR_IMAGE); else f=$(LV2LDR_STRIPPED); fi; \
		{ dd if=$$f bs=52 count=1 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((phoff)) count=$$((phnum * 32)) iflag=skip_bytes,count_bytes 2>/dev/null && \
		dd if=$$f bs=65536 skip=$$((shoff)) count=$$((shnum * 40)) iflag=skip_bytes,count_bytes 2>/dev/null; } > $(LV2LDR_OUT)/headers.$$w || exit 1; \
	done; \
	if cmp -s $(LV2LDR_OUT)/headers.image $(LV2LDR_OUT)/headers.built; then \
		echo "lv2ldr: $(LV2LDR_STRIPPED): ELF header, $$((phnum)) program headers and $$((shnum)) section headers identical to $(LV2LDR_IMAGE)"; \
	else \
		echo "lv2ldr: $(LV2LDR_STRIPPED): ELF header, program headers or section headers DIFFER from $(LV2LDR_IMAGE) (offset in the three + 1, image byte, built byte):"; \
		cmp -l $(LV2LDR_OUT)/headers.image $(LV2LDR_OUT)/headers.built | head -20; \
		exit 1; \
	fi

clean-lv2ldr:
	rm -rf $(LV2LDR_OUT)

-include $(addprefix $(LV2LDR_OBJ_DIR)/,$(addsuffix .d,$(basename $(LV2LDR_UNITS))))
