ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x110 ? 0x110 : .;
    */lv2ldr/src/hash_reader.o(.text._Z12bytes_differPKhS0_i)
    . = . < 0x170 ? 0x170 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processorD1Ev)
    */spu_token_processor/src/token.o(.text._ZN15token_processorD2Ev)
    . = . < 0x178 ? 0x178 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processor10check_idpsEPK8qa_token)
    . = . < 0x1b0 ? 0x1b0 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processor16verify_signatureEPKh)
    . = . < 0x270 ? 0x270 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processor7decryptEPKhP8qa_token)
    . = . < 0x320 ? 0x320 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processorC1EPKhS1_S1_S1_jS1_)
    */spu_token_processor/src/token.o(.text._ZN15token_processorC2EPKhS1_S1_S1_jS1_)
    . = . < 0x390 ? 0x390 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processor6updateEPK13token_requestPh)
    . = . < 0x690 ? 0x690 : .;
    */spu_token_processor/src/token.o(.text._ZN15token_processor6verifyEPK13token_request)
    . = . < 0x7c8 ? 0x7c8 : .;
    */spu_token_processor/src/main.o(.text._Z12read_requestyyP13token_request)
    . = . < 0x8a8 ? 0x8a8 : .;
    */spu_token_processor/src/main.o(.text.main)
    . = . < 0xd48 ? 0xd48 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD1Ev)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD2Ev)
    . = . < 0xd50 ? 0xd50 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC1EP11dma_channel6key128S2_S2_S2_)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC2EP11dma_channel6key128S2_S2_S2_)
    . = . < 0xdd8 ? 0xdd8 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_reader4readERKyRKjS3_S3_RKPhS3_)
    . = . < 0xf98 ? 0xf98 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x1398 ? 0x1398 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x1540 ? 0x1540 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0x16b0 ? 0x16b0 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0x19b0 ? 0x19b0 : .;
    */isoldr/crypto/aes_cmac.o(.text)
    . = . < 0x1bc0 ? 0x1bc0 : .;
    */lv0ldr/crypto/sha1.o(.text)
    . = . < 0x21f8 ? 0x21f8 : .;
    */isoldr/crypto/hmac_sha1_once.o(.text)
    . = . < 0x2298 ? 0x2298 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x23b0 ? 0x23b0 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0x25a0 ? 0x25a0 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0x3040 ? 0x3040 : .;
    */lv0ldr/crypto/sha1_transform.o(.text)
    . = . < 0x3eb8 ? 0x3eb8 : .;
    */lv0ldr/crypto/hmac_sha1.o(.text)
    . = . < 0x4070 ? 0x4070 : .;
    */lv0ldr/crypto/sha1_digest.o(.text)
    . = . < 0x4100 ? 0x4100 : .;
    */lv0ldr/crypto/ecdsa_id.o(.text)
    . = . < 0x4220 ? 0x4220 : .;
    */lv0ldr/crypto/ec_curves.o(.text)
    . = . < 0x4318 ? 0x4318 : .;
    */isoldr/crypto/ecdsa.o(.text)
    . = . < 0x46d0 ? 0x46d0 : .;
    */lv0ldr/crypto/ec.o(.text)
    . = . < 0x5400 ? 0x5400 : .;
    */lv0ldr/crypto/ec_mul2.o(.text)
    . = . < 0x5608 ? 0x5608 : .;
    */lv0ldr/crypto/bn_core.o(.text)
    . = . < 0x8cf0 ? 0x8cf0 : .;
    */lv0ldr/crypto/bn_in_range.o(.text)
    . = . < 0x8d60 ? 0x8d60 : .;
    */lv0ldr/crypto/ec_curve_valid.o(.text)
    . = . < 0x8d70 ? 0x8d70 : .;
    */lv0ldr/crypto/bn_from_bytes_not.o(.text)
    . = . < 0x8e70 ? 0x8e70 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x8e88 ? 0x8e88 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x8f30 ? 0x8f30 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x8f60 ? 0x8f60 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x8f78 ? 0x8f78 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x8f90 ? 0x8f90 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x8f98 ? 0x8f98 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x8fc0 ? 0x8fc0 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x9150 ? 0x9150 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0x92d8 ? 0x92d8 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x93c8 ? 0x93c8 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    */spu_token_processor/src/token.o(.rodata.cst16)
    */spu_token_processor/src/main.o(.rodata.cst16)
    */spu_token_processor/src/main.o(.rodata.g_eid_ls)
    */spu_token_processor/src/main.o(.rodata.g_default_idps)
    */spu_token_processor/src/main.o(.rodata.g_token_key)
    */spu_token_processor/src/main.o(.rodata.g_token_hmac_key)
    */spu_token_processor/src/main.o(.rodata.g_token_pub)
    */spu_token_processor/src/eid0_reader.o(.rodata.eid0_seed_a)
    */spu_token_processor/src/eid0_reader.o(.rodata.eid0_seed_0)
    */crypto/*.o(.rodata .rodata.*)
    *(.rodata._ZTV11dma_channel)
    */lv0ldr/src/util/shuffle_table.o(.rodata.shuffle_table)
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80);
  .ctors : { KEEP(*(.ctors)) KEEP(*(.ctors.end)) }
  .dtors : { KEEP(*(.dtors)) KEEP(*(.dtors.end)) }
  .data :
  {
    */spu_token_processor/src/crt0.o(.data)
    */spu_token_processor/src/main.o(.data.g_token_iv)
    */spu_token_processor/src/main.o(.data.g_token_curve)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
