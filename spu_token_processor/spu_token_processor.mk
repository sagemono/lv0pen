SPU_TOKEN_PROCESSOR_OUT := $(BUILD_DIR)/spu_token_processor
SPU_TOKEN_PROCESSOR_OBJ_DIR := $(SPU_TOKEN_PROCESSOR_OUT)/obj
SPU_TOKEN_PROCESSOR_ELF := $(SPU_TOKEN_PROCESSOR_OUT)/spu_token_processor.elf
SPU_TOKEN_PROCESSOR_BIN := $(SPU_TOKEN_PROCESSOR_OUT)/spu_token_processor.bin
SPU_TOKEN_PROCESSOR_LINKED := $(SPU_TOKEN_PROCESSOR_OUT)/spu_token_processor.linked.elf
SPU_TOKEN_PROCESSOR_MAP := $(SPU_TOKEN_PROCESSOR_OUT)/spu_token_processor.map
SPU_TOKEN_PROCESSOR_LDSCRIPT := spu_token_processor/spu_token_processor.ld
SPU_TOKEN_PROCESSOR_GLOBALS := spu_token_processor/spu_token_processor.globals
SPU_TOKEN_PROCESSOR_UNMATCHED := spu_token_processor/spu_token_processor.unmatched

SPU_TOKEN_PROCESSOR_IMAGE := spu_token_processor/image/spu_token_processor.elf
SPU_TOKEN_PROCESSOR_IMAGE_BIN := $(SPU_TOKEN_PROCESSOR_OUT)/image.bin
SPU_TOKEN_PROCESSOR_LOAD_START := 0x880
SPU_TOKEN_PROCESSOR_LOAD_END := 0xC1C0
SPU_TOKEN_PROCESSOR_PATCHES := _ZN15token_processor16verify_signatureEPKh+0x0:1203420b:40800003 _ZN15token_processor16verify_signatureEPKh+0x4:24ffc0d0:35000000

SPU_TOKEN_PROCESSOR_UNITS := \
	spu_token_processor/src/crt0.S \
	lv2ldr/src/hash_reader.cpp \
	spu_token_processor/src/token.cpp \
	spu_token_processor/src/main.cpp \
	spu_token_processor/src/eid0_reader.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_cbc_decrypt.c \
	isoldr/crypto/aes_cmac.c \
	lv0ldr/crypto/sha1.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
	lv0ldr/crypto/sha1_transform.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/ecdsa_id.c \
	lv0ldr/crypto/ec_curves.c \
	lv0ldr/crypto/ec_curve_table.c \
	isoldr/crypto/ecdsa.c \
	lv0ldr/crypto/ec.c \
	lv0ldr/crypto/ec_mul2.c \
	lv0ldr/crypto/bn_core.c \
	lv0ldr/crypto/bn_in_range.c \
	lv0ldr/crypto/ec_curve_valid.c \
	lv0ldr/crypto/bn_from_bytes_not.c \
	spp_verifier/src/runtime/dma_channel.cpp \
	spp_verifier/src/runtime/delete.cpp \
	spp_verifier/src/runtime/exit.c \
	spp_verifier/src/runtime/free.c \
	spp_verifier/src/runtime/heap.c \
	spp_verifier/src/runtime/memcpy.c \
	spp_verifier/src/runtime/memset.c \
	spp_verifier/src/runtime/atexit.c \
	lv0ldr/src/util/shuffle_table.c

SPU_TOKEN_PROCESSOR_OBJS := $(addprefix $(SPU_TOKEN_PROCESSOR_OBJ_DIR)/,$(addsuffix .o,$(basename $(SPU_TOKEN_PROCESSOR_UNITS))))

SPU_TOKEN_PROCESSOR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
SPU_TOKEN_PROCESSOR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
SPU_TOKEN_PROCESSOR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SPU_TOKEN_PROCESSOR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SPU_TOKEN_PROCESSOR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SPU_TOKEN_PROCESSOR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SPU_TOKEN_PROCESSOR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

