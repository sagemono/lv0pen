SC_ISO_OUT := $(BUILD_DIR)/sc_iso
SC_ISO_OBJ_DIR := $(SC_ISO_OUT)/obj
SC_ISO_ELF := $(SC_ISO_OUT)/sc_iso.elf
SC_ISO_BIN := $(SC_ISO_OUT)/sc_iso.bin
SC_ISO_MAP := $(SC_ISO_OUT)/sc_iso.map
SC_ISO_LDSCRIPT := sc_iso/sc_iso.ld
SC_ISO_GLOBALS := sc_iso/sc_iso.globals
SC_ISO_UNMATCHED := sc_iso/sc_iso.unmatched

SC_ISO_IMAGE := sc_iso/image/sc_iso.elf
SC_ISO_IMAGE_BIN := $(SC_ISO_OUT)/image.bin
SC_ISO_LOAD_START := 0x880
SC_ISO_LOAD_END := 0x1C810

SC_ISO_OBJS := \
	$(SC_ISO_OBJ_DIR)/spu_token_processor/src/crt0.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/main.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_crypto.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_iso_module.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_proxy_hdr.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_08.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0d.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_07.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_09.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0b.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_05.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0c.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_04.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/ss_iso_dma_data.o \
	$(SC_ISO_OBJ_DIR)/aim_spu_module/src/dma_log.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_pkt_hdr.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_set_encrypt_key.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_set_decrypt_key.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_encrypt_block.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_decrypt_block.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_cbc_encrypt.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_cbc_decrypt.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_ctr.o \
	$(SC_ISO_OBJ_DIR)/isoldr/crypto/aes_cmac.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1_digest.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_encrypt8.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_decrypt8.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1_transform.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ecdsa_id.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curves.o \
	$(SC_ISO_OBJ_DIR)/isoldr/crypto/ecdsa.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_mul2.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_core.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_in_range.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curve_valid.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_from_bytes_not.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curve_table.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/dma_channel.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/dma_mailbox.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/cxa_pure_virtual.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/secure_com/secure_com.o \
	$(SC_ISO_OBJ_DIR)/sc_iso/src/secure_com/secure_com_lib.o \
	$(SC_ISO_OBJ_DIR)/lv1ldr/src/storage.o \
	$(SC_ISO_OBJ_DIR)/lv1ldr/crypto/prng.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/delete.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/exit.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/free.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/heap.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/src/util/memcmp.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/memset.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/strlen.o \
	$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/atexit.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/libgcc/udivdi3.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/libgcc/umoddi3.o \
	$(SC_ISO_OBJ_DIR)/lv0ldr/src/util/shuffle_table.o

SC_ISO_LD := $(SPU_GCC411_SDK420_BIN)/spu-lv2-ld
SC_ISO_OBJCOPY := $(SPU_GCC411_SDK420_BIN)/spu-lv2-objcopy
SC_ISO_NM := $(SPU_GCC411_SDK420_BIN)/spu-lv2-nm
SC_ISO_READELF := $(SPU_GCC411_SDK420_BIN)/spu-lv2-readelf
SC_ISO_DEPFLAGS := -MMD -MP

