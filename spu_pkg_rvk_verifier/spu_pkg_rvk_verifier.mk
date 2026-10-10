SPU_PKG_RVK_VERIFIER_OUT := $(BUILD_DIR)/spu_pkg_rvk_verifier
SPU_PKG_RVK_VERIFIER_OBJ_DIR := $(SPU_PKG_RVK_VERIFIER_OUT)/obj
SPU_PKG_RVK_VERIFIER_ELF := $(SPU_PKG_RVK_VERIFIER_OUT)/spu_pkg_rvk_verifier.elf
SPU_PKG_RVK_VERIFIER_BIN := $(SPU_PKG_RVK_VERIFIER_OUT)/spu_pkg_rvk_verifier.bin
SPU_PKG_RVK_VERIFIER_LINKED := $(SPU_PKG_RVK_VERIFIER_OUT)/spu_pkg_rvk_verifier.linked.elf
SPU_PKG_RVK_VERIFIER_MAP := $(SPU_PKG_RVK_VERIFIER_OUT)/spu_pkg_rvk_verifier.map
SPU_PKG_RVK_VERIFIER_LDSCRIPT := spu_pkg_rvk_verifier/spu_pkg_rvk_verifier.ld
SPU_PKG_RVK_VERIFIER_GLOBALS := spu_pkg_rvk_verifier/spu_pkg_rvk_verifier.globals
SPU_PKG_RVK_VERIFIER_UNMATCHED := spu_pkg_rvk_verifier/spu_pkg_rvk_verifier.unmatched

SPU_PKG_RVK_VERIFIER_IMAGE := spu_pkg_rvk_verifier/image/spu_pkg_rvk_verifier.elf
SPU_PKG_RVK_VERIFIER_IMAGE_BIN := $(SPU_PKG_RVK_VERIFIER_OUT)/image.bin
SPU_PKG_RVK_VERIFIER_LOAD_START := 0x880
SPU_PKG_RVK_VERIFIER_LOAD_END := 0xF360
SPU_PKG_RVK_VERIFIER_PATCHES := _ZN12sce_verifier16decrypt_metadataEPKhjS1_Ph+0xd4:337fd080:40800003

SPU_PKG_RVK_VERIFIER_UNITS := \
	spu_token_processor/src/crt0.S \
	spu_pkg_rvk_verifier/src/main.cpp \
	spu_pkg_rvk_verifier/src/pkg_verifier.cpp \
	spu_pkg_rvk_verifier/src/rvk_verifier.cpp \
	spu_pkg_rvk_verifier/src/sce_verifier.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_blocks.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_transform.c \
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

SPU_PKG_RVK_VERIFIER_OBJS := $(addprefix $(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/,$(addsuffix .o,$(basename $(SPU_PKG_RVK_VERIFIER_UNITS))))

SPU_PKG_RVK_VERIFIER_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
SPU_PKG_RVK_VERIFIER_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
SPU_PKG_RVK_VERIFIER_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SPU_PKG_RVK_VERIFIER_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SPU_PKG_RVK_VERIFIER_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SPU_PKG_RVK_VERIFIER_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SPU_PKG_RVK_VERIFIER_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

SPU_PKG_RVK_VERIFIER_CFLAGS := -Os -ffunction-sections -fdata-sections
SPU_PKG_RVK_VERIFIER_CRYPTO_CFLAGS := -O3
SPU_PKG_RVK_VERIFIER_DEPFLAGS := -MMD -MP
SPU_PKG_RVK_VERIFIER_INCLUDES := -Ispu_pkg_rvk_verifier/include -Ispp_verifier/include -Ilv0ldr/include
SPU_PKG_RVK_VERIFIER_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
SPU_PKG_RVK_VERIFIER_LV0LDR_INCLUDES := -Ilv0ldr/include
SPU_PKG_RVK_VERIFIER_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
SPU_PKG_RVK_VERIFIER_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2
$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: spu_pkg_rvk_verifier check-spu_pkg_rvk_verifier clean-spu_pkg_rvk_verifier

spu_pkg_rvk_verifier: $(SPU_PKG_RVK_VERIFIER_ELF) $(SPU_PKG_RVK_VERIFIER_BIN)

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spu_pkg_rvk_verifier/%.o: spu_pkg_rvk_verifier/%.cpp
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CXX) $(SPU_PKG_RVK_VERIFIER_CFLAGS) $(SPU_PKG_RVK_VERIFIER_INCLUDES) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CXX) $(SPU_PKG_RVK_VERIFIER_CFLAGS) $(SPU_PKG_RVK_VERIFIER_SPU_TOKEN_PROCESSOR_INCLUDES) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CXX) $(SPU_PKG_RVK_VERIFIER_CFLAGS) $(SPU_PKG_RVK_VERIFIER_SPP_VERIFIER_INCLUDES) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CC) $(SPU_PKG_RVK_VERIFIER_CFLAGS) $(SPU_PKG_RVK_VERIFIER_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CRYPTO_CC) $(SPU_PKG_RVK_VERIFIER_CRYPTO_CFLAGS) $(SPU_PKG_RVK_VERIFIER_ISOLDR_INCLUDES) $(UNIT_FLAGS) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CRYPTO_CC) $(SPU_PKG_RVK_VERIFIER_CRYPTO_CFLAGS) $(SPU_PKG_RVK_VERIFIER_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_CC) $(SPU_PKG_RVK_VERIFIER_CFLAGS) $(SPU_PKG_RVK_VERIFIER_LV0LDR_INCLUDES) $(SPU_PKG_RVK_VERIFIER_DEPFLAGS) -c $< -o $@

