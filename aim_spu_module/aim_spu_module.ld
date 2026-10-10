ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    KEEP(*/aim_spu_module/src/salt.o(.text.salt))
    . = . < 0x10 ? 0x10 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x120 ? 0x120 : .;
    */aim_spu_module/src/main.o(.text._Z11is_valid_eay)
    . = . < 0x160 ? 0x160 : .;
    */aim_spu_module/src/main.o(.text._Z13stop_on_errorj)
    . = . < 0x198 ? 0x198 : .;
    */aim_spu_module/src/main.o(.text.main)
    . = . < 0x4c8 ? 0x4c8 : .;
    */lv2ldr/src/hash_reader.o(.text._Z12bytes_differPKhS0_i)
    . = . < 0x528 ? 0x528 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD1Ev)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD2Ev)
    . = . < 0x530 ? 0x530 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC1EP11dma_channel6key128S2_S2_S2_)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC2EP11dma_channel6key128S2_S2_S2_)
    . = . < 0x5b8 ? 0x5b8 : .;
    */aim_spu_module/src/aim_reader.o(.text.*read_id_type*)
    . = . < 0x678 ? 0x678 : .;
    */aim_spu_module/src/aim_reader.o(.text.*put_id*)
    . = . < 0x878 ? 0x878 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_reader4readERKyRKjS3_S3_RKPhS3_)
    . = . < 0xa38 ? 0xa38 : .;
    */aim_spu_module/src/dma_log.o(.text._Z7get_logv)
    . = . < 0xa40 ? 0xa40 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log4putsEPKc)
    . = . < 0xbb8 ? 0xbb8 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log5resetEv)
    . = . < 0xc18 ? 0xc18 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log10set_bufferERKyRKj)
    . = . < 0xc30 ? 0xc30 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log4initERKyRKj)
    . = . < 0xda0 ? 0xda0 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logD1Ev)
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logD2Ev)
    . = . < 0xda8 ? 0xda8 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logC1Ev)
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logC2Ev)
    . = . < 0xdf0 ? 0xdf0 : .;
    */aim_spu_module/src/dma_log.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xe28 ? 0xe28 : .;
    */aim_spu_module/src/dma_log.o(.text._GLOBAL__D_g_log_ptr)
    . = . < 0xe38 ? 0xe38 : .;
    */aim_spu_module/src/dma_log.o(.text._GLOBAL__I_g_log_ptr)
    . = . < 0xe48 ? 0xe48 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x1248 ? 0x1248 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x13f0 ? 0x13f0 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0x16f0 ? 0x16f0 : .;
    */isoldr/crypto/aes_cmac.o(.text)
    . = . < 0x1900 ? 0x1900 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x1a18 ? 0x1a18 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0x1c08 ? 0x1c08 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0x26a8 ? 0x26a8 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x26c0 ? 0x26c0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x2768 ? 0x2768 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x2798 ? 0x2798 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x27b0 ? 0x27b0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x27c8 ? 0x27c8 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x27d0 ? 0x27d0 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x27f8 ? 0x27f8 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x2988 ? 0x2988 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0x2b10 ? 0x2b10 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x2c00 ? 0x2c00 : .;
    */aim_spu_module/src/runtime/strlen.o(.text.strlen)
    . = . < 0x2c78 ? 0x2c78 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    */aim_spu_module/src/main.o(.rodata.str1.16)
    */spu_token_processor/src/eid0_reader.o(.rodata.eid0_seed_a)
    */spu_token_processor/src/eid0_reader.o(.rodata.eid0_seed_0)
    */aim_spu_module/src/dma_log.o(.rodata.cst16)
    */aim_spu_module/src/dma_log.o(.rodata.str1.16)
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
    */aim_spu_module/src/dma_log.o(.data.g_log_ptr)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */aim_spu_module/src/dma_log.o(.bss.g_log)
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
