SPU_UTOKEN_PROCESSOR_OUT := $(BUILD_DIR)/spu_utoken_processor
SPU_UTOKEN_PROCESSOR_OBJ_DIR := $(SPU_UTOKEN_PROCESSOR_OUT)/obj
SPU_UTOKEN_PROCESSOR_ELF := $(SPU_UTOKEN_PROCESSOR_OUT)/spu_utoken_processor.elf
SPU_UTOKEN_PROCESSOR_BIN := $(SPU_UTOKEN_PROCESSOR_OUT)/spu_utoken_processor.bin
SPU_UTOKEN_PROCESSOR_MAP := $(SPU_UTOKEN_PROCESSOR_OUT)/spu_utoken_processor.map
SPU_UTOKEN_PROCESSOR_LDSCRIPT := spu_utoken_processor/spu_utoken_processor.ld
SPU_UTOKEN_PROCESSOR_GLOBALS := spu_utoken_processor/spu_utoken_processor.globals
SPU_UTOKEN_PROCESSOR_UNMATCHED := spu_utoken_processor/spu_utoken_processor.unmatched

SPU_UTOKEN_PROCESSOR_IMAGE := spu_utoken_processor/image/spu_utoken_processor.elf
SPU_UTOKEN_PROCESSOR_IMAGE_BIN := $(SPU_UTOKEN_PROCESSOR_OUT)/image.bin
SPU_UTOKEN_PROCESSOR_LOAD_START := 0x880
SPU_UTOKEN_PROCESSOR_LOAD_END := 0x5DC0

SPU_UTOKEN_PROCESSOR_UNITS := \
	spu_token_processor/src/crt0.S \
	lv2ldr/src/hash_reader.cpp \
	spu_utoken_processor/src/utoken.cpp \
	spu_utoken_processor/src/main.cpp \
	spu_token_processor/src/eid0_reader.cpp \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_cbc_encrypt.c \
	lv0ldr/crypto/aes_cbc_decrypt.c \
	isoldr/crypto/aes_cmac.c \
	isoldr/crypto/hmac_sha1_once.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/sha1_transform.c \
	spp_verifier/src/runtime/dma_channel.cpp \
	spp_verifier/src/runtime/delete.cpp \
	spp_verifier/src/runtime/exit.c \
	spp_verifier/src/runtime/free.c \
	spp_verifier/src/runtime/heap.c \
	spp_verifier/src/runtime/memcpy.c \
	spp_verifier/src/runtime/memset.c \
	spp_verifier/src/runtime/atexit.c \
	lv0ldr/src/util/shuffle_table.c

SPU_UTOKEN_PROCESSOR_OBJS := $(addprefix $(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/,$(addsuffix .o,$(basename $(SPU_UTOKEN_PROCESSOR_UNITS))))

SPU_UTOKEN_PROCESSOR_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
SPU_UTOKEN_PROCESSOR_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
SPU_UTOKEN_PROCESSOR_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SPU_UTOKEN_PROCESSOR_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SPU_UTOKEN_PROCESSOR_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SPU_UTOKEN_PROCESSOR_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SPU_UTOKEN_PROCESSOR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

SPU_UTOKEN_PROCESSOR_CFLAGS := -Os -ffunction-sections -fdata-sections
SPU_UTOKEN_PROCESSOR_CRYPTO_CFLAGS := -O3
SPU_UTOKEN_PROCESSOR_DEPFLAGS := -MMD -MP
SPU_UTOKEN_PROCESSOR_INCLUDES := -Ispu_utoken_processor/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include
SPU_UTOKEN_PROCESSOR_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
SPU_UTOKEN_PROCESSOR_LV0LDR_INCLUDES := -Ilv0ldr/include
SPU_UTOKEN_PROCESSOR_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
SPU_UTOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
SPU_UTOKEN_PROCESSOR_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: UNIT_FLAGS := -O2
$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2

.PHONY: spu_utoken_processor check-spu_utoken_processor clean-spu_utoken_processor

spu_utoken_processor: $(SPU_UTOKEN_PROCESSOR_ELF) $(SPU_UTOKEN_PROCESSOR_BIN)

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spu_utoken_processor/%.o: spu_utoken_processor/%.cpp
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CXX) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.cpp
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CXX) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_SPU_TOKEN_PROCESSOR_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CXX) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_SPU_TOKEN_PROCESSOR_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CXX) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CC) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CXX) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_LV2LDR_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CRYPTO_CC) $(SPU_UTOKEN_PROCESSOR_CRYPTO_CFLAGS) $(SPU_UTOKEN_PROCESSOR_ISOLDR_INCLUDES) $(UNIT_FLAGS) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CRYPTO_CC) $(SPU_UTOKEN_PROCESSOR_CRYPTO_CFLAGS) $(SPU_UTOKEN_PROCESSOR_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_CC) $(SPU_UTOKEN_PROCESSOR_CFLAGS) $(SPU_UTOKEN_PROCESSOR_LV0LDR_INCLUDES) $(SPU_UTOKEN_PROCESSOR_DEPFLAGS) -c $< -o $@

