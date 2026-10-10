SPP_VERIFIER_OUT := $(BUILD_DIR)/spp_verifier
SPP_VERIFIER_OBJ_DIR := $(SPP_VERIFIER_OUT)/obj
SPP_VERIFIER_ELF := $(SPP_VERIFIER_OUT)/spp_verifier.elf
SPP_VERIFIER_BIN := $(SPP_VERIFIER_OUT)/spp_verifier.bin
SPP_VERIFIER_LINKED := $(SPP_VERIFIER_OUT)/spp_verifier.linked.elf
SPP_VERIFIER_MAP := $(SPP_VERIFIER_OUT)/spp_verifier.map
SPP_VERIFIER_LDSCRIPT := spp_verifier/spp_verifier.ld
SPP_VERIFIER_GLOBALS := spp_verifier/spp_verifier.globals
SPP_VERIFIER_UNMATCHED := spp_verifier/spp_verifier.unmatched

SPP_VERIFIER_IMAGE := spp_verifier/image/spp_verifier.elf
SPP_VERIFIER_IMAGE_BIN := $(SPP_VERIFIER_OUT)/image.bin
SPP_VERIFIER_LOAD_START := 0x80
SPP_VERIFIER_LOAD_END := 0xCED0
SPP_VERIFIER_PATCHES := _Z13verify_headeryj+0x330:33079500:40800003

SPP_VERIFIER_UNITS := \
	spp_verifier/src/crt0.S \
	lv2ldr/src/hash_reader.cpp \
	spp_verifier/src/main.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_blocks.c \
	lv0ldr/crypto/sha1_transform.c \
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
	spp_verifier/src/runtime/atexit.c \
	spp_verifier/src/exit_stop.c \
	lv0ldr/src/util/shuffle_table.c

SPP_VERIFIER_OBJS := $(addprefix $(SPP_VERIFIER_OBJ_DIR)/,$(addsuffix .o,$(basename $(SPP_VERIFIER_UNITS))))

SPP_VERIFIER_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
SPP_VERIFIER_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
SPP_VERIFIER_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SPP_VERIFIER_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SPP_VERIFIER_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SPP_VERIFIER_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SPP_VERIFIER_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

SPP_VERIFIER_CFLAGS := -Os -ffunction-sections -fdata-sections
SPP_VERIFIER_CRYPTO_CFLAGS := -O3
SPP_VERIFIER_DEPFLAGS := -MMD -MP
SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
SPP_VERIFIER_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
SPP_VERIFIER_LV0LDR_INCLUDES := -Ilv0ldr/include
SPP_VERIFIER_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include

