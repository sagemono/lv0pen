SB_ISO_SPU_MODULE_OUT := $(BUILD_DIR)/sb_iso_spu_module
SB_ISO_SPU_MODULE_OBJ_DIR := $(SB_ISO_SPU_MODULE_OUT)/obj
SB_ISO_SPU_MODULE_ELF := $(SB_ISO_SPU_MODULE_OUT)/sb_iso_spu_module.elf
SB_ISO_SPU_MODULE_BIN := $(SB_ISO_SPU_MODULE_OUT)/sb_iso_spu_module.bin
SB_ISO_SPU_MODULE_MAP := $(SB_ISO_SPU_MODULE_OUT)/sb_iso_spu_module.map
SB_ISO_SPU_MODULE_LDSCRIPT := sb_iso_spu_module/sb_iso_spu_module.ld
SB_ISO_SPU_MODULE_GLOBALS := sb_iso_spu_module/sb_iso_spu_module.globals
SB_ISO_SPU_MODULE_UNMATCHED := sb_iso_spu_module/sb_iso_spu_module.unmatched

SB_ISO_SPU_MODULE_IMAGE := sb_iso_spu_module/image/sb_iso_spu_module.elf
SB_ISO_SPU_MODULE_IMAGE_BIN := $(SB_ISO_SPU_MODULE_OUT)/image.bin
SB_ISO_SPU_MODULE_LOAD_START := 0x880
SB_ISO_SPU_MODULE_LOAD_END := 0x5AE0

SB_ISO_SPU_MODULE_UNITS := \
	spu_token_processor/src/crt0.S \
	sb_iso_spu_module/src/main.cpp \
	sb_iso_spu_module/src/session_b.cpp \
	sb_iso_spu_module/src/session_a.cpp \
	sb_iso_spu_module/src/session_c.cpp \
	sb_iso_spu_module/src/keys.cpp \
	sc_iso/src/ss_iso_dma_data.cpp \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_cbc_decrypt.c \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
	spp_verifier/src/runtime/dma_channel.cpp \
	spp_verifier/src/runtime/dma_mailbox.cpp \
	spp_verifier/src/runtime/delete.cpp \
	spp_verifier/src/runtime/exit.c \
	spp_verifier/src/runtime/free.c \
	spp_verifier/src/runtime/heap.c \
	spp_verifier/src/runtime/memcpy.c \
	spp_verifier/src/runtime/memset.c \
	spp_verifier/src/runtime/atexit.c \
	lv0ldr/src/util/shuffle_table.c

SB_ISO_SPU_MODULE_OBJS := $(addprefix $(SB_ISO_SPU_MODULE_OBJ_DIR)/,$(addsuffix .o,$(basename $(SB_ISO_SPU_MODULE_UNITS))))

SB_ISO_SPU_MODULE_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
SB_ISO_SPU_MODULE_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
SB_ISO_SPU_MODULE_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SB_ISO_SPU_MODULE_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SB_ISO_SPU_MODULE_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SB_ISO_SPU_MODULE_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SB_ISO_SPU_MODULE_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

SB_ISO_SPU_MODULE_CFLAGS := -Os -ffunction-sections -fdata-sections
SB_ISO_SPU_MODULE_CRYPTO_CFLAGS := -O3
SB_ISO_SPU_MODULE_DEPFLAGS := -MMD -MP
SB_ISO_SPU_MODULE_INCLUDES := -Isb_iso_spu_module/include -Isc_iso/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include
SB_ISO_SPU_MODULE_LV0LDR_INCLUDES := -Ilv0ldr/include
SB_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
SB_ISO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include

$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2

.PHONY: sb_iso_spu_module check-sb_iso_spu_module clean-sb_iso_spu_module

sb_iso_spu_module: $(SB_ISO_SPU_MODULE_ELF) $(SB_ISO_SPU_MODULE_BIN)

