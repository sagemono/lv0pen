ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x110 ? 0x110 : .;
    */sc_iso/src/main.o(.text.main)
    . = . < 0x698 ? 0x698 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_cryptoC1Ei)
    . = . < 0x6b0 ? 0x6b0 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_cryptoD1Ev)
    . = . < 0x6b8 ? 0x6b8 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto9xts_blockEPKhPhRKiS1_S1_S4_S1_RS0_)
    . = . < 0x898 ? 0x898 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto11cbc_decryptEPKhPhRKiS1_S4_S2_)
    . = . < 0x8f0 ? 0x8f0 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto11cbc_encryptEPKhPhRKiS1_S4_S2_)
    . = . < 0x948 ? 0x948 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto3ctrEPKhPhRKiS1_S4_S2_)
    . = . < 0x9a0 ? 0x9a0 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto5cryptEPKhRKiPhS3_S1_S3_S1_S3_S1_S3_RS0_)
    . = . < 0xbb8 ? 0xbb8 : .;
    */sc_iso/src/sc_crypto.o(.text._ZN9sc_crypto5cryptEPKhRKiPhS3_S1_S3_S4_S3_RS0_)
    . = . < 0xe00 ? 0xe00 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10copy_bytesEPhPKhj)
    . = . < 0xe48 ? 0xe48 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12bytes_differEPKhS1_i)
    . = . < 0xea8 ? 0xea8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10fill_bytesEPhhj)
    . = . < 0xed0 ? 0xed0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_moduleD1Ev)
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_moduleD2Ev)
    . = . < 0xed8 ? 0xed8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10get_randomEPhjRj)
    . = . < 0xf28 ? 0xf28 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9time_diffERKyS1_Ry)
    . = . < 0xfc8 ? 0xfc8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module17use_old_auth_keysEv)
    . = . < 0x1048 ? 0x1048 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_moduleC1EP15ss_iso_dma_dataRKPhRKjS4_S6_PKhS8_RKyS6_)
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_moduleC2EP15ss_iso_dma_dataRKPhRKjS4_S6_PKhS8_RKyS6_)
    . = . < 0x1128 ? 0x1128 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11xts_decryptEP9sc_cryptoPKhRKiPhS5_S3_S5_S3_S5_S3_S5_)
    . = . < 0x1160 ? 0x1160 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11xts_encryptEP9sc_cryptoPKhRKiPhS5_S3_S5_S3_S5_S3_S5_)
    . = . < 0x1198 ? 0x1198 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10make_tweakEPhjj)
    . = . < 0x1230 ? 0x1230 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module20pmode_make_challengeEPK8op32_msgPS0_PhjRj)
    . = . < 0x15f8 ? 0x15f8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module16get_syscon_stateEPh)
    . = . < 0x1b10 ? 0x1b10 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12authenticateEjPKhS1_Pv)
    . = . < 0x2180 ? 0x2180 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12open_sessionEv)
    . = . < 0x2208 ? 0x2208 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module8exec_cmdEP6sc_cmdh)
    . = . < 0x26d8 ? 0x26d8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10exec_plainEP6sc_cmd)
    . = . < 0x26e0 ? 0x26e0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12read_sc_infoEPhRj)
    . = . < 0x27d8 ? 0x27d8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module14get_sc_versionEPh)
    . = . < 0x2850 ? 0x2850 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module16cmd_get_propertyERKPhRKjS2_S4_RjS5_)
    . = . < 0x2a40 ? 0x2a40 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module4execEP6sc_cmd)
    . = . < 0x2a50 ? 0x2a50 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15derive_auth_keyEPhPKh)
    . = . < 0x2ae8 ? 0x2ae8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module16derive_auth_keysEv)
    . = . < 0x2bc8 ? 0x2bc8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module14set_state_byteERKhRj)
    . = . < 0x2cb8 ? 0x2cb8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module14get_state_byteERhRj)
    . = . < 0x2de0 ? 0x2de0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module20install_key_materialERKjS1_Rj)
    . = . < 0x2ee0 ? 0x2ee0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15set_time_offsetERKyRKxRj)
    . = . < 0x2fe0 ? 0x2fe0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9read_timeERKyRyS2_Rj)
    . = . < 0x30f8 ? 0x30f8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12cmd_set_timeERKPhRKjS2_S4_RjS5_)
    . = . < 0x33f8 ? 0x33f8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12cmd_get_timeERKPhRKjS2_S4_RjS5_)
    . = . < 0x38b0 ? 0x38b0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module20pmode_check_responseEPK8op33_msgPS0_PhjRj)
    . = . < 0x3c78 ? 0x3c78 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module18cmd_pmode_srv_authERKPhRKjS2_S4_RjS5_)
    . = . < 0x3fa0 ? 0x3fa0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9cmd_rekeyERKPhRKjS2_S4_RjS5_)
    . = . < 0x4230 ? 0x4230 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cbc_decryptEP9sc_cryptoPKhRKiPhS5_S3_S5_S6_S5_)
    . = . < 0x4268 ? 0x4268 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cbc_encryptEP9sc_cryptoPKhRKiPhS5_S3_S5_S6_S5_)
    . = . < 0x42a0 ? 0x42a0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12encrypt_seedERKPhRKjPKh)
    . = . < 0x4458 ? 0x4458 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module7get_keyERKyRKPhRj)
    . = . < 0x4678 ? 0x4678 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15decrypt_sectorsERKPhRKjS4_)
    . = . < 0x4928 ? 0x4928 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module14read_slot_dataERKPhRKjS4_S4_Rj)
    . = . < 0x4ab0 ? 0x4ab0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9read_dataERKPhRKjS4_Rj)
    . = . < 0x4c48 ? 0x4c48 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11read_recordERKyRKPhRKj)
    . = . < 0x4d90 ? 0x4d90 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module19cmd_get_region_dataERKPhRKjS2_S4_RjS5_)
    . = . < 0x4fa0 ? 0x4fa0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module8read_srhERKPhRKj)
    . = . < 0x5080 ? 0x5080 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cmd_get_srhERKPhRKjS2_S4_RjS5_)
    . = . < 0x5220 ? 0x5220 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11read_mngblkEPv)
    . = . < 0x52a0 ? 0x52a0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module17cmd_get_sc_statusERKPhRKjS2_S4_RjS5_)
    . = . < 0x54c0 ? 0x54c0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15encrypt_sectorsERKPhRKjS4_)
    . = . < 0x5770 ? 0x5770 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15write_slot_dataERKPhRKjS4_S4_Rj)
    . = . < 0x5aa0 ? 0x5aa0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10write_areaEPKhRKjS3_S3_)
    . = . < 0x5bd0 ? 0x5bd0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module8patch_4kERKP11sc_data_hdrRKjRKPhS5_RjS9_)
    . = . < 0x5d48 ? 0x5d48 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_)
    . = . < 0x5f00 ? 0x5f00 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10write_dataERKPhRKjS4_Rj)
    . = . < 0x6230 ? 0x6230 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12write_recordERKyRKPhRKj)
    . = . < 0x6378 ? 0x6378 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module19cmd_set_region_dataERKPhRKjS2_S4_RjS5_)
    . = . < 0x6590 ? 0x6590 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9write_srhERKPhRKj)
    . = . < 0x6688 ? 0x6688 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cmd_set_srhERKPhRKjS2_S4_RjS5_)
    . = . < 0x67f0 ? 0x67f0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module9write_srkERKyRKPhRKj)
    . = . < 0x6900 ? 0x6900 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12write_mngblkEPv)
    . = . < 0x6980 ? 0x6980 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module17cmd_set_sc_statusERKPhRKjS2_S4_RjS5_)
    . = . < 0x6b40 ? 0x6b40 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module13erase_recordsERj)
    . = . < 0x6cc0 ? 0x6cc0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module17cmd_reset_recordsERKPhRKjS2_S4_RjS5_)
    . = . < 0x6ee0 ? 0x6ee0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module21cmd_restore_root_infoERKPhRKjS2_S4_RjS5_)
    . = . < 0x72b8 ? 0x72b8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module20cmd_backup_root_infoERKPhRKjS2_S4_RjS5_)
    . = . < 0x75c0 ? 0x75c0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module15derive_srk2_keyERKPhRKj)
    . = . < 0x75c8 ? 0x75c8 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module12cmd_init_srkERKPhRKjS2_S4_RjS5_)
    . = . < 0x7838 ? 0x7838 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module10crypt_dataERKiRKhRKPhRKjS6_S8_RjS9_)
    . = . < 0x7d80 ? 0x7d80 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cmd_decryptERKPhRKjS2_S4_RjS5_)
    . = . < 0x7df0 ? 0x7df0 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module11cmd_encryptERKPhRKjS2_S4_RjS5_)
    . = . < 0x7e60 ? 0x7e60 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module19cmd_sc_binary_patchERKPhRKjS2_S4_RjS5_)
    . = . < 0x8040 ? 0x8040 : .;
    */sc_iso/src/sc_iso_module.o(.text._ZN13sc_iso_module3runERjRPhS0_)
    . = . < 0x8648 ? 0x8648 : .;
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdrC1Ev)
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdrC2Ev)
    . = . < 0x8650 ? 0x8650 : .;
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdrD1Ev)
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdrD2Ev)
    . = . < 0x8658 ? 0x8658 : .;
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdr10verify_hdrERKPhRKjRjS5_)
    . = . < 0x8760 ? 0x8760 : .;
    */sc_iso/src/sc_proxy_hdr.o(.text._ZN12sc_proxy_hdr8make_hdrERKjS1_RKPhS1_)
    . = . < 0x8830 ? 0x8830 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmdC2Ev)
    . = . < 0x8848 ? 0x8848 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmd3setERKjS1_)
    . = . < 0x8850 ? 0x8850 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmd4makeERKPhRKjRjS5_S4_Pv)
    . = . < 0x8858 ? 0x8858 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmd5checkERKPhRKjS4_Pv)
    . = . < 0x8860 ? 0x8860 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmd3getERKPhRKjRj)
    . = . < 0x8868 ? 0x8868 : .;
    */sc_iso/src/sc_cmd.o(.text._ZN6sc_cmdD2Ev)
    . = . < 0x8880 ? 0x8880 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_083setERKjS1_)
    . = . < 0x88b0 ? 0x88b0 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_083getERKPhRKjRj)
    . = . < 0x8978 ? 0x8978 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_08D0Ev)
    . = . < 0x89c0 ? 0x89c0 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_08D1Ev)
    . = . < 0x89d8 ? 0x89d8 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_08C1ERKj)
    . = . < 0x8a70 ? 0x8a70 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_08C1Ev)
    . = . < 0x8af8 ? 0x8af8 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_084makeERKPhRKjRjS5_S4_Pv)
    . = . < 0x8cb0 ? 0x8cb0 : .;
    */sc_iso/src/sc_cmd_08.o(.text._ZN9sc_cmd_085checkERKPhRKjS4_Pv)
    . = . < 0x8f40 ? 0x8f40 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0d3getERj)
    . = . < 0x8f70 ? 0x8f70 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0d3setERKPhRKjS4_)
    . = . < 0x8fc0 ? 0x8fc0 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0dD0Ev)
    . = . < 0x9008 ? 0x9008 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0dD1Ev)
    . = . < 0x9020 ? 0x9020 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0dC1ERKj)
    . = . < 0x90b8 ? 0x90b8 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0dC1Ev)
    . = . < 0x9140 ? 0x9140 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0d4makeERKPhRKjRjS5_S4_Pv)
    . = . < 0x9300 ? 0x9300 : .;
    */sc_iso/src/sc_cmd_0d.o(.text._ZN9sc_cmd_0d5checkERKPhRKjS4_Pv)
    . = . < 0x95a0 ? 0x95a0 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_073getERyS0_)
    . = . < 0x95e8 ? 0x95e8 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_075checkERKPhRKjS4_Pv)
    . = . < 0x97f0 ? 0x97f0 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_074makeERKPhRKjRjS5_S4_Pv)
    . = . < 0x9958 ? 0x9958 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_073setERKy)
    . = . < 0x9a60 ? 0x9a60 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_07D0Ev)
    . = . < 0x9aa8 ? 0x9aa8 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_07D1Ev)
    . = . < 0x9ac0 ? 0x9ac0 : .;
    */sc_iso/src/sc_cmd_07.o(.text._ZN9sc_cmd_07C1Ev)
    . = . < 0x9b38 ? 0x9b38 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_093getEv)
    . = . < 0x9b40 ? 0x9b40 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_095checkERKPhRKjS4_Pv)
    . = . < 0x9cc0 ? 0x9cc0 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_094makeERKPhRKjRjS5_S4_Pv)
    . = . < 0x9e70 ? 0x9e70 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_093setERKyRKx)
    . = . < 0x9fe8 ? 0x9fe8 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_09D0Ev)
    . = . < 0xa030 ? 0xa030 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_09D1Ev)
    . = . < 0xa048 ? 0xa048 : .;
    */sc_iso/src/sc_cmd_09.o(.text._ZN9sc_cmd_09C1Ev)
    . = . < 0xa0b0 ? 0xa0b0 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0b3getEv)
    . = . < 0xa0b8 ? 0xa0b8 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0b5checkERKPhRKjS4_Pv)
    . = . < 0xa1b8 ? 0xa1b8 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0b4makeERKPhRKjRjS5_S4_Pv)
    . = . < 0xa318 ? 0xa318 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0b3setERKjS1_)
    . = . < 0xa3c8 ? 0xa3c8 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0bD0Ev)
    . = . < 0xa410 ? 0xa410 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0bD1Ev)
    . = . < 0xa428 ? 0xa428 : .;
    */sc_iso/src/sc_cmd_0b.o(.text._ZN9sc_cmd_0bC1Ev)
    . = . < 0xa478 ? 0xa478 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_053setEv)
    . = . < 0xa480 ? 0xa480 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_053getERhS0_)
    . = . < 0xa4d0 ? 0xa4d0 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_055checkERKPhRKjS4_Pv)
    . = . < 0xa648 ? 0xa648 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_054makeERKPhRKjRjS5_S4_Pv)
    . = . < 0xa7a0 ? 0xa7a0 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_05D0Ev)
    . = . < 0xa7e8 ? 0xa7e8 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_05D1Ev)
    . = . < 0xa800 ? 0xa800 : .;
    */sc_iso/src/sc_cmd_05.o(.text._ZN9sc_cmd_05C1Ev)
    . = . < 0xa868 ? 0xa868 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0c3setERKh)
    . = . < 0xa898 ? 0xa898 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0c3getEv)
    . = . < 0xa8a0 ? 0xa8a0 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0c5checkERKPhRKjS4_Pv)
    . = . < 0xa9a0 ? 0xa9a0 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0c4makeERKPhRKjRjS5_S4_Pv)
    . = . < 0xab10 ? 0xab10 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0cD0Ev)
    . = . < 0xab58 ? 0xab58 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0cD1Ev)
    . = . < 0xab70 ? 0xab70 : .;
    */sc_iso/src/sc_cmd_0c.o(.text._ZN9sc_cmd_0cC1Ev)
    . = . < 0xabc8 ? 0xabc8 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_043setEv)
    . = . < 0xabd0 ? 0xabd0 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_043getEPh)
    . = . < 0xac00 ? 0xac00 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_045checkERKPhRKjS4_Pv)
    . = . < 0xad20 ? 0xad20 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_044makeERKPhRKjRjS5_S4_Pv)
    . = . < 0xae68 ? 0xae68 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_04D0Ev)
    . = . < 0xaeb0 ? 0xaeb0 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_04D1Ev)
    . = . < 0xaec8 ? 0xaec8 : .;
    */sc_iso/src/sc_cmd_04.o(.text._ZN9sc_cmd_04C1Ev)
    . = . < 0xaf18 ? 0xaf18 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data7alignedEv)
    . = . < 0xaf60 ? 0xaf60 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data5chunkERKjRj)
    . = . < 0xaff0 ? 0xaff0 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data6notifyERKj)
    . = . < 0xb068 ? 0xb068 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD1Ev)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataD2Ev)
    . = . < 0xb070 ? 0xb070 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC1ERKyRKjRKPhS3_)
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_dataC2ERKyRKjRKPhS3_)
    . = . < 0xb110 ? 0xb110 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data3getERKPhRKjRjS5_S5_)
    . = . < 0xb628 ? 0xb628 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data6finishERKjS1_S1_RKPh)
    . = . < 0xb8e8 ? 0xb8e8 : .;
    */sc_iso/src/ss_iso_dma_data.o(.text._ZN15ss_iso_dma_data3putERKjS1_S1_RKPh)
    . = . < 0xbb58 ? 0xbb58 : .;
    */aim_spu_module/src/dma_log.o(.text._Z7get_logv)
    . = . < 0xbb60 ? 0xbb60 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log9top_powerEy)
    . = . < 0xbc00 ? 0xbc00 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log4putsEPKc)
    . = . < 0xbd78 ? 0xbd78 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log9put_digitEj)
    . = . < 0xbe68 ? 0xbe68 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log10put_numberEPKviji)
    . = . < 0xc0e8 ? 0xc0e8 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log3putEhj)
    . = . < 0xc128 ? 0xc128 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log3putEij)
    . = . < 0xc190 ? 0xc190 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log3putEjj)
    . = . < 0xc1d0 ? 0xc1d0 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log3putEyj)
    . = . < 0xc210 ? 0xc210 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log5resetEv)
    . = . < 0xc270 ? 0xc270 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log10set_bufferERKyRKj)
    . = . < 0xc288 ? 0xc288 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_log4initERKyRKj)
    . = . < 0xc3f8 ? 0xc3f8 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logD1Ev)
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logD2Ev)
    . = . < 0xc400 ? 0xc400 : .;
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logC1Ev)
    */aim_spu_module/src/dma_log.o(.text._ZN7dma_logC2Ev)
    . = . < 0xc448 ? 0xc448 : .;
    */aim_spu_module/src/dma_log.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xc480 ? 0xc480 : .;
    */aim_spu_module/src/dma_log.o(.text._GLOBAL__D_g_log_ptr)
    . = . < 0xc490 ? 0xc490 : .;
    */aim_spu_module/src/dma_log.o(.text._GLOBAL__I_g_log_ptr)
    . = . < 0xc4a0 ? 0xc4a0 : .;
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdrC1ERKh)
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdrC2ERKh)
    . = . < 0xc560 ? 0xc560 : .;
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdrD1Ev)
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdrD2Ev)
    . = . < 0xc568 ? 0xc568 : .;
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdr10verify_hdrEPKhRKjRj)
    . = . < 0xc7f8 ? 0xc7f8 : .;
    */sc_iso/src/sc_pkt_hdr.o(.text._ZN10sc_pkt_hdr8make_hdrEPhRKjS2_)
    . = . < 0xc938 ? 0xc938 : .;
    */lv0ldr/crypto/aes_set_encrypt_key.o(.text)
    . = . < 0xcd38 ? 0xcd38 : .;
    */lv0ldr/crypto/aes_set_decrypt_key.o(.text)
    . = . < 0xce50 ? 0xce50 : .;
    */lv0ldr/crypto/aes_encrypt_block.o(.text)
    . = . < 0xcff8 ? 0xcff8 : .;
    */lv0ldr/crypto/aes_decrypt_block.o(.text)
    . = . < 0xd1e8 ? 0xd1e8 : .;
    */lv0ldr/crypto/aes_cbc_encrypt.o(.text)
    . = . < 0xd358 ? 0xd358 : .;
    */lv0ldr/crypto/aes_cbc_decrypt.o(.text)
    . = . < 0xd658 ? 0xd658 : .;
    */lv0ldr/crypto/aes_ctr.o(.text)
    . = . < 0xd9e8 ? 0xd9e8 : .;
    */isoldr/crypto/aes_cmac.o(.text)
    . = . < 0xdbf8 ? 0xdbf8 : .;
    */lv0ldr/crypto/sha1_digest.o(.text)
    . = . < 0xdc88 ? 0xdc88 : .;
    */lv0ldr/crypto/aes_encrypt8.o(.text)
    . = . < 0xe568 ? 0xe568 : .;
    */lv0ldr/crypto/aes_decrypt8.o(.text)
    . = . < 0xf008 ? 0xf008 : .;
    */lv0ldr/crypto/sha1.o(.text)
    . = . < 0xf640 ? 0xf640 : .;
    */lv0ldr/crypto/sha1_transform.o(.text)
    . = . < 0x104b8 ? 0x104b8 : .;
    */lv0ldr/crypto/ecdsa_id.o(.text)
    . = . < 0x105d8 ? 0x105d8 : .;
    */lv0ldr/crypto/ec_curves.o(.text)
    . = . < 0x106d0 ? 0x106d0 : .;
    */isoldr/crypto/ecdsa.o(.text)
    . = . < 0x10a88 ? 0x10a88 : .;
    */lv0ldr/crypto/ec.o(.text)
    . = . < 0x117b8 ? 0x117b8 : .;
    */lv0ldr/crypto/ec_mul2.o(.text)
    . = . < 0x119c0 ? 0x119c0 : .;
    */lv0ldr/crypto/bn_core.o(.text)
    . = . < 0x150a8 ? 0x150a8 : .;
    */lv0ldr/crypto/bn_in_range.o(.text)
    . = . < 0x15118 ? 0x15118 : .;
    */lv0ldr/crypto/ec_curve_valid.o(.text)
    . = . < 0x15128 ? 0x15128 : .;
    */lv0ldr/crypto/bn_from_bytes_not.o(.text)
    . = . < 0x15228 ? 0x15228 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x15240 ? 0x15240 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel13in_mbox_countERj)
    . = . < 0x15260 ? 0x15260 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x15308 ? 0x15308 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x15338 ? 0x15338 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x15350 ? 0x15350 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x15368 ? 0x15368 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel19write_out_intr_mboxEj)
    . = . < 0x15380 ? 0x15380 : .;
    */spp_verifier/src/runtime/dma_mailbox.o(.text._ZN11dma_channel12read_in_mboxERj)
    . = . < 0x153b8 ? 0x153b8 : .;
    */sc_iso/src/cxa_pure_virtual.o(.text.__cxa_pure_virtual)
    . = . < 0x153c0 ? 0x153c0 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x153f0 ? 0x153f0 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._GLOBAL__I_g_secure_com)
    . = . < 0x15400 ? 0x15400 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z12sc_lib_errorv)
    . = . < 0x15408 ? 0x15408 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_04PK6sc_bufPh)
    . = . < 0x15418 ? 0x15418 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_04PK6sc_buf)
    . = . < 0x15428 ? 0x15428 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z25sc_validate_get_state_cmdPK6sc_bufPh)
    . = . < 0x15438 ? 0x15438 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z23sc_create_get_state_cmdPK6sc_buf)
    . = . < 0x15448 ? 0x15448 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_0bPK6sc_bufjPKv)
    . = . < 0x15460 ? 0x15460 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_0bPK6sc_bufjPKvPKh)
    . = . < 0x15480 ? 0x15480 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_0cPK6sc_bufjPKv)
    . = . < 0x15498 ? 0x15498 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_0cPK6sc_bufjPKvh)
    . = . < 0x154b8 ? 0x154b8 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_05PK6sc_bufjPKvPhS4_)
    . = . < 0x154e8 ? 0x154e8 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_05PK6sc_bufjPKv)
    . = . < 0x15500 ? 0x15500 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_0dPK6sc_bufjPKvPjS4_S4_)
    . = . < 0x15538 ? 0x15538 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_0dPK6sc_bufjPKvjjPKhjj)
    . = . < 0x15580 ? 0x15580 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_08PK6sc_bufjPKvPjS4_S4_Phj)
    . = . < 0x155c8 ? 0x155c8 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_08PK6sc_bufjPKvjjj)
    . = . < 0x15600 ? 0x15600 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_07PK6sc_bufjPKvPhPj)
    . = . < 0x15630 ? 0x15630 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_07PK6sc_bufjPKvh)
    . = . < 0x15650 ? 0x15650 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z11sc_check_09PK6sc_bufjPKvPh)
    . = . < 0x15670 ? 0x15670 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z10sc_make_09PK6sc_bufjPKvhjjj)
    . = . < 0x156b0 ? 0x156b0 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z14sc_check_auth2PK6sc_bufjjPKhPKvPh)
    . = . < 0x156e8 ? 0x156e8 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z13sc_make_auth2PK6sc_bufjPKvPKh)
    . = . < 0x15708 ? 0x15708 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z14sc_check_auth1PK6sc_bufjPKvPKhPh)
    . = . < 0x15738 ? 0x15738 : .;
    */sc_iso/src/secure_com/secure_com.o(.text._Z13sc_make_auth1PK6sc_bufjPKvPKh)
    . = . < 0x15758 ? 0x15758 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib9get_errorEv)
    . = . < 0x15760 ? 0x15760 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib10unwrap_cmdEjjjPKv)
    . = . < 0x15910 ? 0x15910 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib6unwrapEjjj)
    . = . < 0x15ab8 ? 0x15ab8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib8wrap_cmdEjjjPKv)
    . = . < 0x15c68 ? 0x15c68 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib7receiveEPK6sc_bufjjj)
    . = . < 0x15d28 ? 0x15d28 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib11receive_cmdEPK6sc_bufjjjPKv)
    . = . < 0x15dc8 ? 0x15dc8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_0b_cmdEPK6sc_bufjPKv)
    . = . < 0x15e00 ? 0x15e00 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_0c_cmdEPK6sc_bufjPKv)
    . = . < 0x15e38 ? 0x15e38 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_05_cmdEPK6sc_bufjPKvPhS5_)
    . = . < 0x15ed8 ? 0x15ed8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_0d_cmdEPK6sc_bufjPKvPjS5_S5_)
    . = . < 0x16000 ? 0x16000 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_07_cmdEPK6sc_bufjPKvPhPj)
    . = . < 0x160d0 ? 0x160d0 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_09_cmdEPK6sc_bufjPKvPh)
    . = . < 0x16150 ? 0x16150 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_04_cmdEPK6sc_bufPh)
    . = . < 0x161c8 ? 0x161c8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib22validate_get_state_cmdEPK6sc_bufPh)
    . = . < 0x16240 ? 0x16240 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib18validate_auth2_cmdEPK6sc_bufjjPKhPKvPh)
    . = . < 0x163c8 ? 0x163c8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib15validate_08_cmdEPK6sc_bufjPKvPjS5_S5_Phj)
    . = . < 0x16568 ? 0x16568 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib18validate_auth1_cmdEPK6sc_bufjPKvPKhPh)
    . = . < 0x16660 ? 0x16660 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib4wrapEjjj)
    . = . < 0x16788 ? 0x16788 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib4sendEPK6sc_bufjjj)
    . = . < 0x16840 ? 0x16840 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib8send_cmdEPK6sc_bufjjjPKv)
    . = . < 0x168d8 ? 0x168d8 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_0b_cmdEPK6sc_bufjPKvPKh)
    . = . < 0x16950 ? 0x16950 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_0c_cmdEPK6sc_bufjPKvh)
    . = . < 0x16978 ? 0x16978 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_05_cmdEPK6sc_bufjPKv)
    . = . < 0x16990 ? 0x16990 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_0d_cmdEPK6sc_bufjPKvjjPKhjj)
    . = . < 0x16b30 ? 0x16b30 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_08_cmdEPK6sc_bufjPKvjjj)
    . = . < 0x16c18 ? 0x16c18 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_07_cmdEPK6sc_bufjPKvh)
    . = . < 0x16c40 ? 0x16c40 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib7send_09EPK6sc_bufjPKvhPKh)
    . = . < 0x16d08 ? 0x16d08 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_09_cmdEPK6sc_bufjPKvhjjj)
    . = . < 0x16f00 ? 0x16f00 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib13create_04_cmdEPK6sc_buf)
    . = . < 0x16f10 ? 0x16f10 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib20create_get_state_cmdEPK6sc_buf)
    . = . < 0x16f20 ? 0x16f20 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib16create_auth2_cmdEPK6sc_bufjPKvPKh)
    . = . < 0x16fb0 ? 0x16fb0 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_lib16create_auth1_cmdEPK6sc_bufjPKvPKh)
    . = . < 0x17040 ? 0x17040 : .;
    */sc_iso/src/secure_com/secure_com_lib.o(.text._ZN14secure_com_libC1Ev)
    . = . < 0x170c0 ? 0x170c0 : .;
    */lv1ldr/src/storage.o(.text._Z9prng_seedv)
    . = . < 0x170f8 ? 0x170f8 : .;
    */lv1ldr/src/storage.o(.text._Z11prng_randomPh)
    . = . < 0x17120 ? 0x17120 : .;
    */lv1ldr/crypto/prng.o(.text)
    . = . < 0x174a0 ? 0x174a0 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x174a8 ? 0x174a8 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x174d0 ? 0x174d0 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x17660 ? 0x17660 : .;
    */lv0ldr/src/util/memcmp.o(.text.memcmp)
    . = . < 0x17720 ? 0x17720 : .;
    */spp_verifier/src/runtime/memcpy.o(.text.memcpy)
    . = . < 0x178a8 ? 0x178a8 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x17998 ? 0x17998 : .;
    */spp_verifier/src/runtime/strlen.o(.text.strlen)
    . = . < 0x17a10 ? 0x17a10 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    . = . < 0x17a88 ? 0x17a88 : .;
    */lv0ldr/libgcc/libgcc2.o(.text)
    *(.text .text.* .gnu.linkonce.t.*)
    . = . < 0x19008 ? 0x19008 : .;
  }

  .rodata :
  {
    */spu_token_processor/src/crt0.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/main.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_crypto.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_iso_module.o(.rodata.cst16 .rodata.str1.16)
    */sc_iso/src/sc_iso_module.o(.rodata.key_id_07)
    */sc_iso/src/sc_iso_module.o(.rodata.key_id_08)
    */sc_iso/src/sc_iso_module.o(.rodata.s_id_6)
    */sc_iso/src/sc_iso_module.o(.rodata.s_16)
    */sc_iso/src/sc_iso_module.o(.rodata.s_id_1)
    */sc_iso/src/sc_iso_module.o(.rodata.mngblk_offset)
    */sc_iso/src/sc_iso_module.o(.rodata.mngblk_zero)
    */sc_iso/src/sc_iso_module.o(.rodata.g_op33_key)
    */sc_iso/src/sc_iso_module.o(.rodata.g_op33_iv)
    */sc_iso/src/sc_iso_module.o(.rodata.g_op33_pub)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module8patch_4kERKP11sc_data_hdrRKjRKPhS5_RjS9_E6s_4096)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module8patch_4kERKP11sc_data_hdrRKjRKPhS5_RjS9_E5s_130)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module8patch_4kERKP11sc_data_hdrRKjRKPhS5_RjS9_E3s_0)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_E6s_1024)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_E6s_3072)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_E5s_130)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_E5s_129)
    */sc_iso/src/sc_iso_module.o(.rodata._ZZN13sc_iso_module11patch_areasERKP11sc_data_hdrRKjRKPhS5_RjS9_E3s_0)
    */sc_iso/src/sc_iso_module.o(.rodata.g_mode_ff)
    */sc_iso/src/sc_iso_module.o(.rodata.s_service)
    */sc_iso/src/sc_iso_module.o(.rodata.s_ready)
    */sc_iso/src/sc_proxy_hdr.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_08.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_0d.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_07.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_09.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_0b.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_05.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_0c.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_cmd_04.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/ss_iso_dma_data.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */aim_spu_module/src/dma_log.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/sc_pkt_hdr.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_set_encrypt_key.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_set_decrypt_key.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_encrypt_block.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_decrypt_block.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_cbc_encrypt.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_cbc_decrypt.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_ctr.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */isoldr/crypto/aes_cmac.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/sha1_digest.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_encrypt8.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/aes_decrypt8.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/sha1.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/sha1_transform.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ecdsa_id.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ec_curves.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */isoldr/crypto/ecdsa.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ec.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ec_mul2.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/bn_core.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/bn_in_range.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ec_curve_valid.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/bn_from_bytes_not.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/crypto/ec_curve_table.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/dma_channel.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/dma_mailbox.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/cxa_pure_virtual.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/secure_com/secure_com.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */sc_iso/src/secure_com/secure_com_lib.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv1ldr/src/storage.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv1ldr/crypto/prng.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/delete.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/exit.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/free.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/heap.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/src/util/memcmp.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/memcpy.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/memset.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/strlen.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */spp_verifier/src/runtime/atexit.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/libgcc/udivdi3.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/libgcc/umoddi3.o(.rodata .rodata.* .gnu.linkonce.r.*)
    */lv0ldr/src/util/shuffle_table.o(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80);
  .ctors : { KEEP(*(.ctors)) KEEP(*(.ctors.end)) }
  .dtors : { KEEP(*(.dtors)) KEEP(*(.dtors.end)) }
  .data :
  {
    */spu_token_processor/src/crt0.o(.data)
    */sc_iso/src/sc_iso_module.o(.data.g_116144)
    */sc_iso/src/sc_iso_module.o(.data.g_116160)
    */sc_iso/src/sc_iso_module.o(.data.g_116176)
    */sc_iso/src/sc_iso_module.o(.data.g_116192)
    */sc_iso/src/sc_iso_module.o(.data.g_116208)
    */sc_iso/src/sc_iso_module.o(.data.g_116224)
    */sc_iso/src/sc_iso_module.o(.rodata.key_2)
    */sc_iso/src/sc_iso_module.o(.rodata.key_3)
    */sc_iso/src/sc_iso_module.o(.rodata.key_4)
    */sc_iso/src/sc_iso_module.o(.rodata.seed_5)
    */sc_iso/src/sc_iso_module.o(.rodata.seed_6)
    */sc_iso/src/sc_iso_module.o(.rodata.seed_7)
    */sc_iso/src/sc_iso_module.o(.rodata.seed_8)
    */sc_iso/src/sc_iso_module.o(.data.g_116352)
    */sc_iso/src/sc_iso_module.o(.data.g_116368)
    */sc_iso/src/sc_iso_module.o(.data.g_116384)
    */sc_iso/src/sc_iso_module.o(.data.g_116400)
    */sc_iso/src/sc_iso_module.o(.data.g_116416)
    */sc_iso/src/sc_iso_module.o(.data.g_116432)
    */sc_iso/src/sc_iso_module.o(.data.g_state_116448)
    */aim_spu_module/src/dma_log.o(.data.g_log_ptr)
    */sc_iso/src/secure_com/secure_com_lib.o(.data.g_hdr)
    */sc_iso/src/secure_com/secure_com_lib.o(.data.g_key_out)
    */sc_iso/src/secure_com/secure_com_lib.o(.data.g_key_in)
    */sc_iso/src/secure_com/secure_com_lib.o(.data.g_auth_keys)
    */sc_iso/src/secure_com/secure_com_lib.o(.data.g_09_keys)
    */lv1ldr/src/storage.o(.data.g_xkey)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */sc_iso/src/sc_iso_module.o(.bss.g_116752)
    */sc_iso/src/sc_iso_module.o(.bss.g_116768)
    */sc_iso/src/sc_iso_module.o(.bss.g_116784)
    */sc_iso/src/sc_iso_module.o(.bss.g_116800)
    */sc_iso/src/sc_iso_module.o(.bss.g_116816)
    */sc_iso/src/sc_iso_module.o(.bss.g_116832)
    */aim_spu_module/src/dma_log.o(.bss.g_log)
    */sc_iso/src/secure_com/secure_com.o(.bss.g_secure_com)
    */sc_iso/src/secure_com/secure_com_lib.o(.bss.g_iv)
    */sc_iso/src/secure_com/secure_com_lib.o(.bss.g_buf)
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
