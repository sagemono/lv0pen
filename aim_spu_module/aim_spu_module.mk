AIM_SPU_MODULE_OUT := $(BUILD_DIR)/aim_spu_module
AIM_SPU_MODULE_OBJ_DIR := $(AIM_SPU_MODULE_OUT)/obj
AIM_SPU_MODULE_ELF := $(AIM_SPU_MODULE_OUT)/aim_spu_module.elf
AIM_SPU_MODULE_BIN := $(AIM_SPU_MODULE_OUT)/aim_spu_module.bin
AIM_SPU_MODULE_MAP := $(AIM_SPU_MODULE_OUT)/aim_spu_module.map
AIM_SPU_MODULE_LDSCRIPT := aim_spu_module/aim_spu_module.ld
AIM_SPU_MODULE_GLOBALS := aim_spu_module/aim_spu_module.globals
AIM_SPU_MODULE_UNMATCHED := aim_spu_module/aim_spu_module.unmatched

AIM_SPU_MODULE_IMAGE := aim_spu_module/image/aim_spu_module.elf
AIM_SPU_MODULE_IMAGE_BIN := $(AIM_SPU_MODULE_OUT)/image.bin
AIM_SPU_MODULE_LOAD_START := 0x880
AIM_SPU_MODULE_LOAD_END := 0x3DC0

AIM_SPU_MODULE_UNITS := 	aim_spu_module/src/salt.S 	spu_token_processor/src/crt0.S 	aim_spu_module/src/main.cpp 	lv2ldr/src/hash_reader.cpp 	spu_token_processor/src/eid0_reader.cpp 	aim_spu_module/src/aim_reader.cpp 	aim_spu_module/src/dma_log.cpp 	lv0ldr/crypto/aes_set_encrypt_key.c 	lv0ldr/crypto/aes_encrypt_block.c 	lv0ldr/crypto/aes_cbc_decrypt.c 	isoldr/crypto/aes_cmac.c 	lv0ldr/crypto/aes_set_decrypt_key.c 	lv0ldr/crypto/aes_decrypt_block.c 	lv0ldr/crypto/aes_decrypt8.c 	spp_verifier/src/runtime/dma_channel.cpp 	spp_verifier/src/runtime/delete.cpp 	spp_verifier/src/runtime/exit.c 	spp_verifier/src/runtime/free.c 	spp_verifier/src/runtime/heap.c 	spp_verifier/src/runtime/memcpy.c 	spp_verifier/src/runtime/memset.c 	aim_spu_module/src/runtime/strlen.c 	spp_verifier/src/runtime/atexit.c 	lv0ldr/src/util/shuffle_table.c

AIM_SPU_MODULE_OBJS := $(addprefix $(AIM_SPU_MODULE_OBJ_DIR)/,$(addsuffix .o,$(basename $(AIM_SPU_MODULE_UNITS))))

AIM_SPU_MODULE_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
AIM_SPU_MODULE_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
AIM_SPU_MODULE_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
AIM_SPU_MODULE_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
AIM_SPU_MODULE_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
AIM_SPU_MODULE_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
AIM_SPU_MODULE_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

AIM_SPU_MODULE_CFLAGS := -Os -ffunction-sections -fdata-sections
AIM_SPU_MODULE_CRYPTO_CFLAGS := -O3
AIM_SPU_MODULE_DEPFLAGS := -MMD -MP
AIM_SPU_MODULE_INCLUDES := -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include
AIM_SPU_MODULE_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
AIM_SPU_MODULE_LV0LDR_INCLUDES := -Ilv0ldr/include
AIM_SPU_MODULE_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
AIM_SPU_MODULE_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
AIM_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include

$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2
$(AIM_SPU_MODULE_OBJ_DIR)/aim_spu_module/src/runtime/strlen.o: UNIT_FLAGS := -O2

.PHONY: aim_spu_module check-aim_spu_module clean-aim_spu_module

aim_spu_module: $(AIM_SPU_MODULE_ELF) $(AIM_SPU_MODULE_BIN)

