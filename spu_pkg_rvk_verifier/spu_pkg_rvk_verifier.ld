ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x110 ? 0x110 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN13double_bufferC1Ejj)
    . = . < 0x148 ? 0x148 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN11read_cursorC1Ev)
    . = . < 0x180 ? 0x180 : .;
    *(.text._ZN11read_cursor3setEy)
    . = . < 0x1a0 ? 0x1a0 : .;
    *(.text._ZN11read_cursor5startEv)
    . = . < 0x1a8 ? 0x1a8 : .;
    *(.text._ZN11read_cursor8set_sizeEj)
    . = . < 0x1c0 ? 0x1c0 : .;
    *(.text._ZN11read_cursor7advanceEj)
    . = . < 0x1d0 ? 0x1d0 : .;
    *(.text._ZN11read_cursor7currentEv)
    . = . < 0x1f0 ? 0x1f0 : .;
    *(.text._ZN11read_cursor11line_offsetEv)
    . = . < 0x228 ? 0x228 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN12write_cursorC1Ev)
    . = . < 0x260 ? 0x260 : .;
    *(.text._ZN12write_cursor3setEy)
    . = . < 0x270 ? 0x270 : .;
    *(.text._ZN12write_cursor5startEv)
    . = . < 0x278 ? 0x278 : .;
    *(.text._ZN12write_cursor8set_sizeEj)
    . = . < 0x290 ? 0x290 : .;
    *(.text._ZN12write_cursor7advanceEj)
    . = . < 0x2a0 ? 0x2a0 : .;
    *(.text._ZN12write_cursor7currentEv)
    . = . < 0x2c0 ? 0x2c0 : .;
    *(.text._ZN12write_cursor11line_offsetEv)
    . = . < 0x2f8 ? 0x2f8 : .;
    *(.text._ZN12write_cursorD0Ev)
    . = . < 0x310 ? 0x310 : .;
    *(.text._ZN12write_cursorD1Ev)
    . = . < 0x328 ? 0x328 : .;
    *(.text._ZN11read_cursorD0Ev)
    . = . < 0x340 ? 0x340 : .;
    *(.text._ZN11read_cursorD1Ev)
    . = . < 0x358 ? 0x358 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN13double_bufferD0Ev)
    . = . < 0x370 ? 0x370 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN13double_bufferD1Ev)
    . = . < 0x388 ? 0x388 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN10dma_writerD0Ev)
    . = . < 0x3d8 ? 0x3d8 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN10dma_writerD1Ev)
    . = . < 0x420 ? 0x420 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN10dma_readerD1Ev)
    . = . < 0x450 ? 0x450 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._Z13check_revokedPK8pkg_infoPKh)
    . = . < 0x620 ? 0x620 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN10dma_writerC1EjjjP9ea_cursor)
    . = . < 0x6d0 ? 0x6d0 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN10dma_readerC1EjjjP9ea_cursor)
    . = . < 0x760 ? 0x760 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN12rvk_verifier6verifyEPK13verify_paramsPK10sce_headerPK15metadata_headerPK16metadata_sectionSB_PKhPh)
    . = . < 0xa60 ? 0xa60 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN12pkg_verifier6verifyEPK13verify_paramsPK10sce_headerPK15metadata_headerPK16metadata_sectionSB_SB_PKhP8pkg_info)
    . = . < 0xd48 ? 0xd48 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN12rvk_verifier12read_headersEPK13verify_paramsP10sce_headerP15metadata_headerP16metadata_sectionS8_Ph)
    . = . < 0x1078 ? 0x1078 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._ZN12pkg_verifier12read_headersEPK13verify_paramsP10sce_headerP15metadata_headerP16metadata_sectionS8_S8_Ph)
    . = . < 0x13d0 ? 0x13d0 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._Z11verify_listPK13verify_paramsPh)
    . = . < 0x1580 ? 0x1580 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._Z14verify_packagePK13verify_paramsP8pkg_info)
    . = . < 0x1750 ? 0x1750 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text._Z11read_paramsyyP13verify_params)
    . = . < 0x1830 ? 0x1830 : .;
    *(.text._ZN11read_cursor5chunkEj)
    . = . < 0x1898 ? 0x1898 : .;
    *(.text._ZN12write_cursor5chunkEj)
    . = . < 0x1900 ? 0x1900 : .;
    */spu_pkg_rvk_verifier/src/main.o(.text.main)
    . = . < 0x1b90 ? 0x1b90 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifier11verify_infoEPK16metadata_sectionPKhyjyjPh)
    . = . < 0x1c50 ? 0x1c50 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifier10check_infoEPK8pkg_infojj)
    . = . < 0x1c90 ? 0x1c90 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifier12verify_blockEPK16metadata_sectionPKhyjyjPh)
    . = . < 0x1d50 ? 0x1d50 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifier11verify_bodyEPK16metadata_sectionPKhyjyj)
    . = . < 0x1e38 ? 0x1e38 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifierD0Ev)
    . = . < 0x1e80 ? 0x1e80 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifierD1Ev)
    . = . < 0x1e98 ? 0x1e98 : .;
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifierC1EP10dma_readerP10dma_writerPKhS5_S5_j)
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.text._ZN12pkg_verifierC2EP10dma_readerP10dma_writerPKhS5_S5_j)
    . = . < 0x1ed8 ? 0x1ed8 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifier13verify_headerEPK16metadata_sectionPKhyjyjPh)
    . = . < 0x1f98 ? 0x1f98 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifier17check_list_headerEPK10rvk_headerjj)
    . = . < 0x1fd8 ? 0x1fd8 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifier14verify_entriesEPK16metadata_sectionPKhyjyjPh)
    . = . < 0x20d0 ? 0x20d0 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifierD0Ev)
    . = . < 0x2118 ? 0x2118 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifierD1Ev)
    . = . < 0x2130 ? 0x2130 : .;
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifierC1EP10dma_readerP10dma_writerPKhS5_S5_j)
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.text._ZN12rvk_verifierC2EP10dma_readerP10dma_writerPKhS5_S5_j)
    . = . < 0x2170 ? 0x2170 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_reader8set_sizeEj)
    . = . < 0x2198 ? 0x2198 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer4seekEy)
    . = . < 0x21b8 ? 0x21b8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer8set_sizeEj)
    . = . < 0x21e0 ? 0x21e0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer10get_bufferEP7ls_spanPj)
    . = . < 0x2298 ? 0x2298 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifierC1EP10dma_readerP10dma_writerPKhS5_S5_j)
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifierC2EP10dma_readerP10dma_writerPKhS5_S5_j)
    . = . < 0x22f8 ? 0x22f8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier12check_headerEPK10sce_headerjjj)
    . = . < 0x2368 ? 0x2368 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier19check_section_countEPK15metadata_headerj)
    . = . < 0x2390 ? 0x2390 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier13check_sectionEPK16metadata_sectionjj)
    . = . < 0x23d0 ? 0x23d0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._Z8span_setP7ls_spanPhj)
    . = . < 0x2400 ? 0x2400 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._Z9span_fillP7ls_spanPKhS2_)
    . = . < 0x24b8 ? 0x24b8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifierD0Ev)
    . = . < 0x24d0 ? 0x24d0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifierD1Ev)
    . = . < 0x24e8 ? 0x24e8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifierD2Ev)
    . = . < 0x2500 ? 0x2500 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer4waitEv)
    . = . < 0x2510 ? 0x2510 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_reader4waitEv)
    . = . < 0x2518 ? 0x2518 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_reader4seekEy)
    . = . < 0x2580 ? 0x2580 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer5flushEP7ls_span)
    . = . < 0x26c0 ? 0x26c0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer3putEP7ls_spanPj)
    . = . < 0x2748 ? 0x2748 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_writer6finishEP7ls_span)
    . = . < 0x2798 ? 0x2798 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_reader5fetchEv)
    . = . < 0x28a0 ? 0x28a0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN10dma_reader4readEP7ls_span)
    . = . < 0x29c8 ? 0x29c8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier13transfer_tailEyjyjPKhPhP13hmac_sha1_ctxS2_)
    . = . < 0x2c70 ? 0x2c70 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier15transfer_blocksEyjyjPKhPhP13hmac_sha1_ctxS2_)
    . = . < 0x2ed8 ? 0x2ed8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier16verify_signatureEPKh)
    . = . < 0x2f80 ? 0x2f80 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier16decrypt_metadataEPKhjS1_Ph)
    . = . < 0x3088 ? 0x3088 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier18read_metadata_infoEyjPhS0_)
    . = . < 0x31f0 ? 0x31f0 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier13read_metadataEyPK10sce_headerPh)
    . = . < 0x3348 ? 0x3348 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier14verify_sectionEPK16metadata_sectionPKhyjyjPh)
    . = . < 0x36a8 ? 0x36a8 : .;
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.text._ZN12sce_verifier11read_headerEyjP10sce_header)
    . = . < 0x37e8 ? 0x37e8 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x3be8 ? 0x3be8 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x3d90 ? 0x3d90 : .;
    */lv0ldr/crypto/aes_modes.o(.text)
    . = . < 0x4420 ? 0x4420 : .;
    */lv0ldr/crypto/sha1_digest.o(.text)
    . = . < 0x44b0 ? 0x44b0 : .;
    */lv0ldr/crypto/hmac_sha1.o(.text)
    . = . < 0x4668 ? 0x4668 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x4780 ? 0x4780 : .;
    */lv0ldr/crypto/aes_blocks.o(.text)
    . = . < 0x5cf0 ? 0x5cf0 : .;
    */lv0ldr/crypto/sha1.o(.text)
    . = . < 0x6328 ? 0x6328 : .;
    */lv0ldr/crypto/sha1_transform.o(.text)
    . = . < 0x71a0 ? 0x71a0 : .;
    */lv0ldr/crypto/ecdsa_id.o(.text)
    . = . < 0x72c0 ? 0x72c0 : .;
    */lv0ldr/crypto/ec_curves.o(.text)
    . = . < 0x73b8 ? 0x73b8 : .;
    */isoldr/crypto/ecdsa.o(.text)
    . = . < 0x7770 ? 0x7770 : .;
    */lv0ldr/crypto/ec.o(.text)
    . = . < 0x84a0 ? 0x84a0 : .;
    */lv0ldr/crypto/ec_mul2.o(.text)
    . = . < 0x86a8 ? 0x86a8 : .;
    */lv0ldr/crypto/bn_core.o(.text)
    . = . < 0xbd90 ? 0xbd90 : .;
    */lv0ldr/crypto/bn_in_range.o(.text)
    . = . < 0xbe00 ? 0xbe00 : .;
    */lv0ldr/crypto/ec_curve_valid.o(.text)
    . = . < 0xbe10 ? 0xbe10 : .;
    */lv0ldr/crypto/bn_from_bytes_not.o(.text)
    . = . < 0xbf10 ? 0xbf10 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0xbf28 ? 0xbf28 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0xbfd0 ? 0xbfd0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0xc000 ? 0xc000 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0xc018 ? 0xc018 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0xc030 ? 0xc030 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0xc038 ? 0xc038 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0xc060 ? 0xc060 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0xc1f0 ? 0xc1f0 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0xc378 ? 0xc378 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0xc468 ? 0xc468 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    */spu_pkg_rvk_verifier/src/main.o(.rodata.cst16)
    *(.rodata._ZTV12write_cursor)
    *(.rodata._ZTV11read_cursor)
    */spu_pkg_rvk_verifier/src/main.o(.rodata._ZTV10dma_writer)
    */spu_pkg_rvk_verifier/src/main.o(.rodata._ZTV13double_buffer)
    */spu_pkg_rvk_verifier/src/main.o(.rodata.g_rvk_key)
    */spu_pkg_rvk_verifier/src/main.o(.rodata.g_rvk_pub)
    */spu_pkg_rvk_verifier/src/main.o(.rodata.g_pkg_key)
    */spu_pkg_rvk_verifier/src/main.o(.rodata.g_pkg_pub)
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.rodata.cst16)
    */spu_pkg_rvk_verifier/src/pkg_verifier.o(.rodata._ZTV12pkg_verifier)
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.rodata.cst16)
    */spu_pkg_rvk_verifier/src/rvk_verifier.o(.rodata._ZTV12rvk_verifier)
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.rodata.cst16)
    */spu_pkg_rvk_verifier/src/sce_verifier.o(.rodata._ZTV12sce_verifier)
    */crypto/*.o(.rodata .rodata.*)
    */spp_verifier/src/runtime/dma_channel.o(.rodata._ZTV11dma_channel)
    */lv0ldr/src/util/shuffle_table.o(.rodata.shuffle_table)
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80);
  .ctors : { KEEP(*(.ctors)) KEEP(*(.ctors.end)) }
  .dtors : { KEEP(*(.dtors)) KEEP(*(.dtors.end)) }
  .data :
  {
    */spu_token_processor/src/crt0.o(.data)
    */spu_pkg_rvk_verifier/src/main.o(.data.g_pkg_iv)
    */spu_pkg_rvk_verifier/src/main.o(.data.g_pkg_curve)
    */spu_pkg_rvk_verifier/src/main.o(.data.g_rvk_iv)
    */spu_pkg_rvk_verifier/src/main.o(.data.g_rvk_curve)
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
