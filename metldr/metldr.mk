METLDR_OUT := $(BUILD_DIR)/metldr
METLDR_OBJ_DIR := $(METLDR_OUT)/obj
METLDR_ELF := $(METLDR_OUT)/metldr.elf
METLDR_BIN := $(METLDR_OUT)/metldr.bin
METLDR_MAP := $(METLDR_OUT)/metldr.map
METLDR_LDSCRIPT := metldr/metldr.ld
METLDR_GLOBALS := metldr/metldr.globals

METLDR_IMAGE := metldr/image/metldr.bin
METLDR_LOAD_START := 0x400
METLDR_LOAD_START_BYTES := 1024
METLDR_LOAD_END := 0xF160

METLDR_UNITS := \
	metldr/src/entry.S \
	lv0ldr/src/boot/crt0.S \
	metldr/src/loader_global.cpp \
	lv0ldr/src/util/memcpy.c \
	metldr/src/main.cpp \
	metldr/src/loader.cpp \
	metldr/src/authenticator.cpp \
	metldr/src/cipher_aes256_cbc.cpp \
	metldr/src/auth_elf.cpp \
	metldr/src/cipher_aes128_ctr.cpp \
	metldr/src/verifier_ecdsa.cpp \
	metldr/src/auth_transfer.cpp \
	metldr/src/auth_decrypt.cpp \
	metldr/src/elf_reader.cpp \
	metldr/src/hmac_digest.cpp \
	metldr/src/auth_section.cpp \
	lv0ldr/src/util/memcmp.c \
	metldr/src/auth_buffers.cpp \
	lv0ldr/src/dma/dma_queue.cpp \
	lv0ldr/src/dma/dma_buffer.cpp \
	lv0ldr/src/dma/dma_buffer_io.cpp \
	metldr/src/local_buffer.cpp \
	lv0ldr/crypto/aes_modes.c \
	lv0ldr/crypto/sha1_digest.c \
	lv0ldr/crypto/hmac_sha1.c \
	lv0ldr/crypto/aes.c \
	lv0ldr/crypto/sha1.c \
	lv0ldr/crypto/sha1_transform.c \
	lv0ldr/crypto/ecdsa_id.c \
	lv0ldr/crypto/ec_curves.c \
	lv0ldr/crypto/ecdsa.c \
	lv0ldr/crypto/ec.c \
	lv0ldr/crypto/ec_mul2.c \
	lv0ldr/crypto/bn.c \
	lv0ldr/crypto/bn_range.c \
	lv0ldr/crypto/ec_curve_valid.c \
	lv0ldr/crypto/ec_curve_table.c \
	metldr/src/mfc.cpp \
	lv0ldr/src/util/cxx_runtime.cpp \
	lv0ldr/src/boot/exit.cpp \
	lv0ldr/src/util/memset.c \
	lv0ldr/src/util/shuffle_table.c

METLDR_OBJS := $(addprefix $(METLDR_OBJ_DIR)/,$(addsuffix .o,$(basename $(METLDR_UNITS))))

METLDR_CC := $(SPU_GCC402_BIN)/spu-lv2-gcc
METLDR_CXX := $(SPU_GCC402_BIN)/spu-lv2-g++
METLDR_LD := $(SPU_GCC402_BIN)/spu-lv2-ld
METLDR_OBJCOPY := $(SPU_GCC402_BIN)/spu-lv2-objcopy
METLDR_NM := $(SPU_GCC402_BIN)/spu-lv2-nm
METLDR_CRYPTO_CC := $(SPU_GCC341_BIN)/spu-lv2-gcc

METLDR_CFLAGS := -Os -ffunction-sections -fdata-sections
METLDR_CRYPTO_CFLAGS := -O3
METLDR_DEPFLAGS := -MMD -MP
METLDR_INCLUDES := -Imetldr/include -Ilv0ldr/include

$(METLDR_OBJ_DIR)/lv0ldr/crypto/aes_modes.o: UNIT_FLAGS := -fno-aggressive-cmov

.PHONY: metldr check-metldr clean-metldr

metldr: $(METLDR_ELF) $(METLDR_BIN)

$(METLDR_OBJ_DIR)/lv0ldr/crypto/%.o: lv0ldr/crypto/%.c
	@mkdir -p $(@D)
	$(METLDR_CRYPTO_CC) $(METLDR_CRYPTO_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.c
	@mkdir -p $(@D)
	$(METLDR_CC) $(METLDR_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR_CXX) $(METLDR_CFLAGS) $(LV0LDR_INCLUDES) $(UNIT_FLAGS) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_OBJ_DIR)/lv0ldr/%.o: lv0ldr/%.S
	@mkdir -p $(@D)
	$(METLDR_CXX) $(METLDR_CFLAGS) $(LV0LDR_INCLUDES) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_OBJ_DIR)/metldr/%.o: metldr/%.cpp
	@mkdir -p $(@D)
	$(METLDR_CXX) $(METLDR_CFLAGS) $(METLDR_INCLUDES) $(UNIT_FLAGS) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_OBJ_DIR)/metldr/%.o: metldr/%.S
	@mkdir -p $(@D)
	$(METLDR_CXX) $(METLDR_CFLAGS) $(METLDR_INCLUDES) $(METLDR_DEPFLAGS) -c $< -o $@

$(METLDR_ELF): $(METLDR_OBJS) $(METLDR_LDSCRIPT)
	$(METLDR_LD) -T $(METLDR_LDSCRIPT) --gc-sections -Map $(METLDR_MAP) -o $@ $(METLDR_OBJS)

$(METLDR_BIN): $(METLDR_ELF)
	$(METLDR_OBJCOPY) -O binary -j .text -j .rodata -j .data -j .ctors -j .dtors --gap-fill 0 --pad-to $(METLDR_LOAD_END) $< $@
	truncate -s $$(( $(METLDR_LOAD_END) - $(METLDR_LOAD_START) )) $@

check-metldr: $(METLDR_BIN) $(METLDR_ELF)
	@if cmp -s -i $(METLDR_LOAD_START_BYTES):0 $(METLDR_IMAGE) $(METLDR_BIN); then \
		echo "metldr: $(METLDR_LOAD_START)-$(METLDR_LOAD_END) identical to $(METLDR_IMAGE)"; \
	else \
		echo "metldr: $(METLDR_LOAD_START)-$(METLDR_LOAD_END) DIFFERS from $(METLDR_IMAGE):"; \
		cmp -i $(METLDR_LOAD_START_BYTES):0 $(METLDR_IMAGE) $(METLDR_BIN); \
		echo "$$(cmp -l -i $(METLDR_LOAD_START_BYTES):0 $(METLDR_IMAGE) $(METLDR_BIN) 2>/dev/null | wc -l) bytes differ"; \
		exit 1; \
	fi
	@$(METLDR_NM) $(METLDR_ELF) | cut -d' ' -f1,3 > $(METLDR_OUT)/globals.built
	@if grep -Fxv -f $(METLDR_OUT)/globals.built $(METLDR_GLOBALS); then \
		echo "metldr: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "metldr: all $$(wc -l < $(METLDR_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-metldr:
	rm -rf $(METLDR_OUT)

-include $(METLDR_OBJS:.o=.d)
