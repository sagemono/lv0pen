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
    */spu_utoken_processor/src/utoken.o(.text._ZN11read_cursorC1Ev)
    . = . < 0x1a8 ? 0x1a8 : .;
    *(.text._ZN11read_cursor3setEy)
    . = . < 0x1c8 ? 0x1c8 : .;
    *(.text._ZN11read_cursor5startEv)
    . = . < 0x1d0 ? 0x1d0 : .;
    *(.text._ZN11read_cursor8set_sizeEj)
    . = . < 0x1e8 ? 0x1e8 : .;
    *(.text._ZN11read_cursor7advanceEj)
    . = . < 0x1f8 ? 0x1f8 : .;
    *(.text._ZN11read_cursor7currentEv)
    . = . < 0x218 ? 0x218 : .;
    *(.text._ZN11read_cursor11line_offsetEv)
    . = . < 0x250 ? 0x250 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN12write_cursorC1Ev)
    . = . < 0x288 ? 0x288 : .;
    *(.text._ZN12write_cursor3setEy)
    . = . < 0x298 ? 0x298 : .;
    *(.text._ZN12write_cursor5startEv)
    . = . < 0x2a0 ? 0x2a0 : .;
    *(.text._ZN12write_cursor8set_sizeEj)
    . = . < 0x2b8 ? 0x2b8 : .;
    *(.text._ZN12write_cursor7advanceEj)
    . = . < 0x2c8 ? 0x2c8 : .;
    *(.text._ZN12write_cursor7currentEv)
    . = . < 0x2e8 ? 0x2e8 : .;
    *(.text._ZN12write_cursor11line_offsetEv)
    . = . < 0x320 ? 0x320 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN13double_bufferC1Ejj)
    . = . < 0x358 ? 0x358 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utokenD1Ev)
    */spu_utoken_processor/src/utoken.o(.text._ZN6utokenD2Ev)
    . = . < 0x360 ? 0x360 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken12check_headerEPKhy)
    . = . < 0x578 ? 0x578 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken10check_idpsEPKh)
    . = . < 0x610 ? 0x610 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN13double_bufferD0Ev)
    . = . < 0x628 ? 0x628 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN13double_bufferD1Ev)
    . = . < 0x640 ? 0x640 : .;
    *(.text._ZN12write_cursorD0Ev)
    . = . < 0x658 ? 0x658 : .;
    *(.text._ZN12write_cursorD1Ev)
    . = . < 0x670 ? 0x670 : .;
    *(.text._ZN11read_cursorD0Ev)
    . = . < 0x688 ? 0x688 : .;
    *(.text._ZN11read_cursorD1Ev)
    . = . < 0x6a0 ? 0x6a0 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN10dma_writerD0Ev)
    . = . < 0x6f0 ? 0x6f0 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN10dma_writerD1Ev)
    . = . < 0x738 ? 0x738 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN10dma_readerD1Ev)
    . = . < 0x768 ? 0x768 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN10dma_writerC1EjjjP9ea_cursor)
    . = . < 0x818 ? 0x818 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN10dma_readerC1EjjjP9ea_cursor)
    . = . < 0x8a8 ? 0x8a8 : .;
    */spu_utoken_processor/src/utoken.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x958 ? 0x958 : .;
    */spu_utoken_processor/src/utoken.o(.text._GLOBAL__D_g_read_cursor)
    . = . < 0x968 ? 0x968 : .;
    */spu_utoken_processor/src/utoken.o(.text._GLOBAL__I_g_read_cursor)
    . = . < 0x978 ? 0x978 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utokenC1EPKhPhS1_S1_)
    */spu_utoken_processor/src/utoken.o(.text._ZN6utokenC2EPKhPhS1_S1_)
    . = . < 0x9c8 ? 0x9c8 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken3putEPK13utoken_paramsPKh)
    . = . < 0xb20 ? 0xb20 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken3getEPK13utoken_paramsPh)
    . = . < 0xc70 ? 0xc70 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken4sealEPK13utoken_paramsPh)
    . = . < 0xd88 ? 0xd88 : .;
    */spu_utoken_processor/src/utoken.o(.text._ZN6utoken4openEPK13utoken_paramsPh)
    . = . < 0xec8 ? 0xec8 : .;
    *(.text._ZN12write_cursor5chunkEj)
    . = . < 0xf30 ? 0xf30 : .;
    *(.text._ZN11read_cursor5chunkEj)
    . = . < 0xf98 ? 0xf98 : .;
    */spu_utoken_processor/src/main.o(.text._Z11read_paramsyyP13utoken_params)
    . = . < 0x1078 ? 0x1078 : .;
    */spu_utoken_processor/src/main.o(.text.main)
    . = . < 0x1478 ? 0x1478 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD1Ev)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerD2Ev)
    . = . < 0x1480 ? 0x1480 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC1EP11dma_channel6key128S2_S2_S2_)
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_readerC2EP11dma_channel6key128S2_S2_S2_)
    . = . < 0x1508 ? 0x1508 : .;
    */spu_token_processor/src/eid0_reader.o(.text._ZN11eid0_reader4readERKyRKjS3_S3_RKPhS3_)
    . = . < 0x16c8 ? 0x16c8 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x1ac8 ? 0x1ac8 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x1c70 ? 0x1c70 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0x1de0 ? 0x1de0 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0x20e0 ? 0x20e0 : .;
    */isoldr/crypto/aes_cmac.o(.text)
    . = . < 0x22f0 ? 0x22f0 : .;
    */isoldr/crypto/hmac_sha1_once.o(.text)
    . = . < 0x2390 ? 0x2390 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x24a8 ? 0x24a8 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0x2698 ? 0x2698 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0x3138 ? 0x3138 : .;
    */lv0ldr/crypto/hmac_sha1.o(.text)
    . = . < 0x32f0 ? 0x32f0 : .;
    */lv0ldr/crypto/sha1.o(.text)
    . = . < 0x3928 ? 0x3928 : .;
    */lv0ldr/crypto/sha1_digest.o(.text)
    . = . < 0x39b8 ? 0x39b8 : .;
    */lv0ldr/crypto/sha1_transform.o(.text)
    . = . < 0x4830 ? 0x4830 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x4848 ? 0x4848 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x48f0 ? 0x48f0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x4920 ? 0x4920 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x4938 ? 0x4938 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x4950 ? 0x4950 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x4958 ? 0x4958 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x4980 ? 0x4980 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x4b10 ? 0x4b10 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0x4c98 ? 0x4c98 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x4d88 ? 0x4d88 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    */spu_utoken_processor/src/utoken.o(.rodata.cst16)
    */spu_utoken_processor/src/main.o(.rodata.cst16)
    */spu_utoken_processor/src/utoken.o(.rodata._ZTV10dma_writer)
    */spu_utoken_processor/src/utoken.o(.rodata._ZTV13double_buffer)
    *(.rodata._ZTV12write_cursor)
    *(.rodata._ZTV11read_cursor)
    */spu_utoken_processor/src/main.o(.rodata.g_eid0_ls)
    */spu_utoken_processor/src/main.o(.rodata.g_default_idps)
    */spu_utoken_processor/src/main.o(.rodata.g_token_key)
    */spu_utoken_processor/src/main.o(.rodata.g_token_hmac_key)
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
    */spu_utoken_processor/src/main.o(.data.g_token_iv)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */spu_utoken_processor/src/utoken.o(.bss.g_read_cursor)
    */spu_utoken_processor/src/utoken.o(.bss.g_write_cursor)
    */spu_utoken_processor/src/utoken.o(.bss.g_reader)
    */spu_utoken_processor/src/utoken.o(.bss.g_writer)
    */spu_utoken_processor/src/main.o(.bss.g_token)
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
