ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x110 ? 0x110 : .;
    */sb_iso_spu_module/src/main.o(.text._ZN9sb_module5serveEP15ss_iso_dma_data)
    . = . < 0x5e8 ? 0x5e8 : .;
    */sb_iso_spu_module/src/main.o(.text.main)
    . = . < 0x720 ? 0x720 : .;
    */sb_iso_spu_module/src/session_b.o(.text._Z12bytes_differPKhS0_i)
    . = . < 0x780 ? 0x780 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b9xor_bytesEPKhS1_Phj)
    . = . < 0x7d8 ? 0x7d8 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b9put_replyERjS0_PhS0_PKh)
    . = . < 0x8c0 ? 0x8c0 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b5checkEjjPKhjj)
    . = . < 0x9b8 ? 0x9b8 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b10finish_xorEjjPKhj)
    . = . < 0xa10 ? 0xa10 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b8put_peerERjS0_PhS0_)
    . = . < 0xb10 ? 0xb10 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b13put_challengeERjS0_PhS0_)
    . = . < 0xc28 ? 0xc28 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b6verifyEjjPKhj)
    . = . < 0xd48 ? 0xd48 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b3runEv)
    . = . < 0x1120 ? 0x1120 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b11fill_randomEPhj)
    . = . < 0x1188 ? 0x1188 : .;
    */sb_iso_spu_module/src/session_b.o(.text._ZN9session_b4initEP15ss_iso_dma_dataPhjjS2_)
    . = . < 0x1348 ? 0x1348 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a5checkEjjPKhjj)
    . = . < 0x13b0 ? 0x13b0 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a12check_answerEjjPKhj)
    . = . < 0x13d8 ? 0x13d8 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a8put_typeEPhj)
    . = . < 0x1558 ? 0x1558 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a11fill_randomEPhj)
    . = . < 0x1598 ? 0x1598 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a9put_replyERjS0_PhS0_PKh)
    . = . < 0x1680 ? 0x1680 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a7put_keyERjS0_PhS0_)
    . = . < 0x17f8 ? 0x17f8 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a3runEj)
    . = . < 0x1aa0 ? 0x1aa0 : .;
    */sb_iso_spu_module/src/session_a.o(.text._ZN9session_a4initEP15ss_iso_dma_datajjjPKh)
    . = . < 0x1c20 ? 0x1c20 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c4initEP15ss_iso_dma_dataj)
    . = . < 0x1c90 ? 0x1c90 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c5checkEjjPKhjj)
    . = . < 0x1cf8 ? 0x1cf8 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c12check_answerEjjPKhj)
    . = . < 0x1d20 ? 0x1d20 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c9put_replyERjS0_PhS0_PKh)
    . = . < 0x1e08 ? 0x1e08 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c8put_doneERjS0_PhS0_)
    . = . < 0x1e68 ? 0x1e68 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c7put_keyERjS0_PhS0_)
    . = . < 0x1ee8 ? 0x1ee8 : .;
    */sb_iso_spu_module/src/session_c.o(.text._ZN9session_c3runEv)
    . = . < 0x2218 ? 0x2218 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data7alignedEv)
    . = . < 0x2260 ? 0x2260 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data5chunkERKjRj)
    . = . < 0x22f0 ? 0x22f0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data6notifyERKj)
    . = . < 0x2348 ? 0x2348 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD1Ev)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD2Ev)
    . = . < 0x2350 ? 0x2350 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC1ERKyRKjRKPhS3_)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC2ERKyRKjRKPhS3_)
    . = . < 0x23f0 ? 0x23f0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data3getERKPhRKjRjS5_S5_)
    . = . < 0x27e0 ? 0x27e0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data6finishERKjS1_S1_RKPh)
    . = . < 0x2a10 ? 0x2a10 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data3putERKjS1_S1_RKPh)
    . = . < 0x2c20 ? 0x2c20 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0x2d90 ? 0x2d90 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0x3090 ? 0x3090 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0x3490 ? 0x3490 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0x35a8 ? 0x35a8 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0x3750 ? 0x3750 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0x3940 ? 0x3940 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0x43e0 ? 0x43e0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x43f8 ? 0x43f8 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel13in_mbox_countERj)
    . = . < 0x4418 ? 0x4418 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel18out_intr_mbox_roomERj)
    . = . < 0x4438 ? 0x4438 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x44e0 ? 0x44e0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x4510 ? 0x4510 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x4528 ? 0x4528 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x4540 ? 0x4540 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel19write_out_intr_mboxEj)
    . = . < 0x4558 ? 0x4558 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel12read_in_mboxERj)
    . = . < 0x4590 ? 0x4590 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x4598 ? 0x4598 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x45c0 ? 0x45c0 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x4750 ? 0x4750 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0x48d8 ? 0x48d8 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x49c8 ? 0x49c8 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    */sb_iso_spu_module/src/main.o(.rodata.s_error)
    */sb_iso_spu_module/src/main.o(.rodata.s_ok)
    */sb_iso_spu_module/src/session_b.o(.rodata.s_error)
    */sb_iso_spu_module/src/session_a.o(.rodata.s_error)
    */sb_iso_spu_module/src/session_c.o(.rodata.s_error)
    */sb_iso_spu_module/src/session_c.o(.rodata.s_ok)
    */sb_iso_spu_module/src/session_c.o(.rodata.s_key)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_enc_key0)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_dec_key0)
    */sb_iso_spu_module/src/keys.o(.rodata.g_a_key0)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_enc_key1)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_dec_key1)
    */sb_iso_spu_module/src/keys.o(.rodata.g_a_key1)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_enc_key2)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_dec_key2)
    */sb_iso_spu_module/src/keys.o(.rodata.g_a_key2)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_enc_key3)
    */sb_iso_spu_module/src/keys.o(.rodata.g_b_dec_key3)
    */sb_iso_spu_module/src/keys.o(.rodata.g_a_key3)
    */sb_iso_spu_module/src/keys.o(.rodata.g_type_enc_key4)
    */sb_iso_spu_module/src/keys.o(.rodata.g_type_enc_key5)
    */sb_iso_spu_module/src/keys.o(.rodata.g_type_iv)
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
    */sb_iso_spu_module/src/keys.o(.data.g_key_15)
    */sb_iso_spu_module/src/keys.o(.data.g_key_16)
    KEEP(*/sb_iso_spu_module/src/keys.o(.data.g_unused_5ac0))
    */sb_iso_spu_module/src/keys.o(.data.g_key_17)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */sb_iso_spu_module/src/main.o(.bss.g_key_a)
    */sb_iso_spu_module/src/main.o(.bss.g_key_b)
    */sb_iso_spu_module/src/main.o(.bss.g_type_key)
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