$(SPU_UTOKEN_PROCESSOR_ELF): $(SPU_UTOKEN_PROCESSOR_OBJS) $(SPU_UTOKEN_PROCESSOR_LDSCRIPT)
	$(SPU_UTOKEN_PROCESSOR_LD) -T $(SPU_UTOKEN_PROCESSOR_LDSCRIPT) --gc-sections -Map $(SPU_UTOKEN_PROCESSOR_MAP) -o $@ $(SPU_UTOKEN_PROCESSOR_OBJS)

$(SPU_UTOKEN_PROCESSOR_BIN): $(SPU_UTOKEN_PROCESSOR_ELF)
	$(SPU_UTOKEN_PROCESSOR_OBJCOPY) -O binary $< $@

$(SPU_UTOKEN_PROCESSOR_IMAGE_BIN): $(SPU_UTOKEN_PROCESSOR_IMAGE)
	@mkdir -p $(@D)
	$(SPU_UTOKEN_PROCESSOR_OBJCOPY) -O binary $< $@

check-spu_utoken_processor: $(SPU_UTOKEN_PROCESSOR_BIN) $(SPU_UTOKEN_PROCESSOR_IMAGE_BIN) $(SPU_UTOKEN_PROCESSOR_ELF)
	@$(SPU_UTOKEN_PROCESSOR_READELF) -lW $(SPU_UTOKEN_PROCESSOR_IMAGE) | grep LOAD > $(SPU_UTOKEN_PROCESSOR_OUT)/segments.image
	@$(SPU_UTOKEN_PROCESSOR_READELF) -lW $(SPU_UTOKEN_PROCESSOR_ELF) | grep LOAD > $(SPU_UTOKEN_PROCESSOR_OUT)/segments.built
	@if cmp -s $(SPU_UTOKEN_PROCESSOR_OUT)/segments.image $(SPU_UTOKEN_PROCESSOR_OUT)/segments.built; then \
		echo "spu_utoken_processor: program headers identical to $(SPU_UTOKEN_PROCESSOR_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "spu_utoken_processor: program headers DIFFER from $(SPU_UTOKEN_PROCESSOR_IMAGE) (image, then build):"; \
		cat $(SPU_UTOKEN_PROCESSOR_OUT)/segments.image $(SPU_UTOKEN_PROCESSOR_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SPU_UTOKEN_PROCESSOR_IMAGE_BIN) $(SPU_UTOKEN_PROCESSOR_BIN) > $(SPU_UTOKEN_PROCESSOR_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SPU_UTOKEN_PROCESSOR_OUT)/differing.raw | sed 's/^ //' > $(SPU_UTOKEN_PROCESSOR_OUT)/differing.built; \
	if [ ! -s $(SPU_UTOKEN_PROCESSOR_OUT)/differing.built ]; then \
		echo "spu_utoken_processor: $(SPU_UTOKEN_PROCESSOR_LOAD_START)-$(SPU_UTOKEN_PROCESSOR_LOAD_END) identical to $(SPU_UTOKEN_PROCESSOR_IMAGE)"; \
	elif cmp -s $(SPU_UTOKEN_PROCESSOR_OUT)/differing.built $(SPU_UTOKEN_PROCESSOR_UNMATCHED); then \
		echo "spu_utoken_processor: $(SPU_UTOKEN_PROCESSOR_LOAD_START)-$(SPU_UTOKEN_PROCESSOR_LOAD_END) identical to $(SPU_UTOKEN_PROCESSOR_IMAGE) but for the $$(wc -l < $(SPU_UTOKEN_PROCESSOR_UNMATCHED)) bytes of the three functions that do not match yet (utoken::check_header, 8 bytes short and padded to the image's size; eid0_reader::read; memcpy) and of two of utoken.cpp's 16-byte constants, which its order of functions puts the other way round, with the loads of them in utoken::put, utoken::get and read_params ($(SPU_UTOKEN_PROCESSOR_UNMATCHED))"; \
	else \
		echo "spu_utoken_processor: $(SPU_UTOKEN_PROCESSOR_LOAD_START)-$(SPU_UTOKEN_PROCESSOR_LOAD_END) DIFFERS from $(SPU_UTOKEN_PROCESSOR_IMAGE) (offset from $(SPU_UTOKEN_PROCESSOR_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SPU_UTOKEN_PROCESSOR_UNMATCHED) $(SPU_UTOKEN_PROCESSOR_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SPU_UTOKEN_PROCESSOR_OUT)/differing.built) bytes differ, $$(wc -l < $(SPU_UTOKEN_PROCESSOR_UNMATCHED)) of them expected ($(SPU_UTOKEN_PROCESSOR_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SPU_UTOKEN_PROCESSOR_NM) $(SPU_UTOKEN_PROCESSOR_ELF) | cut -d' ' -f1,3 > $(SPU_UTOKEN_PROCESSOR_OUT)/globals.built
	@if grep -Fxv -f $(SPU_UTOKEN_PROCESSOR_OUT)/globals.built $(SPU_UTOKEN_PROCESSOR_GLOBALS); then \
		echo "spu_utoken_processor: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "spu_utoken_processor: all $$(wc -l < $(SPU_UTOKEN_PROCESSOR_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-spu_utoken_processor:
	rm -rf $(SPU_UTOKEN_PROCESSOR_OUT)

-include $(SPU_UTOKEN_PROCESSOR_OBJS:.o=.d)