SPU_TOKEN_PROCESSOR_CFLAGS := -Os -ffunction-sections -fdata-sections
SPU_TOKEN_PROCESSOR_CRYPTO_CFLAGS := -O3
SPU_TOKEN_PROCESSOR_DEPFLAGS := -MMD -MP
SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include
SPU_TOKEN_PROCESSOR_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
SPU_TOKEN_PROCESSOR_LV0LDR_INCLUDES := -Ilv0ldr/include
SPU_TOKEN_PROCESSOR_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
SPU_TOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2

.PHONY: spu_token_processor check-spu_token_processor clean-spu_token_processor

spu_token_processor: $(SPU_TOKEN_PROCESSOR_ELF) $(SPU_TOKEN_PROCESSOR_BIN)

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.cpp
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CXX) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_INCLUDES) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CXX) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_INCLUDES) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CXX) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CC) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CXX) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_LV2LDR_INCLUDES) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CRYPTO_CC) $(SPU_TOKEN_PROCESSOR_CRYPTO_CFLAGS) $(SPU_TOKEN_PROCESSOR_ISOLDR_INCLUDES) $(UNIT_FLAGS) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CRYPTO_CC) $(SPU_TOKEN_PROCESSOR_CRYPTO_CFLAGS) $(SPU_TOKEN_PROCESSOR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_CC) $(SPU_TOKEN_PROCESSOR_CFLAGS) $(SPU_TOKEN_PROCESSOR_LV0LDR_INCLUDES) $(SPU_TOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_TOKEN_PROCESSOR_LINKED): $(SPU_TOKEN_PROCESSOR_OBJS) $(SPU_TOKEN_PROCESSOR_LDSCRIPT)
	$(SPU_TOKEN_PROCESSOR_LD) -T $(SPU_TOKEN_PROCESSOR_LDSCRIPT) --gc-sections -Map $(SPU_TOKEN_PROCESSOR_MAP) -o $@ $(SPU_TOKEN_PROCESSOR_OBJS)