# The secure-communication library (src/secure_com/) and the PRNG glue
# linked with it (lv1ldr's storage.cpp) are built by the CELL 4.1.2.6 Beta
# compiler, whose preprocessor needs all of a unit's headers in one
# directory: the toolchain's, the shared homes', the unit's home's, this
# module's, lv1ldr's encdec/sys/ and the unit's own directory, later ones
# taking the place of earlier ones.
SC_ISO_BETA_SYS_HEADERS := $(wildcard $(SPU_GCC411_BETA_BIN)/../lib/gcc/spu-lv2/4.1.1/include/*.h) \
	$(wildcard $(SPU_GCC411_BETA_BIN)/../../../target/spu/include/*.h)
SC_ISO_BETA_SHARED_HEADERS := $(wildcard lv0ldr/include/*.h) $(wildcard isoldr/include/*.h) \
	$(wildcard spp_verifier/include/*.h) $(wildcard spu_token_processor/include/*.h) \
	$(wildcard aim_spu_module/include/*.h)
SC_ISO_BETA_SYS_DIRS := $(SPU_GCC411_BETA_BIN)/../lib/gcc/spu-lv2/4.1.1/include $(SPU_GCC411_BETA_BIN)/../../../target/spu/include
SC_ISO_BETA_SHARED_DIRS := lv0ldr/include isoldr/include spp_verifier/include spu_token_processor/include aim_spu_module/include
SC_ISO_SECURE_COM_INCLUDE_DIR := $(SC_ISO_OBJ_DIR)/secure_com_include
SC_ISO_STORAGE_INCLUDE_DIR := $(SC_ISO_OBJ_DIR)/storage_include

# libgcc2.c's members: this compiler's code for them is not always the
# same, so each is compiled again (with an unused define one character
# longer each time) until its .text is the image's
SC_ISO_LIBGCC_ATTEMPTS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
SC_ISO_LIBGCC_TEXT_MD5_udivdi3 := f8c44df433ffc1a66dbd5ca4a0a63475
SC_ISO_LIBGCC_TEXT_MD5_umoddi3 := 8711d716069e15311f73103fe9d98eb2

.PHONY: sc_iso check-sc_iso clean-sc_iso

sc_iso: $(SC_ISO_ELF) $(SC_ISO_BIN)

$(SC_ISO_SECURE_COM_INCLUDE_DIR): $(SC_ISO_BETA_SYS_HEADERS) $(SC_ISO_BETA_SHARED_HEADERS) $(wildcard sc_iso/include/*.h) $(wildcard lv1ldr/encdec/sys/*.h) $(wildcard sc_iso/src/secure_com/*.h)
	rm -rf $@
	mkdir -p $@
	for d in $(SC_ISO_BETA_SYS_DIRS) $(SC_ISO_BETA_SHARED_DIRS) sc_iso/include lv1ldr/encdec/sys sc_iso/src/secure_com; do 		for f in $$d/*.h; do if [ -f "$$f" ]; then cp -f "$$f" $@/ || exit 1; fi; done; 	done

$(SC_ISO_STORAGE_INCLUDE_DIR): $(SC_ISO_BETA_SYS_HEADERS) $(SC_ISO_BETA_SHARED_HEADERS) $(wildcard lv1ldr/include/*.h) $(wildcard sc_iso/include/*.h) $(wildcard lv1ldr/encdec/sys/*.h)
	rm -rf $@
	mkdir -p $@
	for d in $(SC_ISO_BETA_SYS_DIRS) $(SC_ISO_BETA_SHARED_DIRS) lv1ldr/include sc_iso/include lv1ldr/encdec/sys lv1ldr/src; do 		for f in $$d/*.h; do if [ -f "$$f" ]; then cp -f "$$f" $@/ || exit 1; fi; done; 	done

$(SC_ISO_OBJ_DIR)/spu_token_processor/src/crt0.o: spu_token_processor/src/crt0.S
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Ispu_token_processor/include -Ispp_verifier/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/main.o: sc_iso/src/main.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_crypto.o: sc_iso/src/sc_crypto.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_iso_module.o: sc_iso/src/sc_iso_module.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_proxy_hdr.o: sc_iso/src/sc_proxy_hdr.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd.o: sc_iso/src/sc_cmd.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_08.o: sc_iso/src/sc_cmd_08.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0d.o: sc_iso/src/sc_cmd_0d.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_07.o: sc_iso/src/sc_cmd_07.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_09.o: sc_iso/src/sc_cmd_09.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0b.o: sc_iso/src/sc_cmd_0b.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_05.o: sc_iso/src/sc_cmd_05.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_0c.o: sc_iso/src/sc_cmd_0c.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_cmd_04.o: sc_iso/src/sc_cmd_04.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/ss_iso_dma_data.o: sc_iso/src/ss_iso_dma_data.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/aim_spu_module/src/dma_log.o: aim_spu_module/src/dma_log.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/sc_pkt_hdr.o: sc_iso/src/sc_pkt_hdr.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_set_encrypt_key.o: lv0ldr/crypto/aes_set_encrypt_key.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_set_decrypt_key.o: lv0ldr/crypto/aes_set_decrypt_key.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_encrypt_block.o: lv0ldr/crypto/aes_encrypt_block.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_decrypt_block.o: lv0ldr/crypto/aes_decrypt_block.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_cbc_encrypt.o: lv0ldr/crypto/aes_cbc_encrypt.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_cbc_decrypt.o: lv0ldr/crypto/aes_cbc_decrypt.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_ctr.o: lv0ldr/crypto/aes_ctr.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include -fno-aggressive-cmov $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/isoldr/crypto/aes_cmac.o: isoldr/crypto/aes_cmac.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1_digest.o: lv0ldr/crypto/sha1_digest.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_encrypt8.o: lv0ldr/crypto/aes_encrypt8.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/aes_decrypt8.o: lv0ldr/crypto/aes_decrypt8.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1.o: lv0ldr/crypto/sha1.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/sha1_transform.o: lv0ldr/crypto/sha1_transform.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ecdsa_id.o: lv0ldr/crypto/ecdsa_id.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curves.o: lv0ldr/crypto/ec_curves.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/isoldr/crypto/ecdsa.o: isoldr/crypto/ecdsa.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec.o: lv0ldr/crypto/ec.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_mul2.o: lv0ldr/crypto/ec_mul2.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_core.o: lv0ldr/crypto/bn_core.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_in_range.o: lv0ldr/crypto/bn_in_range.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curve_valid.o: lv0ldr/crypto/ec_curve_valid.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/bn_from_bytes_not.o: lv0ldr/crypto/bn_from_bytes_not.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/crypto/ec_curve_table.o: lv0ldr/crypto/ec_curve_table.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/dma_channel.o: spp_verifier/src/runtime/dma_channel.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/dma_mailbox.o: spp_verifier/src/runtime/dma_mailbox.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/cxa_pure_virtual.o: sc_iso/src/cxa_pure_virtual.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Isc_iso/include -Iaim_spu_module/include -Ispu_token_processor/include -Ispp_verifier/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/secure_com/secure_com.o: sc_iso/src/secure_com/secure_com.cpp $(SC_ISO_SECURE_COM_INCLUDE_DIR)
	@mkdir -p $(@D)
	$(SPU_GCC411_BETA_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -nostdinc -I$(SC_ISO_SECURE_COM_INCLUDE_DIR) $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/sc_iso/src/secure_com/secure_com_lib.o: sc_iso/src/secure_com/secure_com_lib.cpp $(SC_ISO_SECURE_COM_INCLUDE_DIR)
	@mkdir -p $(@D)
	$(SPU_GCC411_BETA_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -nostdinc -I$(SC_ISO_SECURE_COM_INCLUDE_DIR) $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv1ldr/src/storage.o: lv1ldr/src/storage.cpp $(SC_ISO_STORAGE_INCLUDE_DIR)
	@mkdir -p $(@D)
	$(SPU_GCC411_BETA_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -nostdinc -I$(SC_ISO_STORAGE_INCLUDE_DIR) $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv1ldr/crypto/prng.o: lv1ldr/crypto/prng.c
	@mkdir -p $(@D)
	$(SPU_GCC341_BIN)/spu-lv2-gcc -O3 -Ilv1ldr/include -Iisoldr/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/delete.o: spp_verifier/src/runtime/delete.cpp
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-g++ -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/exit.o: spp_verifier/src/runtime/exit.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/free.o: spp_verifier/src/runtime/free.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/heap.o: spp_verifier/src/runtime/heap.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/src/util/memcmp.o: lv0ldr/src/util/memcmp.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/memcpy.o: spp_verifier/src/runtime/memcpy.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/memset.o: spp_verifier/src/runtime/memset.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/strlen.o: spp_verifier/src/runtime/strlen.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/spp_verifier/src/runtime/atexit.o: spp_verifier/src/runtime/atexit.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ispp_verifier/include -Ilv0ldr/include -O2 $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_OBJ_DIR)/lv0ldr/libgcc/udivdi3.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(SC_ISO_LIBGCC_ATTEMPTS); do \
		$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions -D__LIBGCC2_PAD=$$pad -DL_udivdi3 -c $< -o $@ || exit 1; \
		$(SC_ISO_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(SC_ISO_LIBGCC_TEXT_MD5_udivdi3)" ]; then rm -f $@.text; exit 0; fi; \
		pad=$${pad}x; \
	done; \
	echo "sc_iso: __udivdi3 never came out as expected"; rm -f $@ $@.text; exit 1

$(SC_ISO_OBJ_DIR)/lv0ldr/libgcc/umoddi3.o: lv0ldr/libgcc/libgcc2.c
	@mkdir -p $(@D)
	@pad=; \
	for attempt in $(SC_ISO_LIBGCC_ATTEMPTS); do \
		$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -O2 -fPIC -mwarn-reloc -D__IN_LIBGCC2 -fexceptions -fnon-call-exceptions -D__LIBGCC2_PAD=$$pad -DL_umoddi3 -c $< -o $@ || exit 1; \
		$(SC_ISO_OBJCOPY) -O binary -j .text $@ $@.text || exit 1; \
		if [ "$$(md5sum < $@.text | cut -c1-32)" = "$(SC_ISO_LIBGCC_TEXT_MD5_umoddi3)" ]; then rm -f $@.text; exit 0; fi; \
		pad=$${pad}x; \
	done; \
	echo "sc_iso: __umoddi3 never came out as expected"; rm -f $@ $@.text; exit 1

$(SC_ISO_OBJ_DIR)/lv0ldr/src/util/shuffle_table.o: lv0ldr/src/util/shuffle_table.c
	@mkdir -p $(@D)
	$(SPU_GCC411_SDK420_BIN)/spu-lv2-gcc -Os -ffunction-sections -fdata-sections -Ilv0ldr/include $(SC_ISO_DEPFLAGS) -c $< -o $@

$(SC_ISO_ELF): $(SC_ISO_OBJS) $(SC_ISO_LDSCRIPT)
	$(SC_ISO_LD) -T $(SC_ISO_LDSCRIPT) --gc-sections -Map $(SC_ISO_MAP) -o $@ $(SC_ISO_OBJS)

$(SC_ISO_BIN): $(SC_ISO_ELF)
	$(SC_ISO_OBJCOPY) -O binary $< $@

$(SC_ISO_IMAGE_BIN): $(SC_ISO_IMAGE)
	@mkdir -p $(@D)
	$(SC_ISO_OBJCOPY) -O binary $< $@

check-sc_iso: $(SC_ISO_BIN) $(SC_ISO_IMAGE_BIN) $(SC_ISO_ELF)
	@$(SC_ISO_READELF) -lW $(SC_ISO_IMAGE) | grep LOAD > $(SC_ISO_OUT)/segments.image
	@$(SC_ISO_READELF) -lW $(SC_ISO_ELF) | grep LOAD > $(SC_ISO_OUT)/segments.built
	@if cmp -s $(SC_ISO_OUT)/segments.image $(SC_ISO_OUT)/segments.built; then \
		echo "sc_iso: program headers identical to $(SC_ISO_IMAGE) (2 PT_LOADs, offsets, addresses, file and memory sizes, flags)"; \
	else \
		echo "sc_iso: program headers DIFFER from $(SC_ISO_IMAGE) (image, then build):"; \
		cat $(SC_ISO_OUT)/segments.image $(SC_ISO_OUT)/segments.built; \
		exit 1; \
	fi
	@cmp -l $(SC_ISO_IMAGE_BIN) $(SC_ISO_BIN) > $(SC_ISO_OUT)/differing.raw; \
	if [ $$? -gt 1 ]; then exit 1; fi; \
	tr -s ' ' < $(SC_ISO_OUT)/differing.raw | sed 's/^ //' > $(SC_ISO_OUT)/differing.built; \
	if [ ! -s $(SC_ISO_OUT)/differing.built ]; then \
		echo "sc_iso: $(SC_ISO_LOAD_START)-$(SC_ISO_LOAD_END) identical to $(SC_ISO_IMAGE)"; \
	elif cmp -s $(SC_ISO_OUT)/differing.built $(SC_ISO_UNMATCHED); then \
		echo "sc_iso: $(SC_ISO_LOAD_START)-$(SC_ISO_LOAD_END) identical to $(SC_ISO_IMAGE) but for the $$(wc -l < $(SC_ISO_UNMATCHED)) bytes of the functions that do not match yet ($(SC_ISO_UNMATCHED))"; \
	else \
		echo "sc_iso: $(SC_ISO_LOAD_START)-$(SC_ISO_LOAD_END) DIFFERS from $(SC_ISO_IMAGE) (offset from $(SC_ISO_LOAD_START) + 1, image byte, built byte):"; \
		diff $(SC_ISO_UNMATCHED) $(SC_ISO_OUT)/differing.built | grep '^[<>]' | head -20; \
		echo "$$(wc -l < $(SC_ISO_OUT)/differing.built) bytes differ, $$(wc -l < $(SC_ISO_UNMATCHED)) of them expected ($(SC_ISO_UNMATCHED))"; \
		exit 1; \
	fi
	@$(SC_ISO_NM) $(SC_ISO_ELF) | cut -d' ' -f1,3 > $(SC_ISO_OUT)/globals.built
	@if grep -Fxv -f $(SC_ISO_OUT)/globals.built $(SC_ISO_GLOBALS); then \
		echo "sc_iso: the globals above are not at the image's addresses"; \
		exit 1; \
	else \
		echo "sc_iso: all $$(wc -l < $(SC_ISO_GLOBALS)) globals at the image's addresses (.rodata, .data, .bss)"; \
	fi

clean-sc_iso:
	rm -rf $(SC_ISO_OUT)

-include $(SC_ISO_OBJS:.o=.d)
