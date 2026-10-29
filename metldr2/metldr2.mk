METLDR2_OUT := $(BUILD_DIR)/metldr2
METLDR2_OBJ_DIR := $(METLDR2_OUT)/obj
METLDR2_ELF := $(METLDR2_OUT)/metldr2.elf
METLDR2_BIN := $(METLDR2_OUT)/metldr2.bin
METLDR2_MAP := $(METLDR2_OUT)/metldr2.map
METLDR2_LDSCRIPT := metldr2/metldr2.ld
METLDR2_GLOBALS := metldr2/metldr2.globals

METLDR2_IMAGE := metldr2/image/metldr2.bin
METLDR2_LOAD_START := 0x400
METLDR2_LOAD_END := 0xFCE0

METLDR2_UNITS := \
	metldr2/src/entry.S \
	metldr2/src/exit_stop.c \
	isoldr/src/crt0.S \
	metldr2/src/loader_global.cpp \
	metldr2/src/main.cpp \
	metldr2/src/loader.cpp \
	metldr2/src/authenticator.cpp \
	isoldr/src/cipher_aes128_ctr.cpp \
	isoldr/src/cipher_aes256_cbc.cpp \
	isoldr/src/verifier_ecdsa.cpp \
	metldr2/src/auth_buffers.cpp \
	metldr2/src/auth_segments.cpp \
	metldr2/src/elf_reader.cpp \
	isoldr/src/hmac_digest.cpp \
	lv2ldr/src/hash_reader.cpp \
	isoldr/src/dma_queue.cpp \
	isoldr/src/dma_buffer.cpp \
	isoldr/src/dma_buffer_io.cpp \
	isoldr/src/local_buffer.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes_set_encrypt_key.c \
	lv0ldr/crypto/aes_set_decrypt_key.c \
	lv0ldr/crypto/aes_encrypt_block.c \
	lv0ldr/crypto/aes_encrypt8.c \
	lv0ldr/crypto/aes_decrypt_block.c \
	lv0ldr/crypto/aes_decrypt8.c \
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
	metldr2/src/mfc.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	lv1ldr/src/auth_header.cpp \
	metldr2/src/auth_check.cpp \
	metldr2/src/auth_data.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv0ldr/src/util/memcmp.c \
	isoldr/src/memset.c \
	isoldr/src/memcpy.c \
	lv0ldr/src/util/shuffle_table.c

METLDR2_OBJS := $(addprefix $(METLDR2_OBJ_DIR)/,$(addsuffix .o,$(basename $(METLDR2_UNITS))))

METLDR2_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
METLDR2_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
METLDR2_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
METLDR2_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
METLDR2_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
METLDR2_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

METLDR2_CFLAGS := -Os -ffunction-sections -fdata-sections
METLDR2_CRYPTO_CFLAGS := -O3
METLDR2_DEPFLAGS := -MMD -MP
METLDR2_INCLUDES := -Imetldr2/include -Iappldr/include -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
METLDR2_LV0LDR_INCLUDES := -Ilv0ldr/include
METLDR2_ISOLDR_INCLUDES := -Iisoldr/include -Ilv0ldr/include
METLDR2_LV1LDR_INCLUDES := -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include
METLDR2_LV2LDR_INCLUDES := -Ilv2ldr/include -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include

$(METLDR2_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: metldr2 check-metldr2 clean-metldr2

metldr2: $(METLDR2_ELF) $(METLDR2_BIN)

$(METLDR2_OBJ_DIR)/metldr2/%.o: metldr2/%.c
	@mkdir -p $(@D)
	$(METLDR2_CC) $(METLDR2_CFLAGS) $(METLDR2_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/metldr2/%.o: metldr2/%.cpp
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/metldr2/%.o: metldr2/%.S
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/isoldr/crypto/%.o: isoldr/crypto/%.c
	@mkdir -p $(@D)
	$(METLDR2_CRYPTO_CC) $(METLDR2_CRYPTO_CFLAGS) $(METLDR2_ISOLDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.c
	@mkdir -p $(@D)
	$(METLDR2_CC) $(METLDR2_CFLAGS) $(METLDR2_ISOLDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_ISOLDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/isoldr/%.o: isoldr/%.S
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_ISOLDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(METLDR2_CRYPTO_CC) $(METLDR2_CRYPTO_CFLAGS) $(METLDR2_LV0LDR_INCLUDES) $(UNIT_FLAGS) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(METLDR2_CC) $(METLDR2_CFLAGS) $(METLDR2_LV0LDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_LV0LDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/lv1ldr/%.o: lv1ldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_LV1LDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_OBJ_DIR)/lv2ldr/%.o: lv2ldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR2_CXX) $(METLDR2_CFLAGS) $(METLDR2_LV2LDR_INCLUDES) $(METLDR2_DEPFLAGS) -c $< -o $@

$(METLDR2_ELF): $(METLDR2_OBJS) $(METLDR2_LDSCRIPT)
	$(METLDR2_LD) -T $(METLDR2_LDSCRIPT) --gc-sections -Map $(METLDR2_MAP) -o $@ $(METLDR2_OBJS)

$(METLDR2_BIN): $(METLDR2_ELF)
	$(METLDR2_OBJCOPY) -O binary -j .text -j .rodata -j .data -j .ctors -j .dtors --gap-fill 0 --pad-to $(METLDR2_LOAD_END) $< $@
	truncate -s $$(( $(METLDR2_LOAD_END) - $(METLDR2_LOAD_START) )) $@

check-metldr2: $(METLDR2_BIN) $(METLDR2_ELF)
	@if cmp -s $(METLDR2_IMAGE) $(METLDR2_BIN); then \
		echo "metldr2: $(METLDR2_LOAD_START)-$(METLDR2_LOAD_END) identical to $(METLDR2_IMAGE)"; \
	else \
		echo "metldr2: $(METLDR2_LOAD_START)-$(METLDR2_LOAD_END) DIFFERS from $(METLDR2_IMAGE) (offset from $(METLDR2_LOAD_START) + 1, image byte, built byte):"; \
		cmp -l $(METLDR2_IMAGE) $(METLDR2_BIN) | head -20; \
		echo "$$(cmp -l $(METLDR2_IMAGE) $(METLDR2_BIN) 2>/dev/null | wc -l) bytes differ"; \
		exit 1; \
	fi
	@$(METLDR2_NM) $(METLDR2_ELF) | cut -d' ' -f1,3 > $(METLDR2_OUT)/globals.built
	@if grep -Fxv -f $(METLDR2_OUT)/globals.built $(METLDR2_GLOBALS); then \
		echo "metldr2: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "metldr2: all $$(wc -l < $(METLDR2_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-metldr2:
	rm -rf $(METLDR2_OUT)

-include $(METLDR2_OBJS:.o=.d)
