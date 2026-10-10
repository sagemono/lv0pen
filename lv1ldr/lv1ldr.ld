ENTRY(_start)

SECTIONS
{
  . = 0x800;

  .bss :
  {
    */lv1ldr/src/loader_data.o(.bss.g_auth)
    */lv1ldr/src/loader_global.o(.bss.g_loader)
    */lv1ldr/encdec/encdec.o(.bss.kbuf)
    */lv1ldr/encdec/encdec.o(.bss.kout)
    */lv0ldr/src/log/log.o(.bss.g_log_handlers)
    */lv1ldr/src/uart.o(.bss._ZGVZ8get_uartvE7console)
    */lv1ldr/src/uart.o(.bss._ZZ8get_uartvE7console)
    */isoldr/src/cipher_aes256_cbc.o(.bss.g_aes256_cbc)
    */isoldr/src/verifier_ecdsa.o(.bss.verify_hash)
    */lv1ldr/src/auth_buffers.o(.bss.g_part_area)
    */lv1ldr/src/auth_buffers.o(.bss.g_part_digest)
    */lv1ldr/src/auth_buffers.o(.bss.g_header)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_tail)
    */isoldr/src/hmac_digest.o(.bss.g_hmac)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_offset)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_total)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_out)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_done)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_avail)
    */lv1ldr/src/auth_transfer.o(.bss.g_inf_p)
    */isoldr/src/cipher_aes128_ctr.o(.bss.g_aes128_ctr)
    */isoldr/src/dma_buffer.o(.bss.g_dma_mfc)
    */lv1ldr/zlib/lv1ldr_inflate.o(.bss.g_strm)
    */lv1ldr/zlib/inflate.o(.bss.lv1ldr_window_used)
    */lv1ldr/zlib/inflate.o(.bss.lv1ldr_state_used)
    */lv1ldr/zlib/inflate.o(.bss.lv1ldr_state)
    */lv1ldr/zlib/inflate.o(.bss.lv1ldr_window)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
  }

  . = 0x12c00;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */isoldr/src/crt0.o(.text)
    . = . < 0x88 ? 0x88 : .;
    */lv1ldr/src/loader.o(.text._ZN6loaderC1Ev)
    */lv1ldr/src/loader.o(.text._ZN6loaderC2Ev)
    . = . < 0xd0 ? 0xd0 : .;
    */lv1ldr/src/loader.o(.text._ZN6loader12read_requestEv)
    . = . < 0x178 ? 0x178 : .;
    *(.text._ZN9encryptorD0Ev)
    . = . < 0x190 ? 0x190 : .;
    *(.text._ZN9encryptorD1Ev)
    . = . < 0x1a8 ? 0x1a8 : .;
    *(.text._ZN9encryptorD2Ev)
    . = . < 0x1c0 ? 0x1c0 : .;
    *(.text._ZN20aes256_cbc_encryptorD0Ev)
    . = . < 0x208 ? 0x208 : .;
    *(.text._ZN20aes256_cbc_encryptorD1Ev)
    . = . < 0x220 ? 0x220 : .;
    *(.text._ZN17tagged_dma_bufferD1Ev)
    . = . < 0x238 ? 0x238 : .;
    */lv1ldr/src/loader_boot.o(.text._ZN6loader13load_segmentsEv)
    . = . < 0x4f0 ? 0x4f0 : .;
    */lv1ldr/src/loader_boot.o(.text._ZN6loader12auth_requestEv)
    . = . < 0x588 ? 0x588 : .;
    */lv1ldr/src/loader_boot.o(.text._ZN6loader19check_request_tokenEv)
    . = . < 0x630 ? 0x630 : .;
    */lv0ldr/src/dma/dma.o(.text._Z8dma_waitv)
    . = . < 0x680 ? 0x680 : .;
    */lv1ldr/src/mmio_access.o(.text._Z7write64yy)
    . = . < 0x6f0 ? 0x6f0 : .;
    */lv0ldr/src/dma/dma.o(.text._Z21copy_qwords_from_mmioyPcj)
    . = . < 0x798 ? 0x798 : .;
    */lv1ldr/src/mmio_access.o(.text._Z19copy_qwords_to_mmioyPcj)
    . = . < 0x838 ? 0x838 : .;
    */lv1ldr/src/loader.o(.text._ZN6loader3runEv)
    . = . < 0x1018 ? 0x1018 : .;
    */lv1ldr/src/loader_global.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1040 ? 0x1040 : .;
    */lv1ldr/src/loader_global.o(.text._GLOBAL__I_g_loader)
    . = . < 0x1050 ? 0x1050 : .;
    */lv1ldr/src/main.o(.text.main)
    . = . < 0x11b8 ? 0x11b8 : .;
    *(.text._ZN6cipher6vslot6EPKv)
    . = . < 0x11c0 ? 0x11c0 : .;
    *(.text._ZN6cipher6vslot7EPv)
    . = . < 0x11c8 ? 0x11c8 : .;
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digestC1Ev)
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digestC2Ev)
    . = . < 0x1210 ? 0x1210 : .;
    */lv1ldr/src/qa_flag.o(.text._Z12bytes_differPKhS0_i)
    . = . < 0x1270 ? 0x1270 : .;
    */lv1ldr/src/qa_flag.o(.text._ZN14qa_flag_reader4initEv)
    . = . < 0x1278 ? 0x1278 : .;
    *(.text._ZN6cipherD0Ev)
    . = . < 0x1290 ? 0x1290 : .;
    *(.text._ZN6cipherD1Ev)
    . = . < 0x12a8 ? 0x12a8 : .;
    *(.text._ZN6cipherD2Ev)
    . = . < 0x12c0 ? 0x12c0 : .;
    *(.text._ZN17aes256_cbc_cipherD0Ev)
    . = . < 0x1308 ? 0x1308 : .;
    *(.text._ZN17aes256_cbc_cipherD1Ev)
    . = . < 0x1320 ? 0x1320 : .;
    *(.text._ZN8verifierD0Ev)
    . = . < 0x1338 ? 0x1338 : .;
    *(.text._ZN8verifierD1Ev)
    . = . < 0x1350 ? 0x1350 : .;
    *(.text._ZN8verifierD2Ev)
    . = . < 0x1368 ? 0x1368 : .;
    *(.text._ZN14ecdsa_verifierD0Ev)
    . = . < 0x13b0 ? 0x13b0 : .;
    *(.text._ZN14ecdsa_verifierD1Ev)
    . = . < 0x13c8 ? 0x13c8 : .;
    */lv1ldr/src/qa_flag.o(.text._ZN14qa_flag_reader5checkEjPKhPhS1_S1_)
    . = . < 0x1580 ? 0x1580 : .;
    */lv1ldr/src/mmio_access.o(.text._Z6read32y)
    . = . < 0x1608 ? 0x1608 : .;
    */lv1ldr/src/sb_product.o(.text._Z15is_sb_product_4v)
    . = . < 0x1640 ? 0x1640 : .;
    */lv1ldr/src/sb_product.o(.text._Z15is_sb_product_3v)
    . = . < 0x1678 ? 0x1678 : .;
    */lv1ldr/src/sb_product.o(.text._Z15is_sb_product_2v)
    . = . < 0x16b0 ? 0x16b0 : .;
    */lv1ldr/src/sb_product.o(.text._Z16is_sb_product_dxv)
    . = . < 0x16e8 ? 0x16e8 : .;
    */lv1ldr/src/init_device.o(.text._ZN6loader13init_device_1EyyyPK9device_idPKhPKU8__vectorhS7_S7_S7_S7_S4_S4_S4_)
    . = . < 0x1c18 ? 0x1c18 : .;
    */lv1ldr/src/setup.o(.text._ZN6loader8init_logEj)
    . = . < 0x1c78 ? 0x1c78 : .;
    */lv1ldr/src/storage.o(.text._Z9prng_seedv)
    . = . < 0x1cb0 ? 0x1cb0 : .;
    */lv1ldr/src/storage.o(.text._Z11prng_randomPh)
    . = . < 0x1cd8 ? 0x1cd8 : .;
    */lv1ldr/encdec/encdec.o(.text._Z11highest_bitj)
    . = . < 0x1d10 ? 0x1d10 : .;
    *(.text._Z7write32yj)
    . = . < 0x1d80 ? 0x1d80 : .;
    */lv1ldr/encdec/encdec.o(.text._Z10set_windowyjjj)
    . = . < 0x1e38 ? 0x1e38 : .;
    */lv1ldr/encdec/encdec.o(.text._Z13set_window_4kyjjj)
    . = . < 0x1ef0 ? 0x1ef0 : .;
    */lv1ldr/encdec/encdec.o(.text._Z11cbc_decryptPU8__vectorhPKS_iPKhiS2_)
    . = . < 0x1f18 ? 0x1f18 : .;
    */lv1ldr/encdec/encdec.o(.text._Z11cbc_encryptPU8__vectorhPKS_iPKhiS2_)
    . = . < 0x1f40 ? 0x1f40 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN4kgen3genEyPKjS1_PKhS3_Pjyyb)
    . = . < 0x2720 ? 0x2720 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN4kgen4initEyyy)
    . = . < 0x2988 ? 0x2988 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map3setEyy)
    . = . < 0x29c0 ? 0x29c0 : .;
    *(.text._Z6read16y)
    . = . < 0x2a50 ? 0x2a50 : .;
    *(.text._Z7write16yt)
    . = . < 0x2ac0 ? 0x2ac0 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map14get_dma_statusEyyy)
    . = . < 0x2bc8 ? 0x2bc8 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map7put_keyEyPK6key192jjPKhPKU8__vectorhS2_P5iv128)
    . = . < 0x2dc8 ? 0x2dc8 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map8put_key2EyPK6key192jjPKhPKU8__vectorhS2_P5iv128)
    . = . < 0x2fc8 ? 0x2fc8 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map8put_descEyjjjtj)
    . = . < 0x3098 ? 0x3098 : .;
    */lv1ldr/encdec/encdec.o(.text._ZN6io_map5startEyyy)
    . = . < 0x3130 ? 0x3130 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdecC1Ev)
    */lv1ldr/encdec/activate.o(.text._ZN6encdecC2Ev)
    . = . < 0x3148 ? 0x3148 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdec9bit_countEj)
    . = . < 0x3180 ? 0x3180 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdec8next_bitEji)
    . = . < 0x31b0 ? 0x31b0 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdec19get_key192_by_indexEP11key192_nodei)
    . = . < 0x31e8 ? 0x31e8 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdec6enableEy)
    . = . < 0x32d8 ? 0x32d8 : .;
    */lv1ldr/encdec/activate.o(.text._ZN6encdec8activateEPKhS1_PK6key192PKjS6_yyPK7ata_cfgPK10encdec_cfg)
    . = . < 0x3b20 ? 0x3b20 : .;
    *(.text._ZN6encdecD0Ev)
    . = . < 0x3b38 ? 0x3b38 : .;
    *(.text._ZN6encdecD1Ev)
    . = . < 0x3b50 ? 0x3b50 : .;
    */lv0ldr/src/util/cxx_runtime.o(.text.__cxa_pure_virtual)
    . = . < 0x3b58 ? 0x3b58 : .;
    */lv0ldr/src/log/log.o(.text._Z15log_set_handleriPFlPKcE)
    . = . < 0x3b90 ? 0x3b90 : .;
    */lv0ldr/src/log/log.o(.text._Z17log_set_thresholdi)
    . = . < 0x3bb0 ? 0x3bb0 : .;
    */lv0ldr/src/log/log.o(.text._Z12log_dispatchiiPKcP13__va_list_tag)
    . = . < 0x3c50 ? 0x3c50 : .;
    */lv0ldr/src/log/log.o(.text._Z9log_debugPKcz)
    . = . < 0x3de8 ? 0x3de8 : .;
    */lv0ldr/src/log/log.o(.text._Z8log_infoPKcz)
    . = . < 0x3f80 ? 0x3f80 : .;
    */lv0ldr/src/log/log.o(.text._Z11log_messagePKcz)
    . = . < 0x4118 ? 0x4118 : .;
    */lv0ldr/src/boot/exit.o(.text.exit)
    . = . < 0x4120 ? 0x4120 : .;
    */lv1ldr/src/abort.o(.text.abort)
    . = . < 0x4128 ? 0x4128 : .;
    */lv1ldr/src/printf.o(.text._Z19lv0_vfprintf_enginePFilPlllEljPKhP13__va_list_tag)
    . = . < 0x5650 ? 0x5650 : .;
    */lv1ldr/src/printf.o(.text._Z17snprintf_put_charPPcPjli)
    . = . < 0x56f8 ? 0x56f8 : .;
    */lv1ldr/src/printf.o(.text._Z13lv0_vsnprintfPcmPKcP13__va_list_tag)
    . = . < 0x5740 ? 0x5740 : .;
    */lv0ldr/src/util/memcmp.o(.text.memcmp)
    . = . < 0x57e8 ? 0x57e8 : .;
    */isoldr/src/memset.o(.text.memset)
    . = . < 0x58b0 ? 0x58b0 : .;
    */isoldr/src/memcpy.o(.text.memcpy)
    . = . < 0x5a10 ? 0x5a10 : .;
    */lv1ldr/src/uart.o(.text.strlen)
    . = . < 0x5a88 ? 0x5a88 : .;
    */lv1ldr/src/uart.o(.text._Z8get_uartv)
    . = . < 0x5ad0 ? 0x5ad0 : .;
    */lv1ldr/src/uart.o(.text._ZN4uart4putcEl)
    . = . < 0x5b80 ? 0x5b80 : .;
    */lv1ldr/src/uart.o(.text._ZN4uart4putsEPKc)
    . = . < 0x5c30 ? 0x5c30 : .;
    */lv1ldr/src/uart.o(.text._ZN4uart4initEyiilbl)
    . = . < 0x5e00 ? 0x5e00 : .;
    */lv1ldr/src/uart.o(.text._Z17uart_console_putsPKc)
    . = . < 0x5e50 ? 0x5e50 : .;
    */lv1ldr/src/wb_aes.o(.text._Z12wb_aes_blockPKhPhPK10wb_aes_key)
    . = . < 0x6b20 ? 0x6b20 : .;
    */lv1ldr/src/wb_aes.o(.text._Z8wb_cryptPKjPKhjS2_Ph)
    . = . < 0x6bf0 ? 0x6bf0 : .;
    *(.text._ZN17aes256_cbc_cipherC1Ev)
    . = . < 0x6c40 ? 0x6c40 : .;
    */isoldr/src/cipher_aes256_cbc.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x6c78 ? 0x6c78 : .;
    */isoldr/src/cipher_aes256_cbc.o(.text._GLOBAL__D_g_aes256_cbc)
    . = . < 0x6c88 ? 0x6c88 : .;
    */isoldr/src/cipher_aes256_cbc.o(.text._GLOBAL__I_g_aes256_cbc)
    . = . < 0x6c98 ? 0x6c98 : .;
    */isoldr/src/cipher_aes256_cbc.o(.text._ZN17aes256_cbc_cipher7decryptEPKviPv)
    . = . < 0x6ce0 ? 0x6ce0 : .;
    */isoldr/src/cipher_aes256_cbc.o(.text._ZN17aes256_cbc_cipher7set_keyEPKhS1_)
    . = . < 0x6d08 ? 0x6d08 : .;
    *(.text._ZN20aes256_cbc_encryptorC1Ev)
    . = . < 0x6d58 ? 0x6d58 : .;
    */isoldr/src/aes256_cbc_encryptor.o(.text._ZN20aes256_cbc_encryptor7set_keyEPKhPK9cipher_iv)
    . = . < 0x6d98 ? 0x6d98 : .;
    */isoldr/src/aes256_cbc_encryptor.o(.text._ZN20aes256_cbc_encryptor7encryptEPKviPv)
    . = . < 0x6de0 ? 0x6de0 : .;
    */isoldr/src/verifier_ecdsa.o(.text._ZN14ecdsa_verifier6verifyEPKhiS1_)
    . = . < 0x6e68 ? 0x6e68 : .;
    */isoldr/src/verifier_ecdsa.o(.text._ZN14ecdsa_verifierC1EPKhj)
    */isoldr/src/verifier_ecdsa.o(.text._ZN14ecdsa_verifierC2EPKhj)
    . = . < 0x6ef0 ? 0x6ef0 : .;
    */lv1ldr/src/loader_params.o(.text._ZN11param_block4initEv)
    . = . < 0x6ef8 ? 0x6ef8 : .;
    */lv1ldr/src/loader_params.o(.text._ZN11param_block6verifyEPKhj)
    . = . < 0x6fa8 ? 0x6fa8 : .;
    */lv1ldr/src/loader_params.o(.text._ZN11param_block4openEPKhjS1_S1_S1_S1_PjPhS3_)
    . = . < 0x7128 ? 0x7128 : .;
    *(.text._ZN9elf_imageC1Ev)
    . = . < 0x71a0 ? 0x71a0 : .;
    */lv1ldr/src/authenticator.o(.text._ZN9elf_image4loadEPKv)
    . = . < 0x73f0 ? 0x73f0 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator10set_offsetEy)
    . = . < 0x7408 ? 0x7408 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator14check_app_infoEPK13auth_app_infoj)
    . = . < 0x7470 ? 0x7470 : .;
    *(.text._ZN17aes128_ctr_cipherD0Ev)
    . = . < 0x74b8 ? 0x74b8 : .;
    *(.text._ZN17aes128_ctr_cipherD1Ev)
    . = . < 0x74d0 ? 0x74d0 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator9check_appEP9sce_imagej)
    . = . < 0x7560 ? 0x7560 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator11load_headerEjPKhS1_S1_PKjPP10dma_bufferj)
    . = . < 0x7a50 ? 0x7a50 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator11clear_partsEv)
    . = . < 0x7a88 ? 0x7a88 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticatorC1Ev)
    */lv1ldr/src/authenticator.o(.text._ZN13authenticatorC2Ev)
    . = . < 0x7c10 ? 0x7c10 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator19read_program_headerEjP10Elf64_Phdr)
    . = . < 0x7c38 ? 0x7c38 : .;
    */lv1ldr/src/authenticator.o(.text._ZN13authenticator15read_elf_headerEP10Elf64_Ehdr)
    . = . < 0x7c88 ? 0x7c88 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_block4loadEPK10auth_entryy)
    . = . < 0x7e50 ? 0x7e50 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_blockC1Ev)
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_blockC2Ev)
    . = . < 0x7ec0 ? 0x7ec0 : .;
    */isoldr/src/hmac_digest.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x7ee8 ? 0x7ee8 : .;
    */isoldr/src/hmac_digest.o(.text._GLOBAL__I_g_hmac)
    . = . < 0x7ef8 ? 0x7ef8 : .;
    */isoldr/src/hmac_digest.o(.text._GLOBAL__D_g_hmac)
    . = . < 0x7f08 ? 0x7f08 : .;
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digest6updateEPKhi)
    . = . < 0x7f38 ? 0x7f38 : .;
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digest5finalEPh)
    . = . < 0x7f48 ? 0x7f48 : .;
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digest4initEPKhj)
    . = . < 0x7f50 ? 0x7f50 : .;
    */isoldr/src/hmac_digest.o(.text._ZN11hmac_digest6digestEPKhiPhS1_j)
    . = . < 0x7f88 ? 0x7f88 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN11header_data5clearEv)
    . = . < 0x7fd0 ? 0x7fd0 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN9auth_tailC1Ev)
    */lv1ldr/src/auth_buffers.o(.text._ZN9auth_tailC2Ev)
    . = . < 0x7fe8 ? 0x7fe8 : .;
    */lv1ldr/src/auth_segments.o(.text._ZN13authenticator12load_segmentEjjyyPP10dma_bufferiS2_i)
    . = . < 0x8240 ? 0x8240 : .;
    */lv1ldr/src/auth_transfer.o(.text._ZN13authenticator17transfer_deflatedEy)
    . = . < 0x84c0 ? 0x84c0 : .;
    */lv1ldr/src/auth_transfer.o(.text._ZN13authenticator15transfer_storedEv)
    . = . < 0x8768 ? 0x8768 : .;
    */lv1ldr/src/auth_transfer.o(.text._ZN13authenticator13inflate_pieceEP10dma_bufferyPy)
    . = . < 0x8dc8 ? 0x8dc8 : .;
    *(.text._ZN17aes128_ctr_cipherC1Ev)
    . = . < 0x8e18 ? 0x8e18 : .;
    */isoldr/src/cipher_aes128_ctr.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x8e50 ? 0x8e50 : .;
    */isoldr/src/cipher_aes128_ctr.o(.text._GLOBAL__D_g_aes128_ctr)
    . = . < 0x8e60 ? 0x8e60 : .;
    */isoldr/src/cipher_aes128_ctr.o(.text._GLOBAL__I_g_aes128_ctr)
    . = . < 0x8e70 ? 0x8e70 : .;
    */isoldr/src/cipher_aes128_ctr.o(.text._ZN17aes128_ctr_cipher7decryptEPKviPv)
    . = . < 0x8ec0 ? 0x8ec0 : .;
    */isoldr/src/cipher_aes128_ctr.o(.text._ZN17aes128_ctr_cipher7set_keyEPKhS1_)
    . = . < 0x8ee8 ? 0x8ee8 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator7prependEPK10dma_bufferj)
    . = . < 0x8fd0 ? 0x8fd0 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator17check_part_digestEPK12auth_section)
    . = . < 0x9060 ? 0x9060 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN9auth_sink6appendEPKU8__vectorhS2_)
    . = . < 0x90b0 ? 0x90b0 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator10read_partsEv)
    . = . < 0x9180 ? 0x9180 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator9save_tailEPK10dma_buffer)
    . = . < 0x9220 ? 0x9220 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator12restore_tailEPK10dma_buffer)
    . = . < 0x92b0 ? 0x92b0 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN13authenticator12find_sectionEjjP12auth_section)
    . = . < 0x9348 ? 0x9348 : .;
    */lv1ldr/src/elf_reader.o(.text._ZN9elf_image10get_headerEP10Elf64_Ehdr)
    . = . < 0x9438 ? 0x9438 : .;
    */lv1ldr/src/elf_reader.o(.text._ZN9elf_image18get_program_headerEjP10Elf64_Phdr)
    . = . < 0x9550 ? 0x9550 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_state4loadEPK10auth_entryy)
    . = . < 0x96d0 ? 0x96d0 : .;
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_stateC1Ev)
    */lv1ldr/src/auth_buffers.o(.text._ZN10auth_stateC2Ev)
    . = . < 0x9748 ? 0x9748 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue12enqueue_readEP10dma_buffer)
    . = . < 0x98e0 ? 0x98e0 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue7dequeueEPP10dma_bufferP9list_nodeMS0_FbvE)
    . = . < 0x9a30 ? 0x9a30 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue12dequeue_readEPP10dma_buffer)
    . = . < 0x9ad0 ? 0x9ad0 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue5drainEP9list_nodeM10dma_bufferFbvE)
    . = . < 0x9b40 ? 0x9b40 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue5closeEv)
    . = . < 0x9c00 ? 0x9c00 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queueC1Ev)
    */isoldr/src/dma_queue.o(.text._ZN9dma_queueC2Ev)
    . = . < 0x9d00 ? 0x9d00 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue11add_writtenEP10dma_buffer)
    . = . < 0x9db0 ? 0x9db0 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue13enqueue_writeEP10dma_buffer)
    . = . < 0x9f28 ? 0x9f28 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue13dequeue_writeEPP10dma_buffer)
    . = . < 0x9fa0 ? 0x9fa0 : .;
    */isoldr/src/dma_queue.o(.text._ZN9dma_queue4openEyjyj)
    . = . < 0xa0d0 ? 0xa0d0 : .;
    */isoldr/src/dma_buffer.o(.text._ZN10dma_buffer10set_bufferEjj)
    . = . < 0xa128 ? 0xa128 : .;
    *(.text._ZN10dma_bufferD0Ev)
    . = . < 0xa140 ? 0xa140 : .;
    *(.text._ZN10dma_bufferD1Ev)
    . = . < 0xa158 ? 0xa158 : .;
    *(.text._ZN10dma_bufferD2Ev)
    . = . < 0xa170 ? 0xa170 : .;
    */isoldr/src/dma_buffer.o(.text._ZN10dma_bufferC1Ev)
    */isoldr/src/dma_buffer.o(.text._ZN10dma_bufferC2Ev)
    . = . < 0xa210 ? 0xa210 : .;
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_buffer9max_chunkEj)
    . = . < 0xa268 ? 0xa268 : .;
    *(.text._ZN17tagged_dma_bufferD0Ev)
    . = . < 0xa2b0 ? 0xa2b0 : .;
    *(.text._ZN7dma_mfcC1Ev)
    . = . < 0xa2b8 ? 0xa2b8 : .;
    */isoldr/src/dma_buffer.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xa2e0 ? 0xa2e0 : .;
    */isoldr/src/dma_buffer.o(.text._GLOBAL__I__ZN10dma_bufferC2Ev)
    . = . < 0xa2f0 ? 0xa2f0 : .;
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_buffer8transferEyyjj)
    . = . < 0xa4b0 ? 0xa4b0 : .;
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_buffer8put_doneEv)
    . = . < 0xa4e0 ? 0xa4e0 : .;
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_buffer8get_doneEv)
    . = . < 0xa510 ? 0xa510 : .;
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_bufferC1Ej)
    */isoldr/src/dma_buffer.o(.text._ZN17tagged_dma_bufferC2Ej)
    . = . < 0xa568 ? 0xa568 : .;
    */isoldr/src/dma_buffer_io.o(.text._ZN17tagged_dma_buffer3putEyjPj)
    . = . < 0xa6e0 ? 0xa6e0 : .;
    */isoldr/src/dma_buffer_io.o(.text._ZN17tagged_dma_buffer3getEyjPj)
    . = . < 0xa850 ? 0xa850 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0xac50 ? 0xac50 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0xadf8 ? 0xadf8 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0xaf68 ? 0xaf68 : .;
    */lv0ldr/crypto/aes_modes.o(.text)
    . = . < 0xb5f8 ? 0xb5f8 : .;
    */isoldr/crypto/aes_cmac.o(.text)
    . = . < 0xb808 ? 0xb808 : .;
    */lv0ldr/crypto/sha1_digest.o(.text)
    . = . < 0xb898 ? 0xb898 : .;
    */lv0ldr/crypto/hmac_sha1.o(.text)
    . = . < 0xba50 ? 0xba50 : .;
    */isoldr/crypto/hmac_sha1_once.o(.text)
    . = . < 0xbaf0 ? 0xbaf0 : .;
    */lv1ldr/crypto/prng.o(.text)
    . = . < 0xbe70 ? 0xbe70 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0xbf88 ? 0xbf88 : .;
    */lv0ldr/crypto/aes_blocks.o(.text)
    . = . < 0xd4f8 ? 0xd4f8 : .;
    */lv0ldr/crypto/sha1_transform.o(.text)
    . = . < 0xe370 ? 0xe370 : .;
    */lv0ldr/crypto/sha1.o(.text)
    . = . < 0xe9a8 ? 0xe9a8 : .;
    */isoldr/crypto/ecdsa.o(.text)
    . = . < 0xed60 ? 0xed60 : .;
    */lv0ldr/crypto/ec.o(.text)
    . = . < 0xfa90 ? 0xfa90 : .;
    */lv0ldr/crypto/ec_mul2.o(.text)
    . = . < 0xfc98 ? 0xfc98 : .;
    */lv0ldr/crypto/bn_core.o(.text)
    . = . < 0x13380 ? 0x13380 : .;
    */lv0ldr/crypto/bn_in_range.o(.text)
    . = . < 0x133f0 ? 0x133f0 : .;
    */lv0ldr/crypto/ec_curve_valid.o(.text)
    . = . < 0x13400 ? 0x13400 : .;
    */lv0ldr/crypto/ecdsa_id.o(.text)
    . = . < 0x13520 ? 0x13520 : .;
    */lv0ldr/crypto/ec_curves.o(.text)
    . = . < 0x13618 ? 0x13618 : .;
    */lv0ldr/crypto/bn_from_bytes_not.o(.text)
    . = . < 0x13718 ? 0x13718 : .;
    */isoldr/src/mfc.o(.text._ZN8mfc_baseC1Ev)
    */isoldr/src/mfc.o(.text._ZN8mfc_baseC2Ev)
    . = . < 0x13720 ? 0x13720 : .;
    */isoldr/src/mfc.o(.text._Z9mfc_issuejyjjjj)
    . = . < 0x13750 ? 0x13750 : .;
    */isoldr/src/mfc.o(.text._Z12mfc_tag_donej)
    . = . < 0x13790 ? 0x13790 : .;
    */lv0ldr/src/util/cxx_runtime.o(.text._ZdlPv)
    . = . < 0x13798 ? 0x13798 : .;
    */isoldr/src/ch64.o(.text._Z12ch64_requestv)
    . = . < 0x137c0 ? 0x137c0 : .;
    */lv1ldr/src/ch72.o(.text._Z8ch72_putiPKji)
    . = . < 0x13810 ? 0x13810 : .;
    */lv1ldr/src/ch72.o(.text._Z8ch72_getiPji)
    . = . < 0x13868 ? 0x13868 : .;
    */lv1ldr/src/ch72.o(.text._Z16ch72_put_requestjPKh)
    . = . < 0x13948 ? 0x13948 : .;
    */lv1ldr/src/ch72.o(.text._Z14ch72_get_replyPh)
    . = . < 0x13a08 ? 0x13a08 : .;
    */lv1ldr/src/ch72.o(.text._Z17ch72_get_all_onesv)
    . = . < 0x13a90 ? 0x13a90 : .;
    */lv1ldr/src/ch72.o(.text._Z11ch72_unwrapPvPKviPKhiPKU8__vectorh)
    . = . < 0x13b58 ? 0x13b58 : .;
    */lv1ldr/src/ch72.o(.text._Z14ch72_unwrap_ivPvPKvi)
    . = . < 0x13b68 ? 0x13b68 : .;
    */lv1ldr/src/ch72.o(.text._Z15ch72_unwrap_keyPvPKvi)
    . = . < 0x13b78 ? 0x13b78 : .;
    */lv1ldr/src/auth_header.o(.text._ZN9sce_image12get_sectionsEv)
    . = . < 0x13b98 ? 0x13b98 : .;
    */lv1ldr/src/auth_header.o(.text._ZN9sce_image17get_section_countEPj)
    . = . < 0x13c08 ? 0x13c08 : .;
    */lv1ldr/src/auth_header.o(.text._ZN9sce_image13get_key_countEPj)
    . = . < 0x13c78 ? 0x13c78 : .;
    */lv1ldr/src/auth_header.o(.text._ZN9sce_image14get_ext_headerEPjS0_)
    . = . < 0x13cf0 ? 0x13cf0 : .;
    */lv1ldr/src/auth_check.o(.text._Z16check_sce_headerPKU8__vectorhPKvtj)
    . = . < 0x13e00 ? 0x13e00 : .;
    */lv1ldr/src/auth_check.o(.text._Z17check_meta_headerPKU8__vectorhPK9auth_metaj)
    . = . < 0x13f40 ? 0x13f40 : .;
    */lv1ldr/src/auth_check.o(.text._Z14check_sectionsP9sce_image)
    . = . < 0x140b0 ? 0x140b0 : .;
    */lv1ldr/src/auth_check.o(.text._Z18check_control_infoPK12auth_controly)
    . = . < 0x141d8 ? 0x141d8 : .;
    */lv1ldr/src/auth_check.o(.text._Z16check_ext_headerP9sce_image)
    . = . < 0x14810 ? 0x14810 : .;
    */lv1ldr/src/auth_check.o(.text._ZN9sce_image12get_app_infoEPj)
    . = . < 0x14878 ? 0x14878 : .;
    */lv1ldr/zlib/lv1ldr_inflate.o(.text.lv1ldr_inflate_end)
    . = . < 0x148b0 ? 0x148b0 : .;
    */lv1ldr/zlib/lv1ldr_inflate.o(.text.lv1ldr_inflate_init)
    . = . < 0x14918 ? 0x14918 : .;
    */lv1ldr/zlib/lv1ldr_inflate.o(.text.lv1ldr_inflate)
    . = . < 0x14a68 ? 0x14a68 : .;
    */lv1ldr/zlib/inflate.o(.text.inflateEnd)
    . = . < 0x14b08 ? 0x14b08 : .;
    */lv1ldr/zlib/inflate.o(.text.updatewindow)
    . = . < 0x14dc0 ? 0x14dc0 : .;
    */lv1ldr/zlib/inflate.o(.text.inflate)
    . = . < 0x16af0 ? 0x16af0 : .;
    */lv1ldr/zlib/inflate.o(.text.inflateInit_)
    . = . < 0x16cc0 ? 0x16cc0 : .;
    */lv1ldr/zlib/inftrees.o(.text.inflate_table)
    . = . < 0x17520 ? 0x17520 : .;
    */lv1ldr/zlib/adler32.o(.text.adler32)
    . = . < 0x17c48 ? 0x17c48 : .;
    */lv1ldr/zlib/zutil.o(.text.zcfree)
    . = . < 0x17c50 ? 0x17c50 : .;
    */lv1ldr/zlib/zutil.o(.text.zcalloc)
    . = . < 0x17c58 ? 0x17c58 : .;
    */lv1ldr/zlib/inffast.o(.text.inflate_fast)
    . = . < 0x185b0 ? 0x185b0 : .;
    */lv0ldr/libgcc/divdi3.o(.text)
    . = . < 0x190e8 ? 0x190e8 : .;
    */lv0ldr/libgcc/moddi3.o(.text)
    . = . < 0x19c80 ? 0x19c80 : .;
    */lv0ldr/libgcc/udivdi3.o(.text)
    . = . < 0x1a738 ? 0x1a738 : .;
    */lv0ldr/libgcc/umoddi3.o(.text)
    *(.text .text.* .gnu.linkonce.t.*)
  } =0x00200000

  .rodata :
  {
    */lv1ldr/src/loader.o(.rodata.cst16)
    */lv1ldr/src/mmio_access.o(.rodata.cst16)
    */lv1ldr/src/loader_data.o(.rodata.ch72_run_cmd)
    */lv1ldr/src/loader_data.o(.rodata.params_max_size)
    KEEP(*/lv1ldr/src/loader_data.o(.rodata.lv1_curve))
    *(.rodata._ZTV20aes256_cbc_encryptor)
    *(.rodata._ZTV9encryptor)
    KEEP(*/lv1ldr/src/loader_data.o(.rodata.lv1_pub))
    */lv1ldr/src/loader_data.o(.rodata.ch72_payload_mask)
    */lv1ldr/src/loader_data.o(.rodata.wb_aes_rk)
    */lv1ldr/src/loader_data.o(.rodata.ch72_payload_seed)
    */lv1ldr/src/loader_data.o(.rodata.params_wrap_0)
    */lv1ldr/src/loader_data.o(.rodata.params_wrap_1)
    */lv1ldr/src/loader_data.o(.rodata.storage_key)
    */lv1ldr/src/loader_data.o(.rodata.storage_iv)
    */lv1ldr/src/loader_data.o(.rodata.storage_wrapped_0)
    */lv1ldr/src/loader_data.o(.rodata.storage_wrapped_1)
    */lv1ldr/src/loader_data.o(.rodata.storage_wrapped_2)
    */lv1ldr/src/loader_data.o(.rodata.storage_const_0)
    */lv1ldr/src/loader_data.o(.rodata.storage_const_1)
    */lv1ldr/src/loader_data.o(.rodata.storage_const_2)
    */lv1ldr/src/loader_data.o(.rodata.storage_const_3)
    */lv1ldr/src/loader_data.o(.rodata.qa_flag_bits)
    */lv1ldr/src/qa_flag.o(.rodata.str1.16)
    *(.rodata._ZTV17aes256_cbc_cipher)
    *(.rodata._ZTV6cipher)
    *(.rodata._ZTV14ecdsa_verifier)
    *(.rodata._ZTV8verifier)
    */isoldr/src/qa_flag_data.o(.rodata.qa_flag_pub)
    */lv1ldr/src/sb_product.o(.rodata.cst16)
    */lv1ldr/src/init_device.o(.rodata.str1.16)
    */lv1ldr/src/storage.o(.rodata.xseed)
    */lv1ldr/encdec/encdec.o(.rodata.cst16)
    */lv1ldr/encdec/encdec.o(.rodata.str1.16)
    */lv1ldr/encdec/activate.o(.rodata.cst16)
    */lv1ldr/encdec/activate.o(.rodata.str1.16)
    *(.rodata._ZTV6encdec)
    */lv1ldr/src/printf.o(.rodata.str1.16)
    */lv1ldr/src/printf.o(.rodata.cst16)
    */lv0ldr/src/util/shuffle_table.o(.rodata.shuffle_table)
    */lv1ldr/src/uart.o(.rodata.cst16)
    */lv1ldr/src/wb_aes_data.o(.rodata.wb_aes_table_0)
    */lv1ldr/src/wb_aes_data.o(.rodata.wb_aes_table_1)
    */lv1ldr/src/wb_aes_data.o(.rodata.wb_aes_table_2)
    */lv1ldr/src/params_data.o(.rodata.params_seed)
    */lv1ldr/src/params_data.o(.rodata.params_pub)
    */lv1ldr/src/params_data.o(.rodata.params_curve)
    */lv1ldr/src/authenticator.o(.rodata.cst16)
    *(.rodata._ZTV17aes128_ctr_cipher)
    */lv1ldr/src/auth_buffers.o(.rodata.cst16)
    *(.rodata._ZTV10dma_buffer)
    *(.rodata._ZTV17tagged_dma_buffer)
    */crypto/*.o(.rodata .rodata.*)
    */lv1ldr/src/auth_data.o(.rodata.ch72_mac_data_0)
    */lv1ldr/src/auth_data.o(.rodata.ch72_mac_data_1)
    */lv1ldr/src/auth_data.o(.rodata.ch72_cbc_iv_0)
    */lv1ldr/src/auth_data.o(.rodata.ch72_cbc_iv_1)
    */lv1ldr/src/auth_data.o(.rodata.key_slots)
    */lv1ldr/src/auth_data.o(.rodata.digest_slots)
    */lv1ldr/src/auth_data.o(.rodata.sig_sizes)
    */lv1ldr/src/auth_check.o(.rodata.cst16)
    */lv1ldr/src/auth_data.o(.rodata.control_digest)
    */lv1ldr/zlib/lv1ldr_inflate.o(.rodata .rodata.*)
    */lv1ldr/zlib/inflate.o(.rodata .rodata.*)
    */lv1ldr/zlib/inftrees.o(.rodata .rodata.*)
    */lv1ldr/src/init_device_data.o(.rodata.sb_mmio_base)
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80) + (. & (0x80 - 1));
  .data :
  {
    KEEP(*/lv1ldr/src/loader_data.o(.data.lv1_key))
    KEEP(*/lv1ldr/src/loader_data.o(.data.lv1_iv))
    */lv0ldr/src/dma/dma.o(.data._ZZ8dma_waitvE5first)
    */isoldr/src/qa_flag_data.o(.data.qa_flag_hmac_key)
    */isoldr/src/qa_flag_data.o(.data.qa_flag_key)
    */isoldr/src/qa_flag_data.o(.data.qa_flag_iv)
    */isoldr/src/qa_flag_data.o(.data.qa_flag_curve)
    */lv1ldr/src/loader_data.o(.data.request_token)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek1_dx)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek2_dx)
    */lv1ldr/src/init_device_data.o(.data.encdec_key_dx)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek1_2)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek2_2)
    */lv1ldr/src/init_device_data.o(.data.encdec_key_2)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek1_3)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek2_3)
    */lv1ldr/src/init_device_data.o(.data.encdec_key_3)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek1_4)
    */lv1ldr/src/init_device_data.o(.data.encdec_kek2_4)
    */lv1ldr/src/init_device_data.o(.data.encdec_key_4)
    */lv1ldr/src/storage.o(.data.g_xkey)
    */lv0ldr/src/log/log.o(.data.g_log_threshold)
    */lv1ldr/src/auth_transfer.o(.data.g_inf_end)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .ctors : { KEEP(*(.ctors)) __CTOR_END__ = .; }
  .dtors : { KEEP(*(.dtors)) }

  /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame) }
}
