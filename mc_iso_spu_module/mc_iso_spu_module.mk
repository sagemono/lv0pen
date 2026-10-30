MC_ISO_SPU_MODULE_OUT := $(BUILD_DIR)/mc_iso_spu_module
MC_ISO_SPU_MODULE_OBJ_DIR := $(MC_ISO_SPU_MODULE_OUT)/obj
MC_ISO_SPU_MODULE_ELF := $(MC_ISO_SPU_MODULE_OUT)/mc_iso_spu_module.elf
MC_ISO_SPU_MODULE_BIN := $(MC_ISO_SPU_MODULE_OUT)/mc_iso_spu_module.bin
MC_ISO_SPU_MODULE_MAP := $(MC_ISO_SPU_MODULE_OUT)/mc_iso_spu_module.map
MC_ISO_SPU_MODULE_LDSCRIPT := mc_iso_spu_module/mc_iso_spu_module.ld
MC_ISO_SPU_MODULE_GLOBALS := mc_iso_spu_module/mc_iso_spu_module.globals
MC_ISO_SPU_MODULE_UNMATCHED := mc_iso_spu_module/mc_iso_spu_module.unmatched

MC_ISO_SPU_MODULE_IMAGE := mc_iso_spu_module/image/mc_iso_spu_module.elf
MC_ISO_SPU_MODULE_IMAGE_BIN := $(MC_ISO_SPU_MODULE_OUT)/image.bin
MC_ISO_SPU_MODULE_LOAD_START := 0x880
MC_ISO_SPU_MODULE_LOAD_END := 0x7C40

MC_ISO_SPU_MODULE_UNITS := \
	spu_token_processor/src/crt0.S \
	mc_iso_spu_module/src/mc_module.cpp \
	sc_iso/src/ss_iso_dma_data.cpp \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_cbc_decrypt.c \
	mc_iso_spu_module/crypto/des_cbc.c \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
	mc_iso_spu_module/crypto/des.c \
	spp_verifier/src/runtime/dma_channel.cpp \
	spp_verifier/src/runtime/dma_mailbox.cpp \
	spp_verifier/src/runtime/delete.cpp \
	mc_iso_spu_module/src/runtime/atexit.c \
	spp_verifier/src/runtime/free.c \
	spp_verifier/src/runtime/heap.c \
	spp_verifier/src/runtime/memcpy.c \
	lv0ldr/src/util/shuffle_table.c

MC_ISO_SPU_MODULE_OBJS := $(addprefix $(MC_ISO_SPU_MODULE_OBJ_DIR)/,$(addsuffix .o,$(basename $(MC_ISO_SPU_MODULE_UNITS))))

# the module's code is older than the other modules': SDK 2.50's GCC 4.1.1,
# whose toolchain has no spu_intrinsics.h nor spu_mfcio.h (lv1ldr's)
MC_ISO_SPU_MODULE_CC := $(SPU_GCC411_SDK250_BIN)/spu-lv2-gcc
MC_ISO_SPU_MODULE_CXX := $(SPU_GCC411_SDK250_BIN)/spu-lv2-g++
MC_ISO_SPU_MODULE_LD := $(SPU_GCC411_SDK250_BIN)/spu-lv2-ld
MC_ISO_SPU_MODULE_OBJCOPY := $(SPU_GCC411_SDK250_BIN)/spu-lv2-objcopy
MC_ISO_SPU_MODULE_NM := $(SPU_GCC411_SDK250_BIN)/spu-lv2-nm
MC_ISO_SPU_MODULE_READELF := $(SPU_GCC411_SDK250_BIN)/spu-lv2-readelf
MC_ISO_SPU_MODULE_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

MC_ISO_SPU_MODULE_CFLAGS := -Os -ffunction-sections -fdata-sections
MC_ISO_SPU_MODULE_CRYPTO_CFLAGS := -O3
MC_ISO_SPU_MODULE_DEPFLAGS := -MMD -MP
MC_ISO_SPU_MODULE_SYS_INCLUDES := -Ilv1ldr/encdec/sys
MC_ISO_SPU_MODULE_CRYPTO_INCLUDES := -Imc_iso_spu_module/include -Isc_iso/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include
MC_ISO_SPU_MODULE_INCLUDES := $(MC_ISO_SPU_MODULE_CRYPTO_INCLUDES) $(MC_ISO_SPU_MODULE_SYS_INCLUDES)
MC_ISO_SPU_MODULE_LV0LDR_INCLUDES := -Ilv0ldr/include
MC_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include $(MC_ISO_SPU_MODULE_SYS_INCLUDES)
MC_ISO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include $(MC_ISO_SPU_MODULE_SYS_INCLUDES)