$(SPU_PKG_RVK_VERIFIER_LINKED): $(SPU_PKG_RVK_VERIFIER_OBJS) $(SPU_PKG_RVK_VERIFIER_LDSCRIPT)
	$(SPU_PKG_RVK_VERIFIER_LD) -T $(SPU_PKG_RVK_VERIFIER_LDSCRIPT) --gc-sections -Map $(SPU_PKG_RVK_VERIFIER_MAP) -o $@ $(SPU_PKG_RVK_VERIFIER_OBJS)

$(SPU_PKG_RVK_VERIFIER_ELF): $(SPU_PKG_RVK_VERIFIER_LINKED)
	@cp $< $@.tmp; \
	for p in $(SPU_PKG_RVK_VERIFIER_PATCHES); do \
		at=$${p%%:*}; sym=$${at%+*}; old=$${p#*:}; old=$${old%%:*}; new=$${p##*:}; \
		a=$$(( 0x$$($(SPU_PKG_RVK_VERIFIER_NM) $< | grep " $$sym$$" | cut -d' ' -f1) + $${at##*+} )); \
		off=$$($(SPU_PKG_RVK_VERIFIER_READELF) -lW $< | grep LOAD | while read t o v p f rest; do \
			if [ $$a -ge $$((v)) ] && [ $$a -lt $$((v + f)) ]; then echo $$((o + a - v)); fi; \
		done); \
		if [ "$$(od -An -tx1 -j$$off -N4 $@.tmp | tr -d ' \n')" != "$$old" ]; then \
			echo "spu_pkg_rvk_verifier: $$at is not the compiler's $$old, the word the image's patch replaces"; \
			rm -f $@.tmp; \
			exit 1; \
		fi; \
		printf "$$(printf '\\%03o' $$((0x$$new >> 24 & 255)) $$((0x$$new >> 16 & 255)) $$((0x$$new >> 8 & 255)) $$((0x$$new & 255)))" | \
			dd of=$@.tmp bs=1 seek=$$off conv=notrunc 2>/dev/null || exit 1; \
		echo "spu_pkg_rvk_verifier: $$at ($$(printf 0x%X $$a)) patched from $$old to $$new, as the image has it"; \
	done; \
	mv $@.tmp $@

$(SPU_PKG_RVK_VERIFIER_BIN): $(SPU_PKG_RVK_VERIFIER_ELF)
	$(SPU_PKG_RVK_VERIFIER_OBJCOPY) -O binary $< $@

$(SPU_PKG_RVK_VERIFIER_IMAGE_BIN): $(SPU_PKG_RVK_VERIFIER_IMAGE)
	@mkdir -p $(@D)
	$(SPU_PKG_RVK_VERIFIER_OBJCOPY) -O binary $< $@

check-spu_pkg_rvk_verifier: $(SPU_PKG_RVK_VERIFIER_BIN) $(SPU_PKG_RVK_VERIFIER_IMAGE_BIN) $(SPU_PKG_RVK_VERIFIER_ELF)
	@$(SPU_PKG_RVK_VERIFIER_READELF) -lW $(SPU_PKG_RVK_VERIFIER_IMAGE) | grep LOAD > $(SPU_PKG_RVK_VERIFIER_OUT)/segments.image
	@$(SPU_PKG_RVK_VERIFIER_READELF) -lW $(SPU_PKG_RVK_VERIFIER_ELF) | grep LOAD > $(SPU_PKG_RVK_VERIFIER_OUT)/segments.built
	@if cmp -s $(SPU_PKG_RVK_VERIFIER_OUT)/segments.image $(SPU_PKG_RVK_VERIFIER_OUT)/segments.built; then \
		echo "spu_pkg_rvk_verifier: program headers identical to $(SPU_PKG_RVK_VERIFIER_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "spu_pkg_rvk_verifier: program headers DIFFER from $(SPU_PKG_RVK_VERIFIER_IMAGE) (image, then build):"; \
		cat $(SPU_PKG_RVK_VERIFIER_OUT)/segments.image $(SPU_PKG_RVK_VERIFIER_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SPU_PKG_RVK_VERIFIER_IMAGE_BIN) $(SPU_PKG_RVK_VERIFIER_BIN) > $(SPU_PKG_RVK_VERIFIER_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SPU_PKG_RVK_VERIFIER_OUT)/differing.raw | sed 's/^ //' > $(SPU_PKG_RVK_VERIFIER_OUT)/differing.built; \
	if [ ! -s $(SPU_PKG_RVK_VERIFIER_OUT)/differing.built ]; then \
		echo "spu_pkg_rvk_verifier: $(SPU_PKG_RVK_VERIFIER_LOAD_START)-$(SPU_PKG_RVK_VERIFIER_LOAD_END) identical to $(SPU_PKG_RVK_VERIFIER_IMAGE)"; \
	elif cmp -s $(SPU_PKG_RVK_VERIFIER_OUT)/differing.built $(SPU_PKG_RVK_VERIFIER_UNMATCHED); then \
		echo "spu_pkg_rvk_verifier: $(SPU_PKG_RVK_VERIFIER_LOAD_START)-$(SPU_PKG_RVK_VERIFIER_LOAD_END) identical to $(SPU_PKG_RVK_VERIFIER_IMAGE) but for the $$(wc -l < $(SPU_PKG_RVK_VERIFIER_UNMATCHED)) bytes of the six functions that do not match yet (rvk_verifier::verify and memcpy; pkg_verifier::verify_body, rvk_verifier::verify_entries and sce_verifier::verify_section, short and padded to the image's sizes; sce_verifier::sce_verifier, 4 bytes long, which puts the 22 functions after it 8 bytes later, up to verify_section) and of the branches and vtable entries that reach the moved functions ($(SPU_PKG_RVK_VERIFIER_UNMATCHED))"; \
	else \
		echo "spu_pkg_rvk_verifier: $(SPU_PKG_RVK_VERIFIER_LOAD_START)-$(SPU_PKG_RVK_VERIFIER_LOAD_END) DIFFERS from $(SPU_PKG_RVK_VERIFIER_IMAGE) (offset from $(SPU_PKG_RVK_VERIFIER_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SPU_PKG_RVK_VERIFIER_UNMATCHED) $(SPU_PKG_RVK_VERIFIER_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SPU_PKG_RVK_VERIFIER_OUT)/differing.built) bytes differ, $$(wc -l < $(SPU_PKG_RVK_VERIFIER_UNMATCHED)) of them expected ($(SPU_PKG_RVK_VERIFIER_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SPU_PKG_RVK_VERIFIER_NM) $(SPU_PKG_RVK_VERIFIER_ELF) | cut -d' ' -f1,3 > $(SPU_PKG_RVK_VERIFIER_OUT)/globals.built
	@if grep -Fxv -f $(SPU_PKG_RVK_VERIFIER_OUT)/globals.built $(SPU_PKG_RVK_VERIFIER_GLOBALS); then \
		echo "spu_pkg_rvk_verifier: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "spu_pkg_rvk_verifier: all $$(wc -l < $(SPU_PKG_RVK_VERIFIER_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-spu_pkg_rvk_verifier:
	rm -rf $(SPU_PKG_RVK_VERIFIER_OUT)

-include $(SPU_PKG_RVK_VERIFIER_OBJS:.o=.d)
