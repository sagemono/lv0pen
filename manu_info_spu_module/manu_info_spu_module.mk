MANU_INFO_SPU_MODULE_OUT := $(BUILD_DIR)/manu_info_spu_module
MANU_INFO_SPU_MODULE_OBJ_DIR := $(MANU_INFO_SPU_MODULE_OUT)/obj
MANU_INFO_SPU_MODULE_ELF := $(MANU_INFO_SPU_MODULE_OUT)/manu_info_spu_module.elf
MANU_INFO_SPU_MODULE_BIN := $(MANU_INFO_SPU_MODULE_OUT)/manu_info_spu_module.bin
MANU_INFO_SPU_MODULE_MAP := $(MANU_INFO_SPU_MODULE_OUT)/manu_info_spu_module.map
MANU_INFO_SPU_MODULE_LDSCRIPT := manu_info_spu_module/manu_info_spu_module.ld
MANU_INFO_SPU_MODULE_GLOBALS := manu_info_spu_module/manu_info_spu_module.globals

MANU_INFO_SPU_MODULE_IMAGE := manu_info_spu_module/image/manu_info_spu_module.elf
MANU_INFO_SPU_MODULE_IMAGE_BIN := $(MANU_INFO_SPU_MODULE_OUT)/image.bin
MANU_INFO_SPU_MODULE_LOAD_START := 0x880
MANU_INFO_SPU_MODULE_LOAD_END := 0x1020

MANU_INFO_SPU_MODULE_UNITS := manu_info_spu_module/src/salt.S spu_token_processor/src/crt0.S manu_info_spu_module/src/main.cpp spp_verifier/src/runtime/dma_channel.cpp spp_verifier/src/runtime/delete.cpp spp_verifier/src/runtime/exit.c spp_verifier/src/runtime/free.c spp_verifier/src/runtime/heap.c spp_verifier/src/runtime/memset.c spp_verifier/src/runtime/atexit.c

MANU_INFO_SPU_MODULE_OBJS := $(addprefix $(MANU_INFO_SPU_MODULE_OBJ_DIR)/,$(addsuffix .o,$(basename $(MANU_INFO_SPU_MODULE_UNITS))))

MANU_INFO_SPU_MODULE_CC := $(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc
MANU_INFO_SPU_MODULE_CXX := $(SPU_GCC411_SDK420_BIN)/spu-lv2-g++
MANU_INFO_SPU_MODULE_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
MANU_INFO_SPU_MODULE_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
MANU_INFO_SPU_MODULE_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
MANU_INFO_SPU_MODULE_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf

MANU_INFO_SPU_MODULE_CFLAGS := -Os -ffunction-sections -fdata-sections
MANU_INFO_SPU_MODULE_DEPFLAGS := -MMD -MP
MANU_INFO_SPU_MODULE_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include
MANU_INFO_SPU_MODULE_SPP_VERIFIER_INCLUDES := -Ispp_verifier/include -Ilv0ldr/include
MANU_INFO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES := -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/exit.o: UNIT_FLAGS := -O2
$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/free.o: UNIT_FLAGS := -O2
$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/memset.o: UNIT_FLAGS := -O2
$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: UNIT_FLAGS := -O2

.PHONY: manu_info_spu_module check-manu_info_spu_module clean-manu_info_spu_module

manu_info_spu_module: $(MANU_INFO_SPU_MODULE_ELF) $(MANU_INFO_SPU_MODULE_BIN)

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/manu_info_spu_module/%.o: manu_info_spu_module/%.cpp
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CXX) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_INCLUDES) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/manu_info_spu_module/%.o: manu_info_spu_module/%.S
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CXX) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_INCLUDES) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/manu_info_spu_module/%.o: manu_info_spu_module/%.c
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CC) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_INCLUDES) $(UNIT_FLAGS) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.cpp
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CXX) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spu_token_processor/%.o: spu_token_processor/%.S
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CXX) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_SPU_TOKEN_PROCESSOR_INCLUDES) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.cpp
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CXX) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_OBJ_DIR)/spp_verifier/%.o: spp_verifier/%.c
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_CC) $(MANU_INFO_SPU_MODULE_CFLAGS) $(MANU_INFO_SPU_MODULE_SPP_VERIFIER_INCLUDES) $(UNIT_FLAGS) $(MANU_INFO_SPU_MODULE_DEPFLAGS) -c $< -o $@