$(MC_ISO_SPU_MODULE_OBJ_DIR)/mc_iso_spu_module/src/runtime/atexit.o: UNIT_FLAGS := -O2
$(MC_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(MC_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2

.PHONY: mc_iso_spu_module check-mc_iso_spu_module clean-mc_iso_spu_module

mc_iso_spu_module: $(MC_ISO_SPU_MODULE_ELF) $(MC_ISO_SPU_MODULE_BIN)

$(MC_ISO_SPU_MODULE_OBJ_DIR)/mc_iso_spu_module/crypto/%.o: mc_iso_spu_module/crypto/%.c
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CRYPTO_CC) $(MC_ISO_SPU_MODULE_CRYPTO_CFLAGS) $(MC_ISO_SPU_MODULE_CRYPTO_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/mc_iso_spu_module/%.o: mc_iso_spu_module/%.cpp
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CXX) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/mc_iso_spu_module/%.o: mc_iso_spu_module/%.c
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CC) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_INCLUDES) $(UNIT_FLAGS) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/sc_iso/%.o: sc_iso/%.cpp
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CXX) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CXX) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CXX) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CC) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CRYPTO_CC) $(MC_ISO_SPU_MODULE_CRYPTO_CFLAGS) $(MC_ISO_SPU_MODULE_LV0LDR_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_CC) $(MC_ISO_SPU_MODULE_CFLAGS) $(MC_ISO_SPU_MODULE_LV0LDR_INCLUDES) $(MC_ISO_SPU_MODULE_SYS_INCLUDES) $(MC_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MC_ISO_SPU_MODULE_ELF): $(MC_ISO_SPU_MODULE_OBJS) $(MC_ISO_SPU_MODULE_LDSCRIPT)
	$(MC_ISO_SPU_MODULE_LD) -T $(MC_ISO_SPU_MODULE_LDSCRIPT) --gc-sections -Map $(MC_ISO_SPU_MODULE_MAP) -o $@ $(MC_ISO_SPU_MODULE_OBJS)

$(MC_ISO_SPU_MODULE_BIN): $(MC_ISO_SPU_MODULE_ELF)
	$(MC_ISO_SPU_MODULE_OBJCOPY) -O binary $< $@

$(MC_ISO_SPU_MODULE_IMAGE_BIN): $(MC_ISO_SPU_MODULE_IMAGE)
	@mkdir -p $(@D)
	$(MC_ISO_SPU_MODULE_OBJCOPY) -O binary $< $@

check-mc_iso_spu_module: $(MC_ISO_SPU_MODULE_BIN) $(MC_ISO_SPU_MODULE_IMAGE_BIN) $(MC_ISO_SPU_MODULE_ELF)
	@$(MC_ISO_SPU_MODULE_READELF) -lW $(MC_ISO_SPU_MODULE_IMAGE) | grep LOAD > $(MC_ISO_SPU_MODULE_OUT)/segments.image
	@$(MC_ISO_SPU_MODULE_READELF) -lW $(MC_ISO_SPU_MODULE_ELF) | grep LOAD > $(MC_ISO_SPU_MODULE_OUT)/segments.built
	@if cmp -s $(MC_ISO_SPU_MODULE_OUT)/segments.image $(MC_ISO_SPU_MODULE_OUT)/segments.built; then \
		echo "mc_iso_spu_module: program headers identical to $(MC_ISO_SPU_MODULE_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "mc_iso_spu_module: program headers DIFFER from $(MC_ISO_SPU_MODULE_IMAGE) (image, then build):"; \
		cat $(MC_ISO_SPU_MODULE_OUT)/segments.image $(MC_ISO_SPU_MODULE_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(MC_ISO_SPU_MODULE_IMAGE_BIN) $(MC_ISO_SPU_MODULE_BIN) > $(MC_ISO_SPU_MODULE_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(MC_ISO_SPU_MODULE_OUT)/differing.raw | sed 's/^ //' > $(MC_ISO_SPU_MODULE_OUT)/differing.built; \
	if [ ! -s $(MC_ISO_SPU_MODULE_OUT)/differing.built ]; then \
		echo "mc_iso_spu_module: $(MC_ISO_SPU_MODULE_LOAD_START)-$(MC_ISO_SPU_MODULE_LOAD_END) identical to $(MC_ISO_SPU_MODULE_IMAGE)"; \
	elif cmp -s $(MC_ISO_SPU_MODULE_OUT)/differing.built $(MC_ISO_SPU_MODULE_UNMATCHED); then \
		echo "mc_iso_spu_module: $(MC_ISO_SPU_MODULE_LOAD_START)-$(MC_ISO_SPU_MODULE_LOAD_END) identical to $(MC_ISO_SPU_MODULE_IMAGE) but for the $$(wc -l < $(MC_ISO_SPU_MODULE_UNMATCHED)) bytes of the functions that do not match yet ($(MC_ISO_SPU_MODULE_UNMATCHED))"; \
	else \
		echo "mc_iso_spu_module: $(MC_ISO_SPU_MODULE_LOAD_START)-$(MC_ISO_SPU_MODULE_LOAD_END) DIFFERS from $(MC_ISO_SPU_MODULE_IMAGE) (offset from $(MC_ISO_SPU_MODULE_LOAD_START) + 1, image byte, built byte):"; \
		diff $(MC_ISO_SPU_MODULE_UNMATCHED) $(MC_ISO_SPU_MODULE_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(MC_ISO_SPU_MODULE_OUT)/differing.built) bytes differ, $$(wc -l < $(MC_ISO_SPU_MODULE_UNMATCHED)) of them expected ($(MC_ISO_SPU_MODULE_UNMATCHED))"; \
		exit 1; \
	fi
	@$(MC_ISO_SPU_MODULE_NM) $(MC_ISO_SPU_MODULE_ELF) | cut -d' ' -f1,3 > $(MC_ISO_SPU_MODULE_OUT)/globals.built
	@if grep -Fxv -f $(MC_ISO_SPU_MODULE_OUT)/globals.built $(MC_ISO_SPU_MODULE_GLOBALS); then \
		echo "mc_iso_spu_module: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "mc_iso_spu_module: all $$(wc -l < $(MC_ISO_SPU_MODULE_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-mc_iso_spu_module:
	rm -rf $(MC_ISO_SPU_MODULE_OUT)

-include $(MC_ISO_SPU_MODULE_OBJS:.o=.d)