$(SB_ISO_SPU_MODULE_OBJ_DIR)/sb_iso_spu_module/%.o: sb_iso_spu_module/%.cpp
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/sb_iso_spu_module/%.o: sb_iso_spu_module/%.S
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/sb_iso_spu_module/%.o: sb_iso_spu_module/%.c
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CC) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_INCLUDES) $(UNIT_FLAGS) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/sc_iso/%.o: sc_iso/%.cpp
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.cpp
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CXX) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CC) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CRYPTO_CC) $(SB_ISO_SPU_MODULE_CRYPTO_CFLAGS) $(SB_ISO_SPU_MODULE_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_CC) $(SB_ISO_SPU_MODULE_CFLAGS) $(SB_ISO_SPU_MODULE_LV0LDR_INCLUDES) $(SB_ISO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(SB_ISO_SPU_MODULE_ELF): $(SB_ISO_SPU_MODULE_OBJS) $(SB_ISO_SPU_MODULE_LDSCRIPT)
	$(SB_ISO_SPU_MODULE_LD) -T $(SB_ISO_SPU_MODULE_LDSCRIPT) --gc-sections -Map $(SB_ISO_SPU_MODULE_MAP) -o $@ $(SB_ISO_SPU_MODULE_OBJS)

$(SB_ISO_SPU_MODULE_BIN): $(SB_ISO_SPU_MODULE_ELF)
	$(SB_ISO_SPU_MODULE_OBJCOPY) -O binary $< $@

$(SB_ISO_SPU_MODULE_IMAGE_BIN): $(SB_ISO_SPU_MODULE_IMAGE)
	@mkdir -p $(@D)
	$(SB_ISO_SPU_MODULE_OBJCOPY) -O binary $< $@

check-sb_iso_spu_module: $(SB_ISO_SPU_MODULE_BIN) $(SB_ISO_SPU_MODULE_IMAGE_BIN) $(SB_ISO_SPU_MODULE_ELF)
	@$(SB_ISO_SPU_MODULE_READELF) -lW $(SB_ISO_SPU_MODULE_IMAGE) | grep LOAD > $(SB_ISO_SPU_MODULE_OUT)/segments.image
	@$(SB_ISO_SPU_MODULE_READELF) -lW $(SB_ISO_SPU_MODULE_ELF) | grep LOAD > $(SB_ISO_SPU_MODULE_OUT)/segments.built
	@if cmp -s $(SB_ISO_SPU_MODULE_OUT)/segments.image $(SB_ISO_SPU_MODULE_OUT)/segments.built; then \
		echo "sb_iso_spu_module: program headers identical to $(SB_ISO_SPU_MODULE_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "sb_iso_spu_module: program headers DIFFER from $(SB_ISO_SPU_MODULE_IMAGE) (image, then build):"; \
		cat $(SB_ISO_SPU_MODULE_OUT)/segments.image $(SB_ISO_SPU_MODULE_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SB_ISO_SPU_MODULE_IMAGE_BIN) $(SB_ISO_SPU_MODULE_BIN) > $(SB_ISO_SPU_MODULE_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SB_ISO_SPU_MODULE_OUT)/differing.raw | sed 's/^ //' > $(SB_ISO_SPU_MODULE_OUT)/differing.built; \
	if [ ! -s $(SB_ISO_SPU_MODULE_OUT)/differing.built ]; then \
		echo "sb_iso_spu_module: $(SB_ISO_SPU_MODULE_LOAD_START)-$(SB_ISO_SPU_MODULE_LOAD_END) identical to $(SB_ISO_SPU_MODULE_IMAGE)"; \
	elif cmp -s $(SB_ISO_SPU_MODULE_OUT)/differing.built $(SB_ISO_SPU_MODULE_UNMATCHED); then \
		echo "sb_iso_spu_module: $(SB_ISO_SPU_MODULE_LOAD_START)-$(SB_ISO_SPU_MODULE_LOAD_END) identical to $(SB_ISO_SPU_MODULE_IMAGE) but for the $$(wc -l < $(SB_ISO_SPU_MODULE_UNMATCHED)) bytes of the functions that do not match yet ($(SB_ISO_SPU_MODULE_UNMATCHED))"; \
	else \
		echo "sb_iso_spu_module: $(SB_ISO_SPU_MODULE_LOAD_START)-$(SB_ISO_SPU_MODULE_LOAD_END) DIFFERS from $(SB_ISO_SPU_MODULE_IMAGE) (offset from $(SB_ISO_SPU_MODULE_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SB_ISO_SPU_MODULE_UNMATCHED) $(SB_ISO_SPU_MODULE_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SB_ISO_SPU_MODULE_OUT)/differing.built) bytes differ, $$(wc -l < $(SB_ISO_SPU_MODULE_UNMATCHED)) of them expected ($(SB_ISO_SPU_MODULE_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SB_ISO_SPU_MODULE_NM) $(SB_ISO_SPU_MODULE_ELF) | cut -d' ' -f1,3 > $(SB_ISO_SPU_MODULE_OUT)/globals.built
	@if grep -Fxv -f $(SB_ISO_SPU_MODULE_OUT)/globals.built $(SB_ISO_SPU_MODULE_GLOBALS); then \
		echo "sb_iso_spu_module: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "sb_iso_spu_module: all $$(wc -l < $(SB_ISO_SPU_MODULE_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-sb_iso_spu_module:
	rm -rf $(SB_ISO_SPU_MODULE_OUT)

-include $(SB_ISO_SPU_MODULE_OBJS:.o=.d)
