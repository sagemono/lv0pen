ENTRY(_start)

SECTIONS
{
  . = 0x80;

  .init :
  {
    KEEP(*/spp_verifier/src/crt0.o(.init))
  }

  . = 0x880;

  .text :
  {
    */spp_verifier/src/crt0.o(.text)
    */lv2ldr/src/hash_reader.o(.text._Z12bytes_differPKhS0_i)
    */spp_verifier/src/main.o(.text._ZN11read_cursorC1Ev)
    */spp_verifier/src/main.o(.text._ZN11read_cursor3setEy)
    */spp_verifier/src/main.o(.text._ZN11read_cursor5startEv)
    */spp_verifier/src/main.o(.text._ZN11read_cursor8set_sizeEj)
    */spp_verifier/src/main.o(.text._ZN11read_cursor7advanceEj)
    */spp_verifier/src/main.o(.text._ZN11read_cursor7currentEv)
    */spp_verifier/src/main.o(.text._ZN11read_cursor11line_offsetEv)
    */spp_verifier/src/main.o(.text._ZN12write_cursorC1Ev)
    */spp_verifier/src/main.o(.text._ZN12write_cursor3setEy)
    */spp_verifier/src/main.o(.text._ZN12write_cursor5startEv)
    */spp_verifier/src/main.o(.text._ZN12write_cursor8set_sizeEj)
    */spp_verifier/src/main.o(.text._ZN12write_cursor7advanceEj)
    */spp_verifier/src/main.o(.text._ZN12write_cursor7currentEv)
    */spp_verifier/src/main.o(.text._ZN12write_cursor11line_offsetEv)
    */spp_verifier/src/main.o(.text._ZN13double_bufferC1Ejj)
    */spp_verifier/src/main.o(.text._ZN10dma_reader8set_sizeEj)
    */spp_verifier/src/main.o(.text._ZN10dma_writer4seekEy)
    */spp_verifier/src/main.o(.text._ZN10dma_writer8set_sizeEj)
    */spp_verifier/src/main.o(.text._ZN10dma_writer10get_bufferEP7ls_spanPj)
    */spp_verifier/src/main.o(.text._Z8key_slotP8metadataj)
    */spp_verifier/src/main.o(.text._ZN13double_bufferD0Ev)
    */spp_verifier/src/main.o(.text._ZN13double_bufferD1Ev)
    */spp_verifier/src/main.o(.text._ZN12write_cursorD0Ev)
    */spp_verifier/src/main.o(.text._ZN12write_cursorD1Ev)
    */spp_verifier/src/main.o(.text._ZN11read_cursorD0Ev)
    */spp_verifier/src/main.o(.text._ZN11read_cursorD1Ev)
    */spp_verifier/src/main.o(.text._ZN10dma_writerD0Ev)
    */spp_verifier/src/main.o(.text._ZN10dma_writerD1Ev)
    */spp_verifier/src/main.o(.text._ZN10dma_readerD1Ev)
    */spp_verifier/src/main.o(.text._ZN10dma_writer4waitEv)
    */spp_verifier/src/main.o(.text._ZN10dma_reader4waitEv)
    */spp_verifier/src/main.o(.text._ZN10dma_reader4seekEy)
    */spp_verifier/src/main.o(.text._ZN10dma_writer5flushEP7ls_span)
    */spp_verifier/src/main.o(.text._ZN10dma_writer3putEP7ls_spanPj)
    */spp_verifier/src/main.o(.text._ZN10dma_reader5fetchEv)
    */spp_verifier/src/main.o(.text._ZN10dma_reader4readEP7ls_span)
    */spp_verifier/src/main.o(.text._Z14verify_sectionP8metadatayj)
    */spp_verifier/src/main.o(.text._ZN10dma_writerC1EjjjP9ea_cursor)
    */spp_verifier/src/main.o(.text._ZN10dma_readerC1EjjjP9ea_cursor)
    */spp_verifier/src/main.o(.text._Z13verify_headeryj)
    */spp_verifier/src/main.o(.text.main)
    */spp_verifier/src/main.o(.text._Z41__static_initialization_and_destruction_0ii)
    */spp_verifier/src/main.o(.text._GLOBAL__I_g_curve)
    */spp_verifier/src/main.o(.text._ZN11read_cursor5chunkEj)
    */spp_verifier/src/main.o(.text._ZN12write_cursor5chunkEj)
    */lv0ldr/crypto/aes_modes.o(.text)
    */lv0ldr/crypto/sha1.o(.text)
    */lv0ldr/crypto/hmac_sha1.o(.text)
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    */lv0ldr/crypto/aes_blocks.o(.text)
    */lv0ldr/crypto/sha1_transform.o(.text)
    */lv0ldr/crypto/sha1_digest.o(.text)
    */lv0ldr/crypto/ecdsa_id.o(.text)
    */lv0ldr/crypto/ec_curves.o(.text)
    */isoldr/crypto/ecdsa.o(.text)
    */lv0ldr/crypto/ec.o(.text)
    */lv0ldr/crypto/ec_mul2.o(.text)
    */lv0ldr/crypto/bn_core.o(.text)
    */lv0ldr/crypto/bn_in_range.o(.text)
    */lv0ldr/crypto/ec_curve_valid.o(.text)
    */lv0ldr/crypto/bn_from_bytes_not.o(.text)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel8tag_doneEj)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    */spp_verifier/src/runtime/exit.o(.text.exit)
    */spp_verifier/src/runtime/free.o(.text.free)
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    */spp_verifier/src/exit_stop.o(.text.exit_stop)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .fini :
  {
    KEEP(*/spp_verifier/src/crt0.o(.fini))
  }

  .rodata :
  {
    */spp_verifier/src/main.o(.rodata.cst16)
    */spp_verifier/src/main.o(.rodata.g_metadata_key)
    */spp_verifier/src/main.o(.rodata.g_metadata_iv)
    */spp_verifier/src/main.o(.rodata.g_pub)
    */spp_verifier/src/main.o(.rodata._ZTV10dma_writer)
    */spp_verifier/src/main.o(.rodata._ZTV13double_buffer)
    */spp_verifier/src/main.o(.rodata._ZTV12write_cursor)
    */spp_verifier/src/main.o(.rodata._ZTV11read_cursor)
    */crypto/*.o(.rodata .rodata.*)
    */spp_verifier/src/runtime/dma_channel.o(.rodata._ZTV11dma_channel)
    */lv0ldr/src/util/shuffle_table.o(.rodata.shuffle_table)
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80);
  .ctors : { KEEP(*(.ctors)) __CTOR_END__ = .; LONG(0) }
  .dtors : { KEEP(*(.dtors)) LONG(0) }
  .data :
  {
    */spp_verifier/src/crt0.o(.data)
    */spp_verifier/src/main.o(.data.g_curve)
    */spp_verifier/src/main.o(.data.g_metadata_info)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */spp_verifier/src/crt0.o(.bss)
    */spp_verifier/src/main.o(.bss.g_metadata)
    */spp_verifier/src/main.o(.bss.g_metadata_ctr_iv)
    */spp_verifier/src/main.o(.bss.g_section_count)
    */spp_verifier/src/main.o(.bss.g_header_len)
    */spp_verifier/src/main.o(.bss.g_reader)
    */spp_verifier/src/main.o(.bss.g_writer)
    */spp_verifier/src/main.o(.bss.g_in)
    */spp_verifier/src/main.o(.bss.g_out)
    */spp_verifier/src/main.o(.bss.g_out_end)
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }
  _end = .;

  STACK_CHAIN = 0x3dff0;

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