$(AIM_SPU_MODULE_OBJ_DIR)/aim_spu_module/%.o: aim_spu_module/%.cpp
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/aim_spu_module/%.o: aim_spu_module/%.S
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/aim_spu_module/%.o: aim_spu_module/%.c
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CC) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_INCLUDES) $(UNIT_FLAGS) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.cpp
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CC) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CXX) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_LV2LDR_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CRYPTO_CC) $(AIM_SPU_MODULE_CRYPTO_CFLAGS) $(AIM_SPU_MODULE_ISOLDR_INCLUDES) $(UNIT_FLAGS) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CRYPTO_CC) $(AIM_SPU_MODULE_CRYPTO_CFLAGS) $(AIM_SPU_MODULE_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_CC) $(AIM_SPU_MODULE_CFLAGS) $(AIM_SPU_MODULE_LV0LDR_INCLUDES) $(AIM_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(AIM_SPU_MODULE_ELF): $(AIM_SPU_MODULE_OBJS) $(AIM_SPU_MODULE_LDSCRIPT)
	$(AIM_SPU_MODULE_LD) -T $(AIM_SPU_MODULE_LDSCRIPT) --gc-sections -Map $(AIM_SPU_MODULE_MAP) -o $@ $(AIM_SPU_MODULE_OBJS)

$(AIM_SPU_MODULE_BIN): $(AIM_SPU_MODULE_ELF)
	$(AIM_SPU_MODULE_OBJCOPY) -O binary $< $@

$(AIM_SPU_MODULE_IMAGE_BIN): $(AIM_SPU_MODULE_IMAGE)
	@mkdir -p $(@D)
	$(AIM_SPU_MODULE_OBJCOPY) -O binary $< $@

check-aim_spu_module: $(AIM_SPU_MODULE_BIN) $(AIM_SPU_MODULE_IMAGE_BIN) $(AIM_SPU_MODULE_ELF)
	@$(AIM_SPU_MODULE_READELF) -lW $(AIM_SPU_MODULE_IMAGE) | grep LOAD > $(AIM_SPU_MODULE_OUT)/segments.image
	@$(AIM_SPU_MODULE_READELF) -lW $(AIM_SPU_MODULE_ELF) | grep LOAD > $(AIM_SPU_MODULE_OUT)/segments.built
	@if cmp -s $(AIM_SPU_MODULE_OUT)/segments.image $(AIM_SPU_MODULE_OUT)/segments.built; then \
		echo "aim_spu_module: program headers identical to $(AIM_SPU_MODULE_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "aim_spu_module: program headers DIFFER from $(AIM_SPU_MODULE_IMAGE) (image, then build):"; \
		cat $(AIM_SPU_MODULE_OUT)/segments.image $(AIM_SPU_MODULE_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(AIM_SPU_MODULE_IMAGE_BIN) $(AIM_SPU_MODULE_BIN) > $(AIM_SPU_MODULE_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(AIM_SPU_MODULE_OUT)/differing.raw | sed 's/^ //' > $(AIM_SPU_MODULE_OUT)/differing.built; \
	if [ ! -s $(AIM_SPU_MODULE_OUT)/differing.built ]; then \
		echo "aim_spu_module: $(AIM_SPU_MODULE_LOAD_START)-$(AIM_SPU_MODULE_LOAD_END) identical to $(AIM_SPU_MODULE_IMAGE)"; \
	elif cmp -s $(AIM_SPU_MODULE_OUT)/differing.built $(AIM_SPU_MODULE_UNMATCHED); then \
		echo "aim_spu_module: $(AIM_SPU_MODULE_LOAD_START)-$(AIM_SPU_MODULE_LOAD_END) identical to $(AIM_SPU_MODULE_IMAGE) but for the $$(wc -l < $(AIM_SPU_MODULE_UNMATCHED)) bytes of the two functions that do not match yet (eid0_reader::read, memcpy) ($(AIM_SPU_MODULE_UNMATCHED))"; \
	else \
		echo "aim_spu_module: $(AIM_SPU_MODULE_LOAD_START)-$(AIM_SPU_MODULE_LOAD_END) DIFFERS from $(AIM_SPU_MODULE_IMAGE) (offset from $(AIM_SPU_MODULE_LOAD_START) + 1, image byte, built byte):"; \
		diff $(AIM_SPU_MODULE_UNMATCHED) $(AIM_SPU_MODULE_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(AIM_SPU_MODULE_OUT)/differing.built) bytes differ, $$(wc -l < $(AIM_SPU_MODULE_UNMATCHED)) of them expected ($(AIM_SPU_MODULE_UNMATCHED))"; \
		exit 1; \
	fi
	@$(AIM_SPU_MODULE_NM) $(AIM_SPU_MODULE_ELF) | cut -d' ' -f1,3 > $(AIM_SPU_MODULE_OUT)/globals.built
	@if grep -Fxv -f $(AIM_SPU_MODULE_OUT)/globals.built $(AIM_SPU_MODULE_GLOBALS); then \
		echo "aim_spu_module: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "aim_spu_module: all $$(wc -l < $(AIM_SPU_MODULE_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-aim_spu_module:
	rm -rf $(AIM_SPU_MODULE_OUT)

-include $(AIM_SPU_MODULE_OBJS:.o=.d)