$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2
$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/src/exit_stop.o: UNIT_FLAGS := -O2
$(SPP_VERIFIER_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: spp_verifier check-spp_verifier clean-spp_verifier

spp_verifier: $(SPP_VERIFIER_ELF) $(SPP_VERIFIER_BIN)

$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CXX) $(SPP_VERIFIER_CFLAGS) $(SPP_VERIFIER_INCLUDES) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CC) $(SPP_VERIFIER_CFLAGS) $(SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.S
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CXX) $(SPP_VERIFIER_CFLAGS) $(SPP_VERIFIER_INCLUDES) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CXX) $(SPP_VERIFIER_CFLAGS) $(SPP_VERIFIER_LV2LDR_INCLUDES) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CRYPTO_CC) $(SPP_VERIFIER_CRYPTO_CFLAGS) $(SPP_VERIFIER_ISOLDR_INCLUDES) $(UNIT_FLAGS) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CRYPTO_CC) $(SPP_VERIFIER_CRYPTO_CFLAGS) $(SPP_VERIFIER_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(SPP_VERIFIER_CC) $(SPP_VERIFIER_CFLAGS) $(SPP_VERIFIER_LV0LDR_INCLUDES) $(SPP_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPP_VERIFIER_LINKED): $(SPP_VERIFIER_OBJS) $(SPP_VERIFIER_LDSCRIPT)
	$(SPP_VERIFIER_LD) -T $(SPP_VERIFIER_LDSCRIPT) --gc-sections -Map $(SPP_VERIFIER_MAP) -o $@ $(SPP_VERIFIER_OBJS)

$(SPP_VERIFIER_ELF): $(SPP_VERIFIER_LINKED)
	@cp $< $@.tmp; \
	for p in $(SPP_VERIFIER_PATCHES); do \
		at=$${p%%:*}; sym=$${at%+*}; old=$${p#*:}; old=$${old%%:*}; new=$${p##*:}; \
		a=$$(( 0x$$($(SPP_VERIFIER_NM) $< | grep " $$sym$$" | cut -d' ' -f1) + $${at##*+} )); \
		off=$$($(SPP_VERIFIER_READELF) -lW $< | grep LOAD | while read t o v p f rest; do \
			if [ $$a -ge $$((v)) ] && [ $$a -lt $$((v + f)) ]; then echo $$((o + a - v)); fi; \
		done); \
		if [ "$$(od -An -tx1 -j$$off -N4 $@.tmp | tr -d ' \n')" != "$$old" ]; then \
			echo "spp_verifier: $$at is not the compiler's $$old, the word the image's patch replaces"; \
			rm -f $@.tmp; \
			exit 1; \
		fi; \
		printf "$$(printf '\\%03o' $$((0x$$new >> 24 & 255)) $$((0x$$new >> 16 & 255)) $$((0x$$new >> 8 & 255)) $$((0x$$new & 255)))" | \
			dd of=$@.tmp bs=1 seek=$$off conv=notrunc 2>/dev/null || exit 1; \
		echo "spp_verifier: $$at ($$(printf 0x%X $$a)) patched from $$old to $$new, as the image has it"; \
	done; \
	mv $@.tmp $@

$(SPP_VERIFIER_BIN): $(SPP_VERIFIER_ELF)
	$(SPP_VERIFIER_OBJCOPY) -O binary $< $@

$(SPP_VERIFIER_IMAGE_BIN): $(SPP_VERIFIER_IMAGE)
	@mkdir -p $(@D)
	$(SPP_VERIFIER_OBJCOPY) -O binary $< $@

check-spp_verifier: $(SPP_VERIFIER_BIN) $(SPP_VERIFIER_IMAGE_BIN) $(SPP_VERIFIER_ELF)
	@$(SPP_VERIFIER_READELF) -lW $(SPP_VERIFIER_IMAGE) | grep LOAD > $(SPP_VERIFIER_OUT)/segments.image
	@$(SPP_VERIFIER_READELF) -lW $(SPP_VERIFIER_ELF) | grep LOAD > $(SPP_VERIFIER_OUT)/segments.built
	@if cmp -s $(SPP_VERIFIER_OUT)/segments.image $(SPP_VERIFIER_OUT)/segments.built; then \
		echo "spp_verifier: program headers identical to $(SPP_VERIFIER_IMAGE) (3 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "spp_verifier: program headers DIFFER from $(SPP_VERIFIER_IMAGE) (image, then build):"; \
		cat $(SPP_VERIFIER_OUT)/segments.image $(SPP_VERIFIER_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SPP_VERIFIER_IMAGE_BIN) $(SPP_VERIFIER_BIN) > $(SPP_VERIFIER_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SPP_VERIFIER_OUT)/differing.raw | sed 's/^ //' > $(SPP_VERIFIER_OUT)/differing.built; \
	if [ ! -s $(SPP_VERIFIER_OUT)/differing.built ]; then \
		echo "spp_verifier: $(SPP_VERIFIER_LOAD_START)-$(SPP_VERIFIER_LOAD_END) identical to $(SPP_VERIFIER_IMAGE)"; \
	elif cmp -s $(SPP_VERIFIER_OUT)/differing.built $(SPP_VERIFIER_UNMATCHED); then \
		echo "spp_verifier: $(SPP_VERIFIER_LOAD_START)-$(SPP_VERIFIER_LOAD_END) identical to $(SPP_VERIFIER_IMAGE) but for the $$(wc -l < $(SPP_VERIFIER_UNMATCHED)) bytes of the two functions that do not match yet (main, memcpy) ($(SPP_VERIFIER_UNMATCHED))"; \
	else \
		echo "spp_verifier: $(SPP_VERIFIER_LOAD_START)-$(SPP_VERIFIER_LOAD_END) DIFFERS from $(SPP_VERIFIER_IMAGE) (offset from $(SPP_VERIFIER_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SPP_VERIFIER_UNMATCHED) $(SPP_VERIFIER_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SPP_VERIFIER_OUT)/differing.built) bytes differ, $$(wc -l < $(SPP_VERIFIER_UNMATCHED)) of them expected ($(SPP_VERIFIER_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SPP_VERIFIER_NM) $(SPP_VERIFIER_ELF) | cut -d' ' -f1,3 > $(SPP_VERIFIER_OUT)/globals.built
	@if grep -Fxv -f $(SPP_VERIFIER_OUT)/globals.built $(SPP_VERIFIER_GLOBALS); then \
		echo "spp_verifier: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "spp_verifier: all $$(wc -l < $(SPP_VERIFIER_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-spp_verifier:
	rm -rf $(SPP_VERIFIER_OUT)

-include $(SPP_VERIFIER_OBJS:.o=.d)