$(MANU_INFO_SPU_MODULE_ELF): $(MANU_INFO_SPU_MODULE_OBJS) $(MANU_INFO_SPU_MODULE_LDSCRIPT)
	$(MANU_INFO_SPU_MODULE_LD) -T $(MANU_INFO_SPU_MODULE_LDSCRIPT) --gc-sections -Map $(MANU_INFO_SPU_MODULE_MAP) -o $@ $(MANU_INFO_SPU_MODULE_OBJS)

$(MANU_INFO_SPU_MODULE_BIN): $(MANU_INFO_SPU_MODULE_ELF)
	$(MANU_INFO_SPU_MODULE_OBJCOPY) -O binary $< $@

$(MANU_INFO_SPU_MODULE_IMAGE_BIN): $(MANU_INFO_SPU_MODULE_IMAGE)
	@mkdir -p $(@D)
	$(MANU_INFO_SPU_MODULE_OBJCOPY) -O binary $< $@

check-manu_info_spu_module: $(MANU_INFO_SPU_MODULE_BIN) $(MANU_INFO_SPU_MODULE_IMAGE_BIN) $(MANU_INFO_SPU_MODULE_ELF)
	@$(MANU_INFO_SPU_MODULE_READELF) -lW $(MANU_INFO_SPU_MODULE_IMAGE) | grep LOAD > $(MANU_INFO_SPU_MODULE_OUT)/segments.image
	@$(MANU_INFO_SPU_MODULE_READELF) -lW $(MANU_INFO_SPU_MODULE_ELF) | grep LOAD > $(MANU_INFO_SPU_MODULE_OUT)/segments.built
	@if cmp -s $(MANU_INFO_SPU_MODULE_OUT)/segments.image $(MANU_INFO_SPU_MODULE_OUT)/segments.built; then \
		echo "manu_info_spu_module: program headers identical to $(MANU_INFO_SPU_MODULE_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "manu_info_spu_module: program headers DIFFER from $(MANU_INFO_SPU_MODULE_IMAGE) (image, then build):"; \
		cat $(MANU_INFO_SPU_MODULE_OUT)/segments.image $(MANU_INFO_SPU_MODULE_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(MANU_INFO_SPU_MODULE_IMAGE_BIN) $(MANU_INFO_SPU_MODULE_BIN) > $(MANU_INFO_SPU_MODULE_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(MANU_INFO_SPU_MODULE_OUT)/differing.raw | sed 's/^ //' > $(MANU_INFO_SPU_MODULE_OUT)/differing.built; \
	if [ ! -s $(MANU_INFO_SPU_MODULE_OUT)/differing.built ]; then \
		echo "manu_info_spu_module: $(MANU_INFO_SPU_MODULE_LOAD_START)-$(MANU_INFO_SPU_MODULE_LOAD_END) identical to $(MANU_INFO_SPU_MODULE_IMAGE)"; \
	else \
		echo "manu_info_spu_module: $(MANU_INFO_SPU_MODULE_LOAD_START)-$(MANU_INFO_SPU_MODULE_LOAD_END) DIFFERS from $(MANU_INFO_SPU_MODULE_IMAGE) (offset from $(MANU_INFO_SPU_MODULE_LOAD_START) + 1, image byte, built byte):"; \
		head -20 $(MANU_INFO_SPU_MODULE_OUT)/differing.built; \
		echo "$$(wc -l < $(MANU_INFO_SPU_MODULE_OUT)/differing.built) bytes differ"; \
		exit 1; \
	fi
	@$(MANU_INFO_SPU_MODULE_NM) $(MANU_INFO_SPU_MODULE_ELF) | cut -d' ' -f1,3 > $(MANU_INFO_SPU_MODULE_OUT)/globals.built
	@if grep -Fxv -f $(MANU_INFO_SPU_MODULE_OUT)/globals.built $(MANU_INFO_SPU_MODULE_GLOBALS); then \
		echo "manu_info_spu_module: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "manu_info_spu_module: all $$(wc -l < $(MANU_INFO_SPU_MODULE_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-manu_info_spu_module:
	rm -rf $(MANU_INFO_SPU_MODULE_OUT)

-include $(MANU_INFO_SPU_MODULE_OBJS:.o=.d)
