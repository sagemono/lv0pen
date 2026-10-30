ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x110 ? 0x110 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module9xor_bytesEPKhS1_Phj)
    . = . < 0x168 ? 0x168 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module11fill_randomEPhj)
    . = . < 0x1d0 ? 0x1d0 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module6unpackEPKhPyS2_S2_S2_)
    . = . < 0x5b8 ? 0x5b8 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module4packEPhyyyy)
    . = . < 0x900 ? 0x900 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module4initE6key128S0_)
    . = . < 0x950 ? 0x950 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module11check_chainEyyyyyyyPyS0_Ph)
    . = . < 0xbb8 ? 0xbb8 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module5chainEyyyyyPhS0_S0_)
    . = . < 0xde0 ? 0xde0 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module6deriveEyyPhS0_)
    . = . < 0xf28 ? 0xf28 : .;
    */mc_iso_spu_module/src/mc_module.o(.text._ZN9mc_module5serveEP15ss_iso_dma_data)
    . = . < 0x1e80 ? 0x1e80 : .;
    */mc_iso_spu_module/src/mc_module.o(.text.main)
    . = . < 0x1fc0 ? 0x1fc0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data7alignedEv)
    . = . < 0x2008 ? 0x2008 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data5chunkERKjRj)
    . = . < 0x2098 ? 0x2098 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD1Ev)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD2Ev)
    . = . < 0x20a0 ? 0x20a0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC1ERKyRKjS3_S3_)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC2ERKyRKjS3_S3_)
    . = . < 0x2140 ? 0x2140 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data3getERKPhRKjRjS5_S5_)
    . = . < 0x2530 ? 0x2530 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data6finishERKjS1_S1_RKPh)
    . = . < 0x2760 ? 0x2760 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0x28d0 ? 0x28d0 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0x2bd0 ? 0x2bd0 : .;
    */mc_iso_spu_module/crypto/des_cbc.o(.text)
    . = . < 0x33f0 ? 0x33f0 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x37f0 ? 0x37f0 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x3908 ? 0x3908 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x3ab0 ? 0x3ab0 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0x3ca0 ? 0x3ca0 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0x4740 ? 0x4740 : .;
    */mc_iso_spu_module/crypto/des.o(.text)
    . = . < 0x5ff0 ? 0x5ff0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x6008 ? 0x6008 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel13in_mbox_countERj)
    . = . < 0x6028 ? 0x6028 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel18out_intr_mbox_roomERj)
    . = . < 0x6048 ? 0x6048 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x60f0 ? 0x60f0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x6120 ? 0x6120 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x6138 ? 0x6138 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x6150 ? 0x6150 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel19write_out_intr_mboxEj)
    . = . < 0x6168 ? 0x6168 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel12read_in_mboxERj)
    . = . < 0x6198 ? 0x6198 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x61a0 ? 0x61a0 : .;
    KEEP(*/mc_iso_spu_module/src/runtime/atexit.o(.text.atexit))
    . = . < 0x61e0 ? 0x61e0 : .;
    */mc_iso_spu_module/src/runtime/atexit.o(.text.exit)
    . = . < 0x6240 ? 0x6240 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x63d0 ? 0x63d0 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    *(.text .text.* .gnu.linkonce.t.*)
  } =0x00200000

  .rodata :
  {
    */mc_iso_spu_module/src/mc_module.o(.rodata.cst16)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_iv)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_mask0)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_key0)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_mask1)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_key1)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_error)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_ok)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_mismatch)
    */mc_iso_spu_module/src/mc_module.o(.rodata.s_unknown)
    */sc_iso/src/ss_iso_dma_data.o(.rodata.cst16)
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
    */mc_iso_spu_module/src/mc_module.o(.data.g_chain_iv)
    */mc_iso_spu_module/src/runtime/atexit.o(.data.g_atexit_free)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */mc_iso_spu_module/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