$(SPU_TOKEN_PROCESSOR_ELF): $(SPU_TOKEN_PROCESSOR_LINKED)
	@cp $< $@.tmp; \
	for p in $(SPU_TOKEN_PROCESSOR_PATCHES); do \
		at=$${p%%:*}; sym=$${at%+*}; old=$${p#*:}; old=$${old%%:*}; new=$${p##*:}; \
		a=$$(( 0x$$($(SPU_TOKEN_PROCESSOR_NM) $< | grep " $$sym$$" | cut -d' ' -f1) + $${at##*+} )); \
		off=$$($(SPU_TOKEN_PROCESSOR_READELF) -lW $< | grep LOAD | while read t o v p f rest; do \
			if [ $$a -ge $$((v)) ] && [ $$a -lt $$((v + f)) ]; then echo $$((o + a - v)); fi; \
		done); \
		if [ "$$(od -An -tx1 -j$$off -N4 $@.tmp | tr -d ' \n')" != "$$old" ]; then \
			echo "spu_token_processor: $$at is not the compiler's $$old, the word the image's patch replaces"; \
			rm -f $@.tmp; \
			exit 1; \
		fi; \
		printf "$$(printf '\\%03o' $$((0x$$new >> 24 & 255)) $$((0x$$new >> 16 & 255)) $$((0x$$new >> 8 & 255)) $$((0x$$new & 255)))" | \
			dd of=$@.tmp bs=1 seek=$$off conv=notrunc 2>/dev/null || exit 1; \
		echo "spu_token_processor: $$at ($$(printf 0x%X $$a)) patched from $$old to $$new, as the image has it"; \
	done; \
	mv $@.tmp $@

$(SPU_TOKEN_PROCESSOR_BIN): $(SPU_TOKEN_PROCESSOR_ELF)
	$(SPU_TOKEN_PROCESSOR_OBJCOPY) -O binary $< $@

$(SPU_TOKEN_PROCESSOR_IMAGE_BIN): $(SPU_TOKEN_PROCESSOR_IMAGE)
	@mkdir -p $(@D)
	$(SPU_TOKEN_PROCESSOR_OBJCOPY) -O binary $< $@

check-spu_token_processor: $(SPU_TOKEN_PROCESSOR_BIN) $(SPU_TOKEN_PROCESSOR_IMAGE_BIN) $(SPU_TOKEN_PROCESSOR_ELF)
	@$(SPU_TOKEN_PROCESSOR_READELF) -lW $(SPU_TOKEN_PROCESSOR_IMAGE) | grep LOAD > $(SPU_TOKEN_PROCESSOR_OUT)/segments.image
	@$(SPU_TOKEN_PROCESSOR_READELF) -lW $(SPU_TOKEN_PROCESSOR_ELF) | grep LOAD > $(SPU_TOKEN_PROCESSOR_OUT)/segments.built
	@if cmp -s $(SPU_TOKEN_PROCESSOR_OUT)/segments.image $(SPU_TOKEN_PROCESSOR_OUT)/segments.built; then \
		echo "spu_token_processor: program headers identical to $(SPU_TOKEN_PROCESSOR_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "spu_token_processor: program headers DIFFER from $(SPU_TOKEN_PROCESSOR_IMAGE) (image, then build):"; \
		cat $(SPU_TOKEN_PROCESSOR_OUT)/segments.image $(SPU_TOKEN_PROCESSOR_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SPU_TOKEN_PROCESSOR_IMAGE_BIN) $(SPU_TOKEN_PROCESSOR_BIN) > $(SPU_TOKEN_PROCESSOR_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SPU_TOKEN_PROCESSOR_OUT)/differing.raw | sed 's/^ //' > $(SPU_TOKEN_PROCESSOR_OUT)/differing.built; \
	if [ ! -s $(SPU_TOKEN_PROCESSOR_OUT)/differing.built ]; then \
		echo "spu_token_processor: $(SPU_TOKEN_PROCESSOR_LOAD_START)-$(SPU_TOKEN_PROCESSOR_LOAD_END) identical to $(SPU_TOKEN_PROCESSOR_IMAGE)"; \
	elif cmp -s $(SPU_TOKEN_PROCESSOR_OUT)/differing.built $(SPU_TOKEN_PROCESSOR_UNMATCHED); then \
		echo "spu_token_processor: $(SPU_TOKEN_PROCESSOR_LOAD_START)-$(SPU_TOKEN_PROCESSOR_LOAD_END) identical to $(SPU_TOKEN_PROCESSOR_IMAGE) but for the $$(wc -l < $(SPU_TOKEN_PROCESSOR_UNMATCHED)) bytes of the two functions that do not match yet (eid0_reader::read, memcpy) ($(SPU_TOKEN_PROCESSOR_UNMATCHED))"; \
	else \
		echo "spu_token_processor: $(SPU_TOKEN_PROCESSOR_LOAD_START)-$(SPU_TOKEN_PROCESSOR_LOAD_END) DIFFERS from $(SPU_TOKEN_PROCESSOR_IMAGE) (offset from $(SPU_TOKEN_PROCESSOR_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SPU_TOKEN_PROCESSOR_UNMATCHED) $(SPU_TOKEN_PROCESSOR_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SPU_TOKEN_PROCESSOR_OUT)/differing.built) bytes differ, $$(wc -l < $(SPU_TOKEN_PROCESSOR_UNMATCHED)) of them expected ($(SPU_TOKEN_PROCESSOR_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SPU_TOKEN_PROCESSOR_NM) $(SPU_TOKEN_PROCESSOR_ELF) | cut -d' ' -f1,3 > $(SPU_TOKEN_PROCESSOR_OUT)/globals.built
	@if grep -Fxv -f $(SPU_TOKEN_PROCESSOR_OUT)/globals.built $(SPU_TOKEN_PROCESSOR_GLOBALS); then \
		echo "spu_token_processor: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "spu_token_processor: all $$(wc -l < $(SPU_TOKEN_PROCESSOR_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-spu_token_processor:
	rm -rf $(SPU_TOKEN_PROCESSOR_OUT)

-include $(SPU_TOKEN_PROCESSOR_OBJS:.o=.d)
