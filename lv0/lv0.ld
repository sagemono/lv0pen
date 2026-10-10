OUTPUT_ARCH(powerpc:common64)
g_lv1ldr_report = 0x80cbd80;
lv0_image_start = 0x8000000;
lv1ldr_aes_iv = 0x1b8;
lv1ldr_aes_rk = 0x108;
SECTIONS
{
  .vec 0x0 : { */lv0/src/boot/start.o(.vec) }
  . = . < 0x8000000 ? 0x8000000 : .;
  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/boot/c_entry.o(.text.c_entry)
    . = . < 0x44 ? 0x44 : .;
    */lv0/src/boot/run_global_constructors.o(.text._Z23run_global_constructorsv)
    . = . < 0x94 ? 0x94 : .;
    */lv0/src/boot/hang_forever.o(.text._Z12hang_foreverv)
    . = . < 0x98 ? 0x98 : .;
    */lv0/src/clock/read_timebase.o(.text._Z13read_timebasev)
    . = . < 0xa8 ? 0xa8 : .;
    */lv0/src/clock/get_time_ms.o(.text._Z11get_time_msv)
    . = . < 0xd4 ? 0xd4 : .;
    */lv0/src/clock/get_time_us.o(.text._Z11get_time_usv)
    . = . < 0xfc ? 0xfc : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loader16apply_boot_orderEv)
    . = . < 0x164 ? 0x164 : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loader17load_component_toEjPKcPvm)
    . = . < 0x230 ? 0x230 : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0x358 ? 0x358 : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loaderD0Ev)
    . = . < 0x390 ? 0x390 : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loaderD1Ev)
    . = . < 0x39c ? 0x39c : .;
    */lv0/src/component/boot_order_component_loader.o(.text._ZN27boot_order_component_loaderC1EP16component_loaderS1_)
    . = . < 0x3f0 ? 0x3f0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager18pack_class0_statusEmPm)
    . = . < 0x46c ? 0x46c : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager19merge_class0_statusEmPmS0_)
    . = . < 0x4d0 ? 0x4d0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21handle_class2_stoppedEmmPlS0_S0_)
    . = . < 0x584 ? 0x584 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_managerD1Ev)
    . = . < 0x588 ? 0x588 : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z23read_eeprom_nv1_u32_at4Pj)
    . = . < 0x5d4 ? 0x5d4 : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z23read_eeprom_nv1_u16_at2Pt)
    . = . < 0x620 ? 0x620 : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z25write_eeprom_nv1_u32_at12j)
    . = . < 0x684 ? 0x684 : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z24write_eeprom_nv1_u32_at4j)
    . = . < 0x6e8 ? 0x6e8 : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z24write_eeprom_nv1_u16_at2t)
    . = . < 0x74c ? 0x74c : .;
    */lv0/src/storage/eeprom_nv1.o(.text._Z18finish_nv1_requestPvtj)
    . = . < 0x7e0 ? 0x7e0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager14send_ls_recordEm)
    . = . < 0x870 ? 0x870 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager14check_spu_idleEl)
    . = . < 0x8c4 ? 0x8c4 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager13load_qa_tokenEv)
    . = . < 0x9b0 ? 0x9b0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager19send_loader_addressEll)
    . = . < 0x9fc ? 0x9fc : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager15restore_mfc_sr1El)
    . = . < 0xa44 ? 0xa44 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21ack_mailbox_interruptEmPlS0_)
    . = . < 0xa70 ? 0xa70 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager18send_loader_paramsEm)
    . = . < 0xc00 ? 0xc00 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21handle_class2_runningEmmPlS0_S0_)
    . = . < 0xcf8 ? 0xcf8 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager24handle_mailbox_interruptEmPlS0_)
    . = . < 0xe94 ? 0xe94 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21handle_stop_interruptEmPlS0_S0_)
    . = . < 0x10c0 ? 0x10c0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager18ack_halt_interruptEmPlS0_S0_)
    . = . < 0x1100 ? 0x1100 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager23ack_tag_group_interruptEmPlS0_S0_)
    . = . < 0x1140 ? 0x1140 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21handle_class2_loadingEmmPlS0_S0_i)
    . = . < 0x12a8 ? 0x12a8 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager20handle_class2_loadedEmmPlS0_S0_)
    . = . < 0x12b0 ? 0x12b0 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager21handle_class2_exitingEmmPlS0_S0_)
    . = . < 0x13f8 ? 0x13f8 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager23handle_class2_interruptEmmPlS0_S0_)
    . = . < 0x15d4 ? 0x15d4 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager23handle_class0_interruptEmlPlS0_S0_)
    . = . < 0x172c ? 0x172c : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager20start_isolation_loadElm)
    . = . < 0x1798 ? 0x1798 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager12start_loaderEmj)
    . = . < 0x1884 ? 0x1884 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager11load_lv1ldrEmPKv)
    . = . < 0x193c ? 0x193c : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_manager14exit_isolationEm)
    . = . < 0x1a04 ? 0x1a04 : .;
    */lv0/src/spu/spu_manager.o(.text._ZN11spu_managerC1Ev)
    . = . < 0x1a54 ? 0x1a54 : .;
    */lv0/src/spu/spu_manager.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1a98 ? 0x1a98 : .;
    */lv0/src/spu/spu_manager.o(.text._GLOBAL__I__ZN11spu_managerC2Ev)
    . = . < 0x1aa8 ? 0x1aa8 : .;
    */lv0/src/component/component_loader.o(.text._ZN16component_loaderC2Ev)
    . = . < 0x1ab4 ? 0x1ab4 : .;
    */lv0/src/component/component_loader.o(.text._ZN16component_loaderD0Ev)
    . = . < 0x1ac0 ? 0x1ac0 : .;
    */lv0/src/component/component_loader.o(.text._ZN16component_loaderD1Ev)
    . = . < 0x1acc ? 0x1acc : .;
    */lv0/src/component/component_loader.o(.text._ZN16component_loaderD2Ev)
    . = . < 0x1ad8 ? 0x1ad8 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager18get_component_nameEjPmPPKc)
    . = . < 0x1b5c ? 0x1b5c : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager13get_componentEjPmS0_)
    . = . < 0x1c04 ? 0x1c04 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager11find_recordEjPP16component_record)
    . = . < 0x1c70 ? 0x1c70 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager11load_recordEP16component_record)
    . = . < 0x1d10 ? 0x1d10 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager15load_componentsEv)
    . = . < 0x1d98 ? 0x1d98 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager23overwrite_2nd_componentEv)
    . = . < 0x1e88 ? 0x1e88 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_managerD0Ev)
    . = . < 0x1efc ? 0x1efc : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_managerD1Ev)
    . = . < 0x1f60 ? 0x1f60 : .;
    */lv0/src/component/component.o(.text._ZN9componentD0Ev)
    . = . < 0x1f6c ? 0x1f6c : .;
    */lv0/src/component/component.o(.text._ZN9componentD1Ev)
    . = . < 0x1f78 ? 0x1f78 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager18load_2nd_componentEv)
    . = . < 0x20bc ? 0x20bc : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_manager10initializeEP16component_loader)
    . = . < 0x21d4 ? 0x21d4 : .;
    */lv0/src/component/componnet_manager.o(.text._ZN17componnet_managerC1EP16component_recordiP20component_alt_recordi)
    . = . < 0x2244 ? 0x2244 : .;
    */lv0/src/component/loaded_component.o(.text._ZN16loaded_componentC1Ev)
    . = . < 0x2264 ? 0x2264 : .;
    */lv0/src/component/loaded_component.o(.text._ZN16loaded_component3getEjPmS0_)
    . = . < 0x22b4 ? 0x22b4 : .;
    */lv0/src/component/loaded_component.o(.text._ZN16loaded_component10initializeEjm)
    . = . < 0x22c8 ? 0x22c8 : .;
    */lv0/src/component/component__base_dtor.o(.text.component__base_dtor)
    . = . < 0x22d4 ? 0x22d4 : .;
    */lv0/src/component/loaded_component.o(.text._ZN16loaded_componentD0Ev)
    . = . < 0x230c ? 0x230c : .;
    */lv0/src/component/loaded_component.o(.text._ZN16loaded_componentD1Ev)
    . = . < 0x2318 ? 0x2318 : .;
    */lv0/src/platform/is_mambo.o(.text._Z8is_mambov)
    . = . < 0x2320 ? 0x2320 : .;
    */lv0/src/io/get_gbe_macaddr.o(.text._Z15get_gbe_macaddrj)
    . = . < 0x23a4 ? 0x23a4 : .;
    */lv0/src/platform/platform_config.o(.text._Z15get_be_revisionv)
    . = . < 0x2480 ? 0x2480 : .;
    */lv0/src/platform/platform_config.o(.text._Z15get_sb_revisionv)
    . = . < 0x2618 ? 0x2618 : .;
    */lv0/src/platform/platform_config.o(.text._Z18get_spu_fault_maskv)
    . = . < 0x2688 ? 0x2688 : .;
    */lv0/src/platform/platform_config.o(.text._Z16first_usable_spuv)
    . = . < 0x26d8 ? 0x26d8 : .;
    */lv0/src/platform/xio_memory.o(.text._Z20get_xio_channel_maskv)
    . = . < 0x26f4 ? 0x26f4 : .;
    */lv0/src/platform/xio_memory.o(.text._Z23get_xio_channel_size_mbi)
    . = . < 0x2730 ? 0x2730 : .;
    */lv0/src/platform/platform_config.o(.text._Z17get_mambo_versionv)
    . = . < 0x2738 ? 0x2738 : .;
    */lv0/src/platform/platform_config.o(.text._Z13read_rsx_rdcyPmj)
    . = . < 0x2810 ? 0x2810 : .;
    */lv0/src/syscon/get_boot_gos.o(.text._Z12get_boot_gosv)
    . = . < 0x28ac ? 0x28ac : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z23read_eeprom_gbe_macaddrPm)
    . = . < 0x28f8 ? 0x28f8 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z27read_eeprom_dgbe_ip_gatewayPj)
    . = . < 0x2944 ? 0x2944 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_gateway.o(.text._Z26get_eeprom_dgbe_ip_gatewayv)
    . = . < 0x29a4 ? 0x29a4 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z27read_eeprom_dgbe_ip_netmaskPj)
    . = . < 0x29f0 ? 0x29f0 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_netmask.o(.text._Z26get_eeprom_dgbe_ip_netmaskv)
    . = . < 0x2a4c ? 0x2a4c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z27read_eeprom_dgbe_ip_addressPj)
    . = . < 0x2a98 ? 0x2a98 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_address.o(.text._Z26get_eeprom_dgbe_ip_addressv)
    . = . < 0x2af8 ? 0x2af8 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_8Pm)
    . = . < 0x2b44 ? 0x2b44 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_7Pm)
    . = . < 0x2b90 ? 0x2b90 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_6Pm)
    . = . < 0x2bdc ? 0x2bdc : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_5Pm)
    . = . < 0x2c28 ? 0x2c28 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_4Pm)
    . = . < 0x2c74 ? 0x2c74 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_3Pm)
    . = . < 0x2cc0 ? 0x2cc0 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_2Pm)
    . = . < 0x2d0c ? 0x2d0c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_rsx_rdcy_1Pm)
    . = . < 0x2d58 ? 0x2d58 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z22read_eeprom_sata_paramPj)
    . = . < 0x2da4 ? 0x2da4 : .;
    */lv0/src/storage/get_eeprom_sata_param.o(.text._Z21get_eeprom_sata_paramv)
    . = . < 0x2df0 ? 0x2df0 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z29read_eeprom_restrict_spu_flagPh)
    . = . < 0x2e3c ? 0x2e3c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z34read_eeprom_core_os_bank_indicatorPh)
    . = . < 0x2e88 ? 0x2e88 : .;
    */lv0/src/storage/get_eeprom_core_os_bank_indicator.o(.text._Z33get_eeprom_core_os_bank_indicatorv)
    . = . < 0x2ed4 ? 0x2ed4 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z21read_eeprom_ref_clockPh)
    . = . < 0x2f20 ? 0x2f20 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z33read_eeprom_core_clock_multiplierPh)
    . = . < 0x2f6c ? 0x2f6c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z32read_eeprom_cellos_spu_configurePj)
    . = . < 0x2fb8 ? 0x2fb8 : .;
    */lv0/src/storage/get_eeprom_cellos_spu_configure.o(.text._Z31get_eeprom_cellos_spu_configurev)
    . = . < 0x3004 ? 0x3004 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z28read_eeprom_flash_ext_formatPh)
    . = . < 0x3050 ? 0x3050 : .;
    */lv0/src/storage/get_eeprom_flash_ext_format.o(.text._Z27get_eeprom_flash_ext_formatv)
    . = . < 0x309c ? 0x309c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z24read_eeprom_cellos_flagsPt)
    . = . < 0x30e8 ? 0x30e8 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.text._Z23get_eeprom_cellos_flagsv)
    . = . < 0x3134 ? 0x3134 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z19read_eeprom_qa_flagPh)
    . = . < 0x3180 ? 0x3180 : .;
    */lv0/src/platform/is_qaf_enabled.o(.text._Z14is_qaf_enabledv)
    . = . < 0x31d0 ? 0x31d0 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z17read_eeprom_48c08Ph)
    . = . < 0x321c ? 0x321c : .;
    */lv0/src/platform/platform_config.o(.text._Z19get_nv_entry2_byte8v)
    . = . < 0x3250 ? 0x3250 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z24read_eeprom_release_modePh)
    . = . < 0x329c ? 0x329c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z30read_eeprom_select_dgbe_devicePh)
    . = . < 0x32e8 ? 0x32e8 : .;
    */lv0/src/storage/get_eeprom_select_dgbe_device.o(.text._Z29get_eeprom_select_dgbe_devicev)
    . = . < 0x334c ? 0x334c : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z30read_eeprom_os_boot_order_flagPh)
    . = . < 0x3398 ? 0x3398 : .;
    */lv0/src/storage/get_eeprom_os_boot_order_flag.o(.text._Z29get_eeprom_os_boot_order_flagv)
    . = . < 0x3400 ? 0x3400 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z30write_eeprom_restrict_spu_flagh)
    . = . < 0x345c ? 0x345c : .;
    */lv0/src/platform/platform_config.o(.text._Z21get_enabled_spu_countv)
    . = . < 0x3544 ? 0x3544 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z25write_eeprom_release_modeh)
    . = . < 0x35a8 ? 0x35a8 : .;
    */lv0/src/storage/set_release_mode.o(.text._Z16set_release_modeh)
    . = . < 0x3644 ? 0x3644 : .;
    */lv0/src/syscon/get_sc_version.o(.text._Z14get_sc_versionhPt)
    . = . < 0x36e4 ? 0x36e4 : .;
    */lv0/src/platform/get_hw_config.o(.text._Z13get_hw_configi)
    . = . < 0x37a4 ? 0x37a4 : .;
    */lv0/src/storage/eeprom_settings.o(.text._Z20read_eeprom_qa_tokenPv)
    . = . < 0x3800 ? 0x3800 : .;
    */lv0/src/platform/read_qa_token.o(.text._Z20read_eeprom_nv0_at19Pv)
    . = . < 0x381c ? 0x381c : .;
    */lv0/src/platform/read_qa_token.o(.text._Z13read_qa_tokenPv)
    . = . < 0x3870 ? 0x3870 : .;
    */lv0/src/platform/get_wake_source.o(.text._Z15get_wake_sourcev)
    . = . < 0x38b8 ? 0x38b8 : .;
    */lv0/src/platform/platform_config.o(.text._Z27get_syscon_protocol_versionv)
    . = . < 0x38f8 ? 0x38f8 : .;
    */lv0/src/platform/platform_config.o(.text._Z14get_flash_bootv)
    . = . < 0x3944 ? 0x3944 : .;
    */lv0/src/platform/platform_config.o(.text._Z16get_flash_formatv)
    . = . < 0x3b04 ? 0x3b04 : .;
    */lv0/src/platform/platform_config.o(.text._Z16read_sdk_versionPcm)
    . = . < 0x3b98 ? 0x3b98 : .;
    */lv0/src/platform/platform_config.o(.text._Z15get_platform_idv)
    . = . < 0x3bfc ? 0x3bfc : .;
    */lv0/src/platform/platform_config.o(.text._Z19get_reference_clockv)
    . = . < 0x3c7c ? 0x3c7c : .;
    */lv0/src/platform/platform_config.o(.text._Z25get_core_clock_multiplierv)
    . = . < 0x3ce4 ? 0x3ce4 : .;
    */lv0/src/platform/get_debug_interface_parm.o(.text._Z24get_debug_interface_parmv)
    . = . < 0x3d84 ? 0x3d84 : .;
    */lv0/src/platform/platform_config.o(.text._Z19calc_spu_faultbm_hiv)
    . = . < 0x3e24 ? 0x3e24 : .;
    */lv0/src/platform/platform_config.o(.text._Z19calc_spu_faultbm_lov)
    . = . < 0x3ef0 ? 0x3ef0 : .;
    */lv0/src/platform/xio_memory.o(.text._Z21get_total_memory_sizev)
    . = . < 0x3f70 ? 0x3f70 : .;
    */lv0/src/component/file_hooks.o(.text._Z22install_ext_file_hooksv)
    . = . < 0x3f74 ? 0x3f74 : .;
    */lv0/src/component/file_hooks.o(.text._Z18call_get_file_hookPKcmPvmPm)
    . = . < 0x3fac ? 0x3fac : .;
    */lv0/src/component/file_hooks.o(.text._Z23call_get_file_size_hookPKcPm)
    . = . < 0x3fe4 ? 0x3fe4 : .;
    */lv0/src/util/cxa_pure_virtual.o(.text.__cxa_pure_virtual)
    . = . < 0x3fe8 ? 0x3fe8 : .;
    */lv0/src/platform/debug_interface.o(.text._Z19get_debug_interfacev)
    . = . < 0x3ff8 ? 0x3ff8 : .;
    */lv0/src/platform/debug_interface.o(.text._Z19set_debug_interfacei)
    . = . < 0x4004 ? 0x4004 : .;
    */lv0/src/platform/debug_interface.o(.text._Z27is_host_file_io_unavailablev)
    . = . < 0x4040 ? 0x4040 : .;
    */lv0/src/storage/read_eeprom_select_net_device.o(.text._Z29read_eeprom_select_net_devicePh)
    . = . < 0x408c ? 0x408c : .;
    */lv0/src/storage/get_eeprom_select_net_device.o(.text._Z28get_eeprom_select_net_devicev)
    . = . < 0x4120 ? 0x4120 : .;
    */lv0/src/boot/application.o(.text._ZN11applicationC2Ev)
    . = . < 0x412c ? 0x412c : .;
    */lv0/src/boot/application.o(.text._ZN11application32reset_timebase_and_hid6_rev16_18Ev)
    . = . < 0x415c ? 0x415c : .;
    */lv0/src/boot/application.o(.text._ZN11application29reset_timebase_and_hid6_otherEv)
    . = . < 0x418c ? 0x418c : .;
    */lv0/src/boot/application.o(.text._ZN11application23reset_timebase_and_hid6Ev)
    . = . < 0x41e0 ? 0x41e0 : .;
    */lv0/src/boot/application.o(.text._ZN11application13init_device_1Ev)
    . = . < 0x4274 ? 0x4274 : .;
    */lv0/src/boot/application.o(.text._ZN11application13init_device_0Ev)
    . = . < 0x42fc ? 0x42fc : .;
    */lv0/src/boot/application.o(.text._ZN11application16finalize_devicesEv)
    . = . < 0x4378 ? 0x4378 : .;
    */lv0/src/io/iopt_pack_entry_v1.o(.text._Z18iopt_pack_entry_v1ljPKc)
    . = . < 0x4434 ? 0x4434 : .;
    */lv0/src/io/iopt_unpack_entry_v1.o(.text._Z20iopt_unpack_entry_v1ljPj)
    . = . < 0x44b8 ? 0x44b8 : .;
    */lv0/src/io/iopt.o(.text._Z18iopt_pack_entry_v2ljPj)
    . = . < 0x4524 ? 0x4524 : .;
    */lv0/src/io/iopt_unpack_entry_v2.o(.text._Z20iopt_unpack_entry_v2ljPV11chan_status)
    . = . < 0x4598 ? 0x4598 : .;
    */lv0/src/io/iopt.o(.text._Z19iopt_set_address_v2ljj)
    . = . < 0x45b0 ? 0x45b0 : .;
    */lv0/src/io/iopt.o(.text._ZN10iopt_codec12unpack_entryEljPv)
    . = . < 0x45fc ? 0x45fc : .;
    */lv0/src/io/iopt.o(.text._ZN10iopt_codec10pack_entryEljPv)
    . = . < 0x4648 ? 0x4648 : .;
    */lv0/src/io/iopt.o(.text._ZN10iopt_codec11set_addressEljj)
    . = . < 0x467c ? 0x467c : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader23find_embedded_componentEjPPK18embedded_component)
    . = . < 0x46d4 ? 0x46d4 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader18load_2nd_componentEjPvm)
    . = . < 0x474c ? 0x474c : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader26load_component_from_memoryEjP13memory_budgetmmPmS2_)
    . = . < 0x485c ? 0x485c : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader17load_component_toEjPKcPvm)
    . = . < 0x48b4 ? 0x48b4 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader19read_component_dataEllmPvm)
    . = . < 0x4914 ? 0x4914 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader27find_composite_region_entryElljlP22composite_region_entry)
    . = . < 0x49e0 ? 0x49e0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader28read_composite_region_headerEllP23composite_region_header)
    . = . < 0x4a48 ? 0x4a48 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader24read_region_table_headerEllP19region_table_header)
    . = . < 0x4ac8 ? 0x4ac8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader23find_region_table_entryElljPKcP18region_table_entry)
    . = . < 0x4ba0 ? 0x4ba0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader30load_component_from_rom_regionEPKclP13memory_budgetmmPmS4_)
    . = . < 0x4dbc ? 0x4dbc : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_)
    . = . < 0x5028 ? 0x5028 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader26load_component_from_regionEPKcS1_P13memory_budgetmmPmS4_)
    . = . < 0x525c ? 0x525c : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0x53c0 ? 0x53c0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loaderD0Ev)
    . = . < 0x53f8 ? 0x53f8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loaderD1Ev)
    . = . < 0x5404 ? 0x5404 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loaderD2Ev)
    . = . < 0x5410 ? 0x5410 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loaderC1ElPKcS1_PK18embedded_componentjm)
    . = . < 0x54a4 ? 0x54a4 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN31flash_format_1_component_loaderC2ElPKcS1_PK18embedded_componentjm)
    . = . < 0x5538 ? 0x5538 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN27flash_bank_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_)
    . = . < 0x578c ? 0x578c : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN27flash_bank_component_loaderD0Ev)
    . = . < 0x57c4 ? 0x57c4 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN27flash_bank_component_loaderD1Ev)
    . = . < 0x57d0 ? 0x57d0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.text._ZN27flash_bank_component_loaderC1ElPKcS1_PK18embedded_componentjS1_S1_)
    . = . < 0x5848 ? 0x5848 : .;
    */lv0/src/io/sb_registers.o(.text._Z18sb_dx_write32_1c70ll)
    . = . < 0x5874 ? 0x5874 : .;
    */lv0/src/io/sb_registers.o(.text._Z18sb_dx_write32_1c74ll)
    . = . < 0x58a0 ? 0x58a0 : .;
    */lv0/src/io/sb_registers.o(.text._Z18sb_dx_write32_1f60ll)
    . = . < 0x58cc ? 0x58cc : .;
    */lv0/src/io/sb_registers.o(.text._ZN9sb_device14write8_100000aEjl)
    . = . < 0x58f8 ? 0x58f8 : .;
    */lv0/src/io/sb_registers.o(.text._ZN9sb_device13read8_1000009EjPh)
    . = . < 0x5928 ? 0x5928 : .;
    */lv0/src/io/sb_registers.o(.text._ZN9sb_device14write8_100003aEl)
    . = . < 0x5934 ? 0x5934 : .;
    */lv0/src/io/sb_registers.o(.text._ZN9sb_device13read8_1000039Ev)
    . = . < 0x5970 ? 0x5970 : .;
    */lv0/src/io/init_sb_device.o(.text._ZN9sb_device10initializeEli)
    . = . < 0x5a38 ? 0x5a38 : .;
    */lv0/src/io/get_sb_device.o(.text._Z13get_sb_devicev)
    . = . < 0x5a64 ? 0x5a64 : .;
    */lv0/src/component/host_get_file_size.o(.text._Z18host_get_file_sizePKcPm)
    . = . < 0x5acc ? 0x5acc : .;
    */lv0/src/component/host_put_file.o(.text._Z13host_put_filePKcmPKvmPm)
    . = . < 0x5b64 ? 0x5b64 : .;
    */lv0/src/component/host_get_file.o(.text._Z13host_get_filePKcmPvmPm)
    . = . < 0x5bfc ? 0x5bfc : .;
    */lv0/src/log/default_print_hook.o(.text._Z18default_print_hookPKc)
    . = . < 0x5c20 ? 0x5c20 : .;
    */lv0/src/storage/storage_device__get_mapped_address.o(.text.storage_device__get_mapped_address)
    . = . < 0x5c28 ? 0x5c28 : .;
    */lv0/src/storage/storage_device__get_mapped_address.o(.text.storage_device__get_block_size)
    . = . < 0x5c30 ? 0x5c30 : .;
    */lv0/src/storage/storage.o(.text._ZN7storage16get_device_countEv)
    . = . < 0x5c70 ? 0x5c70 : .;
    */lv0/src/storage/storage.o(.text._ZN7storage18get_mapped_addressEmmPm)
    . = . < 0x5d48 ? 0x5d48 : .;
    */lv0/src/storage/storage_manager.o(.text._ZN15storage_managerD0Ev)
    . = . < 0x5d54 ? 0x5d54 : .;
    */lv0/src/storage/storage_manager.o(.text._ZN15storage_managerD1Ev)
    . = . < 0x5d60 ? 0x5d60 : .;
    */lv0/src/storage/storage.o(.text._ZN7storage8get_sizeEmPm)
    . = . < 0x5e3c ? 0x5e3c : .;
    */lv0/src/storage/storage.o(.text._ZN7storage4readEmmmPcPm)
    . = . < 0x5f5c ? 0x5f5c : .;
    */lv0/src/storage/storage.o(.text._ZN7storage5closeEm)
    . = . < 0x6094 ? 0x6094 : .;
    */lv0/src/storage/storage.o(.text._ZN7storage4openEm)
    . = . < 0x61cc ? 0x61cc : .;
    */lv0/src/storage/storage.o(.text._GLOBAL__I_g_storage)
    . = . < 0x61d0 ? 0x61d0 : .;
    */lv0/src/io/iommu.o(.text._Z18flush_dcache_rangemj)
    . = . < 0x61f0 ? 0x61f0 : .;
    */lv0/src/io/get_iommu_context.o(.text._ZN13iommu_contextC1Ev)
    . = . < 0x620c ? 0x620c : .;
    */lv0/src/io/get_iommu_context.o(.text._Z17get_iommu_contextv)
    . = . < 0x6254 ? 0x6254 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context19allocate_iost_entryEPPl)
    . = . < 0x629c ? 0x629c : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context19allocate_iopt_entryEPmj)
    . = . < 0x62f8 ? 0x62f8 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context18clear_iopt_entriesEmj)
    . = . < 0x6324 ? 0x6324 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context21invalidate_iost_cacheEm)
    . = . < 0x63b4 ? 0x63b4 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context22invalidate4_iopt_cacheEmj)
    . = . < 0x6468 ? 0x6468 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context15free_io_addressEm)
    . = . < 0x6540 ? 0x6540 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context10initializeEmmjmj)
    . = . < 0x6648 ? 0x6648 : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context7disableEv)
    . = . < 0x667c ? 0x667c : .;
    */lv0/src/io/iommu.o(.text._ZN13iommu_context19allocate_io_addressEjljijjjj)
    . = . < 0x6928 ? 0x6928 : .;
    */lv0/src/spu/spuctl.o(.text._Z18get_spu_slot_statel)
    . = . < 0x693c ? 0x693c : .;
    */lv0/src/spu/spuctl.o(.text._Z12get_spu_slotl)
    . = . < 0x694c ? 0x694c : .;
    */lv0/src/spu/spuctl.o(.text._Z22get_spu_slot_saved_sr1l)
    . = . < 0x6960 ? 0x6960 : .;
    */lv0/src/spu/spuctl.o(.text._Z21change_spu_slot_statel14spu_slot_state)
    . = . < 0x6a38 ? 0x6a38 : .;
    */lv0/src/spu/spuctl.o(.text._Z24set_spu_slot_params_sentl)
    . = . < 0x6a50 ? 0x6a50 : .;
    */lv0/src/spu/spuctl.o(.text._Z24get_spu_slot_params_sentm)
    . = . < 0x6a64 ? 0x6a64 : .;
    */lv0/src/spu/spuctl.o(.text._Z24spuctl_suspend_dma_queuem)
    . = . < 0x6aa4 ? 0x6aa4 : .;
    */lv0/src/spu/spuctl.o(.text._Z22spuctl_purge_dma_queuem)
    . = . < 0x6ae4 ? 0x6ae4 : .;
    */lv0/src/spu/spuctl.o(.text._Z24spuctl_restart_dma_queuem)
    . = . < 0x6b24 ? 0x6b24 : .;
    */lv0/src/spu/spuctl.o(.text._Z24spuctl_set_channel_countlll)
    . = . < 0x6b64 ? 0x6b64 : .;
    */lv0/src/spu/spuctl.o(.text._Z20spuctl_read_out_mboxlPi)
    . = . < 0x6b68 ? 0x6b68 : .;
    */lv0/src/spu/spuctl.o(.text._Z30spuctl_clear_mailbox_interruptl)
    . = . < 0x6b6c ? 0x6b6c : .;
    */lv0/src/spu/spuctl.o(.text._Z25spuctl_read_out_intr_mboxlPj)
    . = . < 0x6b70 ? 0x6b70 : .;
    */lv0/src/spu/spuctl.o(.text._Z28spuctl_is_proxy_dma_completelc)
    . = . < 0x6b94 ? 0x6b94 : .;
    */lv0/src/spu/spuctl.o(.text._Z22spuctl_issue_proxy_dmaljljjjj)
    . = . < 0x6b98 ? 0x6b98 : .;
    */lv0/src/spu/spuctl.o(.text._Z22spuctl_write_in_mbox64lm)
    . = . < 0x6b9c ? 0x6b9c : .;
    */lv0/src/spu/spuctl.o(.text._Z19spuctl_stop_requestl)
    . = . < 0x6ba0 ? 0x6ba0 : .;
    */lv0/src/spu/spuctl.o(.text._Z17spuctl_get_statusl)
    . = . < 0x6ba4 ? 0x6ba4 : .;
    */lv0/src/spu/spuctl.o(.text._Z29spuctl_request_isolation_exitl)
    . = . < 0x6c00 ? 0x6c00 : .;
    */lv0/src/spu/spuctl.o(.text._Z25spuctl_clear_sr1_relocatel)
    . = . < 0x6c04 ? 0x6c04 : .;
    */lv0/src/spu/spuctl.o(.text._Z18spuctl_set_mfc_sr1lm)
    . = . < 0x6c08 ? 0x6c08 : .;
    */lv0/src/spu/spuctl.o(.text._Z18spuctl_get_mfc_sr1l)
    . = . < 0x6c0c ? 0x6c0c : .;
    */lv0/src/spu/spuctl.o(.text._Z17save_spu_slot_sr1l)
    . = . < 0x6c48 ? 0x6c48 : .;
    */lv0/src/spu/spuctl.o(.text._Z31spuctl_clear_master_run_controll)
    . = . < 0x6c4c ? 0x6c4c : .;
    */lv0/src/spu/spuctl.o(.text._Z30spuctl_clear_class0_interruptslm)
    . = . < 0x6c50 ? 0x6c50 : .;
    */lv0/src/spu/spuctl.o(.text._Z27spuctl_clear_stop_interruptl)
    . = . < 0x6c54 ? 0x6c54 : .;
    */lv0/src/spu/spuctl.o(.text._Z28spuctl_write_signal_notify64lm)
    . = . < 0x6c98 ? 0x6c98 : .;
    */lv0/src/spu/spuctl.o(.text._Z29spuctl_request_isolation_loadlmPv)
    . = . < 0x6ce0 ? 0x6ce0 : .;
    */lv0/src/spu/spuctl.o(.text._Z24start_spu_isolation_loadlm)
    . = . < 0x6d4c ? 0x6d4c : .;
    */lv0/src/boot/alt_sys_parm.o(.text._Z13boot_parm_nopP9boot_parm)
    . = . < 0x6d54 ? 0x6d54 : .;
    */lv0/src/boot/alt_sys_parm.o(.text._Z19create_alt_sys_parmP9boot_parmPKcm)
    . = . < 0x6e3c ? 0x6e3c : .;
    */lv0/src/boot/alt_sys_parm.o(.text._Z18build_alt_sys_parmP9boot_parm)
    . = . < 0x7304 ? 0x7304 : .;
    */lv0/src/boot/alt_sys_parm.o(.text._Z21fill_boot_parm_headerl)
    . = . < 0x73f8 ? 0x73f8 : .;
    */lv0/src/boot/boot_parm.o(.text._ZN9boot_parmC1Ev)
    . = . < 0x7420 ? 0x7420 : .;
    */lv0/src/boot/boot_parm.o(.text._Z13get_boot_parmv)
    . = . < 0x7468 ? 0x7468 : .;
    */lv0/src/boot/alt_sys_parm.o(.text._Z15setup_boot_parmP9boot_parm)
    . = . < 0x74a4 ? 0x74a4 : .;
    */lv0/src/boot/boot_loader.o(.text._ZN11boot_loader17print_boot_bannerEv)
    . = . < 0x7530 ? 0x7530 : .;
    */lv0/src/boot/boot_loader.o(.text._ZN11boot_loader17load_and_boot_lv1Ev)
    . = . < 0x7714 ? 0x7714 : .;
    */lv0/src/boot/boot_loader.o(.text._ZN11boot_loader3runEv)
    . = . < 0x7898 ? 0x7898 : .;
    */lv0/src/boot/boot_loader.o(.text._ZN11boot_loaderC1Ev)
    . = . < 0x78d4 ? 0x78d4 : .;
    */lv0/src/boot/boot_loader.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x78f4 ? 0x78f4 : .;
    */lv0/src/boot/boot_loader.o(.text._GLOBAL__I__ZN11boot_loaderC2Ev)
    . = . < 0x7904 ? 0x7904 : .;
    */lv0/src/boot/boot_loader.o(.text._ZN11boot_loader14component_initEv)
    . = . < 0x7c28 ? 0x7c28 : .;
    */lv0/src/boot/boot_cell_os.o(.text._Z10run_lv1ldrlPKvPj)
    . = . < 0x7e40 ? 0x7e40 : .;
    */lv0/src/boot/boot_cell_os.o(.text._ZN11boot_loader12boot_cell_osEmPj)
    . = . < 0x8490 ? 0x8490 : .;
    */lv0/src/boot/boot_cell_os.o(.text._Z14decrypt_lv1ldrPvj)
    . = . < 0x852c ? 0x852c : .;
    */lv0/src/boot/component_info.o(.text._Z24get_component_info_tablev)
    . = . < 0x8538 ? 0x8538 : .;
    */lv0/src/boot/component_info.o(.text._ZN20component_info_table3setEjmm)
    . = . < 0x8640 ? 0x8640 : .;
    */lv0/src/boot/component_info.o(.text._ZN20component_info_table5clearEv)
    . = . < 0x8698 ? 0x8698 : .;
    */lv0/src/boot/component_info.o(.text._ZN20component_info_table10copy_entryEPmS0_mm)
    . = . < 0x86cc ? 0x86cc : .;
    */lv0/src/boot/component_info.o(.text._ZN20component_info_table3getEjPmS0_)
    . = . < 0x87b8 ? 0x87b8 : .;
    */lv0/src/util/lv0_memmove.o(.text._Z11lv0_memmovePcPKcm)
    . = . < 0x9148 ? 0x9148 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budgetC1Ev)
    . = . < 0x9168 ? 0x9168 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budget8allocateEmm)
    . = . < 0x91b8 ? 0x91b8 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budget13get_used_sizeEPl)
    . = . < 0x91e4 ? 0x91e4 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budget13get_free_sizeEPl)
    . = . < 0x9218 ? 0x9218 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budget10initializeEmm)
    . = . < 0x9288 ? 0x9288 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budgetD0Ev)
    . = . < 0x9294 ? 0x9294 : .;
    */lv0/src/platform/memory_budgets.o(.text._ZN13memory_budgetD1Ev)
    . = . < 0x92a0 ? 0x92a0 : .;
    */lv0/src/platform/memory_budgets.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x9338 ? 0x9338 : .;
    */lv0/src/platform/memory_budgets.o(.text._GLOBAL__I__ZN13memory_budgetC2Ev)
    . = . < 0x9348 ? 0x9348 : .;
    */lv0/src/log/log_ring.o(.text._Z12get_log_ringv)
    . = . < 0x9380 ? 0x9380 : .;
    */lv0/src/log/log_ring.o(.text._ZN8log_ring10initializeEmmPFlllE)
    . = . < 0x939c ? 0x939c : .;
    */lv0/src/log/log_ring.o(.text._ZN8log_ring4dumpEPFlllE)
    . = . < 0x9438 ? 0x9438 : .;
    */lv0/src/log/log_ring.o(.text._ZN8log_ring12write_stringEPKc)
    . = . < 0x9518 ? 0x9518 : .;
    */lv0/src/log/log_ring.o(.text._Z21write_log_to_log_ringPKc)
    . = . < 0x9548 ? 0x9548 : .;
    */lv0/src/util/lv0_memset.o(.text._Z10lv0_memsetPvim)
    . = . < 0x9648 ? 0x9648 : .;
    */lv0/src/console/init_debug_interface.o(.text._Z14link_to_cp_ch4PP7cp_linkP12pci_devparamP9cp_bridge)
    . = . < 0x9724 ? 0x9724 : .;
    */lv0/src/console/init_debug_interface.o(.text._Z14link_to_cp_ch0PP7cp_linkP12pci_devparamP9cp_bridge)
    . = . < 0x9800 ? 0x9800 : .;
    */lv0/src/console/finalize_debug_interface.o(.text._Z24finalize_debug_interfacePc)
    . = . < 0x9850 ? 0x9850 : .;
    */lv0/src/console/init_debug_interface.o(.text._Z20init_debug_interfaceP16physical_console)
    . = . < 0x9a84 ? 0x9a84 : .;
    */lv0/src/platform/pio.o(.text._Z7get_piov)
    . = . < 0x9ab0 ? 0x9ab0 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio12write_fff500Et)
    . = . < 0x9ad8 ? 0x9ad8 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio11read_fff504EPt)
    . = . < 0x9b00 ? 0x9b00 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio11read_fff500EPt)
    . = . < 0x9b28 ? 0x9b28 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio13reverse_bits8Eh)
    . = . < 0x9b68 ? 0x9b68 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio16read_output_byteEv)
    . = . < 0x9bc8 ? 0x9bc8 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio17write_output_byteEh)
    . = . < 0x9c04 ? 0x9c04 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio32is_emmcbridge_interrupt_assertedEv)
    . = . < 0x9c80 ? 0x9c80 : .;
    */lv0/src/platform/pio.o(.text._ZN3pio25is_ss2_interrupt_assertedEv)
    . = . < 0x9cfc ? 0x9cfc : .;
    */lv0/src/platform/pio.o(.text._ZN3pio9configureEPcjj)
    . = . < 0x9d10 ? 0x9d10 : .;
    */lv0/src/platform/pio.o(.text._Z25poll_emmcbridge_interruptv)
    . = . < 0x9d5c ? 0x9d5c : .;
    */lv0/src/platform/pio.o(.text._Z18poll_ss2_interruptv)
    . = . < 0x9da8 ? 0x9da8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform10stub_slot4EPvl)
    . = . < 0x9db0 ? 0x9db0 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform10stub_slot5EPv)
    . = . < 0x9db8 ? 0x9db8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform10stub_slot6Ev)
    . = . < 0x9dc0 ? 0x9dc0 : .;
    */lv0/src/platform/platform.o(.text._Z24write_eeprom_nv1_u32_at8j)
    . = . < 0x9e24 ? 0x9e24 : .;
    */lv0/src/platform/platform.o(.text._Z18handle_fatal_errorj)
    . = . < 0x9ea8 ? 0x9ea8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform3logEjPKcz)
    . = . < 0x9f18 ? 0x9f18 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform20print_chip_revisionsEl)
    . = . < 0xa218 ? 0xa218 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform16print_spu_enableEl)
    . = . < 0xa34c ? 0xa34c : .;
    */lv0/src/platform/platform.o(.text._ZN8platform17print_memory_infoEi)
    . = . < 0xa4bc ? 0xa4bc : .;
    */lv0/src/storage/get_eeprom_bootrom_trace_leve.o(.text._Z23read_eeprom_nv4_u64_at5Pm)
    . = . < 0xa50c ? 0xa50c : .;
    */lv0/src/storage/get_eeprom_bootrom_trace_leve.o(.text._Z29get_eeprom_bootrom_trace_levePh)
    . = . < 0xa558 ? 0xa558 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform13set_post_codeEll)
    . = . < 0xa5d0 ? 0xa5d0 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform20post_load_lv1_failedEv)
    . = . < 0xa5dc ? 0xa5dc : .;
    */lv0/src/platform/platform.o(.text._ZN8platform18post_load_lv1_doneEv)
    . = . < 0xa5e8 ? 0xa5e8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform19post_load_lv1_startEv)
    . = . < 0xa5f4 ? 0xa5f4 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform14post_init_doneEv)
    . = . < 0xa600 ? 0xa600 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform10initializeEv)
    . = . < 0xa7e8 ? 0xa7e8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform8finalizeEv)
    . = . < 0xa890 ? 0xa890 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform17print_system_infoEv)
    . = . < 0xa8d8 ? 0xa8d8 : .;
    */lv0/src/platform/platform.o(.text._ZN8platform13initialize_ioEv)
    . = . < 0xaa94 ? 0xaa94 : .;
    */lv0/src/platform/platform.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xaacc ? 0xaacc : .;
    */lv0/src/platform/platform.o(.text._GLOBAL__I_g_unused_416e8)
    . = . < 0xaadc ? 0xaadc : .;
    */lv0/src/platform/platform_pwrmgr.o(.text._ZN15platform_pwrmgr6rebootEv)
    . = . < 0xab1c ? 0xab1c : .;
    */lv0/src/platform/platform_pwrmgr.o(.text._ZN15platform_pwrmgr8shutdownEv)
    . = . < 0xab5c ? 0xab5c : .;
    */lv0/src/platform/platform_pwrmgr.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xab8c ? 0xab8c : .;
    */lv0/src/platform/platform_pwrmgr.o(.text._GLOBAL__I__ZN15platform_pwrmgr8shutdownEv)
    . = . < 0xab9c ? 0xab9c : .;
    */lv0/src/util/delay.o(.text._Z16delay_hdec_ticksi)
    . = . < 0xabfc ? 0xabfc : .;
    */lv0/src/util/delay.o(.text._Z8delay_msi)
    . = . < 0xac18 ? 0xac18 : .;
    */lv0/src/component/host_file_loader.o(.text._ZN16host_file_loader17load_component_toEjPKcPvm)
    . = . < 0xacc4 ? 0xacc4 : .;
    */lv0/src/component/host_file_loader.o(.text._ZN16host_file_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0xade4 ? 0xade4 : .;
    */lv0/src/component/host_file_loader.o(.text._ZN16host_file_loaderD0Ev)
    . = . < 0xae1c ? 0xae1c : .;
    */lv0/src/component/host_file_loader.o(.text._ZN16host_file_loaderD1Ev)
    . = . < 0xae28 ? 0xae28 : .;
    */lv0/src/component/host_file_loader.o(.text._ZN16host_file_loaderC1Ev)
    . = . < 0xae5c ? 0xae5c : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storageC1Ev)
    . = . < 0xae74 ? 0xae74 : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storage8set_baseEm)
    . = . < 0xae7c ? 0xae7c : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storage4openEv)
    . = . < 0xae84 ? 0xae84 : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storage5closeEv)
    . = . < 0xae8c ? 0xae8c : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storage18get_mapped_addressEmPm)
    . = . < 0xaf38 ? 0xaf38 : .;
    */lv0/src/flash/mapped_storage.o(.text._ZN14mapped_storage4readEmmPcPm)
    . = . < 0xb074 ? 0xb074 : .;
    */lv0/src/syscon/syscon_clock.o(.text._Z22syscon_get_clock_valuePhhhi)
    . = . < 0xb16c ? 0xb16c : .;
    */lv0/src/syscon/syscon_clock.o(.text._Z20syscon_get_ref_clockPji)
    . = . < 0xb17c ? 0xb17c : .;
    */lv0/src/syscon/syscon_get_hw_config.o(.text._Z20syscon_get_hw_configPmS_)
    . = . < 0xb248 ? 0xb248 : .;
    */lv0/src/syscon/syscon_get_platform_id.o(.text._Z22syscon_get_platform_idPh)
    . = . < 0xb338 ? 0xb338 : .;
    */lv0/src/syscon/syscon_get_core_clock_multiplier.o(.text._Z32syscon_get_core_clock_multiplierPl)
    . = . < 0xb478 ? 0xb478 : .;
    */lv0/src/syscon/sc_console.o(.text._Z19syscon_write_bufferPKhm)
    . = . < 0xb540 ? 0xb540 : .;
    */lv0/src/syscon/sc_console.o(.text._Z19syscon_write_stringPKc)
    . = . < 0xb5cc ? 0xb5cc : .;
    */lv0/src/util/copy_qwords_to_mmio.o(.text._Z19copy_qwords_to_mmioPcS_j)
    . = . < 0xb5f4 ? 0xb5f4 : .;
    */lv0/src/util/copy_qwords_from_mmio.o(.text._Z21copy_qwords_from_mmioPcS_j)
    . = . < 0xb61c ? 0xb61c : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon20get_protocol_versionEv)
    . = . < 0xb624 ? 0xb624 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon18raise_sc_interruptEj)
    . = . < 0xb664 ? 0xb664 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon22get_swbint_status_ver1Ev)
    . = . < 0xb6c0 ? 0xb6c0 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon17get_swbint_statusEv)
    . = . < 0xb6d8 ? 0xb6d8 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon11wait_swbintEj)
    . = . < 0xb750 ? 0xb750 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon16sc_calc_checksumEPhjPt)
    . = . < 0xb784 ? 0xb784 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon20sc_calc_checksum_negEPhjPt)
    . = . < 0xb7b8 ? 0xb7b8 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon18read_cmpl_msg_ver1EP10sc_commandPvjPiP10sc_msg_ids)
    . = . < 0xba30 ? 0xba30 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon13read_cmpl_msgEP10sc_commandPvjPiP10sc_msg_ids)
    . = . < 0xba4c ? 0xba4c : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon19write_init_msg_ver1EjPKvjP10sc_msg_idsi)
    . = . < 0xbc6c ? 0xbc6c : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon14write_init_msgEjPKvjP10sc_msg_ids)
    . = . < 0xbc90 ? 0xbc90 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon16send_and_receiveEjPKvjPvjPii)
    . = . < 0xbea0 ? 0xbea0 : .;
    */lv0/src/syscon/syscon.o(.text._ZN6syscon10initializeElii)
    . = . < 0xbf64 ? 0xbf64 : .;
    */lv0/src/syscon/get_syscon_device.o(.text._Z17get_syscon_devicev)
    . = . < 0xbf98 ? 0xbf98 : .;
    */lv0/src/syscon/sc_nv_storage.o(.text._ZN13sc_nv_storage12check_statusEj)
    . = . < 0xc01c ? 0xc01c : .;
    */lv0/src/syscon/sc_nv_storage.o(.text._ZN13sc_nv_storage5writeEjjjPKv)
    . = . < 0xc128 ? 0xc128 : .;
    */lv0/src/syscon/sc_nv_storage.o(.text._ZN13sc_nv_storage4readEjjjPvPj)
    . = . < 0xc268 ? 0xc268 : .;
    */lv0/src/syscon/syscon_syspm_get_33.o(.text._Z19syscon_syspm_get_33Pj)
    . = . < 0xc300 ? 0xc300 : .;
    */lv0/src/syscon/syscon_set_wake_source.o(.text._Z22syscon_set_wake_sourcei)
    . = . < 0xc398 ? 0xc398 : .;
    */lv0/src/syscon/syscon_power_off.o(.text._Z20syscon_get_wake_infoPhS_S_S_S_Pj)
    . = . < 0xc4e0 ? 0xc4e0 : .;
    */lv0/src/syscon/syscon_power_off.o(.text._Z26syscon_power_off_with_codeiii)
    . = . < 0xc5e4 ? 0xc5e4 : .;
    */lv0/src/syscon/syscon_reboot.o(.text._Z13syscon_rebootv)
    . = . < 0xc620 ? 0xc620 : .;
    */lv0/src/syscon/syscon_shutdown.o(.text._Z15syscon_shutdownv)
    . = . < 0xc65c ? 0xc65c : .;
    */lv0/src/component/component_loaders.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xc828 ? 0xc828 : .;
    */lv0/src/component/component_loaders.o(.text._GLOBAL__I_g_flash_format_1_component_loader)
    . = . < 0xc838 ? 0xc838 : .;
    */lv0/src/component/component_loaders.o(.text._Z23select_component_loaderv)
    . = . < 0xc8b0 ? 0xc8b0 : .;
    */lv0/src/flash/flash_storage.o(.text._ZN21flash_storage_manager16get_device_tableEv)
    . = . < 0xc8b8 ? 0xc8b8 : .;
    */lv0/src/flash/flash_storage.o(.text._ZN21flash_storage_manager16get_device_countEv)
    . = . < 0xc8ec ? 0xc8ec : .;
    */lv0/src/storage/storage_manager__base_dtor.o(.text.storage_manager__base_dtor)
    . = . < 0xc8f8 ? 0xc8f8 : .;
    */lv0/src/flash/flash_storage.o(.text._ZN21flash_storage_managerD0Ev)
    . = . < 0xc930 ? 0xc930 : .;
    */lv0/src/flash/flash_storage.o(.text._ZN21flash_storage_managerD1Ev)
    . = . < 0xc93c ? 0xc93c : .;
    */lv0/src/flash/flash_storage.o(.text._Z23init_nand_flash_storagePcmm)
    . = . < 0xcbc8 ? 0xcbc8 : .;
    */lv0/src/flash/flash_storage.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0xcc60 ? 0xcc60 : .;
    */lv0/src/flash/flash_storage.o(.text._GLOBAL__I__Z23init_nand_flash_storagePcmm)
    . = . < 0xcc70 ? 0xcc70 : .;
    */lv0/src/console/uart.o(.text._Z8get_uartv)
    . = . < 0xcc9c ? 0xcc9c : .;
    */lv0/src/console/uart.o(.text._ZN4uart4putcEi)
    . = . < 0xccd0 ? 0xccd0 : .;
    */lv0/src/console/uart.o(.text._ZN4uart4putsEPKc)
    . = . < 0xcd60 ? 0xcd60 : .;
    */lv0/src/console/uart.o(.text._ZN4uart4initEPVjijiii)
    . = . < 0xce10 ? 0xce10 : .;
    */lv0/src/console/uart.o(.text._Z17uart_console_putsPKc)
    . = . < 0xce40 ? 0xce40 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage4openEv)
    . = . < 0xce6c ? 0xce6c : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage12sector_startEm)
    . = . < 0xcf34 ? 0xcf34 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage11sector_sizeEm)
    . = . < 0xcfa8 ? 0xcfa8 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage19unlock_bypass_enterEm)
    . = . < 0xd000 ? 0xd000 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage18unlock_bypass_exitEm)
    . = . < 0xd048 ? 0xd048 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage8get_sizeEPm)
    . = . < 0xd088 ? 0xd088 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage14get_block_sizeEPm)
    . = . < 0xd098 ? 0xd098 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage6swap16Ej)
    . = . < 0xd0bc ? 0xd0bc : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage8identifyEv)
    . = . < 0xd2d8 ? 0xd2d8 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage15for_each_sectorEmmMS_FimE)
    . = . < 0xd3c0 ? 0xd3c0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage13protect_rangeEmm)
    . = . < 0xd4f0 ? 0xd4f0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage9dyb_enterEm)
    . = . < 0xd548 ? 0xd548 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage11cmdset_exitEm)
    . = . < 0xd590 ? 0xd590 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage16dyb_is_protectedEm)
    . = . < 0xd5e0 ? 0xd5e0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage11dyb_protectEm)
    . = . < 0xd640 ? 0xd640 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage13dyb_unprotectEm)
    . = . < 0xd6a0 ? 0xd6a0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage18is_protected_type8Em)
    . = . < 0xd6c4 ? 0xd6c4 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage13protect_type8Em)
    . = . < 0xd6c8 ? 0xd6c8 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage15unprotect_type8Em)
    . = . < 0xd6cc ? 0xd6cc : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage12sector_groupEm)
    . = . < 0xd760 ? 0xd760 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage18is_protected_type7Em)
    . = . < 0xd81c ? 0xd81c : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage13progress_tickEv)
    . = . < 0xd8c0 ? 0xd8c0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage10halt_blinkEv)
    . = . < 0xd924 ? 0xd924 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage20program_write_bufferEmPKtj)
    . = . < 0xdba4 ? 0xdba4 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage21unlock_bypass_programEmt)
    . = . < 0xdcc4 ? 0xdcc4 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage7programEmPKcm)
    . = . < 0xde08 ? 0xde08 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage12erase_sectorEm)
    . = . < 0xdf14 ? 0xdf14 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage12write_sectorEmmPKcm)
    . = . < 0xe038 ? 0xe038 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage5writeEmmPKcPm)
    . = . < 0xe1b0 ? 0xe1b0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storageC1Ev)
    . = . < 0xe1f4 ? 0xe1f4 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage4initEmmij)
    . = . < 0xe360 ? 0xe360 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage9translateEm)
    . = . < 0xe3d8 ? 0xe3d8 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage15unprotect_type7Em)
    . = . < 0xe43c ? 0xe43c : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN16spansion_storage13protect_type7Em)
    . = . < 0xe4a0 ? 0xe4a0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plain4readEmmPcPm)
    . = . < 0xe568 ? 0xe568 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plainD0Ev)
    . = . < 0xe574 ? 0xe574 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plainD1Ev)
    . = . < 0xe580 ? 0xe580 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plainC1Ev)
    . = . < 0xe5b4 ? 0xe5b4 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plain9translateEm)
    . = . < 0xe5c0 ? 0xe5c0 : .;
    */lv0/src/flash/spansion_storage.o(.text._ZN22spansion_storage_plain18get_mapped_addressEmPm)
    . = . < 0xe648 ? 0xe648 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z15get_spu_ls_addrl)
    . = . < 0xe65c ? 0xe65c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z14get_spu_statusl)
    . = . < 0xe67c ? 0xe67c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z17get_spu_mbox_statl)
    . = . < 0xe698 ? 0xe698 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z26get_spu_in_mbox_free_countl)
    . = . < 0xe6bc ? 0xe6bc : .;
    */lv0/src/spu/spu_mmio.o(.text._Z17write_spu_in_mboxlj)
    . = . < 0xe728 ? 0xe728 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z22get_spu_out_mbox_countl)
    . = . < 0xe74c ? 0xe74c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z17read_spu_out_mboxlPi)
    . = . < 0xe7bc ? 0xe7bc : .;
    */lv0/src/spu/spu_mmio.o(.text._Z27get_spu_out_intr_mbox_countl)
    . = . < 0xe7e0 ? 0xe7e0 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z26clear_spu_mfc_sr1_relocatel)
    . = . < 0xe808 ? 0xe808 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25set_spu_mfc_sr1_ls_decodel)
    . = . < 0xe82c ? 0xe82c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z26set_spu_mfc_sr1_master_runl)
    . = . < 0xe850 ? 0xe850 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25set_spu_mfc_sr1_bus_tlbiem)
    . = . < 0xe874 ? 0xe874 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z14is_spu_stoppedlPj)
    . = . < 0xe8d4 ? 0xe8d4 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z16init_spu_mfc_sr1l)
    . = . < 0xe914 ? 0xe914 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z17get_spu_stop_codelm)
    . = . < 0xe91c ? 0xe91c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z19write_spu_in_mbox64lm)
    . = . < 0xe988 ? 0xe988 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25clear_spu_int_stat_class0lm)
    . = . < 0xe9a4 ? 0xe9a4 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z16get_spu_int_statli)
    . = . < 0xe9ec ? 0xe9ec : .;
    */lv0/src/spu/spu_mmio.o(.text._Z23set_spu_mfc_rm_boundarylm)
    . = . < 0xea0c ? 0xea0c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z28clear_spu_mfc_sr1_master_runl)
    . = . < 0xea34 ? 0xea34 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z15set_spu_mfc_sr1lm)
    . = . < 0xea50 ? 0xea50 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z15get_spu_mfc_sr1l)
    . = . < 0xea68 ? 0xea68 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z30clear_spu_int_stat_class2_haltl)
    . = . < 0xea88 ? 0xea88 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z35clear_spu_int_stat_class2_tag_groupl)
    . = . < 0xeaa8 ? 0xeaa8 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z30clear_spu_int_stat_class2_stopl)
    . = . < 0xeac8 ? 0xeac8 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z33clear_spu_int_stat_class2_mailboxl)
    . = . < 0xeae8 ? 0xeae8 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z21set_spu_channel_countll)
    . = . < 0xeb08 ? 0xeb08 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z21set_spu_channel_indexll)
    . = . < 0xeb28 ? 0xeb28 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z28is_spu_mfc_dma_queue_runningm)
    . = . < 0xeb50 ? 0xeb50 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25is_spu_mfc_purge_completem)
    . = . < 0xeb7c ? 0xeb7c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z27is_spu_mfc_suspend_completem)
    . = . < 0xeba8 ? 0xeba8 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25restart_spu_mfc_dma_queuem)
    . = . < 0xebd0 ? 0xebd0 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z23purge_spu_mfc_dma_queuem)
    . = . < 0xebf8 ? 0xebf8 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25suspend_spu_mfc_dma_queuem)
    . = . < 0xec1c ? 0xec1c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z28set_spu_privcntl_load_enablel)
    . = . < 0xec3c ? 0xec3c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z22read_spu_out_intr_mboxlPj)
    . = . < 0xecac ? 0xecac : .;
    */lv0/src/spu/spu_mmio.o(.text._Z21enqueue_spu_proxy_dmaljljjjj)
    . = . < 0xecec ? 0xecec : .;
    */lv0/src/spu/spu_mmio.o(.text._Z24request_spu_isolate_exitl)
    . = . < 0xed0c ? 0xed0c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z24request_spu_isolate_loadl)
    . = . < 0xed2c ? 0xed2c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z16request_spu_stopl)
    . = . < 0xed4c ? 0xed4c : .;
    */lv0/src/spu/spu_mmio.o(.text._Z21write_spu_sig_notify2lj)
    . = . < 0xed68 ? 0xed68 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z21write_spu_sig_notify1lj)
    . = . < 0xed84 ? 0xed84 : .;
    */lv0/src/spu/spu_mmio.o(.text._Z25is_spu_proxy_tag_completelc)
    . = . < 0xedcc ? 0xedcc : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl8set_baseEl)
    . = . < 0xedd8 ? 0xedd8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl10get_statusEv)
    . = . < 0xedfc ? 0xedfc : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl12write_sectorEPKti)
    . = . < 0xeed8 ? 0xeed8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl21start_identify_deviceEv)
    . = . < 0xef18 ? 0xef18 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl18start_erase_sectorEjt)
    . = . < 0xef60 ? 0xef60 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl18start_write_sectorEjt)
    . = . < 0xefa8 ? 0xefa8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl10start_idleEv)
    . = . < 0xeff8 ? 0xeff8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl17start_read_sectorEjtt)
    . = . < 0xf040 ? 0xf040 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl13get_reg_44016Ev)
    . = . < 0xf064 ? 0xf064 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl8get_intrEv)
    . = . < 0xf088 ? 0xf088 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl4idleEv)
    . = . < 0xf0f8 ? 0xf0f8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl11read_sectorEPti)
    . = . < 0xf188 ? 0xf188 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl15identify_deviceEPt)
    . = . < 0xf264 ? 0xf264 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._ZN15nand_flash_ctrl12erase_sectorEv)
    . = . < 0xf2d4 ? 0xf2d4 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.text._Z19get_nand_flash_ctrlv)
    . = . < 0xf300 ? 0xf300 : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flashC2Ev)
    . = . < 0xf32c ? 0xf32c : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash4openEv)
    . = . < 0xf334 ? 0xf334 : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash5closeEv)
    . = . < 0xf33c ? 0xf33c : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash14get_block_sizeEPm)
    . = . < 0xf34c ? 0xf34c : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash8get_sizeEPm)
    . = . < 0xf3cc ? 0xf3cc : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash13configure_dmaEllbPcmm)
    . = . < 0xf4a4 ? 0xf4a4 : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash5writeEmmPKcPm)
    . = . < 0xf7e8 ? 0xf7e8 : .;
    */lv0/src/flash/nand_flash.o(.text._ZN10nand_flash4readEmmPcPm)
    . = . < 0xfb54 ? 0xfb54 : .;
    */lv0/src/flash/nand_flash_abs.o(.text._ZN14nand_flash_abs8get_sizeEPm)
    . = . < 0xfb98 ? 0xfb98 : .;
    */lv0/src/flash/nand_flash_abs.o(.text._ZN14nand_flash_abs5writeEmmPKcPm)
    . = . < 0xfbbc ? 0xfbbc : .;
    */lv0/src/flash/nand_flash_abs.o(.text._ZN14nand_flash_abs4readEmmPcPm)
    . = . < 0xfbe0 ? 0xfbe0 : .;
    */lv0/src/flash/nand_flash_abs.o(.text._ZN14nand_flash_absC1Ev)
    . = . < 0xfc14 ? 0xfc14 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._Z29set_nand_flash_ready_callbackPFlvE)
    . = . < 0xfc24 ? 0xfc24 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._Z19is_nand_flash_readyv)
    . = . < 0xfc5c ? 0xfc5c : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl21erase_sectors_poll_16EjtPFhvE)
    . = . < 0xfd50 ? 0xfd50 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl18erase_sectors_pollEjjPFhvE)
    . = . < 0xfdf4 ? 0xfdf4 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl17erase_sector_pollEjj)
    . = . < 0xfe00 ? 0xfe00 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl21write_sectors_poll_16EPKcjtPFhvE)
    . = . < 0xffa8 ? 0xffa8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl18write_sectors_pollEPKcjjPFhvE)
    . = . < 0x1006c ? 0x1006c : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl17write_sector_pollEPKcjj)
    . = . < 0x10078 ? 0x10078 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.text._Z18is_nand_flash_bootv)
    . = . < 0x100ac ? 0x100ac : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._Z8kick_dmamj)
    . = . < 0x10134 ? 0x10134 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._Z14get_dma_statusv)
    . = . < 0x10198 ? 0x10198 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl16read_sectors_dmaEmjjPFhvE)
    . = . < 0x103d0 ? 0x103d0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl15read_sector_dmaEmjj)
    . = . < 0x103dc ? 0x103dc : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl17read_sectors_pollEPcjjPFhvE)
    . = . < 0x105cc ? 0x105cc : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._ZN15nand_flash_ctrl16read_sector_pollEPcjj)
    . = . < 0x105d8 ? 0x105d8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.text._Z15get_device_sizePm)
    . = . < 0x106e8 ? 0x106e8 : .;
    */lv0/src/util/lv0_strlen.o(.text._Z10lv0_strlenPKc)
    . = . < 0x10708 ? 0x10708 : .;
    */lv0/src/util/lv0_strncmp.o(.text._Z11lv0_strncmpPKcS0_m)
    . = . < 0x10758 ? 0x10758 : .;
    */lv0/src/util/lv0_strncpy.o(.text._Z11lv0_strncpyPcPKcl)
    . = . < 0x107ac ? 0x107ac : .;
    */lv0/src/util/lv0_strnlen.o(.text._Z11lv0_strnlenPKcm)
    . = . < 0x107d8 ? 0x107d8 : .;
    */lv0/src/log/printf.o(.text._Z19lv0_vfprintf_enginePFilPlliEljPKhPPh)
    . = . < 0x11490 ? 0x11490 : .;
    */lv0/src/log/printf.o(.text._Z17snprintf_put_charPPcPjli)
    . = . < 0x114fc ? 0x114fc : .;
    */lv0/src/log/printf.o(.text._Z13lv0_vsnprintfPcmPKcPv)
    . = . < 0x1153c ? 0x1153c : .;
    */lv0/src/log/printf.o(.text._Z12lv0_snprintfPcmPKcz)
    . = . < 0x11598 ? 0x11598 : .;
    */lv0/src/storage/nv_storage.o(.text._Z26is_sc_nv_storage_supportedv)
    . = . < 0x11614 ? 0x11614 : .;
    */lv0/src/storage/nv_storage.o(.text._ZN10nv_storage5writeEjjjPvj)
    . = . < 0x116b4 ? 0x116b4 : .;
    */lv0/src/storage/nv_storage.o(.text._ZN10nv_storage4readEjjjPvj)
    . = . < 0x11758 ? 0x11758 : .;
    */lv0/src/storage/nv_storage.o(.text._Z13load_nv_entryj)
    . = . < 0x117cc ? 0x117cc : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.text._ZN16systemio_dmac_dxC1Ev)
    . = . < 0x117e8 ? 0x117e8 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.text._Z20get_systemio_dmac_dxv)
    . = . < 0x11830 ? 0x11830 : .;
    */lv0/src/io/systemio_dmac_dx.o(.text._ZN16systemio_dmac_dx9configureEmmmmm)
    . = . < 0x11850 ? 0x11850 : .;
    */lv0/src/io/systemio_dmac_dx.o(.text._ZN16systemio_dmac_dx14start_transferEmmj)
    . = . < 0x1199c ? 0x1199c : .;
    */lv0/src/io/systemio_dmac_dx.o(.text._ZN16systemio_dmac_dx8kick_dmaEmj)
    . = . < 0x119b0 ? 0x119b0 : .;
    */lv0/src/io/systemio_dmac_dx.o(.text._ZN16systemio_dmac_dx14get_dma_statusEv)
    . = . < 0x11a28 ? 0x11a28 : .;
    */lv0/src/io/systemio_dmac_px.o(.text._Z20get_systemio_dmac_pxv)
    . = . < 0x11a60 ? 0x11a60 : .;
    */lv0/src/io/systemio_dmac_px.o(.text._ZN16systemio_dmac_px9configureEmmmj)
    . = . < 0x11a9c ? 0x11a9c : .;
    */lv0/src/io/systemio_dmac_px.o(.text._ZN16systemio_dmac_px26is_ebus_interrupt_assertedEv)
    . = . < 0x11af0 ? 0x11af0 : .;
    */lv0/src/io/systemio_dmac_px.o(.text._Z19poll_ebus_interruptv)
    . = . < 0x11b18 ? 0x11b18 : .;
    */lv0/src/io/systemio_dmac_px.o(.text._ZN16systemio_dmac_px14get_dma_statusEv)
    . = . < 0x11b80 ? 0x11b80 : .;
    */lv0/src/io/systemio_dmac_px.o(.text._ZN16systemio_dmac_px8kick_dmaEmj)
    . = . < 0x11c4c ? 0x11c4c : .;
    */lv0/src/log/log.o(.text._Z15log_set_handleriPFlPKcE)
    . = . < 0x11c74 ? 0x11c74 : .;
    */lv0/src/log/log.o(.text._Z17log_set_thresholdi)
    . = . < 0x11c94 ? 0x11c94 : .;
    */lv0/src/log/log.o(.text._Z17log_get_thresholdv)
    . = . < 0x11ca0 ? 0x11ca0 : .;
    */lv0/src/log/log.o(.text._Z21log_set_final_handlerPFljE)
    . = . < 0x11cb0 ? 0x11cb0 : .;
    */lv0/src/log/log.o(.text._Z12log_dispatchiiPKcPv)
    . = . < 0x11d50 ? 0x11d50 : .;
    */lv0/src/log/log.o(.text._Z9log_errorjPKcz)
    . = . < 0x11dc0 ? 0x11dc0 : .;
    */lv0/src/log/log.o(.text._Z11log_messagePKcz)
    . = . < 0x11e18 ? 0x11e18 : .;
    */lv0/src/log/log.o(.text._Z10log_printfiiPKcz)
    . = . < 0x11e5c ? 0x11e5c : .;
    */lv0/src/flash/flash_probe.o(.text._Z10bswap16_lot)
    . = . < 0x11e70 ? 0x11e70 : .;
    */lv0/src/flash/flash_probe.o(.text._Z18identify_nor_flashPVt)
    . = . < 0x12098 ? 0x12098 : .;
    */lv0/src/flash/flash_probe.o(.text._Z13syscon_printfPKcz)
    . = . < 0x12108 ? 0x12108 : .;
    */lv0/src/flash/flash_probe.o(.text._Z11uart_printfPKcz)
    . = . < 0x1217c ? 0x1217c : .;
    */lv0/src/flash/flash_probe.o(.text._Z16probe_nand_flashl)
    . = . < 0x12284 ? 0x12284 : .;
    */lv0/src/flash/flash_probe.o(.text._Z17detect_flash_typel)
    . = . < 0x122d8 ? 0x122d8 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridgeC1Ev)
    . = . < 0x122e8 ? 0x122e8 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge10cfg_read16Ej)
    . = . < 0x12308 ? 0x12308 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge11cfg_write32Ejj)
    . = . < 0x12318 ? 0x12318 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge10cfg_read32Ej)
    . = . < 0x12328 ? 0x12328 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge18bridge_cfg_write32Ejj)
    . = . < 0x12338 ? 0x12338 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge17bridge_cfg_read32Ej)
    . = . < 0x12348 ? 0x12348 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge3mapEj)
    . = . < 0x1235c ? 0x1235c : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge6read32Ej)
    . = . < 0x12394 ? 0x12394 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge7write32Ejj)
    . = . < 0x123d8 ? 0x123d8 : .;
    */lv0/src/console/cp_bridge.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x123f8 ? 0x123f8 : .;
    */lv0/src/console/cp_bridge.o(.text._GLOBAL__I_g_cp_bridge)
    . = . < 0x12408 ? 0x12408 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge10set_windowEjj)
    . = . < 0x12468 ? 0x12468 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge10window_lowEv)
    . = . < 0x12474 ? 0x12474 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge11window_highEv)
    . = . < 0x12480 ? 0x12480 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge4initEv)
    . = . < 0x12654 ? 0x12654 : .;
    */lv0/src/console/cp_bridge.o(.text._Z13get_cp_bridgev)
    . = . < 0x126fc ? 0x126fc : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge9write_rawEjj)
    . = . < 0x12708 ? 0x12708 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge9write_ctlEjj)
    . = . < 0x1274c ? 0x1274c : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge8read_rawEj)
    . = . < 0x12758 ? 0x12758 : .;
    */lv0/src/console/cp_bridge.o(.text._ZN9cp_bridge8read_ctlEj)
    . = . < 0x12790 ? 0x12790 : .;
    */lv0/src/console/internal_console.o(.text._ZN16internal_console5writeEPKcl)
    . = . < 0x12858 ? 0x12858 : .;
    */lv0/src/console/internal_console.o(.text._ZN16internal_console10initializeEv)
    . = . < 0x128e4 ? 0x128e4 : .;
    */lv0/src/console/internal_console.o(.text._Z23log_to_internal_consolePKc)
    . = . < 0x1291c ? 0x1291c : .;
    */lv0/src/console/internal_console.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1297c ? 0x1297c : .;
    */lv0/src/console/internal_console.o(.text._GLOBAL__I__ZN16internal_console10initializeEv)
    . = . < 0x1298c ? 0x1298c : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channel4pollEv)
    . = . < 0x12994 ? 0x12994 : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channel7receiveEv)
    . = . < 0x129ac ? 0x129ac : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channel10on_receiveEv)
    . = . < 0x129b0 ? 0x129b0 : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channel5flushEv)
    . = . < 0x129e0 ? 0x129e0 : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channel4initEPcm)
    . = . < 0x12a44 ? 0x12a44 : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channelD1Ev)
    . = . < 0x12a78 ? 0x12a78 : .;
    */lv0/src/console/log_cons_channel.o(.text._ZN16log_cons_channelC1Ev)
    . = . < 0x12ae8 ? 0x12ae8 : .;
    */lv0/src/io/gbe_work.o(.text._ZN8gbe_workC1Ev)
    . = . < 0x12b04 ? 0x12b04 : .;
    */lv0/src/io/gbe_work.o(.text._Z12get_gbe_workv)
    . = . < 0x12b0c ? 0x12b0c : .;
    */lv0/src/io/gbe_work.o(.text._ZN8gbe_work8allocateEmm)
    . = . < 0x12b34 ? 0x12b34 : .;
    */lv0/src/io/gbe_work.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x12b54 ? 0x12b54 : .;
    */lv0/src/io/gbe_work.o(.text._GLOBAL__I__ZN8gbe_workC2Ev)
    . = . < 0x12b64 ? 0x12b64 : .;
    */lv0/src/io/gbe_work.o(.text._ZN8gbe_work8finalizeEv)
    . = . < 0x12bb4 ? 0x12bb4 : .;
    */lv0/src/io/gbe_work.o(.text._ZN8gbe_work10initializeEm)
    . = . < 0x12ce4 ? 0x12ce4 : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN15pci_mini_driverC1Ev)
    . = . < 0x12d0c ? 0x12d0c : .;
    */lv0/src/io/pci_mini_driver.o(.text._Z19get_pci_mini_driverv)
    . = . < 0x12d14 ? 0x12d14 : .;
    */lv0/src/io/pci_mini_driver.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x12d34 ? 0x12d34 : .;
    */lv0/src/io/pci_mini_driver.o(.text._GLOBAL__I_pci_mini_driver_sc_version_subcmd)
    . = . < 0x12d44 ? 0x12d44 : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN15pci_mini_driver8delay_msEm)
    . = . < 0x12d80 ? 0x12d80 : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN15pci_mini_driver8scan_busEi)
    . = . < 0x1318c ? 0x1318c : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN15pci_mini_driver15init_controllerEv)
    . = . < 0x13628 ? 0x13628 : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN15pci_mini_driver10initializeEv)
    . = . < 0x1365c ? 0x1365c : .;
    */lv0/src/io/pci_mini_driver.o(.text._ZN10bus_driver12get_devparamEttiPP12pci_devparam)
    . = . < 0x13744 ? 0x13744 : .;
    */lv0/src/io/pci_bus_power.o(.text._Z16set_power_statusI7pci_busEih)
    . = . < 0x137d8 ? 0x137d8 : .;
    */lv0/src/io/pci_bus_power.o(.text._Z16get_power_statusI7pci_busEiPj)
    . = . < 0x1387c ? 0x1387c : .;
    */lv0/src/io/pci_bus_power.o(.text._ZN15pci_mini_driver13set_bus_powerEl)
    . = . < 0x139ac ? 0x139ac : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channel7receiveEPvlPt)
    . = . < 0x139bc ? 0x139bc : .;
    */lv0/src/console/cp_ch4_channel.o(.text._Z18get_cp_ch4_channelv)
    . = . < 0x139c4 ? 0x139c4 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channel10disconnectEv)
    . = . < 0x13a14 ? 0x13a14 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channel7connectEv)
    . = . < 0x13a80 ? 0x13a80 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channelD0Ev)
    . = . < 0x13ab8 ? 0x13ab8 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channelD1Ev)
    . = . < 0x13ac4 ? 0x13ac4 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channelC1Ev)
    . = . < 0x13af8 ? 0x13af8 : .;
    */lv0/src/console/cp_ch4_channel.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x13b8c ? 0x13b8c : .;
    */lv0/src/console/cp_ch4_channel.o(.text._GLOBAL__I__ZN14cp_ch4_channelC2Ev)
    . = . < 0x13b9c ? 0x13b9c : .;
    */lv0/src/console/cp_ch4_channel.o(.text._ZN14cp_ch4_channel4sendEPvtPt)
    . = . < 0x13ed8 ? 0x13ed8 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10set_deviceEP12pci_devparamP9cp_bridgej)
    . = . < 0x13f38 ? 0x13f38 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel19find_lowest_set_bitEj)
    . = . < 0x13f68 ? 0x13f68 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel6vslot7Ev)
    . = . < 0x13f70 ? 0x13f70 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel7connectEv)
    . = . < 0x13f78 ? 0x13f78 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel6vslot6Ev)
    . = . < 0x13f80 ? 0x13f80 : .;
    */lv0/src/console/cp_channel.o(.text._Z14get_cp_channelv)
    . = . < 0x13f88 ? 0x13f88 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel9read_bar0Ej)
    . = . < 0x1401c ? 0x1401c : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10write_bar0Ejj)
    . = . < 0x140c4 ? 0x140c4 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel8write_chEjj)
    . = . < 0x14164 ? 0x14164 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel9read_bar1Ej)
    . = . < 0x141ec ? 0x141ec : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel11read_configEPj)
    . = . < 0x1425c ? 0x1425c : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10write_bar1Ejj)
    . = . < 0x142f8 ? 0x142f8 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10cfg_read32Ej)
    . = . < 0x14324 ? 0x14324 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel11cfg_write32Ejj)
    . = . < 0x1435c ? 0x1435c : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel5resetEv)
    . = . < 0x143a0 ? 0x143a0 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10disconnectEv)
    . = . < 0x143c4 ? 0x143c4 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel10initializeEP12pci_devparamP9cp_bridgej)
    . = . < 0x14558 ? 0x14558 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channelD1Ev)
    . = . < 0x14564 ? 0x14564 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channelC1Ev)
    . = . < 0x145c8 ? 0x145c8 : .;
    */lv0/src/console/cp_channel.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1465c ? 0x1465c : .;
    */lv0/src/console/cp_channel.o(.text._GLOBAL__I__ZN10cp_channel19find_lowest_set_bitEj)
    . = . < 0x1466c ? 0x1466c : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channelC2Ev)
    . = . < 0x146d0 ? 0x146d0 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel5setupEv)
    . = . < 0x148cc ? 0x148cc : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel7receiveEPvlPt)
    . = . < 0x14af4 ? 0x14af4 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channel4sendEPvtPt)
    . = . < 0x14c54 ? 0x14c54 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channelD2Ev)
    . = . < 0x14c60 ? 0x14c60 : .;
    */lv0/src/console/cp_channel.o(.text._ZN10cp_channelD0Ev)
    . = . < 0x14c6c ? 0x14c6c : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_consoleC1Ev)
    . = . < 0x14c8c ? 0x14c8c : .;
    */lv0/src/console/physical_console.o(.text._Z20get_physical_consolev)
    . = . < 0x14c94 ? 0x14c94 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console5bswapItEET_S1_)
    . = . < 0x14cb0 ? 0x14cb0 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console5bswapIlEET_S1_)
    . = . < 0x14ce0 ? 0x14ce0 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console13decode_headerEPtS0_S0_Pl)
    . = . < 0x14d68 ? 0x14d68 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console13encode_headerEPhttl)
    . = . < 0x14dec ? 0x14dec : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console7deliverEP11ring_buffertl)
    . = . < 0x14eb0 ? 0x14eb0 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console7receiveEv)
    . = . < 0x14fbc ? 0x14fbc : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console4pollEv)
    . = . < 0x14fd4 ? 0x14fd4 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console4sendEP11ring_buffertl)
    . = . < 0x15140 ? 0x15140 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console10initializeEv)
    . = . < 0x151f4 ? 0x151f4 : .;
    */lv0/src/console/physical_console.o(.text._ZN16physical_console8finalizeEv)
    . = . < 0x151fc ? 0x151fc : .;
    */lv0/src/console/physical_console.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1521c ? 0x1521c : .;
    */lv0/src/console/physical_console.o(.text._GLOBAL__I__ZN16physical_consoleC2Ev)
    . = . < 0x1522c ? 0x1522c : .;
    */lv0/src/console/file_transfer.o(.text._Z15get_ft_channelsv)
    . = . < 0x15234 ? 0x15234 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer15status_to_errorEm)
    . = . < 0x152a0 ? 0x152a0 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer5bswapIlEET_S1_)
    . = . < 0x152d8 ? 0x152d8 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer14decode_messageEPlS0_S0_)
    . = . < 0x15340 ? 0x15340 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer14encode_messageEPvll)
    . = . < 0x1539c ? 0x1539c : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer12read_channelEP16log_cons_channelPvl)
    . = . < 0x1544c ? 0x1544c : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer13write_channelEP16log_cons_channelPvl)
    . = . < 0x154cc ? 0x154cc : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer14send_file_nameEPKc)
    . = . < 0x1555c ? 0x1555c : .;
    */lv0/src/console/file_transfer.o(.text._Z26init_log_cons_ctl_and_datav)
    . = . < 0x1564c ? 0x1564c : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer15file_command_15EPKcm)
    . = . < 0x15788 ? 0x15788 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer14authorize_fileEPKvmPv)
    . = . < 0x15980 ? 0x15980 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer13get_file_nameEPvmm)
    . = . < 0x15b60 ? 0x15b60 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer13get_file_sizeEPKcPm)
    . = . < 0x15cf4 ? 0x15cf4 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer8put_fileEPKcmPKvmPm)
    . = . < 0x15f28 ? 0x15f28 : .;
    */lv0/src/console/file_transfer.o(.text._ZN13file_transfer8get_fileEPKcmPvmPm)
    . = . < 0x16160 ? 0x16160 : .;
    */lv0/src/console/file_transfer.o(.text._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1621c ? 0x1621c : .;
    */lv0/src/console/file_transfer.o(.text._GLOBAL__I__Z26init_log_cons_ctl_and_datav)
    . = . < 0x1622c ? 0x1622c : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_bufferC1Ev)
    . = . < 0x1624c ? 0x1624c : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_buffer6attachElP18ring_buffer_header)
    . = . < 0x16294 ? 0x16294 : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_bufferD1Ev)
    . = . < 0x16298 ? 0x16298 : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_buffer3popEPh)
    . = . < 0x16304 ? 0x16304 : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_buffer5writeEPKhmPm)
    . = . < 0x16408 ? 0x16408 : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_buffer4pushEh)
    . = . < 0x164d0 ? 0x164d0 : .;
    */lv0/src/console/ring_buffer.o(.text._ZN11ring_buffer4readEPhmPm)
    . = . < 0x16570 ? 0x16570 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessorC1EPc)
    . = . < 0x16588 ? 0x16588 : .;
    */lv0/src/util/mmio_accessor.o(.text._Z17get_mmio_accessorv)
    . = . < 0x165f8 ? 0x165f8 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor16is_trace_enabledEv)
    . = . < 0x16600 ? 0x16600 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor6read32El)
    . = . < 0x16684 ? 0x16684 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor6read64El)
    . = . < 0x16704 ? 0x16704 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor7write32Elj)
    . = . < 0x16784 ? 0x16784 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor7write64Ell)
    . = . < 0x16804 ? 0x16804 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor8on_writeElll)
    . = . < 0x16808 ? 0x16808 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessor7on_readElll)
    . = . < 0x1680c ? 0x1680c : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessorD0Ev)
    . = . < 0x16818 ? 0x16818 : .;
    */lv0/src/util/mmio_accessor.o(.text._ZN13mmio_accessorD1Ev)
    . = . < 0x16824 ? 0x16824 : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_link6read32El)
    . = . < 0x16874 ? 0x16874 : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_link7write32Ell)
    . = . < 0x168bc ? 0x168bc : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_linkD0Ev)
    . = . < 0x168c8 ? 0x168c8 : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_linkD1Ev)
    . = . < 0x168d4 ? 0x168d4 : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_link8delay_msEm)
    . = . < 0x16910 ? 0x16910 : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_link8delay_usEm)
    . = . < 0x1694c ? 0x1694c : .;
    */lv0/src/console/cp_link.o(.text._ZN7cp_linkC2Ev)
    . = . < 0x1698c ? 0x1698c : .;
    */lv0/src/io/pci_config.o(.text._Z23pci_make_config_addressP12pci_devparaml)
    . = . < 0x169ec ? 0x169ec : .;
    */lv0/src/io/pci_config.o(.text._Z18pci_config_write32P12pci_devparamlj)
    . = . < 0x16a48 ? 0x16a48 : .;
    */lv0/src/io/pci_config.o(.text._Z17pci_config_read32P12pci_devparaml)
    . = . < 0x16a98 ? 0x16a98 : .;
    */lv0/src/io/pci_config.o(.text._Z17pci_config_read16P12pci_devparaml)
    . = . < 0x16b04 ? 0x16b04 : .;
    */lv0/src/util/operator_delete.o(.text._ZdlPv)
    . = . < 0x16b14 ? 0x16b14 : .;
    */lv0/src/util/spin_lock.o(.text._ZN15spin_lock_guard7acquireEv)
    . = . < 0x16b4c ? 0x16b4c : .;
    */lv0/src/util/spin_lock.o(.text._ZN15spin_lock_guardC1Ev)
    . = . < 0x16b58 ? 0x16b58 : .;
    */lv0/src/util/spin_lock_release.o(.text._ZN15spin_lock_guard7releaseEv)
    . = . < 0x16b68 ? 0x16b68 : .;
    */lv0/src/util/spin_lock.o(.text._ZN15spin_lock_guardD1Ev)
    . = . < 0x16b78 ? 0x16b78 : .;
    */lv0/src/util/aes.o(.text.aes_encrypt_block)
    . = . < 0x17540 ? 0x17540 : .;
    */lv0/src/util/aes.o(.text.aes_ctr_crypt)
  }
  . = . < 0x8017600 ? 0x8017600 : .;
  .rodata :
  {
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/platform/platform_config.o(.rodata._ZZ24get_flash_format_versionlE12__FUNCTION__)
    . = . < 0x20 ? 0x20 : .;
    */lv0/src/syscon/get_boot_gos.o(.rodata.str.LC2)
    . = . < 0x48 ? 0x48 : .;
    */lv0/src/syscon/get_boot_gos.o(.rodata.str.LC4)
    . = . < 0x6f ? 0x6f : .;
    . = . < 0x70 ? 0x70 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_gateway.o(.rodata.str.LC0)
    . = . < 0xa0 ? 0xa0 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_netmask.o(.rodata.str.LC0)
    . = . < 0xd0 ? 0xd0 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_address.o(.rodata.str.LC0)
    . = . < 0x100 ? 0x100 : .;
    */lv0/src/storage/get_eeprom_sata_param.o(.rodata.str.LC0)
    . = . < 0x130 ? 0x130 : .;
    */lv0/src/storage/get_eeprom_core_os_bank_indicator.o(.rodata.str.LC0)
    . = . < 0x168 ? 0x168 : .;
    */lv0/src/storage/get_eeprom_cellos_spu_configure.o(.rodata.str.LC0)
    . = . < 0x1a0 ? 0x1a0 : .;
    */lv0/src/storage/get_eeprom_flash_ext_format.o(.rodata.str.LC0)
    . = . < 0x1d0 ? 0x1d0 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.rodata.str.LC0)
    . = . < 0x200 ? 0x200 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.rodata.get_eeprom_gx_config_msg)
    . = . < 0x230 ? 0x230 : .;
    */lv0/src/storage/get_eeprom_select_dgbe_device.o(.rodata.str.LC0)
    . = . < 0x268 ? 0x268 : .;
    */lv0/src/storage/get_eeprom_os_boot_order_flag.o(.rodata.str.LC0)
    . = . < 0x2a0 ? 0x2a0 : .;
    */lv0/src/platform/platform_config.o(.rodata.str.LC22)
    . = . < 0x2d8 ? 0x2d8 : .;
    */lv0/src/platform/platform_config.o(.rodata.str.LC24)
    . = . < 0x309 ? 0x309 : .;
    . = . < 0x310 ? 0x310 : .;
    */lv0/src/storage/set_release_mode.o(.rodata.str.LC1)
    . = . < 0x338 ? 0x338 : .;
    */lv0/src/platform/get_hw_config.o(.rodata.str.LC1)
    . = . < 0x350 ? 0x350 : .;
    */lv0/src/platform/get_hw_config.o(.rodata.str.LC5)
    . = . < 0x36c ? 0x36c : .;
    . = . < 0x370 ? 0x370 : .;
    */lv0/src/platform/platform_config.o(.rodata.str.LC17)
    . = . < 0x3c0 ? 0x3c0 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_gno)
    . = . < 0x3c8 ? 0x3c8 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_cok)
    . = . < 0x3d0 ? 0x3d0 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_cyt2)
    . = . < 0x3d8 ? 0x3d8 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_cyt3)
    . = . < 0x3e0 ? 0x3e0 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_shr)
    . = . < 0x3e8 ? 0x3e8 : .;
    */lv0/src/platform/platform_config.o(.rodata.platform_name_cyt1)
    . = . < 0x3f0 ? 0x3f0 : .;
    */lv0/src/storage/get_eeprom_select_net_device.o(.rodata.str.LC0)
    . = . < 0x428 ? 0x428 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata._ZZN31flash_format_1_component_loader26load_component_from_memoryEjP13memory_budgetmmPmS2_E12__FUNCTION__)
    . = . < 0x448 ? 0x448 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata._ZZN31flash_format_1_component_loader30load_component_from_rom_regionEPKclP13memory_budgetmmPmS4_E12__FUNCTION__)
    . = . < 0x468 ? 0x468 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata._ZZN31flash_format_1_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_E12__FUNCTION__)
    . = . < 0x488 ? 0x488 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata._ZZN31flash_format_1_component_loader26load_component_from_regionEPKcS1_P13memory_budgetmmPmS4_E12__FUNCTION__)
    . = . < 0x4a8 ? 0x4a8 : .;
    */lv0/src/component/host_file_loader.o(.rodata.str.LC3)
    . = . < 0x4f2 ? 0x4f2 : .;
    . = . < 0x4f8 ? 0x4f8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC3)
    . = . < 0x538 ? 0x538 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC7)
    . = . < 0x568 ? 0x568 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC9)
    . = . < 0x598 ? 0x598 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC28)
    . = . < 0x5c0 ? 0x5c0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC15)
    . = . < 0x5e0 ? 0x5e0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC31)
    . = . < 0x5e8 ? 0x5e8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.rodata.str.LC33)
    . = . < 0x622 ? 0x622 : .;
    . = . < 0x628 ? 0x628 : .;
    */lv0/src/io/sb_registers.o(.rodata.sb_device_iopt_desc_template)
    . = . < 0x658 ? 0x658 : .;
    */lv0/src/io/iommu.o(.rodata._ZZN13iommu_context19allocate_io_addressEjljijjjjE13function_name)
    . = . < 0x670 ? 0x670 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC13)
    . = . < 0x690 ? 0x690 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC15)
    . = . < 0x6b8 ? 0x6b8 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC17)
    . = . < 0x6dd ? 0x6dd : .;
    . = . < 0x6e0 ? 0x6e0 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC0)
    . = . < 0x708 ? 0x708 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC3)
    . = . < 0x720 ? 0x720 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC5)
    . = . < 0x758 ? 0x758 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC7)
    . = . < 0x788 ? 0x788 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC9)
    . = . < 0x7a8 ? 0x7a8 : .;
    */lv0/src/io/iommu.o(.rodata.str.LC11)
    . = . < 0x7d0 ? 0x7d0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata._ZZ19create_alt_sys_parmP9boot_parmPKcmE13function_name)
    . = . < 0x7e8 ? 0x7e8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC0)
    . = . < 0x800 ? 0x800 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC3)
    . = . < 0x810 ? 0x810 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC5)
    . = . < 0x828 ? 0x828 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC7)
    . = . < 0x838 ? 0x838 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC10)
    . = . < 0x848 ? 0x848 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC13)
    . = . < 0x858 ? 0x858 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC16)
    . = . < 0x868 ? 0x868 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC18)
    . = . < 0x878 ? 0x878 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC20)
    . = . < 0x890 ? 0x890 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC22)
    . = . < 0x8a0 ? 0x8a0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC24)
    . = . < 0x8b0 ? 0x8b0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC26)
    . = . < 0x8b8 ? 0x8b8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC28)
    . = . < 0x8d0 ? 0x8d0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC31)
    . = . < 0x8e0 ? 0x8e0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC35)
    . = . < 0x918 ? 0x918 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC37)
    . = . < 0x930 ? 0x930 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC39)
    . = . < 0x948 ? 0x948 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC41)
    . = . < 0x958 ? 0x958 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC43)
    . = . < 0x970 ? 0x970 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC45)
    . = . < 0x988 ? 0x988 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC47)
    . = . < 0x9a0 ? 0x9a0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC49)
    . = . < 0x9b8 ? 0x9b8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC51)
    . = . < 0x9d0 ? 0x9d0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC53)
    . = . < 0x9f0 ? 0x9f0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC55)
    . = . < 0xa00 ? 0xa00 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC57)
    . = . < 0xa08 ? 0xa08 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC59)
    . = . < 0xa18 ? 0xa18 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC61)
    . = . < 0xa20 ? 0xa20 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC63)
    . = . < 0xa38 ? 0xa38 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC65)
    . = . < 0xa48 ? 0xa48 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC67)
    . = . < 0xa58 ? 0xa58 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC70)
    . = . < 0xa68 ? 0xa68 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC72)
    . = . < 0xa78 ? 0xa78 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC74)
    . = . < 0xa88 ? 0xa88 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC76)
    . = . < 0xa98 ? 0xa98 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC78)
    . = . < 0xaa8 ? 0xaa8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC81)
    . = . < 0xab8 ? 0xab8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC84)
    . = . < 0xac8 ? 0xac8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC86)
    . = . < 0xad8 ? 0xad8 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC88)
    . = . < 0xaf0 ? 0xaf0 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC90)
    . = . < 0xb08 ? 0xb08 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC93)
    . = . < 0xb20 ? 0xb20 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC95)
    . = . < 0xb40 ? 0xb40 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC97)
    . = . < 0xb50 ? 0xb50 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC100)
    . = . < 0xb60 ? 0xb60 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC102)
    . = . < 0xb78 ? 0xb78 : .;
    */lv0/src/boot/alt_sys_parm.o(.rodata.str.LC105)
    . = . < 0xb85 ? 0xb85 : .;
    . = . < 0xb88 ? 0xb88 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC0)
    . = . < 0xb90 ? 0xb90 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC2)
    . = . < 0xbb0 ? 0xbb0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC4)
    . = . < 0xbc8 ? 0xbc8 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC6)
    . = . < 0xbf0 ? 0xbf0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC8)
    . = . < 0xc08 ? 0xc08 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC10)
    . = . < 0xc10 ? 0xc10 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC12)
    . = . < 0xc58 ? 0xc58 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC17)
    . = . < 0xc88 ? 0xc88 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC19)
    . = . < 0xca8 ? 0xca8 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC21)
    . = . < 0xcb8 ? 0xcb8 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC49)
    . = . < 0xcd0 ? 0xcd0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC52)
    . = . < 0xce8 ? 0xce8 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC56)
    . = . < 0xd18 ? 0xd18 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC58)
    . = . < 0xd38 ? 0xd38 : .;
    */lv0/src/boot/boot_loader.o(.rodata._ZZN11boot_loader14component_initEvE3C.9)
    . = . < 0xd50 ? 0xd50 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.lv1ldr_out_sys_ac_sd_field)
    . = . < 0xd58 ? 0xd58 : .;
    . = . < 0xd60 ? 0xd60 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.lv1ldr_out_sys_ac_misc_field)
    . = . < 0xd68 ? 0xd68 : .;
    . = . < 0xd70 ? 0xd70 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC25)
    . = . < 0xda0 ? 0xda0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC27)
    . = . < 0xdc8 ? 0xdc8 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC29)
    . = . < 0xdf0 ? 0xdf0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC31)
    . = . < 0xe18 ? 0xe18 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC33)
    . = . < 0xe40 ? 0xe40 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC36)
    . = . < 0xe68 ? 0xe68 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC38)
    . = . < 0xe90 ? 0xe90 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC40)
    . = . < 0xeb0 ? 0xeb0 : .;
    */lv0/src/boot/boot_loader.o(.rodata.str.LC42)
    . = . < 0xed8 ? 0xed8 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC3)
    . = . < 0xef8 ? 0xef8 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC5)
    . = . < 0xf28 ? 0xf28 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC7)
    . = . < 0xf58 ? 0xf58 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC9)
    . = . < 0xf88 ? 0xf88 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC11)
    . = . < 0xfc0 ? 0xfc0 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC13)
    . = . < 0x1000 ? 0x1000 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC15)
    . = . < 0x1038 ? 0x1038 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC18)
    . = . < 0x1060 ? 0x1060 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC20)
    . = . < 0x1090 ? 0x1090 : .;
    */lv0/src/boot/boot_cell_os.o(.rodata.str.LC23)
    . = . < 0x10a9 ? 0x10a9 : .;
    . = . < 0x10b0 ? 0x10b0 : .;
    */lv0/src/platform/memory_budgets.o(.rodata.str.LC2)
    . = . < 0x10f0 ? 0x10f0 : .;
    */lv0/src/console/init_debug_interface.o(.rodata.str.LC3)
    . = . < 0x1128 ? 0x1128 : .;
    */lv0/src/console/init_debug_interface.o(.rodata.str.LC6)
    . = . < 0x1160 ? 0x1160 : .;
    */lv0/src/console/init_debug_interface.o(.rodata.str.LC8)
    . = . < 0x1194 ? 0x1194 : .;
    . = . < 0x1198 ? 0x1198 : .;
    */lv0/src/platform/pio.o(.rodata._ZZN3pio32is_emmcbridge_interrupt_assertedEvE12__FUNCTION__)
    . = . < 0x11c0 ? 0x11c0 : .;
    */lv0/src/platform/pio.o(.rodata._ZZN3pio25is_ss2_interrupt_assertedEvE12__FUNCTION__)
    . = . < 0x11e0 ? 0x11e0 : .;
    */lv0/src/platform/pio.o(.rodata.str.LC4)
    . = . < 0x1208 ? 0x1208 : .;
    */lv0/src/platform/platform.o(.rodata.tbr_divider_lfsr_table)
    . = . < 0x1308 ? 0x1308 : .;
    */lv0/src/platform/platform.o(.rodata._ZZN8platform3logEjPKczE10build_date)
    . = . < 0x1330 ? 0x1330 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC40)
    . = . < 0x1348 ? 0x1348 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC42)
    . = . < 0x1360 ? 0x1360 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC44)
    . = . < 0x1378 ? 0x1378 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC46)
    . = . < 0x1390 ? 0x1390 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC48)
    . = . < 0x13a8 ? 0x13a8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC50)
    . = . < 0x13c0 ? 0x13c0 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC52)
    . = . < 0x13d8 ? 0x13d8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC54)
    . = . < 0x13f0 ? 0x13f0 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC56)
    . = . < 0x1408 ? 0x1408 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC58)
    . = . < 0x1420 ? 0x1420 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC60)
    . = . < 0x1438 ? 0x1438 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC62)
    . = . < 0x1448 ? 0x1448 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC64)
    . = . < 0x1458 ? 0x1458 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC66)
    . = . < 0x1468 ? 0x1468 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC68)
    . = . < 0x1478 ? 0x1478 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC70)
    . = . < 0x1488 ? 0x1488 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC72)
    . = . < 0x1498 ? 0x1498 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC74)
    . = . < 0x14a8 ? 0x14a8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC76)
    . = . < 0x14b8 ? 0x14b8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC78)
    . = . < 0x14c8 ? 0x14c8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC80)
    . = . < 0x14d8 ? 0x14d8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC82)
    . = . < 0x14e8 ? 0x14e8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC84)
    . = . < 0x14f8 ? 0x14f8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC86)
    . = . < 0x1508 ? 0x1508 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC88)
    . = . < 0x1518 ? 0x1518 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC90)
    . = . < 0x1528 ? 0x1528 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC26)
    . = . < 0x1540 ? 0x1540 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC28)
    . = . < 0x1548 ? 0x1548 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC30)
    . = . < 0x1550 ? 0x1550 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC32)
    . = . < 0x1558 ? 0x1558 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC34)
    . = . < 0x1560 ? 0x1560 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC36)
    . = . < 0x1568 ? 0x1568 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC92)
    . = . < 0x15a0 ? 0x15a0 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC94)
    . = . < 0x15e8 ? 0x15e8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC96)
    . = . < 0x1608 ? 0x1608 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC98)
    . = . < 0x1618 ? 0x1618 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC100)
    . = . < 0x1638 ? 0x1638 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC102)
    . = . < 0x1650 ? 0x1650 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC104)
    . = . < 0x167e ? 0x167e : .;
    . = . < 0x1680 ? 0x1680 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC18)
    . = . < 0x16b8 ? 0x16b8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC20)
    . = . < 0x16f8 ? 0x16f8 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC4)
    . = . < 0x1728 ? 0x1728 : .;
    */lv0/src/platform/platform.o(.rodata.str.LC7)
    . = . < 0x1758 ? 0x1758 : .;
    */lv0/src/platform/platform_pwrmgr.o(.rodata.str.LC4)
    . = . < 0x1778 ? 0x1778 : .;
    */lv0/src/platform/platform_pwrmgr.o(.rodata.str.LC6)
    . = . < 0x1798 ? 0x1798 : .;
    */lv0/src/platform/platform_pwrmgr.o(.rodata.str.LC8)
    . = . < 0x17b8 ? 0x17b8 : .;
    */lv0/src/platform/platform_pwrmgr.o(.rodata.str.LC10)
    . = . < 0x17d8 ? 0x17d8 : .;
    */lv0/src/component/host_file_loader.o(.rodata._ZZN16host_file_loader14load_componentEjPKcP13memory_budgetjmmPmS4_E12__FUNCTION__)
    . = . < 0x17e8 ? 0x17e8 : .;
    */lv0/src/component/host_file_loader.o(.rodata.str.LC0)
    . = . < 0x1808 ? 0x1808 : .;
    */lv0/src/syscon/syscon.o(.rodata._ZZN6syscon18read_cmpl_msg_ver1EP10sc_commandPvjPiP10sc_msg_idsE13function_name)
    . = . < 0x1820 ? 0x1820 : .;
    */lv0/src/syscon/syscon.o(.rodata._ZZN6syscon19write_init_msg_ver1EjPKvjP10sc_msg_idsiE13function_name)
    . = . < 0x1838 ? 0x1838 : .;
    */lv0/src/syscon/syscon.o(.rodata._ZZN6syscon21send_and_receive_safeEjPKvjPvjPiiE13function_name)
    . = . < 0x1850 ? 0x1850 : .;
    */lv0/src/syscon/syscon.o(.rodata._ZZN6syscon16send_and_receiveEjPKvjPvjPiiE13function_name)
    . = . < 0x1868 ? 0x1868 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC1)
    . = . < 0x1890 ? 0x1890 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC4)
    . = . < 0x18c0 ? 0x18c0 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC6)
    . = . < 0x18e0 ? 0x18e0 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC8)
    . = . < 0x1908 ? 0x1908 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC10)
    . = . < 0x1928 ? 0x1928 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC13)
    . = . < 0x1948 ? 0x1948 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC15)
    . = . < 0x1968 ? 0x1968 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC18)
    . = . < 0x1990 ? 0x1990 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC23)
    . = . < 0x19b0 ? 0x19b0 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC26)
    . = . < 0x19e0 ? 0x19e0 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC30)
    . = . < 0x1a00 ? 0x1a00 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC32)
    . = . < 0x1a50 ? 0x1a50 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC34)
    . = . < 0x1a70 ? 0x1a70 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC36)
    . = . < 0x1a90 ? 0x1a90 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC44)
    . = . < 0x1ab8 ? 0x1ab8 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC46)
    . = . < 0x1ae8 ? 0x1ae8 : .;
    */lv0/src/syscon/syscon.o(.rodata.str.LC48)
    . = . < 0x1b18 ? 0x1b18 : .;
    */lv0/src/syscon/sc_nv_storage.o(.rodata.str.LC0)
    . = . < 0x1b38 ? 0x1b38 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC2)
    . = . < 0x1b40 ? 0x1b40 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC4)
    . = . < 0x1b50 ? 0x1b50 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC9)
    . = . < 0x1b58 ? 0x1b58 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC11)
    . = . < 0x1b60 ? 0x1b60 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC24)
    . = . < 0x1b88 ? 0x1b88 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC26)
    . = . < 0x1b98 ? 0x1b98 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC27)
    . = . < 0x1ba0 ? 0x1ba0 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC28)
    . = . < 0x1ba8 ? 0x1ba8 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC29)
    . = . < 0x1bb0 ? 0x1bb0 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC30)
    . = . < 0x1bc0 ? 0x1bc0 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC31)
    . = . < 0x1bc8 ? 0x1bc8 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC32)
    . = . < 0x1bd8 ? 0x1bd8 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC33)
    . = . < 0x1be0 ? 0x1be0 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC34)
    . = . < 0x1bf0 ? 0x1bf0 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC35)
    . = . < 0x1bf8 ? 0x1bf8 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC36)
    . = . < 0x1c08 ? 0x1c08 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC37)
    . = . < 0x1c18 ? 0x1c18 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC38)
    . = . < 0x1c28 ? 0x1c28 : .;
    */lv0/src/component/component_loaders.o(.rodata.str.LC39)
    . = . < 0x1c31 ? 0x1c31 : .;
    . = . < 0x1c38 ? 0x1c38 : .;
    */lv0/src/flash/flash_storage.o(.rodata.str.LC13)
    . = . < 0x1c70 ? 0x1c70 : .;
    */lv0/src/flash/flash_storage.o(.rodata.str.LC19)
    . = . < 0x1ca7 ? 0x1ca7 : .;
    . = . < 0x1ca8 ? 0x1ca8 : .;
    */lv0/src/platform/memory_map.o(.rodata.be_mmio_base)
    . = . < 0x1cb0 ? 0x1cb0 : .;
    */lv0/src/platform/memory_map.o(.rodata.sb_mmio_base)
    . = . < 0x1cb8 ? 0x1cb8 : .;
    */lv0/src/platform/memory_map.o(.rodata.ioif0_base)
    . = . < 0x1cc0 ? 0x1cc0 : .;
    */lv0/src/platform/memory_map.o(.rodata.flash_rom_base)
    . = . < 0x1cc8 ? 0x1cc8 : .;
    */lv0/src/platform/memory_map.o(.rodata.memory_budget_low_base)
    . = . < 0x1cd0 ? 0x1cd0 : .;
    */lv0/src/platform/memory_map.o(.rodata.memory_budget_low_size)
    . = . < 0x1cd8 ? 0x1cd8 : .;
    */lv0/src/platform/memory_map.o(.rodata.lv1ldr_io_page_addr)
    . = . < 0x1ce0 ? 0x1ce0 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC45)
    . = . < 0x1cf8 ? 0x1cf8 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.program_half_word_name)
    . = . < 0x1d10 ? 0x1d10 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC34)
    . = . < 0x1d28 ? 0x1d28 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC43)
    . = . < 0x1d70 ? 0x1d70 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC47)
    . = . < 0x1db0 ? 0x1db0 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC36)
    . = . < 0x1dc8 ? 0x1dc8 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC38)
    . = . < 0x1dd8 ? 0x1dd8 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC40)
    . = . < 0x1df0 ? 0x1df0 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC32)
    . = . < 0x1e08 ? 0x1e08 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC53)
    . = . < 0x1e18 ? 0x1e18 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC29)
    . = . < 0x1e40 ? 0x1e40 : .;
    */lv0/src/flash/spansion_storage.o(.rodata.str.LC17)
    . = . < 0x1e70 ? 0x1e70 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl20read_block_conditionEvE13function_name)
    . = . < 0x1e88 ? 0x1e88 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl9read_pageEvE13function_name)
    . = . < 0x1e98 ? 0x1e98 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl11erase_flashEvE13function_name)
    . = . < 0x1ea8 ? 0x1ea8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl16low_level_formatEvE13function_name)
    . = . < 0x1ec0 ? 0x1ec0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl15identify_deviceEPtE12__FUNCTION__)
    . = . < 0x1ed0 ? 0x1ed0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl12erase_sectorEvE12__FUNCTION__)
    . = . < 0x1ee0 ? 0x1ee0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl12write_sectorEPKtiE12__FUNCTION__)
    . = . < 0x1ef0 ? 0x1ef0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl4idleEvE12__FUNCTION__)
    . = . < 0x1ef8 ? 0x1ef8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata._ZZN15nand_flash_ctrl11read_sectorEPtiE12__FUNCTION__)
    . = . < 0x1f08 ? 0x1f08 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.rodata.str.LC0)
    . = . < 0x1f30 ? 0x1f30 : .;
    */lv0/src/flash/nand_flash.o(.rodata._ZZN10nand_flash5writeEmmPKcPmE12__FUNCTION__)
    . = . < 0x1f38 ? 0x1f38 : .;
    */lv0/src/flash/nand_flash.o(.rodata._ZZN10nand_flash4readEmmPcPmE12__FUNCTION__)
    . = . < 0x1f40 ? 0x1f40 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC3)
    . = . < 0x1f60 ? 0x1f60 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC6)
    . = . < 0x1f88 ? 0x1f88 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC8)
    . = . < 0x1fb0 ? 0x1fb0 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC10)
    . = . < 0x1fd8 ? 0x1fd8 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC12)
    . = . < 0x2000 ? 0x2000 : .;
    */lv0/src/flash/nand_flash.o(.rodata.str.LC18)
    . = . < 0x2020 ? 0x2020 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata._ZZ15get_device_sizePmE12__FUNCTION__)
    . = . < 0x2030 ? 0x2030 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata._ZZN15nand_flash_ctrl21erase_sectors_poll_16EjtPFhvEE12__FUNCTION__)
    . = . < 0x2048 ? 0x2048 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata._ZZN15nand_flash_ctrl21write_sectors_poll_16EPKcjtPFhvEE12__FUNCTION__)
    . = . < 0x2060 ? 0x2060 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata._ZZN15nand_flash_ctrl16read_sectors_dmaEmjjPFhvEE12__FUNCTION__)
    . = . < 0x2078 ? 0x2078 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata._ZZN15nand_flash_ctrl17read_sectors_pollEPcjjPFhvEE12__FUNCTION__)
    . = . < 0x2090 ? 0x2090 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC24)
    . = . < 0x20d0 ? 0x20d0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC27)
    . = . < 0x2108 ? 0x2108 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC31)
    . = . < 0x2148 ? 0x2148 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC34)
    . = . < 0x2180 ? 0x2180 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC10)
    . = . < 0x21c0 ? 0x21c0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC13)
    . = . < 0x21e8 ? 0x21e8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC15)
    . = . < 0x2218 ? 0x2218 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC17)
    . = . < 0x2240 ? 0x2240 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC19)
    . = . < 0x2278 ? 0x2278 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC21)
    . = . < 0x22a8 ? 0x22a8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC41)
    . = . < 0x22dc ? 0x22dc : .;
    . = . < 0x22e0 ? 0x22e0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC2)
    . = . < 0x2308 ? 0x2308 : .;
    */lv0/src/flash/nand_flash_transfer.o(.rodata.str.LC5)
    . = . < 0x2330 ? 0x2330 : .;
    */lv0/src/log/printf.o(.rodata.str.LC0)
    . = . < 0x2348 ? 0x2348 : .;
    */lv0/src/log/printf.o(.rodata.str.LC2)
    . = . < 0x2359 ? 0x2359 : .;
    . = . < 0x2360 ? 0x2360 : .;
    */lv0/src/storage/nv_storage.o(.rodata.str.LC2)
    . = . < 0x2380 ? 0x2380 : .;
    */lv0/src/storage/nv_storage.o(.rodata.str.LC4)
    . = . < 0x23c8 ? 0x23c8 : .;
    */lv0/src/storage/nv_storage.o(.rodata.str.LC6)
    . = . < 0x2408 ? 0x2408 : .;
    */lv0/src/io/systemio_dmac_dx.o(.rodata._ZZN16systemio_dmac_dx14get_dma_statusEvE12__FUNCTION__)
    . = . < 0x2418 ? 0x2418 : .;
    */lv0/src/io/systemio_dmac_dx.o(.rodata.str.LC1)
    . = . < 0x2438 ? 0x2438 : .;
    */lv0/src/io/systemio_dmac_px.o(.rodata._ZZN16systemio_dmac_px26is_ebus_interrupt_assertedEvE12__FUNCTION__)
    . = . < 0x2458 ? 0x2458 : .;
    */lv0/src/io/systemio_dmac_px.o(.rodata._ZZN16systemio_dmac_px14get_dma_statusEvE12__FUNCTION__)
    . = . < 0x2468 ? 0x2468 : .;
    */lv0/src/io/systemio_dmac_px.o(.rodata.str.LC5)
    . = . < 0x248b ? 0x248b : .;
    . = . < 0x2490 ? 0x2490 : .;
    */lv0/src/io/systemio_dmac_px.o(.rodata.str.LC2)
    . = . < 0x24b8 ? 0x24b8 : .;
    */lv0/src/console/internal_console.o(.rodata.str.LC4)
    . = . < 0x24e8 ? 0x24e8 : .;
    */lv0/src/console/internal_console.o(.rodata.str.LC7)
    . = . < 0x2509 ? 0x2509 : .;
    . = . < 0x2510 ? 0x2510 : .;
    */lv0/src/io/gbe_work.o(.rodata.str.LC3)
    . = . < 0x2530 ? 0x2530 : .;
    */lv0/src/io/gbe_work.o(.rodata.str.LC5)
    . = . < 0x2556 ? 0x2556 : .;
    . = . < 0x2558 ? 0x2558 : .;
    */lv0/src/io/pci_mini_driver.o(.rodata.str.LC27)
    . = . < 0x2590 ? 0x2590 : .;
    */lv0/src/io/pci_bus_power.o(.rodata.str.LC0)
    . = . < 0x25d0 ? 0x25d0 : .;
    */lv0/src/io/pci_bus_power.o(.rodata.str.LC2)
    . = . < 0x260a ? 0x260a : .;
    . = . < 0x2610 ? 0x2610 : .;
    */lv0/src/console/physical_console.o(.rodata.str.LC4)
    . = . < 0x2628 ? 0x2628 : .;
    */lv0/src/console/file_transfer.o(.rodata._ZZN13file_transfer14authorize_fileEPKvmPvE12__FUNCTION__)
    . = . < 0x2638 ? 0x2638 : .;
    */lv0/src/console/file_transfer.o(.rodata._ZZN13file_transfer13get_file_nameEPvmmE12__FUNCTION__)
    . = . < 0x2648 ? 0x2648 : .;
    */lv0/src/console/file_transfer.o(.rodata._ZZN13file_transfer8get_fileEPKcmPvmPmE12__FUNCTION__)
    . = . < 0x2658 ? 0x2658 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC29)
    . = . < 0x2678 ? 0x2678 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC32)
    . = . < 0x2698 ? 0x2698 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC34)
    . = . < 0x26b8 ? 0x26b8 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC37)
    . = . < 0x26d6 ? 0x26d6 : .;
    . = . < 0x26d8 ? 0x26d8 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC5)
    . = . < 0x2708 ? 0x2708 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC9)
    . = . < 0x2730 ? 0x2730 : .;
    */lv0/src/console/file_transfer.o(.rodata.str.LC11)
    . = . < 0x2760 ? 0x2760 : .;
    */lv0/src/util/aes_tables.o(.rodata.aes_in_table)
    . = . < 0x3760 ? 0x3760 : .;
    */lv0/src/util/aes_tables.o(.rodata.aes_round_table)
    . = . < 0x2b760 ? 0x2b760 : .;
    */lv0/src/util/aes_tables.o(.rodata.aes_out_table)
  }
  . = . < 0x8043d60 ? 0x8043d60 : .;
  .data :
  {
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/component/boot_order_component_loader.o(.data.rel.ro._ZTV27boot_order_component_loader)
    . = . < 0x30 ? 0x30 : .;
    */lv0/src/spu/spu_manager.o(.data.rel.g_spu_manager_ptr)
    . = . < 0x38 ? 0x38 : .;
    . = . < 0x40 ? 0x40 : .;
    */lv0/src/spu/spu_manager.o(.data.spu_ls_3e000_init_value)
    . = . < 0x50 ? 0x50 : .;
    */lv0/src/component/component_loader.o(.data.rel.ro._ZTV16component_loader)
    . = . < 0x80 ? 0x80 : .;
    */lv0/src/component/componnet_manager.o(.data.rel.ro._ZTV17componnet_manager)
    . = . < 0xa0 ? 0xa0 : .;
    */lv0/src/component/component.o(.data.rel.ro._ZTV9component)
    . = . < 0xc8 ? 0xc8 : .;
    */lv0/src/component/loaded_component.o(.data.rel.ro._ZTV16loaded_component)
    . = . < 0xf0 ? 0xf0 : .;
    */lv0/src/platform/get_hw_config.o(.data.s_hw_config2)
    . = . < 0xf8 ? 0xf8 : .;
    */lv0/src/platform/get_hw_config.o(.data.s_hw_config)
    . = . < 0x100 ? 0x100 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.data.g_flash_type)
    . = . < 0x108 ? 0x108 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.data.g_flash_format_cache)
    . = . < 0x10c ? 0x10c : .;
    . = . < 0x110 ? 0x110 : .;
    */lv0/src/platform/get_hw_config.o(.data.rel.ro.rsx_rdcy_reader_table)
    . = . < 0x150 ? 0x150 : .;
    */lv0/src/platform/platform_config.o(.data.g_platform_id_cache)
    . = . < 0x158 ? 0x158 : .;
    */lv0/src/platform/platform_config.o(.data.g_spu_fault_mask_cache)
    . = . < 0x160 ? 0x160 : .;
    */lv0/src/platform/platform_config.o(.data.g_sb_revision_cache)
    . = . < 0x164 ? 0x164 : .;
    */lv0/src/platform/platform_config.o(.data.g_be_revision_cache)
    . = . < 0x168 ? 0x168 : .;
    */lv0/src/component/file_hooks.o(.data.rel.g_get_file_hook)
    . = . < 0x170 ? 0x170 : .;
    */lv0/src/component/file_hooks.o(.data.rel.g_put_file_hook)
    . = . < 0x178 ? 0x178 : .;
    */lv0/src/component/file_hooks.o(.data.rel.g_get_file_size_hook)
    . = . < 0x180 ? 0x180 : .;
    */lv0/src/component/file_hooks.o(.data.rel.g_print_hook)
    . = . < 0x188 ? 0x188 : .;
    */lv0/src/platform/debug_interface.o(.data.g_debug_interface)
    . = . < 0x18c ? 0x18c : .;
    . = . < 0x190 ? 0x190 : .;
    */lv0/src/boot/application.o(.data.rel.ro._ZTV11application)
    . = . < 0x1a8 ? 0x1a8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.data.rel.ro._ZTV31flash_format_1_component_loader)
    . = . < 0x1e0 ? 0x1e0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.data.rel.ro._ZTV27flash_bank_component_loader)
    . = . < 0x218 ? 0x218 : .;
    */lv0/src/storage/storage_manager.o(.data.rel.ro._ZTV15storage_manager)
    . = . < 0x248 ? 0x248 : .;
    */lv0/src/boot/boot_loader.o(.data.rel.ro._ZTV11boot_loader)
    . = . < 0x260 ? 0x260 : .;
    */lv0/src/boot/boot_loader.o(.data.g_unused_41648)
    . = . < 0x268 ? 0x268 : .;
    */lv0/src/boot/boot_cell_os.o(.data.g_sys_ac_misc2)
    . = . < 0x270 ? 0x270 : .;
    */lv0/src/boot/boot_cell_os.o(.data.g_sys_ac_product_code)
    . = . < 0x278 ? 0x278 : .;
    . = . < 0x280 ? 0x280 : .;
    */lv0/src/boot/lv1ldr_key.o(.data.g_lv1ldr_key)
    . = . < 0x290 ? 0x290 : .;
    */lv0/src/platform/memory_budgets.o(.data.rel.g_memory_budget_low_ptr)
    . = . < 0x298 ? 0x298 : .;
    */lv0/src/platform/memory_budgets.o(.data.rel.g_memory_budget_high_ptr)
    . = . < 0x2a0 ? 0x2a0 : .;
    */lv0/src/platform/memory_budgets.o(.data.rel.ro._ZTV13memory_budget)
    . = . < 0x2c0 ? 0x2c0 : .;
    */lv0/src/platform/platform.o(.data.rel.ro._ZTV8platform)
    . = . < 0x328 ? 0x328 : .;
    */lv0/src/platform/platform_pwrmgr.o(.data.rel.ro._ZTV15platform_pwrmgr)
    . = . < 0x348 ? 0x348 : .;
    */lv0/src/util/delay.o(.data.timebase_frequency)
    . = . < 0x34c ? 0x34c : .;
    . = . < 0x350 ? 0x350 : .;
    */lv0/src/component/host_file_loader.o(.data.rel.ro._ZTV16host_file_loader)
    . = . < 0x380 ? 0x380 : .;
    */lv0/src/flash/mapped_storage.o(.data.rel.ro._ZTV14mapped_storage)
    . = . < 0x3d0 ? 0x3d0 : .;
    */lv0/src/syscon/syscon.o(.data.g_sc_communication_tag)
    . = . < 0x3d4 ? 0x3d4 : .;
    */lv0/src/syscon/syscon_power_off.o(.data.g_sc_wake_info_format)
    . = . < 0x3d8 ? 0x3d8 : .;
    */lv0/src/component/component_loaders.o(.data.rel.g_embedded_components)
    . = . < 0x4d8 ? 0x4d8 : .;
    */lv0/src/component/component_loaders.o(.data.rel.g_componnet_manager_ptr)
    . = . < 0x4e0 ? 0x4e0 : .;
    */lv0/src/component/component_loaders.o(.data.rel.local.g_component_table)
    . = . < 0x690 ? 0x690 : .;
    */lv0/src/component/component_loaders.o(.data.rel.local.g_component_alt_table)
    . = . < 0x7a8 ? 0x7a8 : .;
    */lv0/src/flash/flash_storage.o(.data.rel.g_storage_manager_ptr)
    . = . < 0x7b0 ? 0x7b0 : .;
    */lv0/src/flash/flash_storage.o(.data.rel.ro._ZTV21flash_storage_manager)
    . = . < 0x7e0 ? 0x7e0 : .;
    */lv0/src/flash/spansion_storage.o(.data.g_nor_chips)
    . = . < 0xb40 ? 0xb40 : .;
    */lv0/src/flash/spansion_storage.o(.data.rel.ro._ZTV16spansion_storage)
    . = . < 0xb98 ? 0xb98 : .;
    */lv0/src/flash/spansion_storage.o(.data.rel.ro._ZTV22spansion_storage_plain)
    . = . < 0xc00 ? 0xc00 : .;
    */lv0/src/flash/nand_flash.o(.data.rel.ro._ZTV10nand_flash)
    . = . < 0xc48 ? 0xc48 : .;
    */lv0/src/flash/nand_flash_abs.o(.data.rel.ro._ZTV14nand_flash_abs)
    . = . < 0xc90 ? 0xc90 : .;
    */lv0/src/storage/nv_storage.o(.data.rel.local.g_nv_entry_table)
    . = . < 0xe10 ? 0xe10 : .;
    */lv0/src/log/log.o(.data.g_log_threshold)
    . = . < 0xe14 ? 0xe14 : .;
    */lv0/src/component/embedded_components.o(.data.embedded_components)
    . = . < 0x7dbf8 ? 0x7dbf8 : .;
    */lv0/src/console/cp_bridge.o(.data._ZZ13get_cp_bridgevE7present)
    . = . < 0x7dbf9 ? 0x7dbf9 : .;
    . = . < 0x7dc00 ? 0x7dc00 : .;
    */lv0/src/console/cp_ch4_channel.o(.data.rel.ro._ZTV14cp_ch4_channel)
    . = . < 0x7dc60 ? 0x7dc60 : .;
    */lv0/src/console/cp_channel.o(.data.rel.ro._ZTV10cp_channel)
    . = . < 0x7dcc0 ? 0x7dcc0 : .;
    */lv0/src/console/physical_console.o(.data.g_physical_console_frame_size)
    . = . < 0x7dcc8 ? 0x7dcc8 : .;
    */lv0/src/util/mmio_accessor.o(.data.rel.ro._ZTV13mmio_accessor)
    . = . < 0x7dd18 ? 0x7dd18 : .;
    */lv0/src/console/cp_link.o(.data.rel.ro._ZTV7cp_link)
  }
  . = . < 0x80c1ae0 ? 0x80c1ae0 : .;
  .opd :
  {
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/boot/c_entry.o(.opd.c_entry)
    . = . < 0x18 ? 0x18 : .;
    */lv0/src/boot/run_global_constructors.o(.opd._Z23run_global_constructorsv)
    . = . < 0x30 ? 0x30 : .;
    */lv0/src/boot/hang_forever.o(.opd._Z12hang_foreverv)
    . = . < 0x48 ? 0x48 : .;
    */lv0/src/clock/read_timebase.o(.opd._Z13read_timebasev)
    . = . < 0x60 ? 0x60 : .;
    */lv0/src/clock/get_time_ms.o(.opd._Z11get_time_msv)
    . = . < 0x78 ? 0x78 : .;
    */lv0/src/clock/get_time_us.o(.opd._Z11get_time_usv)
    . = . < 0x90 ? 0x90 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loader16apply_boot_orderEv)
    . = . < 0xa8 ? 0xa8 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loader17load_component_toEjPKcPvm)
    . = . < 0xc0 ? 0xc0 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0xd8 ? 0xd8 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loaderD0Ev)
    . = . < 0xf0 ? 0xf0 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loaderD1Ev)
    . = . < 0x108 ? 0x108 : .;
    */lv0/src/component/boot_order_component_loader.o(.opd._ZN27boot_order_component_loaderC1EP16component_loaderS1_)
    . = . < 0x120 ? 0x120 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager18pack_class0_statusEmPm)
    . = . < 0x138 ? 0x138 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager19merge_class0_statusEmPmS0_)
    . = . < 0x150 ? 0x150 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21handle_class2_stoppedEmmPlS0_S0_)
    . = . < 0x168 ? 0x168 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_managerD1Ev)
    . = . < 0x180 ? 0x180 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z23read_eeprom_nv1_u32_at4Pj)
    . = . < 0x198 ? 0x198 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z23read_eeprom_nv1_u16_at2Pt)
    . = . < 0x1b0 ? 0x1b0 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z25write_eeprom_nv1_u32_at12j)
    . = . < 0x1c8 ? 0x1c8 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z24write_eeprom_nv1_u32_at4j)
    . = . < 0x1e0 ? 0x1e0 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z24write_eeprom_nv1_u16_at2t)
    . = . < 0x1f8 ? 0x1f8 : .;
    */lv0/src/storage/eeprom_nv1.o(.opd._Z18finish_nv1_requestPvtj)
    . = . < 0x210 ? 0x210 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager14send_ls_recordEm)
    . = . < 0x228 ? 0x228 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager14check_spu_idleEl)
    . = . < 0x240 ? 0x240 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager13load_qa_tokenEv)
    . = . < 0x258 ? 0x258 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager19send_loader_addressEll)
    . = . < 0x270 ? 0x270 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager15restore_mfc_sr1El)
    . = . < 0x288 ? 0x288 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21ack_mailbox_interruptEmPlS0_)
    . = . < 0x2a0 ? 0x2a0 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager18send_loader_paramsEm)
    . = . < 0x2b8 ? 0x2b8 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21handle_class2_runningEmmPlS0_S0_)
    . = . < 0x2d0 ? 0x2d0 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager24handle_mailbox_interruptEmPlS0_)
    . = . < 0x2e8 ? 0x2e8 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21handle_stop_interruptEmPlS0_S0_)
    . = . < 0x300 ? 0x300 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager18ack_halt_interruptEmPlS0_S0_)
    . = . < 0x318 ? 0x318 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager23ack_tag_group_interruptEmPlS0_S0_)
    . = . < 0x330 ? 0x330 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21handle_class2_loadingEmmPlS0_S0_i)
    . = . < 0x348 ? 0x348 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager20handle_class2_loadedEmmPlS0_S0_)
    . = . < 0x360 ? 0x360 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager21handle_class2_exitingEmmPlS0_S0_)
    . = . < 0x378 ? 0x378 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager23handle_class2_interruptEmmPlS0_S0_)
    . = . < 0x390 ? 0x390 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager23handle_class0_interruptEmlPlS0_S0_)
    . = . < 0x3a8 ? 0x3a8 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager20start_isolation_loadElm)
    . = . < 0x3c0 ? 0x3c0 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager12start_loaderEmj)
    . = . < 0x3d8 ? 0x3d8 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager11load_lv1ldrEmPKv)
    . = . < 0x3f0 ? 0x3f0 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_manager14exit_isolationEm)
    . = . < 0x408 ? 0x408 : .;
    */lv0/src/spu/spu_manager.o(.opd._ZN11spu_managerC1Ev)
    . = . < 0x420 ? 0x420 : .;
    */lv0/src/spu/spu_manager.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x438 ? 0x438 : .;
    */lv0/src/spu/spu_manager.o(.opd._GLOBAL__I__ZN11spu_managerC2Ev)
    . = . < 0x450 ? 0x450 : .;
    */lv0/src/component/component_loader.o(.opd._ZN16component_loaderC2Ev)
    . = . < 0x468 ? 0x468 : .;
    */lv0/src/component/component_loader.o(.opd._ZN16component_loaderD0Ev)
    . = . < 0x480 ? 0x480 : .;
    */lv0/src/component/component_loader.o(.opd._ZN16component_loaderD1Ev)
    . = . < 0x498 ? 0x498 : .;
    */lv0/src/component/component_loader.o(.opd._ZN16component_loaderD2Ev)
    . = . < 0x4b0 ? 0x4b0 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager18get_component_nameEjPmPPKc)
    . = . < 0x4c8 ? 0x4c8 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager13get_componentEjPmS0_)
    . = . < 0x4e0 ? 0x4e0 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager11find_recordEjPP16component_record)
    . = . < 0x4f8 ? 0x4f8 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager11load_recordEP16component_record)
    . = . < 0x510 ? 0x510 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager15load_componentsEv)
    . = . < 0x528 ? 0x528 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager23overwrite_2nd_componentEv)
    . = . < 0x540 ? 0x540 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_managerD0Ev)
    . = . < 0x558 ? 0x558 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_managerD1Ev)
    . = . < 0x570 ? 0x570 : .;
    */lv0/src/component/component.o(.opd._ZN9componentD0Ev)
    . = . < 0x588 ? 0x588 : .;
    */lv0/src/component/component.o(.opd._ZN9componentD1Ev)
    . = . < 0x5a0 ? 0x5a0 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager18load_2nd_componentEv)
    . = . < 0x5b8 ? 0x5b8 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_manager10initializeEP16component_loader)
    . = . < 0x5d0 ? 0x5d0 : .;
    */lv0/src/component/componnet_manager.o(.opd._ZN17componnet_managerC1EP16component_recordiP20component_alt_recordi)
    . = . < 0x5e8 ? 0x5e8 : .;
    */lv0/src/component/loaded_component.o(.opd._ZN16loaded_componentC1Ev)
    . = . < 0x600 ? 0x600 : .;
    */lv0/src/component/loaded_component.o(.opd._ZN16loaded_component3getEjPmS0_)
    . = . < 0x618 ? 0x618 : .;
    */lv0/src/component/loaded_component.o(.opd._ZN16loaded_component10initializeEjm)
    . = . < 0x630 ? 0x630 : .;
    */lv0/src/component/component__base_dtor.o(.opd.component__base_dtor)
    . = . < 0x648 ? 0x648 : .;
    */lv0/src/component/loaded_component.o(.opd._ZN16loaded_componentD0Ev)
    . = . < 0x660 ? 0x660 : .;
    */lv0/src/component/loaded_component.o(.opd._ZN16loaded_componentD1Ev)
    . = . < 0x678 ? 0x678 : .;
    */lv0/src/platform/is_mambo.o(.opd._Z8is_mambov)
    . = . < 0x690 ? 0x690 : .;
    */lv0/src/io/get_gbe_macaddr.o(.opd._Z15get_gbe_macaddrj)
    . = . < 0x6a8 ? 0x6a8 : .;
    */lv0/src/platform/platform_config.o(.opd._Z15get_be_revisionv)
    . = . < 0x6c0 ? 0x6c0 : .;
    */lv0/src/platform/platform_config.o(.opd._Z15get_sb_revisionv)
    . = . < 0x6d8 ? 0x6d8 : .;
    */lv0/src/platform/platform_config.o(.opd._Z18get_spu_fault_maskv)
    . = . < 0x6f0 ? 0x6f0 : .;
    */lv0/src/platform/platform_config.o(.opd._Z16first_usable_spuv)
    . = . < 0x708 ? 0x708 : .;
    */lv0/src/platform/xio_memory.o(.opd._Z20get_xio_channel_maskv)
    . = . < 0x720 ? 0x720 : .;
    */lv0/src/platform/xio_memory.o(.opd._Z23get_xio_channel_size_mbi)
    . = . < 0x738 ? 0x738 : .;
    */lv0/src/platform/platform_config.o(.opd._Z17get_mambo_versionv)
    . = . < 0x750 ? 0x750 : .;
    */lv0/src/platform/platform_config.o(.opd._Z13read_rsx_rdcyPmj)
    . = . < 0x768 ? 0x768 : .;
    */lv0/src/syscon/get_boot_gos.o(.opd._Z12get_boot_gosv)
    . = . < 0x780 ? 0x780 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z23read_eeprom_gbe_macaddrPm)
    . = . < 0x798 ? 0x798 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z27read_eeprom_dgbe_ip_gatewayPj)
    . = . < 0x7b0 ? 0x7b0 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_gateway.o(.opd._Z26get_eeprom_dgbe_ip_gatewayv)
    . = . < 0x7c8 ? 0x7c8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z27read_eeprom_dgbe_ip_netmaskPj)
    . = . < 0x7e0 ? 0x7e0 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_netmask.o(.opd._Z26get_eeprom_dgbe_ip_netmaskv)
    . = . < 0x7f8 ? 0x7f8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z27read_eeprom_dgbe_ip_addressPj)
    . = . < 0x810 ? 0x810 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_address.o(.opd._Z26get_eeprom_dgbe_ip_addressv)
    . = . < 0x828 ? 0x828 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_8Pm)
    . = . < 0x840 ? 0x840 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_7Pm)
    . = . < 0x858 ? 0x858 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_6Pm)
    . = . < 0x870 ? 0x870 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_5Pm)
    . = . < 0x888 ? 0x888 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_4Pm)
    . = . < 0x8a0 ? 0x8a0 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_3Pm)
    . = . < 0x8b8 ? 0x8b8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_2Pm)
    . = . < 0x8d0 ? 0x8d0 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_rsx_rdcy_1Pm)
    . = . < 0x8e8 ? 0x8e8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z22read_eeprom_sata_paramPj)
    . = . < 0x900 ? 0x900 : .;
    */lv0/src/storage/get_eeprom_sata_param.o(.opd._Z21get_eeprom_sata_paramv)
    . = . < 0x918 ? 0x918 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z29read_eeprom_restrict_spu_flagPh)
    . = . < 0x930 ? 0x930 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z34read_eeprom_core_os_bank_indicatorPh)
    . = . < 0x948 ? 0x948 : .;
    */lv0/src/storage/get_eeprom_core_os_bank_indicator.o(.opd._Z33get_eeprom_core_os_bank_indicatorv)
    . = . < 0x960 ? 0x960 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z21read_eeprom_ref_clockPh)
    . = . < 0x978 ? 0x978 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z33read_eeprom_core_clock_multiplierPh)
    . = . < 0x990 ? 0x990 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z32read_eeprom_cellos_spu_configurePj)
    . = . < 0x9a8 ? 0x9a8 : .;
    */lv0/src/storage/get_eeprom_cellos_spu_configure.o(.opd._Z31get_eeprom_cellos_spu_configurev)
    . = . < 0x9c0 ? 0x9c0 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z28read_eeprom_flash_ext_formatPh)
    . = . < 0x9d8 ? 0x9d8 : .;
    */lv0/src/storage/get_eeprom_flash_ext_format.o(.opd._Z27get_eeprom_flash_ext_formatv)
    . = . < 0x9f0 ? 0x9f0 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z24read_eeprom_cellos_flagsPt)
    . = . < 0xa08 ? 0xa08 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.opd._Z23get_eeprom_cellos_flagsv)
    . = . < 0xa20 ? 0xa20 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z19read_eeprom_qa_flagPh)
    . = . < 0xa38 ? 0xa38 : .;
    */lv0/src/platform/is_qaf_enabled.o(.opd._Z14is_qaf_enabledv)
    . = . < 0xa50 ? 0xa50 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z17read_eeprom_48c08Ph)
    . = . < 0xa68 ? 0xa68 : .;
    */lv0/src/platform/platform_config.o(.opd._Z19get_nv_entry2_byte8v)
    . = . < 0xa80 ? 0xa80 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z24read_eeprom_release_modePh)
    . = . < 0xa98 ? 0xa98 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z30read_eeprom_select_dgbe_devicePh)
    . = . < 0xab0 ? 0xab0 : .;
    */lv0/src/storage/get_eeprom_select_dgbe_device.o(.opd._Z29get_eeprom_select_dgbe_devicev)
    . = . < 0xac8 ? 0xac8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z30read_eeprom_os_boot_order_flagPh)
    . = . < 0xae0 ? 0xae0 : .;
    */lv0/src/storage/get_eeprom_os_boot_order_flag.o(.opd._Z29get_eeprom_os_boot_order_flagv)
    . = . < 0xaf8 ? 0xaf8 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z30write_eeprom_restrict_spu_flagh)
    . = . < 0xb10 ? 0xb10 : .;
    */lv0/src/platform/platform_config.o(.opd._Z21get_enabled_spu_countv)
    . = . < 0xb28 ? 0xb28 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z25write_eeprom_release_modeh)
    . = . < 0xb40 ? 0xb40 : .;
    */lv0/src/storage/set_release_mode.o(.opd._Z16set_release_modeh)
    . = . < 0xb58 ? 0xb58 : .;
    */lv0/src/syscon/get_sc_version.o(.opd._Z14get_sc_versionhPt)
    . = . < 0xb70 ? 0xb70 : .;
    */lv0/src/platform/get_hw_config.o(.opd._Z13get_hw_configi)
    . = . < 0xb88 ? 0xb88 : .;
    */lv0/src/storage/eeprom_settings.o(.opd._Z20read_eeprom_qa_tokenPv)
    . = . < 0xba0 ? 0xba0 : .;
    */lv0/src/platform/read_qa_token.o(.opd._Z20read_eeprom_nv0_at19Pv)
    . = . < 0xbb8 ? 0xbb8 : .;
    */lv0/src/platform/read_qa_token.o(.opd._Z13read_qa_tokenPv)
    . = . < 0xbd0 ? 0xbd0 : .;
    */lv0/src/platform/get_wake_source.o(.opd._Z15get_wake_sourcev)
    . = . < 0xbe8 ? 0xbe8 : .;
    */lv0/src/platform/platform_config.o(.opd._Z27get_syscon_protocol_versionv)
    . = . < 0xc00 ? 0xc00 : .;
    */lv0/src/platform/platform_config.o(.opd._Z14get_flash_bootv)
    . = . < 0xc18 ? 0xc18 : .;
    */lv0/src/platform/platform_config.o(.opd._Z16get_flash_formatv)
    . = . < 0xc30 ? 0xc30 : .;
    */lv0/src/platform/platform_config.o(.opd._Z16read_sdk_versionPcm)
    . = . < 0xc48 ? 0xc48 : .;
    */lv0/src/platform/platform_config.o(.opd._Z15get_platform_idv)
    . = . < 0xc60 ? 0xc60 : .;
    */lv0/src/platform/platform_config.o(.opd._Z19get_reference_clockv)
    . = . < 0xc78 ? 0xc78 : .;
    */lv0/src/platform/platform_config.o(.opd._Z25get_core_clock_multiplierv)
    . = . < 0xc90 ? 0xc90 : .;
    */lv0/src/platform/get_debug_interface_parm.o(.opd._Z24get_debug_interface_parmv)
    . = . < 0xca8 ? 0xca8 : .;
    */lv0/src/platform/platform_config.o(.opd._Z19calc_spu_faultbm_hiv)
    . = . < 0xcc0 ? 0xcc0 : .;
    */lv0/src/platform/platform_config.o(.opd._Z19calc_spu_faultbm_lov)
    . = . < 0xcd8 ? 0xcd8 : .;
    */lv0/src/platform/xio_memory.o(.opd._Z21get_total_memory_sizev)
    . = . < 0xcf0 ? 0xcf0 : .;
    */lv0/src/component/file_hooks.o(.opd._Z22install_ext_file_hooksv)
    . = . < 0xd08 ? 0xd08 : .;
    */lv0/src/component/file_hooks.o(.opd._Z18call_get_file_hookPKcmPvmPm)
    . = . < 0xd20 ? 0xd20 : .;
    */lv0/src/component/file_hooks.o(.opd._Z23call_get_file_size_hookPKcPm)
    . = . < 0xd38 ? 0xd38 : .;
    */lv0/src/util/cxa_pure_virtual.o(.opd.__cxa_pure_virtual)
    . = . < 0xd50 ? 0xd50 : .;
    */lv0/src/platform/debug_interface.o(.opd._Z19get_debug_interfacev)
    . = . < 0xd68 ? 0xd68 : .;
    */lv0/src/platform/debug_interface.o(.opd._Z19set_debug_interfacei)
    . = . < 0xd80 ? 0xd80 : .;
    */lv0/src/platform/debug_interface.o(.opd._Z27is_host_file_io_unavailablev)
    . = . < 0xd98 ? 0xd98 : .;
    */lv0/src/storage/read_eeprom_select_net_device.o(.opd._Z29read_eeprom_select_net_devicePh)
    . = . < 0xdb0 ? 0xdb0 : .;
    */lv0/src/storage/get_eeprom_select_net_device.o(.opd._Z28get_eeprom_select_net_devicev)
    . = . < 0xdc8 ? 0xdc8 : .;
    */lv0/src/boot/application.o(.opd._ZN11applicationC2Ev)
    . = . < 0xde0 ? 0xde0 : .;
    */lv0/src/boot/application.o(.opd._ZN11application32reset_timebase_and_hid6_rev16_18Ev)
    . = . < 0xdf8 ? 0xdf8 : .;
    */lv0/src/boot/application.o(.opd._ZN11application29reset_timebase_and_hid6_otherEv)
    . = . < 0xe10 ? 0xe10 : .;
    */lv0/src/boot/application.o(.opd._ZN11application23reset_timebase_and_hid6Ev)
    . = . < 0xe28 ? 0xe28 : .;
    */lv0/src/boot/application.o(.opd._ZN11application13init_device_1Ev)
    . = . < 0xe40 ? 0xe40 : .;
    */lv0/src/boot/application.o(.opd._ZN11application13init_device_0Ev)
    . = . < 0xe58 ? 0xe58 : .;
    */lv0/src/boot/application.o(.opd._ZN11application16finalize_devicesEv)
    . = . < 0xe70 ? 0xe70 : .;
    */lv0/src/io/iopt_pack_entry_v1.o(.opd._Z18iopt_pack_entry_v1ljPKc)
    . = . < 0xe88 ? 0xe88 : .;
    */lv0/src/io/iopt_unpack_entry_v1.o(.opd._Z20iopt_unpack_entry_v1ljPj)
    . = . < 0xea0 ? 0xea0 : .;
    */lv0/src/io/iopt.o(.opd._Z18iopt_pack_entry_v2ljPj)
    . = . < 0xeb8 ? 0xeb8 : .;
    */lv0/src/io/iopt_unpack_entry_v2.o(.opd._Z20iopt_unpack_entry_v2ljPV11chan_status)
    . = . < 0xed0 ? 0xed0 : .;
    */lv0/src/io/iopt.o(.opd._Z19iopt_set_address_v2ljj)
    . = . < 0xee8 ? 0xee8 : .;
    */lv0/src/io/iopt.o(.opd._ZN10iopt_codec12unpack_entryEljPv)
    . = . < 0xf00 ? 0xf00 : .;
    */lv0/src/io/iopt.o(.opd._ZN10iopt_codec10pack_entryEljPv)
    . = . < 0xf18 ? 0xf18 : .;
    */lv0/src/io/iopt.o(.opd._ZN10iopt_codec11set_addressEljj)
    . = . < 0xf30 ? 0xf30 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader23find_embedded_componentEjPPK18embedded_component)
    . = . < 0xf48 ? 0xf48 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader18load_2nd_componentEjPvm)
    . = . < 0xf60 ? 0xf60 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader26load_component_from_memoryEjP13memory_budgetmmPmS2_)
    . = . < 0xf78 ? 0xf78 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader17load_component_toEjPKcPvm)
    . = . < 0xf90 ? 0xf90 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader19read_component_dataEllmPvm)
    . = . < 0xfa8 ? 0xfa8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader27find_composite_region_entryElljlP22composite_region_entry)
    . = . < 0xfc0 ? 0xfc0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader28read_composite_region_headerEllP23composite_region_header)
    . = . < 0xfd8 ? 0xfd8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader24read_region_table_headerEllP19region_table_header)
    . = . < 0xff0 ? 0xff0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader23find_region_table_entryElljPKcP18region_table_entry)
    . = . < 0x1008 ? 0x1008 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader30load_component_from_rom_regionEPKclP13memory_budgetmmPmS4_)
    . = . < 0x1020 ? 0x1020 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_)
    . = . < 0x1038 ? 0x1038 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader26load_component_from_regionEPKcS1_P13memory_budgetmmPmS4_)
    . = . < 0x1050 ? 0x1050 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0x1068 ? 0x1068 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loaderD0Ev)
    . = . < 0x1080 ? 0x1080 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loaderD1Ev)
    . = . < 0x1098 ? 0x1098 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loaderD2Ev)
    . = . < 0x10b0 ? 0x10b0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loaderC1ElPKcS1_PK18embedded_componentjm)
    . = . < 0x10c8 ? 0x10c8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN31flash_format_1_component_loaderC2ElPKcS1_PK18embedded_componentjm)
    . = . < 0x10e0 ? 0x10e0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN27flash_bank_component_loader24load_component_from_bankEPKcS1_P13memory_budgetjmmPmS4_)
    . = . < 0x10f8 ? 0x10f8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN27flash_bank_component_loaderD0Ev)
    . = . < 0x1110 ? 0x1110 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN27flash_bank_component_loaderD1Ev)
    . = . < 0x1128 ? 0x1128 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.opd._ZN27flash_bank_component_loaderC1ElPKcS1_PK18embedded_componentjS1_S1_)
    . = . < 0x1140 ? 0x1140 : .;
    */lv0/src/io/sb_registers.o(.opd._Z18sb_dx_write32_1c70ll)
    . = . < 0x1158 ? 0x1158 : .;
    */lv0/src/io/sb_registers.o(.opd._Z18sb_dx_write32_1c74ll)
    . = . < 0x1170 ? 0x1170 : .;
    */lv0/src/io/sb_registers.o(.opd._Z18sb_dx_write32_1f60ll)
    . = . < 0x1188 ? 0x1188 : .;
    */lv0/src/io/sb_registers.o(.opd._ZN9sb_device14write8_100000aEjl)
    . = . < 0x11a0 ? 0x11a0 : .;
    */lv0/src/io/sb_registers.o(.opd._ZN9sb_device13read8_1000009EjPh)
    . = . < 0x11b8 ? 0x11b8 : .;
    */lv0/src/io/sb_registers.o(.opd._ZN9sb_device14write8_100003aEl)
    . = . < 0x11d0 ? 0x11d0 : .;
    */lv0/src/io/sb_registers.o(.opd._ZN9sb_device13read8_1000039Ev)
    . = . < 0x11e8 ? 0x11e8 : .;
    */lv0/src/io/init_sb_device.o(.opd._ZN9sb_device10initializeEli)
    . = . < 0x1200 ? 0x1200 : .;
    */lv0/src/io/get_sb_device.o(.opd._Z13get_sb_devicev)
    . = . < 0x1218 ? 0x1218 : .;
    */lv0/src/component/host_get_file_size.o(.opd._Z18host_get_file_sizePKcPm)
    . = . < 0x1230 ? 0x1230 : .;
    */lv0/src/component/host_put_file.o(.opd._Z13host_put_filePKcmPKvmPm)
    . = . < 0x1248 ? 0x1248 : .;
    */lv0/src/component/host_get_file.o(.opd._Z13host_get_filePKcmPvmPm)
    . = . < 0x1260 ? 0x1260 : .;
    */lv0/src/log/default_print_hook.o(.opd._Z18default_print_hookPKc)
    . = . < 0x1278 ? 0x1278 : .;
    */lv0/src/storage/storage_device__get_mapped_address.o(.opd.storage_device__get_mapped_address)
    . = . < 0x1290 ? 0x1290 : .;
    */lv0/src/storage/storage_device__get_mapped_address.o(.opd.storage_device__get_block_size)
    . = . < 0x12a8 ? 0x12a8 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage16get_device_countEv)
    . = . < 0x12c0 ? 0x12c0 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage18get_mapped_addressEmmPm)
    . = . < 0x12d8 ? 0x12d8 : .;
    */lv0/src/storage/storage_manager.o(.opd._ZN15storage_managerD0Ev)
    . = . < 0x12f0 ? 0x12f0 : .;
    */lv0/src/storage/storage_manager.o(.opd._ZN15storage_managerD1Ev)
    . = . < 0x1308 ? 0x1308 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage8get_sizeEmPm)
    . = . < 0x1320 ? 0x1320 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage4readEmmmPcPm)
    . = . < 0x1338 ? 0x1338 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage5closeEm)
    . = . < 0x1350 ? 0x1350 : .;
    */lv0/src/storage/storage.o(.opd._ZN7storage4openEm)
    . = . < 0x1368 ? 0x1368 : .;
    */lv0/src/storage/storage.o(.opd._GLOBAL__I_g_storage)
    . = . < 0x1380 ? 0x1380 : .;
    */lv0/src/io/iommu.o(.opd._Z18flush_dcache_rangemj)
    . = . < 0x1398 ? 0x1398 : .;
    */lv0/src/io/get_iommu_context.o(.opd._ZN13iommu_contextC1Ev)
    . = . < 0x13b0 ? 0x13b0 : .;
    */lv0/src/io/get_iommu_context.o(.opd._Z17get_iommu_contextv)
    . = . < 0x13c8 ? 0x13c8 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context19allocate_iost_entryEPPl)
    . = . < 0x13e0 ? 0x13e0 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context19allocate_iopt_entryEPmj)
    . = . < 0x13f8 ? 0x13f8 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context18clear_iopt_entriesEmj)
    . = . < 0x1410 ? 0x1410 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context21invalidate_iost_cacheEm)
    . = . < 0x1428 ? 0x1428 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context22invalidate4_iopt_cacheEmj)
    . = . < 0x1440 ? 0x1440 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context15free_io_addressEm)
    . = . < 0x1458 ? 0x1458 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context10initializeEmmjmj)
    . = . < 0x1470 ? 0x1470 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context7disableEv)
    . = . < 0x1488 ? 0x1488 : .;
    */lv0/src/io/iommu.o(.opd._ZN13iommu_context19allocate_io_addressEjljijjjj)
    . = . < 0x14a0 ? 0x14a0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z18get_spu_slot_statel)
    . = . < 0x14b8 ? 0x14b8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z12get_spu_slotl)
    . = . < 0x14d0 ? 0x14d0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z22get_spu_slot_saved_sr1l)
    . = . < 0x14e8 ? 0x14e8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z21change_spu_slot_statel14spu_slot_state)
    . = . < 0x1500 ? 0x1500 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24set_spu_slot_params_sentl)
    . = . < 0x1518 ? 0x1518 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24get_spu_slot_params_sentm)
    . = . < 0x1530 ? 0x1530 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24spuctl_suspend_dma_queuem)
    . = . < 0x1548 ? 0x1548 : .;
    */lv0/src/spu/spuctl.o(.opd._Z22spuctl_purge_dma_queuem)
    . = . < 0x1560 ? 0x1560 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24spuctl_restart_dma_queuem)
    . = . < 0x1578 ? 0x1578 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24spuctl_set_channel_countlll)
    . = . < 0x1590 ? 0x1590 : .;
    */lv0/src/spu/spuctl.o(.opd._Z20spuctl_read_out_mboxlPi)
    . = . < 0x15a8 ? 0x15a8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z30spuctl_clear_mailbox_interruptl)
    . = . < 0x15c0 ? 0x15c0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z25spuctl_read_out_intr_mboxlPj)
    . = . < 0x15d8 ? 0x15d8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z28spuctl_is_proxy_dma_completelc)
    . = . < 0x15f0 ? 0x15f0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z22spuctl_issue_proxy_dmaljljjjj)
    . = . < 0x1608 ? 0x1608 : .;
    */lv0/src/spu/spuctl.o(.opd._Z22spuctl_write_in_mbox64lm)
    . = . < 0x1620 ? 0x1620 : .;
    */lv0/src/spu/spuctl.o(.opd._Z19spuctl_stop_requestl)
    . = . < 0x1638 ? 0x1638 : .;
    */lv0/src/spu/spuctl.o(.opd._Z17spuctl_get_statusl)
    . = . < 0x1650 ? 0x1650 : .;
    */lv0/src/spu/spuctl.o(.opd._Z29spuctl_request_isolation_exitl)
    . = . < 0x1668 ? 0x1668 : .;
    */lv0/src/spu/spuctl.o(.opd._Z25spuctl_clear_sr1_relocatel)
    . = . < 0x1680 ? 0x1680 : .;
    */lv0/src/spu/spuctl.o(.opd._Z18spuctl_set_mfc_sr1lm)
    . = . < 0x1698 ? 0x1698 : .;
    */lv0/src/spu/spuctl.o(.opd._Z18spuctl_get_mfc_sr1l)
    . = . < 0x16b0 ? 0x16b0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z17save_spu_slot_sr1l)
    . = . < 0x16c8 ? 0x16c8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z31spuctl_clear_master_run_controll)
    . = . < 0x16e0 ? 0x16e0 : .;
    */lv0/src/spu/spuctl.o(.opd._Z30spuctl_clear_class0_interruptslm)
    . = . < 0x16f8 ? 0x16f8 : .;
    */lv0/src/spu/spuctl.o(.opd._Z27spuctl_clear_stop_interruptl)
    . = . < 0x1710 ? 0x1710 : .;
    */lv0/src/spu/spuctl.o(.opd._Z28spuctl_write_signal_notify64lm)
    . = . < 0x1728 ? 0x1728 : .;
    */lv0/src/spu/spuctl.o(.opd._Z29spuctl_request_isolation_loadlmPv)
    . = . < 0x1740 ? 0x1740 : .;
    */lv0/src/spu/spuctl.o(.opd._Z24start_spu_isolation_loadlm)
    . = . < 0x1758 ? 0x1758 : .;
    */lv0/src/boot/alt_sys_parm.o(.opd._Z13boot_parm_nopP9boot_parm)
    . = . < 0x1770 ? 0x1770 : .;
    */lv0/src/boot/alt_sys_parm.o(.opd._Z19create_alt_sys_parmP9boot_parmPKcm)
    . = . < 0x1788 ? 0x1788 : .;
    */lv0/src/boot/alt_sys_parm.o(.opd._Z18build_alt_sys_parmP9boot_parm)
    . = . < 0x17a0 ? 0x17a0 : .;
    */lv0/src/boot/alt_sys_parm.o(.opd._Z21fill_boot_parm_headerl)
    . = . < 0x17b8 ? 0x17b8 : .;
    */lv0/src/boot/boot_parm.o(.opd._ZN9boot_parmC1Ev)
    . = . < 0x17d0 ? 0x17d0 : .;
    */lv0/src/boot/boot_parm.o(.opd._Z13get_boot_parmv)
    . = . < 0x17e8 ? 0x17e8 : .;
    */lv0/src/boot/alt_sys_parm.o(.opd._Z15setup_boot_parmP9boot_parm)
    . = . < 0x1800 ? 0x1800 : .;
    */lv0/src/boot/boot_loader.o(.opd._ZN11boot_loader17print_boot_bannerEv)
    . = . < 0x1818 ? 0x1818 : .;
    */lv0/src/boot/boot_loader.o(.opd._ZN11boot_loader17load_and_boot_lv1Ev)
    . = . < 0x1830 ? 0x1830 : .;
    */lv0/src/boot/boot_loader.o(.opd._ZN11boot_loader3runEv)
    . = . < 0x1848 ? 0x1848 : .;
    */lv0/src/boot/boot_loader.o(.opd._ZN11boot_loaderC1Ev)
    . = . < 0x1860 ? 0x1860 : .;
    */lv0/src/boot/boot_loader.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1878 ? 0x1878 : .;
    */lv0/src/boot/boot_loader.o(.opd._GLOBAL__I__ZN11boot_loaderC2Ev)
    . = . < 0x1890 ? 0x1890 : .;
    */lv0/src/boot/boot_loader.o(.opd._ZN11boot_loader14component_initEv)
    . = . < 0x18a8 ? 0x18a8 : .;
    */lv0/src/boot/boot_cell_os.o(.opd._Z10run_lv1ldrlPKvPj)
    . = . < 0x18c0 ? 0x18c0 : .;
    */lv0/src/boot/boot_cell_os.o(.opd._ZN11boot_loader12boot_cell_osEmPj)
    . = . < 0x18d8 ? 0x18d8 : .;
    */lv0/src/boot/boot_cell_os.o(.opd._Z14decrypt_lv1ldrPvj)
    . = . < 0x18f0 ? 0x18f0 : .;
    */lv0/src/boot/component_info.o(.opd._Z24get_component_info_tablev)
    . = . < 0x1908 ? 0x1908 : .;
    */lv0/src/boot/component_info.o(.opd._ZN20component_info_table3setEjmm)
    . = . < 0x1920 ? 0x1920 : .;
    */lv0/src/boot/component_info.o(.opd._ZN20component_info_table5clearEv)
    . = . < 0x1938 ? 0x1938 : .;
    */lv0/src/boot/component_info.o(.opd._ZN20component_info_table10copy_entryEPmS0_mm)
    . = . < 0x1950 ? 0x1950 : .;
    */lv0/src/boot/component_info.o(.opd._ZN20component_info_table3getEjPmS0_)
    . = . < 0x1968 ? 0x1968 : .;
    */lv0/src/util/lv0_memmove.o(.opd._Z11lv0_memmovePcPKcm)
    . = . < 0x1980 ? 0x1980 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budgetC1Ev)
    . = . < 0x1998 ? 0x1998 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budget8allocateEmm)
    . = . < 0x19b0 ? 0x19b0 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budget13get_used_sizeEPl)
    . = . < 0x19c8 ? 0x19c8 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budget13get_free_sizeEPl)
    . = . < 0x19e0 ? 0x19e0 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budget10initializeEmm)
    . = . < 0x19f8 ? 0x19f8 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budgetD0Ev)
    . = . < 0x1a10 ? 0x1a10 : .;
    */lv0/src/platform/memory_budgets.o(.opd._ZN13memory_budgetD1Ev)
    . = . < 0x1a28 ? 0x1a28 : .;
    */lv0/src/platform/memory_budgets.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1a40 ? 0x1a40 : .;
    */lv0/src/platform/memory_budgets.o(.opd._GLOBAL__I__ZN13memory_budgetC2Ev)
    . = . < 0x1a58 ? 0x1a58 : .;
    */lv0/src/log/log_ring.o(.opd._Z12get_log_ringv)
    . = . < 0x1a70 ? 0x1a70 : .;
    */lv0/src/log/log_ring.o(.opd._ZN8log_ring10initializeEmmPFlllE)
    . = . < 0x1a88 ? 0x1a88 : .;
    */lv0/src/log/log_ring.o(.opd._ZN8log_ring4dumpEPFlllE)
    . = . < 0x1aa0 ? 0x1aa0 : .;
    */lv0/src/log/log_ring.o(.opd._ZN8log_ring12write_stringEPKc)
    . = . < 0x1ab8 ? 0x1ab8 : .;
    */lv0/src/log/log_ring.o(.opd._Z21write_log_to_log_ringPKc)
    . = . < 0x1ad0 ? 0x1ad0 : .;
    */lv0/src/util/lv0_memset.o(.opd._Z10lv0_memsetPvim)
    . = . < 0x1ae8 ? 0x1ae8 : .;
    */lv0/src/console/init_debug_interface.o(.opd._Z14link_to_cp_ch4PP7cp_linkP12pci_devparamP9cp_bridge)
    . = . < 0x1b00 ? 0x1b00 : .;
    */lv0/src/console/init_debug_interface.o(.opd._Z14link_to_cp_ch0PP7cp_linkP12pci_devparamP9cp_bridge)
    . = . < 0x1b18 ? 0x1b18 : .;
    */lv0/src/console/finalize_debug_interface.o(.opd._Z24finalize_debug_interfacePc)
    . = . < 0x1b30 ? 0x1b30 : .;
    */lv0/src/console/init_debug_interface.o(.opd._Z20init_debug_interfaceP16physical_console)
    . = . < 0x1b48 ? 0x1b48 : .;
    */lv0/src/platform/pio.o(.opd._Z7get_piov)
    . = . < 0x1b60 ? 0x1b60 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio12write_fff500Et)
    . = . < 0x1b78 ? 0x1b78 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio11read_fff504EPt)
    . = . < 0x1b90 ? 0x1b90 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio11read_fff500EPt)
    . = . < 0x1ba8 ? 0x1ba8 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio13reverse_bits8Eh)
    . = . < 0x1bc0 ? 0x1bc0 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio16read_output_byteEv)
    . = . < 0x1bd8 ? 0x1bd8 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio17write_output_byteEh)
    . = . < 0x1bf0 ? 0x1bf0 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio32is_emmcbridge_interrupt_assertedEv)
    . = . < 0x1c08 ? 0x1c08 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio25is_ss2_interrupt_assertedEv)
    . = . < 0x1c20 ? 0x1c20 : .;
    */lv0/src/platform/pio.o(.opd._ZN3pio9configureEPcjj)
    . = . < 0x1c38 ? 0x1c38 : .;
    */lv0/src/platform/pio.o(.opd._Z25poll_emmcbridge_interruptv)
    . = . < 0x1c50 ? 0x1c50 : .;
    */lv0/src/platform/pio.o(.opd._Z18poll_ss2_interruptv)
    . = . < 0x1c68 ? 0x1c68 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform10stub_slot4EPvl)
    . = . < 0x1c80 ? 0x1c80 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform10stub_slot5EPv)
    . = . < 0x1c98 ? 0x1c98 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform10stub_slot6Ev)
    . = . < 0x1cb0 ? 0x1cb0 : .;
    */lv0/src/platform/platform.o(.opd._Z24write_eeprom_nv1_u32_at8j)
    . = . < 0x1cc8 ? 0x1cc8 : .;
    */lv0/src/platform/platform.o(.opd._Z18handle_fatal_errorj)
    . = . < 0x1ce0 ? 0x1ce0 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform3logEjPKcz)
    . = . < 0x1cf8 ? 0x1cf8 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform20print_chip_revisionsEl)
    . = . < 0x1d10 ? 0x1d10 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform16print_spu_enableEl)
    . = . < 0x1d28 ? 0x1d28 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform17print_memory_infoEi)
    . = . < 0x1d40 ? 0x1d40 : .;
    */lv0/src/storage/get_eeprom_bootrom_trace_leve.o(.opd._Z23read_eeprom_nv4_u64_at5Pm)
    . = . < 0x1d58 ? 0x1d58 : .;
    */lv0/src/storage/get_eeprom_bootrom_trace_leve.o(.opd._Z29get_eeprom_bootrom_trace_levePh)
    . = . < 0x1d70 ? 0x1d70 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform13set_post_codeEll)
    . = . < 0x1d88 ? 0x1d88 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform20post_load_lv1_failedEv)
    . = . < 0x1da0 ? 0x1da0 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform18post_load_lv1_doneEv)
    . = . < 0x1db8 ? 0x1db8 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform19post_load_lv1_startEv)
    . = . < 0x1dd0 ? 0x1dd0 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform14post_init_doneEv)
    . = . < 0x1de8 ? 0x1de8 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform10initializeEv)
    . = . < 0x1e00 ? 0x1e00 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform8finalizeEv)
    . = . < 0x1e18 ? 0x1e18 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform17print_system_infoEv)
    . = . < 0x1e30 ? 0x1e30 : .;
    */lv0/src/platform/platform.o(.opd._ZN8platform13initialize_ioEv)
    . = . < 0x1e48 ? 0x1e48 : .;
    */lv0/src/platform/platform.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1e60 ? 0x1e60 : .;
    */lv0/src/platform/platform.o(.opd._GLOBAL__I_g_unused_416e8)
    . = . < 0x1e78 ? 0x1e78 : .;
    */lv0/src/platform/platform_pwrmgr.o(.opd._ZN15platform_pwrmgr6rebootEv)
    . = . < 0x1e90 ? 0x1e90 : .;
    */lv0/src/platform/platform_pwrmgr.o(.opd._ZN15platform_pwrmgr8shutdownEv)
    . = . < 0x1ea8 ? 0x1ea8 : .;
    */lv0/src/platform/platform_pwrmgr.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x1ec0 ? 0x1ec0 : .;
    */lv0/src/platform/platform_pwrmgr.o(.opd._GLOBAL__I__ZN15platform_pwrmgr8shutdownEv)
    . = . < 0x1ed8 ? 0x1ed8 : .;
    */lv0/src/util/delay.o(.opd._Z16delay_hdec_ticksi)
    . = . < 0x1ef0 ? 0x1ef0 : .;
    */lv0/src/util/delay.o(.opd._Z8delay_msi)
    . = . < 0x1f08 ? 0x1f08 : .;
    */lv0/src/component/host_file_loader.o(.opd._ZN16host_file_loader17load_component_toEjPKcPvm)
    . = . < 0x1f20 ? 0x1f20 : .;
    */lv0/src/component/host_file_loader.o(.opd._ZN16host_file_loader14load_componentEjPKcP13memory_budgetjmmPmS4_)
    . = . < 0x1f38 ? 0x1f38 : .;
    */lv0/src/component/host_file_loader.o(.opd._ZN16host_file_loaderD0Ev)
    . = . < 0x1f50 ? 0x1f50 : .;
    */lv0/src/component/host_file_loader.o(.opd._ZN16host_file_loaderD1Ev)
    . = . < 0x1f68 ? 0x1f68 : .;
    */lv0/src/component/host_file_loader.o(.opd._ZN16host_file_loaderC1Ev)
    . = . < 0x1f80 ? 0x1f80 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storageC1Ev)
    . = . < 0x1f98 ? 0x1f98 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storage8set_baseEm)
    . = . < 0x1fb0 ? 0x1fb0 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storage4openEv)
    . = . < 0x1fc8 ? 0x1fc8 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storage5closeEv)
    . = . < 0x1fe0 ? 0x1fe0 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storage18get_mapped_addressEmPm)
    . = . < 0x1ff8 ? 0x1ff8 : .;
    */lv0/src/flash/mapped_storage.o(.opd._ZN14mapped_storage4readEmmPcPm)
    . = . < 0x2010 ? 0x2010 : .;
    */lv0/src/syscon/syscon_clock.o(.opd._Z22syscon_get_clock_valuePhhhi)
    . = . < 0x2028 ? 0x2028 : .;
    */lv0/src/syscon/syscon_clock.o(.opd._Z20syscon_get_ref_clockPji)
    . = . < 0x2040 ? 0x2040 : .;
    */lv0/src/syscon/syscon_get_hw_config.o(.opd._Z20syscon_get_hw_configPmS_)
    . = . < 0x2058 ? 0x2058 : .;
    */lv0/src/syscon/syscon_get_platform_id.o(.opd._Z22syscon_get_platform_idPh)
    . = . < 0x2070 ? 0x2070 : .;
    */lv0/src/syscon/syscon_get_core_clock_multiplier.o(.opd._Z32syscon_get_core_clock_multiplierPl)
    . = . < 0x2088 ? 0x2088 : .;
    */lv0/src/syscon/sc_console.o(.opd._Z19syscon_write_bufferPKhm)
    . = . < 0x20a0 ? 0x20a0 : .;
    */lv0/src/syscon/sc_console.o(.opd._Z19syscon_write_stringPKc)
    . = . < 0x20b8 ? 0x20b8 : .;
    */lv0/src/util/copy_qwords_to_mmio.o(.opd._Z19copy_qwords_to_mmioPcS_j)
    . = . < 0x20d0 ? 0x20d0 : .;
    */lv0/src/util/copy_qwords_from_mmio.o(.opd._Z21copy_qwords_from_mmioPcS_j)
    . = . < 0x20e8 ? 0x20e8 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon20get_protocol_versionEv)
    . = . < 0x2100 ? 0x2100 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon18raise_sc_interruptEj)
    . = . < 0x2118 ? 0x2118 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon22get_swbint_status_ver1Ev)
    . = . < 0x2130 ? 0x2130 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon17get_swbint_statusEv)
    . = . < 0x2148 ? 0x2148 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon11wait_swbintEj)
    . = . < 0x2160 ? 0x2160 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon16sc_calc_checksumEPhjPt)
    . = . < 0x2178 ? 0x2178 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon20sc_calc_checksum_negEPhjPt)
    . = . < 0x2190 ? 0x2190 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon18read_cmpl_msg_ver1EP10sc_commandPvjPiP10sc_msg_ids)
    . = . < 0x21a8 ? 0x21a8 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon13read_cmpl_msgEP10sc_commandPvjPiP10sc_msg_ids)
    . = . < 0x21c0 ? 0x21c0 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon19write_init_msg_ver1EjPKvjP10sc_msg_idsi)
    . = . < 0x21d8 ? 0x21d8 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon14write_init_msgEjPKvjP10sc_msg_ids)
    . = . < 0x21f0 ? 0x21f0 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon16send_and_receiveEjPKvjPvjPii)
    . = . < 0x2208 ? 0x2208 : .;
    */lv0/src/syscon/syscon.o(.opd._ZN6syscon10initializeElii)
    . = . < 0x2220 ? 0x2220 : .;
    */lv0/src/syscon/get_syscon_device.o(.opd._Z17get_syscon_devicev)
    . = . < 0x2238 ? 0x2238 : .;
    */lv0/src/syscon/sc_nv_storage.o(.opd._ZN13sc_nv_storage12check_statusEj)
    . = . < 0x2250 ? 0x2250 : .;
    */lv0/src/syscon/sc_nv_storage.o(.opd._ZN13sc_nv_storage5writeEjjjPKv)
    . = . < 0x2268 ? 0x2268 : .;
    */lv0/src/syscon/sc_nv_storage.o(.opd._ZN13sc_nv_storage4readEjjjPvPj)
    . = . < 0x2280 ? 0x2280 : .;
    */lv0/src/syscon/syscon_syspm_get_33.o(.opd._Z19syscon_syspm_get_33Pj)
    . = . < 0x2298 ? 0x2298 : .;
    */lv0/src/syscon/syscon_set_wake_source.o(.opd._Z22syscon_set_wake_sourcei)
    . = . < 0x22b0 ? 0x22b0 : .;
    */lv0/src/syscon/syscon_power_off.o(.opd._Z20syscon_get_wake_infoPhS_S_S_S_Pj)
    . = . < 0x22c8 ? 0x22c8 : .;
    */lv0/src/syscon/syscon_power_off.o(.opd._Z26syscon_power_off_with_codeiii)
    . = . < 0x22e0 ? 0x22e0 : .;
    */lv0/src/syscon/syscon_reboot.o(.opd._Z13syscon_rebootv)
    . = . < 0x22f8 ? 0x22f8 : .;
    */lv0/src/syscon/syscon_shutdown.o(.opd._Z15syscon_shutdownv)
    . = . < 0x2310 ? 0x2310 : .;
    */lv0/src/component/component_loaders.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x2328 ? 0x2328 : .;
    */lv0/src/component/component_loaders.o(.opd._GLOBAL__I_g_flash_format_1_component_loader)
    . = . < 0x2340 ? 0x2340 : .;
    */lv0/src/component/component_loaders.o(.opd._Z23select_component_loaderv)
    . = . < 0x2358 ? 0x2358 : .;
    */lv0/src/flash/flash_storage.o(.opd._ZN21flash_storage_manager16get_device_tableEv)
    . = . < 0x2370 ? 0x2370 : .;
    */lv0/src/flash/flash_storage.o(.opd._ZN21flash_storage_manager16get_device_countEv)
    . = . < 0x2388 ? 0x2388 : .;
    */lv0/src/storage/storage_manager__base_dtor.o(.opd.storage_manager__base_dtor)
    . = . < 0x23a0 ? 0x23a0 : .;
    */lv0/src/flash/flash_storage.o(.opd._ZN21flash_storage_managerD0Ev)
    . = . < 0x23b8 ? 0x23b8 : .;
    */lv0/src/flash/flash_storage.o(.opd._ZN21flash_storage_managerD1Ev)
    . = . < 0x23d0 ? 0x23d0 : .;
    */lv0/src/flash/flash_storage.o(.opd._Z23init_nand_flash_storagePcmm)
    . = . < 0x23e8 ? 0x23e8 : .;
    */lv0/src/flash/flash_storage.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x2400 ? 0x2400 : .;
    */lv0/src/flash/flash_storage.o(.opd._GLOBAL__I__Z23init_nand_flash_storagePcmm)
    . = . < 0x2418 ? 0x2418 : .;
    */lv0/src/console/uart.o(.opd._Z8get_uartv)
    . = . < 0x2430 ? 0x2430 : .;
    */lv0/src/console/uart.o(.opd._ZN4uart4putcEi)
    . = . < 0x2448 ? 0x2448 : .;
    */lv0/src/console/uart.o(.opd._ZN4uart4putsEPKc)
    . = . < 0x2460 ? 0x2460 : .;
    */lv0/src/console/uart.o(.opd._ZN4uart4initEPVjijiii)
    . = . < 0x2478 ? 0x2478 : .;
    */lv0/src/console/uart.o(.opd._Z17uart_console_putsPKc)
    . = . < 0x2490 ? 0x2490 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage4openEv)
    . = . < 0x24a8 ? 0x24a8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage12sector_startEm)
    . = . < 0x24c0 ? 0x24c0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage11sector_sizeEm)
    . = . < 0x24d8 ? 0x24d8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage19unlock_bypass_enterEm)
    . = . < 0x24f0 ? 0x24f0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage18unlock_bypass_exitEm)
    . = . < 0x2508 ? 0x2508 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage8get_sizeEPm)
    . = . < 0x2520 ? 0x2520 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage14get_block_sizeEPm)
    . = . < 0x2538 ? 0x2538 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage6swap16Ej)
    . = . < 0x2550 ? 0x2550 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage8identifyEv)
    . = . < 0x2568 ? 0x2568 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage15for_each_sectorEmmMS_FimE)
    . = . < 0x2580 ? 0x2580 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage13protect_rangeEmm)
    . = . < 0x2598 ? 0x2598 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage9dyb_enterEm)
    . = . < 0x25b0 ? 0x25b0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage11cmdset_exitEm)
    . = . < 0x25c8 ? 0x25c8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage16dyb_is_protectedEm)
    . = . < 0x25e0 ? 0x25e0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage11dyb_protectEm)
    . = . < 0x25f8 ? 0x25f8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage13dyb_unprotectEm)
    . = . < 0x2610 ? 0x2610 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage18is_protected_type8Em)
    . = . < 0x2628 ? 0x2628 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage13protect_type8Em)
    . = . < 0x2640 ? 0x2640 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage15unprotect_type8Em)
    . = . < 0x2658 ? 0x2658 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage12sector_groupEm)
    . = . < 0x2670 ? 0x2670 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage18is_protected_type7Em)
    . = . < 0x2688 ? 0x2688 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage13progress_tickEv)
    . = . < 0x26a0 ? 0x26a0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage10halt_blinkEv)
    . = . < 0x26b8 ? 0x26b8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage20program_write_bufferEmPKtj)
    . = . < 0x26d0 ? 0x26d0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage21unlock_bypass_programEmt)
    . = . < 0x26e8 ? 0x26e8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage7programEmPKcm)
    . = . < 0x2700 ? 0x2700 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage12erase_sectorEm)
    . = . < 0x2718 ? 0x2718 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage12write_sectorEmmPKcm)
    . = . < 0x2730 ? 0x2730 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage5writeEmmPKcPm)
    . = . < 0x2748 ? 0x2748 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storageC1Ev)
    . = . < 0x2760 ? 0x2760 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage4initEmmij)
    . = . < 0x2778 ? 0x2778 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage9translateEm)
    . = . < 0x2790 ? 0x2790 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage15unprotect_type7Em)
    . = . < 0x27a8 ? 0x27a8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN16spansion_storage13protect_type7Em)
    . = . < 0x27c0 ? 0x27c0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plain4readEmmPcPm)
    . = . < 0x27d8 ? 0x27d8 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plainD0Ev)
    . = . < 0x27f0 ? 0x27f0 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plainD1Ev)
    . = . < 0x2808 ? 0x2808 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plainC1Ev)
    . = . < 0x2820 ? 0x2820 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plain9translateEm)
    . = . < 0x2838 ? 0x2838 : .;
    */lv0/src/flash/spansion_storage.o(.opd._ZN22spansion_storage_plain18get_mapped_addressEmPm)
    . = . < 0x2850 ? 0x2850 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z15get_spu_ls_addrl)
    . = . < 0x2868 ? 0x2868 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z14get_spu_statusl)
    . = . < 0x2880 ? 0x2880 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z17get_spu_mbox_statl)
    . = . < 0x2898 ? 0x2898 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z26get_spu_in_mbox_free_countl)
    . = . < 0x28b0 ? 0x28b0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z17write_spu_in_mboxlj)
    . = . < 0x28c8 ? 0x28c8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z22get_spu_out_mbox_countl)
    . = . < 0x28e0 ? 0x28e0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z17read_spu_out_mboxlPi)
    . = . < 0x28f8 ? 0x28f8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z27get_spu_out_intr_mbox_countl)
    . = . < 0x2910 ? 0x2910 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z26clear_spu_mfc_sr1_relocatel)
    . = . < 0x2928 ? 0x2928 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25set_spu_mfc_sr1_ls_decodel)
    . = . < 0x2940 ? 0x2940 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z26set_spu_mfc_sr1_master_runl)
    . = . < 0x2958 ? 0x2958 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25set_spu_mfc_sr1_bus_tlbiem)
    . = . < 0x2970 ? 0x2970 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z14is_spu_stoppedlPj)
    . = . < 0x2988 ? 0x2988 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z16init_spu_mfc_sr1l)
    . = . < 0x29a0 ? 0x29a0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z17get_spu_stop_codelm)
    . = . < 0x29b8 ? 0x29b8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z19write_spu_in_mbox64lm)
    . = . < 0x29d0 ? 0x29d0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25clear_spu_int_stat_class0lm)
    . = . < 0x29e8 ? 0x29e8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z16get_spu_int_statli)
    . = . < 0x2a00 ? 0x2a00 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z23set_spu_mfc_rm_boundarylm)
    . = . < 0x2a18 ? 0x2a18 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z28clear_spu_mfc_sr1_master_runl)
    . = . < 0x2a30 ? 0x2a30 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z15set_spu_mfc_sr1lm)
    . = . < 0x2a48 ? 0x2a48 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z15get_spu_mfc_sr1l)
    . = . < 0x2a60 ? 0x2a60 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z30clear_spu_int_stat_class2_haltl)
    . = . < 0x2a78 ? 0x2a78 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z35clear_spu_int_stat_class2_tag_groupl)
    . = . < 0x2a90 ? 0x2a90 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z30clear_spu_int_stat_class2_stopl)
    . = . < 0x2aa8 ? 0x2aa8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z33clear_spu_int_stat_class2_mailboxl)
    . = . < 0x2ac0 ? 0x2ac0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z21set_spu_channel_countll)
    . = . < 0x2ad8 ? 0x2ad8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z21set_spu_channel_indexll)
    . = . < 0x2af0 ? 0x2af0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z28is_spu_mfc_dma_queue_runningm)
    . = . < 0x2b08 ? 0x2b08 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25is_spu_mfc_purge_completem)
    . = . < 0x2b20 ? 0x2b20 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z27is_spu_mfc_suspend_completem)
    . = . < 0x2b38 ? 0x2b38 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25restart_spu_mfc_dma_queuem)
    . = . < 0x2b50 ? 0x2b50 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z23purge_spu_mfc_dma_queuem)
    . = . < 0x2b68 ? 0x2b68 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25suspend_spu_mfc_dma_queuem)
    . = . < 0x2b80 ? 0x2b80 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z28set_spu_privcntl_load_enablel)
    . = . < 0x2b98 ? 0x2b98 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z22read_spu_out_intr_mboxlPj)
    . = . < 0x2bb0 ? 0x2bb0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z21enqueue_spu_proxy_dmaljljjjj)
    . = . < 0x2bc8 ? 0x2bc8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z24request_spu_isolate_exitl)
    . = . < 0x2be0 ? 0x2be0 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z24request_spu_isolate_loadl)
    . = . < 0x2bf8 ? 0x2bf8 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z16request_spu_stopl)
    . = . < 0x2c10 ? 0x2c10 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z21write_spu_sig_notify2lj)
    . = . < 0x2c28 ? 0x2c28 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z21write_spu_sig_notify1lj)
    . = . < 0x2c40 ? 0x2c40 : .;
    */lv0/src/spu/spu_mmio.o(.opd._Z25is_spu_proxy_tag_completelc)
    . = . < 0x2c58 ? 0x2c58 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl8set_baseEl)
    . = . < 0x2c70 ? 0x2c70 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl10get_statusEv)
    . = . < 0x2c88 ? 0x2c88 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl12write_sectorEPKti)
    . = . < 0x2ca0 ? 0x2ca0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl21start_identify_deviceEv)
    . = . < 0x2cb8 ? 0x2cb8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl18start_erase_sectorEjt)
    . = . < 0x2cd0 ? 0x2cd0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl18start_write_sectorEjt)
    . = . < 0x2ce8 ? 0x2ce8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl10start_idleEv)
    . = . < 0x2d00 ? 0x2d00 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl17start_read_sectorEjtt)
    . = . < 0x2d18 ? 0x2d18 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl13get_reg_44016Ev)
    . = . < 0x2d30 ? 0x2d30 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl8get_intrEv)
    . = . < 0x2d48 ? 0x2d48 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl4idleEv)
    . = . < 0x2d60 ? 0x2d60 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl11read_sectorEPti)
    . = . < 0x2d78 ? 0x2d78 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl15identify_deviceEPt)
    . = . < 0x2d90 ? 0x2d90 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._ZN15nand_flash_ctrl12erase_sectorEv)
    . = . < 0x2da8 ? 0x2da8 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.opd._Z19get_nand_flash_ctrlv)
    . = . < 0x2dc0 ? 0x2dc0 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flashC2Ev)
    . = . < 0x2dd8 ? 0x2dd8 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash4openEv)
    . = . < 0x2df0 ? 0x2df0 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash5closeEv)
    . = . < 0x2e08 ? 0x2e08 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash14get_block_sizeEPm)
    . = . < 0x2e20 ? 0x2e20 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash8get_sizeEPm)
    . = . < 0x2e38 ? 0x2e38 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash13configure_dmaEllbPcmm)
    . = . < 0x2e50 ? 0x2e50 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash5writeEmmPKcPm)
    . = . < 0x2e68 ? 0x2e68 : .;
    */lv0/src/flash/nand_flash.o(.opd._ZN10nand_flash4readEmmPcPm)
    . = . < 0x2e80 ? 0x2e80 : .;
    */lv0/src/flash/nand_flash_abs.o(.opd._ZN14nand_flash_abs8get_sizeEPm)
    . = . < 0x2e98 ? 0x2e98 : .;
    */lv0/src/flash/nand_flash_abs.o(.opd._ZN14nand_flash_abs5writeEmmPKcPm)
    . = . < 0x2eb0 ? 0x2eb0 : .;
    */lv0/src/flash/nand_flash_abs.o(.opd._ZN14nand_flash_abs4readEmmPcPm)
    . = . < 0x2ec8 ? 0x2ec8 : .;
    */lv0/src/flash/nand_flash_abs.o(.opd._ZN14nand_flash_absC1Ev)
    . = . < 0x2ee0 ? 0x2ee0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._Z29set_nand_flash_ready_callbackPFlvE)
    . = . < 0x2ef8 ? 0x2ef8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._Z19is_nand_flash_readyv)
    . = . < 0x2f10 ? 0x2f10 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl21erase_sectors_poll_16EjtPFhvE)
    . = . < 0x2f28 ? 0x2f28 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl18erase_sectors_pollEjjPFhvE)
    . = . < 0x2f40 ? 0x2f40 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl17erase_sector_pollEjj)
    . = . < 0x2f58 ? 0x2f58 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl21write_sectors_poll_16EPKcjtPFhvE)
    . = . < 0x2f70 ? 0x2f70 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl18write_sectors_pollEPKcjjPFhvE)
    . = . < 0x2f88 ? 0x2f88 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl17write_sector_pollEPKcjj)
    . = . < 0x2fa0 ? 0x2fa0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.opd._Z18is_nand_flash_bootv)
    . = . < 0x2fb8 ? 0x2fb8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._Z8kick_dmamj)
    . = . < 0x2fd0 ? 0x2fd0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._Z14get_dma_statusv)
    . = . < 0x2fe8 ? 0x2fe8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl16read_sectors_dmaEmjjPFhvE)
    . = . < 0x3000 ? 0x3000 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl15read_sector_dmaEmjj)
    . = . < 0x3018 ? 0x3018 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl17read_sectors_pollEPcjjPFhvE)
    . = . < 0x3030 ? 0x3030 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._ZN15nand_flash_ctrl16read_sector_pollEPcjj)
    . = . < 0x3048 ? 0x3048 : .;
    */lv0/src/flash/nand_flash_transfer.o(.opd._Z15get_device_sizePm)
    . = . < 0x3060 ? 0x3060 : .;
    */lv0/src/util/lv0_strlen.o(.opd._Z10lv0_strlenPKc)
    . = . < 0x3078 ? 0x3078 : .;
    */lv0/src/util/lv0_strncmp.o(.opd._Z11lv0_strncmpPKcS0_m)
    . = . < 0x3090 ? 0x3090 : .;
    */lv0/src/util/lv0_strncpy.o(.opd._Z11lv0_strncpyPcPKcl)
    . = . < 0x30a8 ? 0x30a8 : .;
    */lv0/src/util/lv0_strnlen.o(.opd._Z11lv0_strnlenPKcm)
    . = . < 0x30c0 ? 0x30c0 : .;
    */lv0/src/log/printf.o(.opd._Z19lv0_vfprintf_enginePFilPlliEljPKhPPh)
    . = . < 0x30d8 ? 0x30d8 : .;
    */lv0/src/log/printf.o(.opd._Z17snprintf_put_charPPcPjli)
    . = . < 0x30f0 ? 0x30f0 : .;
    */lv0/src/log/printf.o(.opd._Z13lv0_vsnprintfPcmPKcPv)
    . = . < 0x3108 ? 0x3108 : .;
    */lv0/src/log/printf.o(.opd._Z12lv0_snprintfPcmPKcz)
    . = . < 0x3120 ? 0x3120 : .;
    */lv0/src/storage/nv_storage.o(.opd._Z26is_sc_nv_storage_supportedv)
    . = . < 0x3138 ? 0x3138 : .;
    */lv0/src/storage/nv_storage.o(.opd._ZN10nv_storage5writeEjjjPvj)
    . = . < 0x3150 ? 0x3150 : .;
    */lv0/src/storage/nv_storage.o(.opd._ZN10nv_storage4readEjjjPvj)
    . = . < 0x3168 ? 0x3168 : .;
    */lv0/src/storage/nv_storage.o(.opd._Z13load_nv_entryj)
    . = . < 0x3180 ? 0x3180 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.opd._ZN16systemio_dmac_dxC1Ev)
    . = . < 0x3198 ? 0x3198 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.opd._Z20get_systemio_dmac_dxv)
    . = . < 0x31b0 ? 0x31b0 : .;
    */lv0/src/io/systemio_dmac_dx.o(.opd._ZN16systemio_dmac_dx9configureEmmmmm)
    . = . < 0x31c8 ? 0x31c8 : .;
    */lv0/src/io/systemio_dmac_dx.o(.opd._ZN16systemio_dmac_dx14start_transferEmmj)
    . = . < 0x31e0 ? 0x31e0 : .;
    */lv0/src/io/systemio_dmac_dx.o(.opd._ZN16systemio_dmac_dx8kick_dmaEmj)
    . = . < 0x31f8 ? 0x31f8 : .;
    */lv0/src/io/systemio_dmac_dx.o(.opd._ZN16systemio_dmac_dx14get_dma_statusEv)
    . = . < 0x3210 ? 0x3210 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._Z20get_systemio_dmac_pxv)
    . = . < 0x3228 ? 0x3228 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._ZN16systemio_dmac_px9configureEmmmj)
    . = . < 0x3240 ? 0x3240 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._ZN16systemio_dmac_px26is_ebus_interrupt_assertedEv)
    . = . < 0x3258 ? 0x3258 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._Z19poll_ebus_interruptv)
    . = . < 0x3270 ? 0x3270 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._ZN16systemio_dmac_px14get_dma_statusEv)
    . = . < 0x3288 ? 0x3288 : .;
    */lv0/src/io/systemio_dmac_px.o(.opd._ZN16systemio_dmac_px8kick_dmaEmj)
    . = . < 0x32a0 ? 0x32a0 : .;
    */lv0/src/log/log.o(.opd._Z15log_set_handleriPFlPKcE)
    . = . < 0x32b8 ? 0x32b8 : .;
    */lv0/src/log/log.o(.opd._Z17log_set_thresholdi)
    . = . < 0x32d0 ? 0x32d0 : .;
    */lv0/src/log/log.o(.opd._Z17log_get_thresholdv)
    . = . < 0x32e8 ? 0x32e8 : .;
    */lv0/src/log/log.o(.opd._Z21log_set_final_handlerPFljE)
    . = . < 0x3300 ? 0x3300 : .;
    */lv0/src/log/log.o(.opd._Z12log_dispatchiiPKcPv)
    . = . < 0x3318 ? 0x3318 : .;
    */lv0/src/log/log.o(.opd._Z9log_errorjPKcz)
    . = . < 0x3330 ? 0x3330 : .;
    */lv0/src/log/log.o(.opd._Z11log_messagePKcz)
    . = . < 0x3348 ? 0x3348 : .;
    */lv0/src/log/log.o(.opd._Z10log_printfiiPKcz)
    . = . < 0x3360 ? 0x3360 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z10bswap16_lot)
    . = . < 0x3378 ? 0x3378 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z18identify_nor_flashPVt)
    . = . < 0x3390 ? 0x3390 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z13syscon_printfPKcz)
    . = . < 0x33a8 ? 0x33a8 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z11uart_printfPKcz)
    . = . < 0x33c0 ? 0x33c0 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z16probe_nand_flashl)
    . = . < 0x33d8 ? 0x33d8 : .;
    */lv0/src/flash/flash_probe.o(.opd._Z17detect_flash_typel)
    . = . < 0x33f0 ? 0x33f0 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridgeC1Ev)
    . = . < 0x3408 ? 0x3408 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge10cfg_read16Ej)
    . = . < 0x3420 ? 0x3420 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge11cfg_write32Ejj)
    . = . < 0x3438 ? 0x3438 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge10cfg_read32Ej)
    . = . < 0x3450 ? 0x3450 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge18bridge_cfg_write32Ejj)
    . = . < 0x3468 ? 0x3468 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge17bridge_cfg_read32Ej)
    . = . < 0x3480 ? 0x3480 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge3mapEj)
    . = . < 0x3498 ? 0x3498 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge6read32Ej)
    . = . < 0x34b0 ? 0x34b0 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge7write32Ejj)
    . = . < 0x34c8 ? 0x34c8 : .;
    */lv0/src/console/cp_bridge.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x34e0 ? 0x34e0 : .;
    */lv0/src/console/cp_bridge.o(.opd._GLOBAL__I_g_cp_bridge)
    . = . < 0x34f8 ? 0x34f8 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge10set_windowEjj)
    . = . < 0x3510 ? 0x3510 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge10window_lowEv)
    . = . < 0x3528 ? 0x3528 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge11window_highEv)
    . = . < 0x3540 ? 0x3540 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge4initEv)
    . = . < 0x3558 ? 0x3558 : .;
    */lv0/src/console/cp_bridge.o(.opd._Z13get_cp_bridgev)
    . = . < 0x3570 ? 0x3570 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge9write_rawEjj)
    . = . < 0x3588 ? 0x3588 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge9write_ctlEjj)
    . = . < 0x35a0 ? 0x35a0 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge8read_rawEj)
    . = . < 0x35b8 ? 0x35b8 : .;
    */lv0/src/console/cp_bridge.o(.opd._ZN9cp_bridge8read_ctlEj)
    . = . < 0x35d0 ? 0x35d0 : .;
    */lv0/src/console/internal_console.o(.opd._ZN16internal_console5writeEPKcl)
    . = . < 0x35e8 ? 0x35e8 : .;
    */lv0/src/console/internal_console.o(.opd._ZN16internal_console10initializeEv)
    . = . < 0x3600 ? 0x3600 : .;
    */lv0/src/console/internal_console.o(.opd._Z23log_to_internal_consolePKc)
    . = . < 0x3618 ? 0x3618 : .;
    */lv0/src/console/internal_console.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3630 ? 0x3630 : .;
    */lv0/src/console/internal_console.o(.opd._GLOBAL__I__ZN16internal_console10initializeEv)
    . = . < 0x3648 ? 0x3648 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channel4pollEv)
    . = . < 0x3660 ? 0x3660 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channel7receiveEv)
    . = . < 0x3678 ? 0x3678 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channel10on_receiveEv)
    . = . < 0x3690 ? 0x3690 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channel5flushEv)
    . = . < 0x36a8 ? 0x36a8 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channel4initEPcm)
    . = . < 0x36c0 ? 0x36c0 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channelD1Ev)
    . = . < 0x36d8 ? 0x36d8 : .;
    */lv0/src/console/log_cons_channel.o(.opd._ZN16log_cons_channelC1Ev)
    . = . < 0x36f0 ? 0x36f0 : .;
    */lv0/src/io/gbe_work.o(.opd._ZN8gbe_workC1Ev)
    . = . < 0x3708 ? 0x3708 : .;
    */lv0/src/io/gbe_work.o(.opd._Z12get_gbe_workv)
    . = . < 0x3720 ? 0x3720 : .;
    */lv0/src/io/gbe_work.o(.opd._ZN8gbe_work8allocateEmm)
    . = . < 0x3738 ? 0x3738 : .;
    */lv0/src/io/gbe_work.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3750 ? 0x3750 : .;
    */lv0/src/io/gbe_work.o(.opd._GLOBAL__I__ZN8gbe_workC2Ev)
    . = . < 0x3768 ? 0x3768 : .;
    */lv0/src/io/gbe_work.o(.opd._ZN8gbe_work8finalizeEv)
    . = . < 0x3780 ? 0x3780 : .;
    */lv0/src/io/gbe_work.o(.opd._ZN8gbe_work10initializeEm)
    . = . < 0x3798 ? 0x3798 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN15pci_mini_driverC1Ev)
    . = . < 0x37b0 ? 0x37b0 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._Z19get_pci_mini_driverv)
    . = . < 0x37c8 ? 0x37c8 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x37e0 ? 0x37e0 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._GLOBAL__I_pci_mini_driver_sc_version_subcmd)
    . = . < 0x37f8 ? 0x37f8 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN15pci_mini_driver8delay_msEm)
    . = . < 0x3810 ? 0x3810 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN15pci_mini_driver8scan_busEi)
    . = . < 0x3828 ? 0x3828 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN15pci_mini_driver15init_controllerEv)
    . = . < 0x3840 ? 0x3840 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN15pci_mini_driver10initializeEv)
    . = . < 0x3858 ? 0x3858 : .;
    */lv0/src/io/pci_mini_driver.o(.opd._ZN10bus_driver12get_devparamEttiPP12pci_devparam)
    . = . < 0x3870 ? 0x3870 : .;
    */lv0/src/io/pci_bus_power.o(.opd._Z16set_power_statusI7pci_busEih)
    . = . < 0x3888 ? 0x3888 : .;
    */lv0/src/io/pci_bus_power.o(.opd._Z16get_power_statusI7pci_busEiPj)
    . = . < 0x38a0 ? 0x38a0 : .;
    */lv0/src/io/pci_bus_power.o(.opd._ZN15pci_mini_driver13set_bus_powerEl)
    . = . < 0x38b8 ? 0x38b8 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channel7receiveEPvlPt)
    . = . < 0x38d0 ? 0x38d0 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._Z18get_cp_ch4_channelv)
    . = . < 0x38e8 ? 0x38e8 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channel10disconnectEv)
    . = . < 0x3900 ? 0x3900 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channel7connectEv)
    . = . < 0x3918 ? 0x3918 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channelD0Ev)
    . = . < 0x3930 ? 0x3930 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channelD1Ev)
    . = . < 0x3948 ? 0x3948 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channelC1Ev)
    . = . < 0x3960 ? 0x3960 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3978 ? 0x3978 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._GLOBAL__I__ZN14cp_ch4_channelC2Ev)
    . = . < 0x3990 ? 0x3990 : .;
    */lv0/src/console/cp_ch4_channel.o(.opd._ZN14cp_ch4_channel4sendEPvtPt)
    . = . < 0x39a8 ? 0x39a8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10set_deviceEP12pci_devparamP9cp_bridgej)
    . = . < 0x39c0 ? 0x39c0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel19find_lowest_set_bitEj)
    . = . < 0x39d8 ? 0x39d8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel6vslot7Ev)
    . = . < 0x39f0 ? 0x39f0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel7connectEv)
    . = . < 0x3a08 ? 0x3a08 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel6vslot6Ev)
    . = . < 0x3a20 ? 0x3a20 : .;
    */lv0/src/console/cp_channel.o(.opd._Z14get_cp_channelv)
    . = . < 0x3a38 ? 0x3a38 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel9read_bar0Ej)
    . = . < 0x3a50 ? 0x3a50 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10write_bar0Ejj)
    . = . < 0x3a68 ? 0x3a68 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel8write_chEjj)
    . = . < 0x3a80 ? 0x3a80 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel9read_bar1Ej)
    . = . < 0x3a98 ? 0x3a98 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel11read_configEPj)
    . = . < 0x3ab0 ? 0x3ab0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10write_bar1Ejj)
    . = . < 0x3ac8 ? 0x3ac8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10cfg_read32Ej)
    . = . < 0x3ae0 ? 0x3ae0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel11cfg_write32Ejj)
    . = . < 0x3af8 ? 0x3af8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel5resetEv)
    . = . < 0x3b10 ? 0x3b10 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10disconnectEv)
    . = . < 0x3b28 ? 0x3b28 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel10initializeEP12pci_devparamP9cp_bridgej)
    . = . < 0x3b40 ? 0x3b40 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channelD1Ev)
    . = . < 0x3b58 ? 0x3b58 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channelC1Ev)
    . = . < 0x3b70 ? 0x3b70 : .;
    */lv0/src/console/cp_channel.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3b88 ? 0x3b88 : .;
    */lv0/src/console/cp_channel.o(.opd._GLOBAL__I__ZN10cp_channel19find_lowest_set_bitEj)
    . = . < 0x3ba0 ? 0x3ba0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channelC2Ev)
    . = . < 0x3bb8 ? 0x3bb8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel5setupEv)
    . = . < 0x3bd0 ? 0x3bd0 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel7receiveEPvlPt)
    . = . < 0x3be8 ? 0x3be8 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channel4sendEPvtPt)
    . = . < 0x3c00 ? 0x3c00 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channelD2Ev)
    . = . < 0x3c18 ? 0x3c18 : .;
    */lv0/src/console/cp_channel.o(.opd._ZN10cp_channelD0Ev)
    . = . < 0x3c30 ? 0x3c30 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_consoleC1Ev)
    . = . < 0x3c48 ? 0x3c48 : .;
    */lv0/src/console/physical_console.o(.opd._Z20get_physical_consolev)
    . = . < 0x3c60 ? 0x3c60 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console5bswapItEET_S1_)
    . = . < 0x3c78 ? 0x3c78 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console5bswapIlEET_S1_)
    . = . < 0x3c90 ? 0x3c90 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console13decode_headerEPtS0_S0_Pl)
    . = . < 0x3ca8 ? 0x3ca8 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console13encode_headerEPhttl)
    . = . < 0x3cc0 ? 0x3cc0 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console7deliverEP11ring_buffertl)
    . = . < 0x3cd8 ? 0x3cd8 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console7receiveEv)
    . = . < 0x3cf0 ? 0x3cf0 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console4pollEv)
    . = . < 0x3d08 ? 0x3d08 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console4sendEP11ring_buffertl)
    . = . < 0x3d20 ? 0x3d20 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console10initializeEv)
    . = . < 0x3d38 ? 0x3d38 : .;
    */lv0/src/console/physical_console.o(.opd._ZN16physical_console8finalizeEv)
    . = . < 0x3d50 ? 0x3d50 : .;
    */lv0/src/console/physical_console.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3d68 ? 0x3d68 : .;
    */lv0/src/console/physical_console.o(.opd._GLOBAL__I__ZN16physical_consoleC2Ev)
    . = . < 0x3d80 ? 0x3d80 : .;
    */lv0/src/console/file_transfer.o(.opd._Z15get_ft_channelsv)
    . = . < 0x3d98 ? 0x3d98 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer15status_to_errorEm)
    . = . < 0x3db0 ? 0x3db0 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer5bswapIlEET_S1_)
    . = . < 0x3dc8 ? 0x3dc8 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer14decode_messageEPlS0_S0_)
    . = . < 0x3de0 ? 0x3de0 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer14encode_messageEPvll)
    . = . < 0x3df8 ? 0x3df8 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer12read_channelEP16log_cons_channelPvl)
    . = . < 0x3e10 ? 0x3e10 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer13write_channelEP16log_cons_channelPvl)
    . = . < 0x3e28 ? 0x3e28 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer14send_file_nameEPKc)
    . = . < 0x3e40 ? 0x3e40 : .;
    */lv0/src/console/file_transfer.o(.opd._Z26init_log_cons_ctl_and_datav)
    . = . < 0x3e58 ? 0x3e58 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer15file_command_15EPKcm)
    . = . < 0x3e70 ? 0x3e70 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer14authorize_fileEPKvmPv)
    . = . < 0x3e88 ? 0x3e88 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer13get_file_nameEPvmm)
    . = . < 0x3ea0 ? 0x3ea0 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer13get_file_sizeEPKcPm)
    . = . < 0x3eb8 ? 0x3eb8 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer8put_fileEPKcmPKvmPm)
    . = . < 0x3ed0 ? 0x3ed0 : .;
    */lv0/src/console/file_transfer.o(.opd._ZN13file_transfer8get_fileEPKcmPvmPm)
    . = . < 0x3ee8 ? 0x3ee8 : .;
    */lv0/src/console/file_transfer.o(.opd._Z41__static_initialization_and_destruction_0ii)
    . = . < 0x3f00 ? 0x3f00 : .;
    */lv0/src/console/file_transfer.o(.opd._GLOBAL__I__Z26init_log_cons_ctl_and_datav)
    . = . < 0x3f18 ? 0x3f18 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_bufferC1Ev)
    . = . < 0x3f30 ? 0x3f30 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_buffer6attachElP18ring_buffer_header)
    . = . < 0x3f48 ? 0x3f48 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_bufferD1Ev)
    . = . < 0x3f60 ? 0x3f60 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_buffer3popEPh)
    . = . < 0x3f78 ? 0x3f78 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_buffer5writeEPKhmPm)
    . = . < 0x3f90 ? 0x3f90 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_buffer4pushEh)
    . = . < 0x3fa8 ? 0x3fa8 : .;
    */lv0/src/console/ring_buffer.o(.opd._ZN11ring_buffer4readEPhmPm)
    . = . < 0x3fc0 ? 0x3fc0 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessorC1EPc)
    . = . < 0x3fd8 ? 0x3fd8 : .;
    */lv0/src/util/mmio_accessor.o(.opd._Z17get_mmio_accessorv)
    . = . < 0x3ff0 ? 0x3ff0 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor16is_trace_enabledEv)
    . = . < 0x4008 ? 0x4008 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor6read32El)
    . = . < 0x4020 ? 0x4020 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor6read64El)
    . = . < 0x4038 ? 0x4038 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor7write32Elj)
    . = . < 0x4050 ? 0x4050 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor7write64Ell)
    . = . < 0x4068 ? 0x4068 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor8on_writeElll)
    . = . < 0x4080 ? 0x4080 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessor7on_readElll)
    . = . < 0x4098 ? 0x4098 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessorD0Ev)
    . = . < 0x40b0 ? 0x40b0 : .;
    */lv0/src/util/mmio_accessor.o(.opd._ZN13mmio_accessorD1Ev)
    . = . < 0x40c8 ? 0x40c8 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_link6read32El)
    . = . < 0x40e0 ? 0x40e0 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_link7write32Ell)
    . = . < 0x40f8 ? 0x40f8 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_linkD0Ev)
    . = . < 0x4110 ? 0x4110 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_linkD1Ev)
    . = . < 0x4128 ? 0x4128 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_link8delay_msEm)
    . = . < 0x4140 ? 0x4140 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_link8delay_usEm)
    . = . < 0x4158 ? 0x4158 : .;
    */lv0/src/console/cp_link.o(.opd._ZN7cp_linkC2Ev)
    . = . < 0x4170 ? 0x4170 : .;
    */lv0/src/io/pci_config.o(.opd._Z23pci_make_config_addressP12pci_devparaml)
    . = . < 0x4188 ? 0x4188 : .;
    */lv0/src/io/pci_config.o(.opd._Z18pci_config_write32P12pci_devparamlj)
    . = . < 0x41a0 ? 0x41a0 : .;
    */lv0/src/io/pci_config.o(.opd._Z17pci_config_read32P12pci_devparaml)
    . = . < 0x41b8 ? 0x41b8 : .;
    */lv0/src/io/pci_config.o(.opd._Z17pci_config_read16P12pci_devparaml)
    . = . < 0x41d0 ? 0x41d0 : .;
    */lv0/src/util/operator_delete.o(.opd._ZdlPv)
    . = . < 0x41e8 ? 0x41e8 : .;
    */lv0/src/util/spin_lock.o(.opd._ZN15spin_lock_guard7acquireEv)
    . = . < 0x4200 ? 0x4200 : .;
    */lv0/src/util/spin_lock.o(.opd._ZN15spin_lock_guardC1Ev)
    . = . < 0x4218 ? 0x4218 : .;
    */lv0/src/util/spin_lock_release.o(.opd._ZN15spin_lock_guard7releaseEv)
    . = . < 0x4230 ? 0x4230 : .;
    */lv0/src/util/spin_lock.o(.opd._ZN15spin_lock_guardD1Ev)
    . = . < 0x4248 ? 0x4248 : .;
    */lv0/src/util/aes.o(.opd.aes_encrypt_block)
    . = . < 0x4260 ? 0x4260 : .;
    */lv0/src/util/aes.o(.opd.aes_ctr_crypt)
  }
  . = . < 0x80c5d58 ? 0x80c5d58 : .;
  .ctors :
  {
    __ctors_start = .;
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/spu/spu_manager.o(.ctors)
    . = . < 0x8 ? 0x8 : .;
    */lv0/src/storage/storage.o(.ctors)
    . = . < 0x10 ? 0x10 : .;
    */lv0/src/boot/boot_loader.o(.ctors)
    . = . < 0x18 ? 0x18 : .;
    */lv0/src/platform/memory_budgets.o(.ctors)
    . = . < 0x20 ? 0x20 : .;
    */lv0/src/platform/platform.o(.ctors)
    . = . < 0x28 ? 0x28 : .;
    */lv0/src/platform/platform_pwrmgr.o(.ctors)
    . = . < 0x30 ? 0x30 : .;
    */lv0/src/component/component_loaders.o(.ctors)
    . = . < 0x38 ? 0x38 : .;
    */lv0/src/flash/flash_storage.o(.ctors)
    . = . < 0x40 ? 0x40 : .;
    */lv0/src/console/cp_bridge.o(.ctors)
    . = . < 0x48 ? 0x48 : .;
    */lv0/src/console/internal_console.o(.ctors)
    . = . < 0x50 ? 0x50 : .;
    */lv0/src/io/gbe_work.o(.ctors)
    . = . < 0x58 ? 0x58 : .;
    */lv0/src/io/pci_mini_driver.o(.ctors)
    . = . < 0x60 ? 0x60 : .;
    */lv0/src/console/cp_ch4_channel.o(.ctors)
    . = . < 0x68 ? 0x68 : .;
    */lv0/src/console/cp_channel.o(.ctors)
    . = . < 0x70 ? 0x70 : .;
    */lv0/src/console/physical_console.o(.ctors)
    . = . < 0x78 ? 0x78 : .;
    */lv0/src/console/file_transfer.o(.ctors)
    __ctors_end = .;
  }
  . = . < 0x80c5de0 ? 0x80c5de0 : .;
  .toc :
  {
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/boot/c_entry.o(.toc.LC0)
    . = . < 0x8 ? 0x8 : .;
    */lv0/src/boot/run_global_constructors.o(.toc.LC1)
    . = . < 0x10 ? 0x10 : .;
    */lv0/src/boot/run_global_constructors.o(.toc.LC2)
    . = . < 0x18 ? 0x18 : .;
    */lv0/src/component/boot_order_component_loader.o(.toc.LC0)
    . = . < 0x20 ? 0x20 : .;
    */lv0/src/component/boot_order_component_loader.o(.toc.LC1)
    . = . < 0x28 ? 0x28 : .;
    */lv0/src/storage/eeprom_nv1.o(.toc.LC0)
    . = . < 0x30 ? 0x30 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC5)
    . = . < 0x38 ? 0x38 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC0)
    . = . < 0x40 ? 0x40 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC1)
    . = . < 0x48 ? 0x48 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC2)
    . = . < 0x50 ? 0x50 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC6)
    . = . < 0x58 ? 0x58 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC3)
    . = . < 0x60 ? 0x60 : .;
    */lv0/src/spu/spu_manager.o(.toc.LC10)
    . = . < 0x68 ? 0x68 : .;
    */lv0/src/component/component_loader.o(.toc.LC0)
    . = . < 0x70 ? 0x70 : .;
    */lv0/src/component/componnet_manager.o(.toc.LC0)
    . = . < 0x78 ? 0x78 : .;
    */lv0/src/component/componnet_manager.o(.toc.LC1)
    . = . < 0x80 ? 0x80 : .;
    */lv0/src/component/componnet_manager.o(.toc.LC2)
    . = . < 0x88 ? 0x88 : .;
    */lv0/src/component/component.o(.toc.LC0)
    . = . < 0x90 ? 0x90 : .;
    */lv0/src/component/loaded_component.o(.toc.LC0)
    . = . < 0x98 ? 0x98 : .;
    */lv0/src/component/component__base_dtor.o(.toc.LC0)
    . = . < 0xa0 ? 0xa0 : .;
    */lv0/src/io/get_gbe_macaddr.o(.toc.LC0)
    . = . < 0xa8 ? 0xa8 : .;
    */lv0/src/platform/platform_config.o(.toc.LC0)
    . = . < 0xb0 ? 0xb0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC1)
    . = . < 0xb8 ? 0xb8 : .;
    */lv0/src/platform/platform_config.o(.toc.LC2)
    . = . < 0xc0 ? 0xc0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC3)
    . = . < 0xc8 ? 0xc8 : .;
    */lv0/src/platform/platform_config.o(.toc.LC4)
    . = . < 0xd0 ? 0xd0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC5)
    . = . < 0xd8 ? 0xd8 : .;
    */lv0/src/storage/set_release_mode.o(.toc.LC0)
    . = . < 0xe0 ? 0xe0 : .;
    */lv0/src/syscon/get_boot_gos.o(.toc.LC0)
    . = . < 0xe8 ? 0xe8 : .;
    */lv0/src/syscon/get_boot_gos.o(.toc.LC1)
    . = . < 0xf0 ? 0xf0 : .;
    */lv0/src/syscon/get_boot_gos.o(.toc.LC3)
    . = . < 0xf8 ? 0xf8 : .;
    */lv0/src/syscon/get_boot_gos.o(.toc.LC5)
    . = . < 0x100 ? 0x100 : .;
    */lv0/src/storage/eeprom_settings.o(.toc.LC0)
    . = . < 0x108 ? 0x108 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_gateway.o(.toc.LC1)
    . = . < 0x110 ? 0x110 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_netmask.o(.toc.LC1)
    . = . < 0x118 ? 0x118 : .;
    */lv0/src/storage/get_eeprom_dgbe_ip_address.o(.toc.LC1)
    . = . < 0x120 ? 0x120 : .;
    */lv0/src/storage/get_eeprom_sata_param.o(.toc.LC1)
    . = . < 0x128 ? 0x128 : .;
    */lv0/src/storage/get_eeprom_core_os_bank_indicator.o(.toc.LC1)
    . = . < 0x130 ? 0x130 : .;
    */lv0/src/storage/get_eeprom_cellos_spu_configure.o(.toc.LC1)
    . = . < 0x138 ? 0x138 : .;
    */lv0/src/storage/get_eeprom_flash_ext_format.o(.toc.LC1)
    . = . < 0x140 ? 0x140 : .;
    */lv0/src/storage/get_eeprom_cellos_flags.o(.toc.LC1)
    . = . < 0x148 ? 0x148 : .;
    */lv0/src/storage/get_eeprom_select_dgbe_device.o(.toc.LC1)
    . = . < 0x150 ? 0x150 : .;
    */lv0/src/storage/get_eeprom_os_boot_order_flag.o(.toc.LC1)
    . = . < 0x158 ? 0x158 : .;
    */lv0/src/platform/platform_config.o(.toc.LC23)
    . = . < 0x160 ? 0x160 : .;
    */lv0/src/platform/platform_config.o(.toc.LC25)
    . = . < 0x168 ? 0x168 : .;
    */lv0/src/storage/set_release_mode.o(.toc.LC2)
    . = . < 0x170 ? 0x170 : .;
    */lv0/src/platform/get_hw_config.o(.toc.LC0)
    . = . < 0x178 ? 0x178 : .;
    */lv0/src/platform/get_hw_config.o(.toc.LC2)
    . = . < 0x180 ? 0x180 : .;
    */lv0/src/platform/get_hw_config.o(.toc.LC3)
    . = . < 0x188 ? 0x188 : .;
    */lv0/src/platform/get_hw_config.o(.toc.LC4)
    . = . < 0x190 ? 0x190 : .;
    */lv0/src/platform/get_hw_config.o(.toc.LC6)
    . = . < 0x198 ? 0x198 : .;
    */lv0/src/platform/platform_config.o(.toc.LC6)
    . = . < 0x1a0 ? 0x1a0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC14)
    . = . < 0x1a8 ? 0x1a8 : .;
    */lv0/src/platform/platform_config.o(.toc.LC18)
    . = . < 0x1b0 ? 0x1b0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC19)
    . = . < 0x1b8 ? 0x1b8 : .;
    */lv0/src/platform/platform_config.o(.toc.LC7)
    . = . < 0x1c0 ? 0x1c0 : .;
    */lv0/src/platform/platform_config.o(.toc.LC20)
    . = . < 0x1c8 ? 0x1c8 : .;
    */lv0/src/platform/get_debug_interface_parm.o(.toc.LC0)
    . = . < 0x1d0 ? 0x1d0 : .;
    */lv0/src/component/file_hooks.o(.toc.LC0)
    . = . < 0x1d8 ? 0x1d8 : .;
    */lv0/src/component/file_hooks.o(.toc.LC1)
    . = . < 0x1e0 ? 0x1e0 : .;
    */lv0/src/platform/debug_interface.o(.toc.LC0)
    . = . < 0x1e8 ? 0x1e8 : .;
    */lv0/src/storage/read_eeprom_select_net_device.o(.toc.LC0)
    . = . < 0x1f0 ? 0x1f0 : .;
    */lv0/src/storage/get_eeprom_select_net_device.o(.toc.LC1)
    . = . < 0x1f8 ? 0x1f8 : .;
    */lv0/src/boot/application.o(.toc.LC0)
    . = . < 0x200 ? 0x200 : .;
    */lv0/src/boot/application.o(.toc.LC7)
    . = . < 0x208 ? 0x208 : .;
    */lv0/src/boot/application.o(.toc.LC3)
    . = . < 0x210 ? 0x210 : .;
    */lv0/src/boot/application.o(.toc.LC4)
    . = . < 0x218 ? 0x218 : .;
    */lv0/src/boot/application.o(.toc.LC5)
    . = . < 0x220 ? 0x220 : .;
    */lv0/src/boot/application.o(.toc.LC6)
    . = . < 0x228 ? 0x228 : .;
    */lv0/src/boot/application.o(.toc.LC10)
    . = . < 0x230 ? 0x230 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC1)
    . = . < 0x238 ? 0x238 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC2)
    . = . < 0x240 ? 0x240 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC4)
    . = . < 0x248 ? 0x248 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC5)
    . = . < 0x250 ? 0x250 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC8)
    . = . < 0x258 ? 0x258 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC10)
    . = . < 0x260 ? 0x260 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC27)
    . = . < 0x268 ? 0x268 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC29)
    . = . < 0x270 ? 0x270 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC23)
    . = . < 0x278 ? 0x278 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC16)
    . = . < 0x280 ? 0x280 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC19)
    . = . < 0x288 ? 0x288 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC30)
    . = . < 0x290 ? 0x290 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC32)
    . = . < 0x298 ? 0x298 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC34)
    . = . < 0x2a0 ? 0x2a0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC35)
    . = . < 0x2a8 ? 0x2a8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC5_s85)
    . = . < 0x2b0 ? 0x2b0 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC16_s86)
    . = . < 0x2b8 ? 0x2b8 : .;
    */lv0/src/component/flash_format_1_component_loader.o(.toc.LC38)
    . = . < 0x2c0 ? 0x2c0 : .;
    */lv0/src/io/init_sb_device.o(.toc.LC0)
    . = . < 0x2c8 ? 0x2c8 : .;
    */lv0/src/io/get_sb_device.o(.toc.LC0)
    . = . < 0x2d0 ? 0x2d0 : .;
    */lv0/src/io/get_sb_device.o(.toc.LC1)
    . = . < 0x2d8 ? 0x2d8 : .;
    */lv0/src/storage/storage.o(.toc.LC0)
    . = . < 0x2e0 ? 0x2e0 : .;
    */lv0/src/storage/storage_manager.o(.toc.LC0)
    . = . < 0x2e8 ? 0x2e8 : .;
    */lv0/src/io/get_iommu_context.o(.toc.LC0)
    . = . < 0x2f0 ? 0x2f0 : .;
    */lv0/src/io/get_iommu_context.o(.toc.LC1)
    . = . < 0x2f8 ? 0x2f8 : .;
    */lv0/src/io/iommu.o(.toc.LC14)
    . = . < 0x300 ? 0x300 : .;
    */lv0/src/io/iommu.o(.toc.LC16)
    . = . < 0x308 ? 0x308 : .;
    */lv0/src/io/iommu.o(.toc.LC18)
    . = . < 0x310 ? 0x310 : .;
    */lv0/src/io/iommu.o(.toc.LC1)
    . = . < 0x318 ? 0x318 : .;
    */lv0/src/io/iommu.o(.toc.LC2)
    . = . < 0x320 ? 0x320 : .;
    */lv0/src/io/iommu.o(.toc.LC4)
    . = . < 0x328 ? 0x328 : .;
    */lv0/src/io/iommu.o(.toc.LC6)
    . = . < 0x330 ? 0x330 : .;
    */lv0/src/io/iommu.o(.toc.LC8)
    . = . < 0x338 ? 0x338 : .;
    */lv0/src/io/iommu.o(.toc.LC10)
    . = . < 0x340 ? 0x340 : .;
    */lv0/src/io/iommu.o(.toc.LC12)
    . = . < 0x348 ? 0x348 : .;
    */lv0/src/spu/spuctl.o(.toc.LC0)
    . = . < 0x350 ? 0x350 : .;
    */lv0/src/spu/spuctl.o(.toc.LC3)
    . = . < 0x358 ? 0x358 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC1)
    . = . < 0x360 ? 0x360 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC2)
    . = . < 0x368 ? 0x368 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC4)
    . = . < 0x370 ? 0x370 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC6)
    . = . < 0x378 ? 0x378 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC8)
    . = . < 0x380 ? 0x380 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC9)
    . = . < 0x388 ? 0x388 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC11)
    . = . < 0x390 ? 0x390 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC12)
    . = . < 0x398 ? 0x398 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC14)
    . = . < 0x3a0 ? 0x3a0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC15)
    . = . < 0x3a8 ? 0x3a8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC17)
    . = . < 0x3b0 ? 0x3b0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC19)
    . = . < 0x3b8 ? 0x3b8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC21)
    . = . < 0x3c0 ? 0x3c0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC23)
    . = . < 0x3c8 ? 0x3c8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC25)
    . = . < 0x3d0 ? 0x3d0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC27)
    . = . < 0x3d8 ? 0x3d8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC29)
    . = . < 0x3e0 ? 0x3e0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC30)
    . = . < 0x3e8 ? 0x3e8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC32)
    . = . < 0x3f0 ? 0x3f0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC33)
    . = . < 0x3f8 ? 0x3f8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC34)
    . = . < 0x400 ? 0x400 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC36)
    . = . < 0x408 ? 0x408 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC38)
    . = . < 0x410 ? 0x410 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC40)
    . = . < 0x418 ? 0x418 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC42)
    . = . < 0x420 ? 0x420 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC44)
    . = . < 0x428 ? 0x428 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC46)
    . = . < 0x430 ? 0x430 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC48)
    . = . < 0x438 ? 0x438 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC50)
    . = . < 0x440 ? 0x440 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC52)
    . = . < 0x448 ? 0x448 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC54)
    . = . < 0x450 ? 0x450 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC56)
    . = . < 0x458 ? 0x458 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC58)
    . = . < 0x460 ? 0x460 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC60)
    . = . < 0x468 ? 0x468 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC62)
    . = . < 0x470 ? 0x470 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC64)
    . = . < 0x478 ? 0x478 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC66)
    . = . < 0x480 ? 0x480 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC68)
    . = . < 0x488 ? 0x488 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC69)
    . = . < 0x490 ? 0x490 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC71)
    . = . < 0x498 ? 0x498 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC73)
    . = . < 0x4a0 ? 0x4a0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC75)
    . = . < 0x4a8 ? 0x4a8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC77)
    . = . < 0x4b0 ? 0x4b0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC79)
    . = . < 0x4b8 ? 0x4b8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC80)
    . = . < 0x4c0 ? 0x4c0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC82)
    . = . < 0x4c8 ? 0x4c8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC83)
    . = . < 0x4d0 ? 0x4d0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC85)
    . = . < 0x4d8 ? 0x4d8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC87)
    . = . < 0x4e0 ? 0x4e0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC89)
    . = . < 0x4e8 ? 0x4e8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC91)
    . = . < 0x4f0 ? 0x4f0 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC92)
    . = . < 0x4f8 ? 0x4f8 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC94)
    . = . < 0x500 ? 0x500 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC96)
    . = . < 0x508 ? 0x508 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC98)
    . = . < 0x510 ? 0x510 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC99)
    . = . < 0x518 ? 0x518 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC101)
    . = . < 0x520 ? 0x520 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC103)
    . = . < 0x528 ? 0x528 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC104)
    . = . < 0x530 ? 0x530 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC106)
    . = . < 0x538 ? 0x538 : .;
    */lv0/src/boot/alt_sys_parm.o(.toc.LC107)
    . = . < 0x540 ? 0x540 : .;
    */lv0/src/boot/boot_parm.o(.toc.LC0)
    . = . < 0x548 ? 0x548 : .;
    */lv0/src/boot/boot_parm.o(.toc.LC1)
    . = . < 0x550 ? 0x550 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC1)
    . = . < 0x558 ? 0x558 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC3)
    . = . < 0x560 ? 0x560 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC5)
    . = . < 0x568 ? 0x568 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC7)
    . = . < 0x570 ? 0x570 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC9)
    . = . < 0x578 ? 0x578 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC11)
    . = . < 0x580 ? 0x580 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC13)
    . = . < 0x588 ? 0x588 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC14_s177)
    . = . < 0x590 ? 0x590 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC15)
    . = . < 0x598 ? 0x598 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC16)
    . = . < 0x5a0 ? 0x5a0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC18)
    . = . < 0x5a8 ? 0x5a8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC20)
    . = . < 0x5b0 ? 0x5b0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC22)
    . = . < 0x5b8 ? 0x5b8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC50)
    . = . < 0x5c0 ? 0x5c0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC53)
    . = . < 0x5c8 ? 0x5c8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC55)
    . = . < 0x5d0 ? 0x5d0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC57)
    . = . < 0x5d8 ? 0x5d8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC59)
    . = . < 0x5e0 ? 0x5e0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC60)
    . = . < 0x5e8 ? 0x5e8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC44)
    . = . < 0x5f0 ? 0x5f0 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC45)
    . = . < 0x5f8 ? 0x5f8 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC46)
    . = . < 0x600 ? 0x600 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC23)
    . = . < 0x608 ? 0x608 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC16_s193)
    . = . < 0x610 ? 0x610 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC26)
    . = . < 0x618 ? 0x618 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC28)
    . = . < 0x620 ? 0x620 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC30)
    . = . < 0x628 ? 0x628 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC32)
    . = . < 0x630 ? 0x630 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC34)
    . = . < 0x638 ? 0x638 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC14)
    . = . < 0x640 ? 0x640 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC37)
    . = . < 0x648 ? 0x648 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC39)
    . = . < 0x650 ? 0x650 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC41)
    . = . < 0x658 ? 0x658 : .;
    */lv0/src/boot/boot_loader.o(.toc.LC43)
    . = . < 0x660 ? 0x660 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC2)
    . = . < 0x668 ? 0x668 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC4)
    . = . < 0x670 ? 0x670 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC6)
    . = . < 0x678 ? 0x678 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC8)
    . = . < 0x680 ? 0x680 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC10)
    . = . < 0x688 ? 0x688 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC12)
    . = . < 0x690 ? 0x690 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC14)
    . = . < 0x698 ? 0x698 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC16)
    . = . < 0x6a0 ? 0x6a0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC17)
    . = . < 0x6a8 ? 0x6a8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC19)
    . = . < 0x6b0 ? 0x6b0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC21)
    . = . < 0x6b8 ? 0x6b8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC24)
    . = . < 0x6c0 ? 0x6c0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC25)
    . = . < 0x6c8 ? 0x6c8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC26)
    . = . < 0x6d0 ? 0x6d0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC27)
    . = . < 0x6d8 ? 0x6d8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC28)
    . = . < 0x6e0 ? 0x6e0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC29)
    . = . < 0x6e8 ? 0x6e8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC30)
    . = . < 0x6f0 ? 0x6f0 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC32)
    . = . < 0x6f8 ? 0x6f8 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC31)
    . = . < 0x700 ? 0x700 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC33)
    . = . < 0x708 ? 0x708 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC34)
    . = . < 0x710 ? 0x710 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC35)
    . = . < 0x718 ? 0x718 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC36)
    . = . < 0x720 ? 0x720 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC37)
    . = . < 0x728 ? 0x728 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC0)
    . = . < 0x730 ? 0x730 : .;
    */lv0/src/boot/boot_cell_os.o(.toc.LC1)
    . = . < 0x738 ? 0x738 : .;
    */lv0/src/boot/component_info.o(.toc.LC0)
    . = . < 0x740 ? 0x740 : .;
    */lv0/src/boot/component_info.o(.toc.LC1)
    . = . < 0x748 ? 0x748 : .;
    */lv0/src/platform/memory_budgets.o(.toc.LC0)
    . = . < 0x750 ? 0x750 : .;
    */lv0/src/platform/memory_budgets.o(.toc.LC3)
    . = . < 0x758 ? 0x758 : .;
    */lv0/src/platform/memory_budgets.o(.toc.LC6)
    . = . < 0x760 ? 0x760 : .;
    */lv0/src/platform/memory_budgets.o(.toc.LC7)
    . = . < 0x768 ? 0x768 : .;
    */lv0/src/log/log_ring.o(.toc.LC0)
    . = . < 0x770 ? 0x770 : .;
    */lv0/src/log/log_ring.o(.toc.LC1)
    . = . < 0x778 ? 0x778 : .;
    */lv0/src/console/init_debug_interface.o(.toc.LC0)
    . = . < 0x780 ? 0x780 : .;
    */lv0/src/console/init_debug_interface.o(.toc.LC4)
    . = . < 0x788 ? 0x788 : .;
    */lv0/src/console/init_debug_interface.o(.toc.LC5)
    . = . < 0x790 ? 0x790 : .;
    */lv0/src/console/init_debug_interface.o(.toc.LC7)
    . = . < 0x798 ? 0x798 : .;
    */lv0/src/console/init_debug_interface.o(.toc.LC9)
    . = . < 0x7a0 ? 0x7a0 : .;
    */lv0/src/platform/pio.o(.toc.LC0)
    . = . < 0x7a8 ? 0x7a8 : .;
    */lv0/src/platform/pio.o(.toc.LC1)
    . = . < 0x7b0 ? 0x7b0 : .;
    */lv0/src/platform/pio.o(.toc.LC5)
    . = . < 0x7b8 ? 0x7b8 : .;
    */lv0/src/platform/pio.o(.toc.LC6)
    . = . < 0x7c0 ? 0x7c0 : .;
    */lv0/src/platform/pio.o(.toc.LC10)
    . = . < 0x7c8 ? 0x7c8 : .;
    */lv0/src/platform/platform.o(.toc.LC1)
    . = . < 0x7d0 ? 0x7d0 : .;
    */lv0/src/platform/platform.o(.toc.LC2)
    . = . < 0x7d8 ? 0x7d8 : .;
    */lv0/src/platform/platform.o(.toc.LC25)
    . = . < 0x7e0 ? 0x7e0 : .;
    */lv0/src/platform/platform.o(.toc.LC41)
    . = . < 0x7e8 ? 0x7e8 : .;
    */lv0/src/platform/platform.o(.toc.LC43)
    . = . < 0x7f0 ? 0x7f0 : .;
    */lv0/src/platform/platform.o(.toc.LC45)
    . = . < 0x7f8 ? 0x7f8 : .;
    */lv0/src/platform/platform.o(.toc.LC47)
    . = . < 0x800 ? 0x800 : .;
    */lv0/src/platform/platform.o(.toc.LC49)
    . = . < 0x808 ? 0x808 : .;
    */lv0/src/platform/platform.o(.toc.LC51)
    . = . < 0x810 ? 0x810 : .;
    */lv0/src/platform/platform.o(.toc.LC53)
    . = . < 0x818 ? 0x818 : .;
    */lv0/src/platform/platform.o(.toc.LC55)
    . = . < 0x820 ? 0x820 : .;
    */lv0/src/platform/platform.o(.toc.LC57)
    . = . < 0x828 ? 0x828 : .;
    */lv0/src/platform/platform.o(.toc.LC59)
    . = . < 0x830 ? 0x830 : .;
    */lv0/src/platform/platform.o(.toc.LC61)
    . = . < 0x838 ? 0x838 : .;
    */lv0/src/platform/platform.o(.toc.LC63)
    . = . < 0x840 ? 0x840 : .;
    */lv0/src/platform/platform.o(.toc.LC65)
    . = . < 0x848 ? 0x848 : .;
    */lv0/src/platform/platform.o(.toc.LC67)
    . = . < 0x850 ? 0x850 : .;
    */lv0/src/platform/platform.o(.toc.LC69)
    . = . < 0x858 ? 0x858 : .;
    */lv0/src/platform/platform.o(.toc.LC71)
    . = . < 0x860 ? 0x860 : .;
    */lv0/src/platform/platform.o(.toc.LC73)
    . = . < 0x868 ? 0x868 : .;
    */lv0/src/platform/platform.o(.toc.LC75)
    . = . < 0x870 ? 0x870 : .;
    */lv0/src/platform/platform.o(.toc.LC77)
    . = . < 0x878 ? 0x878 : .;
    */lv0/src/platform/platform.o(.toc.LC79)
    . = . < 0x880 ? 0x880 : .;
    */lv0/src/platform/platform.o(.toc.LC81)
    . = . < 0x888 ? 0x888 : .;
    */lv0/src/platform/platform.o(.toc.LC83)
    . = . < 0x890 ? 0x890 : .;
    */lv0/src/platform/platform.o(.toc.LC85)
    . = . < 0x898 ? 0x898 : .;
    */lv0/src/platform/platform.o(.toc.LC87)
    . = . < 0x8a0 ? 0x8a0 : .;
    */lv0/src/platform/platform.o(.toc.LC89)
    . = . < 0x8a8 ? 0x8a8 : .;
    */lv0/src/platform/platform.o(.toc.LC91)
    . = . < 0x8b0 ? 0x8b0 : .;
    */lv0/src/platform/platform.o(.toc.LC27)
    . = . < 0x8b8 ? 0x8b8 : .;
    */lv0/src/platform/platform.o(.toc.LC29)
    . = . < 0x8c0 ? 0x8c0 : .;
    */lv0/src/platform/platform.o(.toc.LC31)
    . = . < 0x8c8 ? 0x8c8 : .;
    */lv0/src/platform/platform.o(.toc.LC33)
    . = . < 0x8d0 ? 0x8d0 : .;
    */lv0/src/platform/platform.o(.toc.LC35)
    . = . < 0x8d8 ? 0x8d8 : .;
    */lv0/src/platform/platform.o(.toc.LC37)
    . = . < 0x8e0 ? 0x8e0 : .;
    */lv0/src/platform/platform.o(.toc.LC39)
    . = . < 0x8e8 ? 0x8e8 : .;
    */lv0/src/platform/platform.o(.toc.LC93)
    . = . < 0x8f0 ? 0x8f0 : .;
    */lv0/src/platform/platform.o(.toc.LC95)
    . = . < 0x8f8 ? 0x8f8 : .;
    */lv0/src/platform/platform.o(.toc.LC97)
    . = . < 0x900 ? 0x900 : .;
    */lv0/src/platform/platform.o(.toc.LC99)
    . = . < 0x908 ? 0x908 : .;
    */lv0/src/platform/platform.o(.toc.LC101)
    . = . < 0x910 ? 0x910 : .;
    */lv0/src/platform/platform.o(.toc.LC103)
    . = . < 0x918 ? 0x918 : .;
    */lv0/src/platform/platform.o(.toc.LC105)
    . = . < 0x920 ? 0x920 : .;
    */lv0/src/platform/platform.o(.toc.LC3)
    . = . < 0x928 ? 0x928 : .;
    */lv0/src/platform/platform.o(.toc.LC13)
    . = . < 0x930 ? 0x930 : .;
    */lv0/src/platform/platform.o(.toc.LC10)
    . = . < 0x938 ? 0x938 : .;
    */lv0/src/platform/platform.o(.toc.LC15)
    . = . < 0x940 ? 0x940 : .;
    */lv0/src/platform/platform.o(.toc.LC16)
    . = . < 0x948 ? 0x948 : .;
    */lv0/src/platform/platform.o(.toc.LC17)
    . = . < 0x950 ? 0x950 : .;
    */lv0/src/platform/platform.o(.toc.LC19)
    . = . < 0x958 ? 0x958 : .;
    */lv0/src/platform/platform.o(.toc.LC21)
    . = . < 0x960 ? 0x960 : .;
    */lv0/src/platform/platform.o(.toc.LC23)
    . = . < 0x968 ? 0x968 : .;
    */lv0/src/platform/platform.o(.toc.LC6)
    . = . < 0x970 ? 0x970 : .;
    */lv0/src/platform/platform.o(.toc.LC5)
    . = . < 0x978 ? 0x978 : .;
    */lv0/src/platform/platform.o(.toc.LC8)
    . = . < 0x980 ? 0x980 : .;
    */lv0/src/platform/platform.o(.toc.LC9)
    . = . < 0x988 ? 0x988 : .;
    */lv0/src/platform/platform.o(.toc.LC11)
    . = . < 0x990 ? 0x990 : .;
    */lv0/src/platform/platform.o(.toc.LC0)
    . = . < 0x998 ? 0x998 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC0)
    . = . < 0x9a0 ? 0x9a0 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC1)
    . = . < 0x9a8 ? 0x9a8 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC5)
    . = . < 0x9b0 ? 0x9b0 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC7)
    . = . < 0x9b8 ? 0x9b8 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC9)
    . = . < 0x9c0 ? 0x9c0 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC11)
    . = . < 0x9c8 ? 0x9c8 : .;
    */lv0/src/platform/platform_pwrmgr.o(.toc.LC12)
    . = . < 0x9d0 ? 0x9d0 : .;
    */lv0/src/util/delay.o(.toc.LC0)
    . = . < 0x9d8 ? 0x9d8 : .;
    */lv0/src/component/host_file_loader.o(.toc.LC1)
    . = . < 0x9e0 ? 0x9e0 : .;
    */lv0/src/component/host_file_loader.o(.toc.LC4)
    . = . < 0x9e8 ? 0x9e8 : .;
    */lv0/src/component/host_file_loader.o(.toc.LC5)
    . = . < 0x9f0 ? 0x9f0 : .;
    */lv0/src/component/host_file_loader.o(.toc.LC6)
    . = . < 0x9f8 ? 0x9f8 : .;
    */lv0/src/flash/mapped_storage.o(.toc.LC1)
    . = . < 0xa00 ? 0xa00 : .;
    */lv0/src/syscon/syscon_get_core_clock_multiplier.o(.toc.LC0)
    . = . < 0xa08 ? 0xa08 : .;
    */lv0/src/syscon/sc_console.o(.toc.LC0)
    . = . < 0xa10 ? 0xa10 : .;
    */lv0/src/syscon/sc_console.o(.toc.LC1)
    . = . < 0xa18 ? 0xa18 : .;
    */lv0/src/syscon/syscon.o(.toc.LC0)
    . = . < 0xa20 ? 0xa20 : .;
    */lv0/src/syscon/syscon.o(.toc.LC2)
    . = . < 0xa28 ? 0xa28 : .;
    */lv0/src/syscon/syscon.o(.toc.LC3)
    . = . < 0xa30 ? 0xa30 : .;
    */lv0/src/syscon/syscon.o(.toc.LC5)
    . = . < 0xa38 ? 0xa38 : .;
    */lv0/src/syscon/syscon.o(.toc.LC7)
    . = . < 0xa40 ? 0xa40 : .;
    */lv0/src/syscon/syscon.o(.toc.LC9)
    . = . < 0xa48 ? 0xa48 : .;
    */lv0/src/syscon/syscon.o(.toc.LC11)
    . = . < 0xa50 ? 0xa50 : .;
    */lv0/src/syscon/syscon.o(.toc.LC12)
    . = . < 0xa58 ? 0xa58 : .;
    */lv0/src/syscon/syscon.o(.toc.LC14)
    . = . < 0xa60 ? 0xa60 : .;
    */lv0/src/syscon/syscon.o(.toc.LC16)
    . = . < 0xa68 ? 0xa68 : .;
    */lv0/src/syscon/syscon.o(.toc.LC17)
    . = . < 0xa70 ? 0xa70 : .;
    */lv0/src/syscon/syscon.o(.toc.LC19)
    . = . < 0xa78 ? 0xa78 : .;
    */lv0/src/syscon/syscon.o(.toc.LC24)
    . = . < 0xa80 ? 0xa80 : .;
    */lv0/src/syscon/syscon.o(.toc.LC27)
    . = . < 0xa88 ? 0xa88 : .;
    */lv0/src/syscon/syscon.o(.toc.LC31)
    . = . < 0xa90 ? 0xa90 : .;
    */lv0/src/syscon/syscon.o(.toc.LC35)
    . = . < 0xa98 ? 0xa98 : .;
    */lv0/src/syscon/syscon.o(.toc.LC37)
    . = . < 0xaa0 ? 0xaa0 : .;
    */lv0/src/syscon/syscon.o(.toc.LC38)
    . = . < 0xaa8 ? 0xaa8 : .;
    */lv0/src/syscon/syscon.o(.toc.LC40)
    . = . < 0xab0 ? 0xab0 : .;
    */lv0/src/syscon/syscon.o(.toc.LC45)
    . = . < 0xab8 ? 0xab8 : .;
    */lv0/src/syscon/syscon.o(.toc.LC47)
    . = . < 0xac0 ? 0xac0 : .;
    */lv0/src/syscon/syscon.o(.toc.LC49)
    . = . < 0xac8 ? 0xac8 : .;
    */lv0/src/syscon/get_syscon_device.o(.toc.LC0)
    . = . < 0xad0 ? 0xad0 : .;
    */lv0/src/syscon/get_syscon_device.o(.toc.LC1)
    . = . < 0xad8 ? 0xad8 : .;
    */lv0/src/syscon/sc_nv_storage.o(.toc.LC1)
    . = . < 0xae0 ? 0xae0 : .;
    */lv0/src/syscon/syscon_power_off.o(.toc.LC0)
    . = . < 0xae8 ? 0xae8 : .;
    */lv0/src/component/component_loaders.o(.toc.LC1)
    . = . < 0xaf0 ? 0xaf0 : .;
    */lv0/src/component/component_loaders.o(.toc.LC3)
    . = . < 0xaf8 ? 0xaf8 : .;
    */lv0/src/component/component_loaders.o(.toc.LC5)
    . = . < 0xb00 ? 0xb00 : .;
    */lv0/src/component/component_loaders.o(.toc.LC6)
    . = . < 0xb08 ? 0xb08 : .;
    */lv0/src/component/component_loaders.o(.toc.LC7)
    . = . < 0xb10 ? 0xb10 : .;
    */lv0/src/component/component_loaders.o(.toc.LC8)
    . = . < 0xb18 ? 0xb18 : .;
    */lv0/src/component/component_loaders.o(.toc.LC10)
    . = . < 0xb20 ? 0xb20 : .;
    */lv0/src/component/component_loaders.o(.toc.LC12)
    . = . < 0xb28 ? 0xb28 : .;
    */lv0/src/component/component_loaders.o(.toc.LC13)
    . = . < 0xb30 ? 0xb30 : .;
    */lv0/src/component/component_loaders.o(.toc.LC14)
    . = . < 0xb38 ? 0xb38 : .;
    */lv0/src/component/component_loaders.o(.toc.LC15)
    . = . < 0xb40 ? 0xb40 : .;
    */lv0/src/component/component_loaders.o(.toc.LC16)
    . = . < 0xb48 ? 0xb48 : .;
    */lv0/src/component/component_loaders.o(.toc.LC17)
    . = . < 0xb50 ? 0xb50 : .;
    */lv0/src/component/component_loaders.o(.toc.LC18)
    . = . < 0xb58 ? 0xb58 : .;
    */lv0/src/component/component_loaders.o(.toc.LC19)
    . = . < 0xb60 ? 0xb60 : .;
    */lv0/src/component/component_loaders.o(.toc.LC20)
    . = . < 0xb68 ? 0xb68 : .;
    */lv0/src/component/component_loaders.o(.toc.LC25)
    . = . < 0xb70 ? 0xb70 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC2)
    . = . < 0xb78 ? 0xb78 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC0)
    . = . < 0xb80 ? 0xb80 : .;
    */lv0/src/storage/storage_manager__base_dtor.o(.toc.LC0)
    . = . < 0xb88 ? 0xb88 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC9)
    . = . < 0xb90 ? 0xb90 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC11)
    . = . < 0xb98 ? 0xb98 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC12)
    . = . < 0xba0 ? 0xba0 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC14)
    . = . < 0xba8 ? 0xba8 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC15)
    . = . < 0xbb0 ? 0xbb0 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC4)
    . = . < 0xbb8 ? 0xbb8 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC5)
    . = . < 0xbc0 ? 0xbc0 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC20)
    . = . < 0xbc8 ? 0xbc8 : .;
    */lv0/src/flash/flash_storage.o(.toc.LC6)
    . = . < 0xbd0 ? 0xbd0 : .;
    */lv0/src/console/uart.o(.toc.LC0)
    . = . < 0xbd8 ? 0xbd8 : .;
    */lv0/src/console/uart.o(.toc.LC1)
    . = . < 0xbe0 ? 0xbe0 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC0)
    . = . < 0xbe8 ? 0xbe8 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC5)
    . = . < 0xbf0 ? 0xbf0 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC6)
    . = . < 0xbf8 ? 0xbf8 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC7)
    . = . < 0xc00 ? 0xc00 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC25)
    . = . < 0xc08 ? 0xc08 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC26)
    . = . < 0xc10 ? 0xc10 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC44)
    . = . < 0xc18 ? 0xc18 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC46)
    . = . < 0xc20 ? 0xc20 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC48)
    . = . < 0xc28 ? 0xc28 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC37)
    . = . < 0xc30 ? 0xc30 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC39)
    . = . < 0xc38 ? 0xc38 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC41)
    . = . < 0xc40 ? 0xc40 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC33)
    . = . < 0xc48 ? 0xc48 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC35)
    . = . < 0xc50 ? 0xc50 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC54)
    . = . < 0xc58 ? 0xc58 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC30)
    . = . < 0xc60 ? 0xc60 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC21)
    . = . < 0xc68 ? 0xc68 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC16)
    . = . < 0xc70 ? 0xc70 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC18)
    . = . < 0xc78 ? 0xc78 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC19)
    . = . < 0xc80 ? 0xc80 : .;
    */lv0/src/flash/spansion_storage.o(.toc.LC13)
    . = . < 0xc88 ? 0xc88 : .;
    */lv0/src/spu/spu_mmio.o(.toc.LC0)
    . = . < 0xc90 ? 0xc90 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC1)
    . = . < 0xc98 ? 0xc98 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC6)
    . = . < 0xca0 ? 0xca0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC8)
    . = . < 0xca8 ? 0xca8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC10)
    . = . < 0xcb0 ? 0xcb0 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC2)
    . = . < 0xcb8 ? 0xcb8 : .;
    */lv0/src/flash/nand_flash_ctrl.o(.toc.LC4)
    . = . < 0xcc0 ? 0xcc0 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.toc.LC0)
    . = . < 0xcc8 ? 0xcc8 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.toc.LC1)
    . = . < 0xcd0 ? 0xcd0 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC1)
    . = . < 0xcd8 ? 0xcd8 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC2)
    . = . < 0xce0 ? 0xce0 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC4)
    . = . < 0xce8 ? 0xce8 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC5)
    . = . < 0xcf0 ? 0xcf0 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC7)
    . = . < 0xcf8 ? 0xcf8 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC9)
    . = . < 0xd00 ? 0xd00 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC11)
    . = . < 0xd08 ? 0xd08 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC13)
    . = . < 0xd10 ? 0xd10 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC15)
    . = . < 0xd18 ? 0xd18 : .;
    */lv0/src/flash/nand_flash.o(.toc.LC19)
    . = . < 0xd20 ? 0xd20 : .;
    */lv0/src/flash/nand_flash_abs.o(.toc.LC0)
    . = . < 0xd28 ? 0xd28 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC0)
    . = . < 0xd30 ? 0xd30 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC25)
    . = . < 0xd38 ? 0xd38 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC26)
    . = . < 0xd40 ? 0xd40 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC28)
    . = . < 0xd48 ? 0xd48 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC32)
    . = . < 0xd50 ? 0xd50 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC33)
    . = . < 0xd58 ? 0xd58 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC35)
    . = . < 0xd60 ? 0xd60 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC11)
    . = . < 0xd68 ? 0xd68 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC12)
    . = . < 0xd70 ? 0xd70 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC14)
    . = . < 0xd78 ? 0xd78 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC16)
    . = . < 0xd80 ? 0xd80 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC18)
    . = . < 0xd88 ? 0xd88 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC20)
    . = . < 0xd90 ? 0xd90 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC22)
    . = . < 0xd98 ? 0xd98 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC40)
    . = . < 0xda0 ? 0xda0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC42)
    . = . < 0xda8 ? 0xda8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC3)
    . = . < 0xdb0 ? 0xdb0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC4)
    . = . < 0xdb8 ? 0xdb8 : .;
    */lv0/src/flash/nand_flash_transfer.o(.toc.LC6)
    . = . < 0xdc0 ? 0xdc0 : .;
    */lv0/src/log/printf.o(.toc.LC1)
    . = . < 0xdc8 ? 0xdc8 : .;
    */lv0/src/log/printf.o(.toc.LC3)
    . = . < 0xdd0 ? 0xdd0 : .;
    */lv0/src/log/printf.o(.toc.LC5)
    . = . < 0xdd8 ? 0xdd8 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC0)
    . = . < 0xde0 ? 0xde0 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC1)
    . = . < 0xde8 ? 0xde8 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC3)
    . = . < 0xdf0 ? 0xdf0 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC5)
    . = . < 0xdf8 ? 0xdf8 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC7)
    . = . < 0xe00 ? 0xe00 : .;
    */lv0/src/storage/nv_storage.o(.toc.LC8)
    . = . < 0xe08 ? 0xe08 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.toc.LC0)
    . = . < 0xe10 ? 0xe10 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.toc.LC1)
    . = . < 0xe18 ? 0xe18 : .;
    */lv0/src/io/systemio_dmac_dx.o(.toc.LC2)
    . = . < 0xe20 ? 0xe20 : .;
    */lv0/src/io/systemio_dmac_dx.o(.toc.LC3)
    . = . < 0xe28 ? 0xe28 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC0)
    . = . < 0xe30 ? 0xe30 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC1)
    . = . < 0xe38 ? 0xe38 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC6)
    . = . < 0xe40 ? 0xe40 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC7)
    . = . < 0xe48 ? 0xe48 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC3)
    . = . < 0xe50 ? 0xe50 : .;
    */lv0/src/io/systemio_dmac_px.o(.toc.LC4)
    . = . < 0xe58 ? 0xe58 : .;
    */lv0/src/log/log.o(.toc.LC0)
    . = . < 0xe60 ? 0xe60 : .;
    */lv0/src/log/log.o(.toc.LC1)
    . = . < 0xe68 ? 0xe68 : .;
    */lv0/src/log/log.o(.toc.LC3)
    . = . < 0xe70 ? 0xe70 : .;
    */lv0/src/console/cp_bridge.o(.toc.LC0)
    . = . < 0xe78 ? 0xe78 : .;
    */lv0/src/console/cp_bridge.o(.toc.LC3)
    . = . < 0xe80 ? 0xe80 : .;
    */lv0/src/console/cp_bridge.o(.toc.LC4)
    . = . < 0xe88 ? 0xe88 : .;
    */lv0/src/console/internal_console.o(.toc.LC0)
    . = . < 0xe90 ? 0xe90 : .;
    */lv0/src/console/internal_console.o(.toc.LC2)
    . = . < 0xe98 ? 0xe98 : .;
    */lv0/src/console/internal_console.o(.toc.LC3)
    . = . < 0xea0 ? 0xea0 : .;
    */lv0/src/console/internal_console.o(.toc.LC5)
    . = . < 0xea8 ? 0xea8 : .;
    */lv0/src/console/internal_console.o(.toc.LC8)
    . = . < 0xeb0 ? 0xeb0 : .;
    */lv0/src/io/gbe_work.o(.toc.LC0)
    . = . < 0xeb8 ? 0xeb8 : .;
    */lv0/src/io/gbe_work.o(.toc.LC2)
    . = . < 0xec0 ? 0xec0 : .;
    */lv0/src/io/gbe_work.o(.toc.LC4)
    . = . < 0xec8 ? 0xec8 : .;
    */lv0/src/io/gbe_work.o(.toc.LC6)
    . = . < 0xed0 ? 0xed0 : .;
    */lv0/src/io/pci_mini_driver.o(.toc.LC0)
    . = . < 0xed8 ? 0xed8 : .;
    */lv0/src/io/pci_mini_driver.o(.toc.LC2)
    . = . < 0xee0 ? 0xee0 : .;
    */lv0/src/io/pci_mini_driver.o(.toc.LC43)
    . = . < 0xee8 ? 0xee8 : .;
    */lv0/src/io/pci_mini_driver.o(.toc.LC28)
    . = . < 0xef0 ? 0xef0 : .;
    */lv0/src/io/pci_bus_power.o(.toc.LC1)
    . = . < 0xef8 ? 0xef8 : .;
    */lv0/src/io/pci_bus_power.o(.toc.LC3)
    . = . < 0xf00 ? 0xf00 : .;
    */lv0/src/console/cp_ch4_channel.o(.toc.LC6)
    . = . < 0xf08 ? 0xf08 : .;
    */lv0/src/console/cp_ch4_channel.o(.toc.LC0)
    . = . < 0xf10 ? 0xf10 : .;
    */lv0/src/console/cp_ch4_channel.o(.toc.LC1)
    . = . < 0xf18 ? 0xf18 : .;
    */lv0/src/console/cp_channel.o(.toc.LC0)
    . = . < 0xf20 ? 0xf20 : .;
    */lv0/src/console/cp_channel.o(.toc.LC1)
    . = . < 0xf28 ? 0xf28 : .;
    */lv0/src/console/cp_channel.o(.toc.LC3)
    . = . < 0xf30 ? 0xf30 : .;
    */lv0/src/console/cp_channel.o(.toc.LC4)
    . = . < 0xf38 ? 0xf38 : .;
    */lv0/src/console/physical_console.o(.toc.LC0)
    . = . < 0xf40 ? 0xf40 : .;
    */lv0/src/console/physical_console.o(.toc.LC7)
    . = . < 0xf48 ? 0xf48 : .;
    */lv0/src/console/physical_console.o(.toc.LC9)
    . = . < 0xf50 ? 0xf50 : .;
    */lv0/src/console/physical_console.o(.toc.LC6)
    . = . < 0xf58 ? 0xf58 : .;
    */lv0/src/console/physical_console.o(.toc.LC3)
    . = . < 0xf60 ? 0xf60 : .;
    */lv0/src/console/physical_console.o(.toc.LC5)
    . = . < 0xf68 ? 0xf68 : .;
    */lv0/src/console/file_transfer.o(.toc.LC2)
    . = . < 0xf70 ? 0xf70 : .;
    */lv0/src/console/file_transfer.o(.toc.LC3)
    . = . < 0xf78 ? 0xf78 : .;
    */lv0/src/console/file_transfer.o(.toc.LC0)
    . = . < 0xf80 ? 0xf80 : .;
    */lv0/src/console/file_transfer.o(.toc.LC1)
    . = . < 0xf88 ? 0xf88 : .;
    */lv0/src/console/file_transfer.o(.toc.LC28)
    . = . < 0xf90 ? 0xf90 : .;
    */lv0/src/console/file_transfer.o(.toc.LC30)
    . = . < 0xf98 ? 0xf98 : .;
    */lv0/src/console/file_transfer.o(.toc.LC33)
    . = . < 0xfa0 ? 0xfa0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC35)
    . = . < 0xfa8 ? 0xfa8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC38)
    . = . < 0xfb0 ? 0xfb0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC19)
    . = . < 0xfb8 ? 0xfb8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC6)
    . = . < 0xfc0 ? 0xfc0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC7)
    . = . < 0xfc8 ? 0xfc8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC8)
    . = . < 0xfd0 ? 0xfd0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC10)
    . = . < 0xfd8 ? 0xfd8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC12)
    . = . < 0xfe0 ? 0xfe0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC13)
    . = . < 0xfe8 ? 0xfe8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC15)
    . = . < 0xff0 ? 0xff0 : .;
    */lv0/src/console/file_transfer.o(.toc.LC18)
    . = . < 0xff8 ? 0xff8 : .;
    */lv0/src/console/file_transfer.o(.toc.LC20)
    . = . < 0x1000 ? 0x1000 : .;
    */lv0/src/console/file_transfer.o(.toc.LC22)
    . = . < 0x1008 ? 0x1008 : .;
    */lv0/src/console/file_transfer.o(.toc.LC24)
    . = . < 0x1010 ? 0x1010 : .;
    */lv0/src/console/file_transfer.o(.toc.LC27)
    . = . < 0x1018 ? 0x1018 : .;
    */lv0/src/util/mmio_accessor.o(.toc.LC0)
    . = . < 0x1020 ? 0x1020 : .;
    */lv0/src/util/mmio_accessor.o(.toc.LC2)
    . = . < 0x1028 ? 0x1028 : .;
    */lv0/src/util/mmio_accessor.o(.toc.LC3)
    . = . < 0x1030 ? 0x1030 : .;
    */lv0/src/util/mmio_accessor.o(.toc.LC4)
    . = . < 0x1038 ? 0x1038 : .;
    */lv0/src/console/cp_link.o(.toc.LC0)
    . = . < 0x1040 ? 0x1040 : .;
    */lv0/src/console/cp_link.o(.toc.LC3)
    . = . < 0x1048 ? 0x1048 : .;
    */lv0/src/util/aes.o(.toc.LC0)
    . = . < 0x1050 ? 0x1050 : .;
    */lv0/src/util/aes.o(.toc.LC1)
    . = . < 0x1058 ? 0x1058 : .;
    */lv0/src/util/aes.o(.toc.LC2)
  }
  __toc_base = ADDR(.toc) + 0x8000;
  . = . < 0x80c6e80 ? 0x80c6e80 : .;
  .bss :
  {
    __bss_start = .;
    . = . < 0x0 ? 0x0 : .;
    */lv0/src/boot/c_entry.o(.bss.g_application_ptr)
    . = . < 0x8 ? 0x8 : .;
    */lv0/src/component/boot_order_component_loader.o(.bss.g_boot_order_checked)
    . = . < 0x80 ? 0x80 : .;
    */lv0/src/spu/spu_manager.o(.bss.g_spu_ls_save_area)
    . = . < 0x1080 ? 0x1080 : .;
    */lv0/src/spu/spu_manager.o(.bss.g_spu_manager)
    . = . < 0x1150 ? 0x1150 : .;
    */lv0/src/spu/spu_manager.o(.bss.s_qa_token_loaded)
    . = . < 0x1154 ? 0x1154 : .;
    */lv0/src/boot/application.o(.bss.g_sys_hw_model_emulate)
    . = . < 0x1158 ? 0x1158 : .;
    */lv0/src/storage/set_release_mode.o(.bss.s_release_mode)
    . = . < 0x1159 ? 0x1159 : .;
    */lv0/src/syscon/get_boot_gos.o(.bss.s_boot_gos)
    . = . < 0x115a ? 0x115a : .;
    */lv0/src/syscon/get_boot_gos.o(.bss.s_boot_gos_read)
    . = . < 0x115b ? 0x115b : .;
    */lv0/src/platform/get_hw_config.o(.bss.s_hw_config_read)
    . = . < 0x1160 ? 0x1160 : .;
    */lv0/src/boot/application.o(.bss.g_platform_ptr)
    . = . < 0x1168 ? 0x1168 : .;
    */lv0/src/boot/application.o(.bss.g_platform_pwrmgr_ptr)
    . = . < 0x1170 ? 0x1170 : .;
    */lv0/src/io/get_sb_device.o(.bss._ZGVZ13get_sb_devicevE6device)
    . = . < 0x1178 ? 0x1178 : .;
    */lv0/src/io/get_sb_device.o(.bss._ZZ13get_sb_devicevE6device)
    . = . < 0x1180 ? 0x1180 : .;
    */lv0/src/storage/storage.o(.bss.g_storage)
    . = . < 0x1188 ? 0x1188 : .;
    */lv0/src/io/get_iommu_context.o(.bss._ZGVZ17get_iommu_contextvE7context)
    . = . < 0x1190 ? 0x1190 : .;
    */lv0/src/io/get_iommu_context.o(.bss._ZZ17get_iommu_contextvE7context)
    . = . < 0x1200 ? 0x1200 : .;
    */lv0/src/spu/spuctl.o(.bss.g_spu_slots)
    . = . < 0x1a00 ? 0x1a00 : .;
    */lv0/src/boot/boot_parm.o(.bss._ZGVZ13get_boot_parmvE4parm)
    . = . < 0x1a08 ? 0x1a08 : .;
    */lv0/src/boot/boot_parm.o(.bss._ZZ13get_boot_parmvE4parm)
    . = . < 0x3680 ? 0x3680 : .;
    */lv0/src/boot/boot_loader.o(.bss.g_boot_loader)
    . = . < 0x3688 ? 0x3688 : .;
    */lv0/src/boot/boot_cell_os.o(.bss.g_sys_ac_sd)
    . = . < 0x3690 ? 0x3690 : .;
    */lv0/src/boot/boot_cell_os.o(.bss.g_sys_ac_misc)
    . = . < 0x3698 ? 0x3698 : .;
    */lv0/src/platform/memory_budgets.o(.bss.g_memory_budget_low)
    . = . < 0x36c0 ? 0x36c0 : .;
    */lv0/src/platform/memory_budgets.o(.bss.g_memory_budget_high)
    . = . < 0x36e8 ? 0x36e8 : .;
    */lv0/src/log/log_ring.o(.bss._ZGVZ12get_log_ringvE4ring)
    . = . < 0x36f0 ? 0x36f0 : .;
    */lv0/src/log/log_ring.o(.bss._ZZ12get_log_ringvE4ring)
    . = . < 0x3710 ? 0x3710 : .;
    */lv0/src/platform/pio.o(.bss._ZGVZ7get_piovE5block)
    . = . < 0x3718 ? 0x3718 : .;
    */lv0/src/platform/pio.o(.bss._ZZ7get_piovE5block)
    . = . < 0x3720 ? 0x3720 : .;
    */lv0/src/platform/platform.o(.bss.g_platform)
    . = . < 0x3730 ? 0x3730 : .;
    */lv0/src/platform/platform_pwrmgr.o(.bss.g_platform_pwrmgr)
    . = . < 0x3738 ? 0x3738 : .;
    */lv0/src/syscon/sc_console.o(.bss.g_sc_console_buffer)
    . = . < 0x3840 ? 0x3840 : .;
    */lv0/src/syscon/syscon.o(.bss.g_sc_msg_buffer)
    . = . < 0x4040 ? 0x4040 : .;
    */lv0/src/syscon/get_syscon_device.o(.bss.g_sc_swbint_pending)
    . = . < 0x4046 ? 0x4046 : .;
    */lv0/src/syscon/get_syscon_device.o(.bss.g_send_and_receive_transaction_id)
    . = . < 0x4048 ? 0x4048 : .;
    */lv0/src/syscon/get_syscon_device.o(.bss._ZGVZ17get_syscon_devicevE6device)
    . = . < 0x4050 ? 0x4050 : .;
    */lv0/src/syscon/get_syscon_device.o(.bss._ZZ17get_syscon_devicevE6device)
    . = . < 0x4068 ? 0x4068 : .;
    */lv0/src/component/component_loaders.o(.bss.g_flash_format_1_component_loader)
    . = . < 0x40a0 ? 0x40a0 : .;
    */lv0/src/component/component_loaders.o(.bss.g_flash_format_2_component_loader)
    . = . < 0x40d8 ? 0x40d8 : .;
    */lv0/src/component/component_loaders.o(.bss.g_flash_format_3_component_loader)
    . = . < 0x4130 ? 0x4130 : .;
    */lv0/src/component/component_loaders.o(.bss.g_host_file_loader)
    . = . < 0x4138 ? 0x4138 : .;
    */lv0/src/component/component_loaders.o(.bss.g_boot_order_component_loader_1)
    . = . < 0x4150 ? 0x4150 : .;
    */lv0/src/component/component_loaders.o(.bss.g_boot_order_component_loader_2)
    . = . < 0x4168 ? 0x4168 : .;
    */lv0/src/component/component_loaders.o(.bss.g_boot_order_component_loader_3)
    . = . < 0x4180 ? 0x4180 : .;
    */lv0/src/component/component_loaders.o(.bss.g_componnet_manager)
    . = . < 0x4418 ? 0x4418 : .;
    */lv0/src/component/component_loaders.o(.bss.g_component_loader_ptr)
    . = . < 0x4420 ? 0x4420 : .;
    */lv0/src/flash/flash_storage.o(.bss.g_nand_flash_abs)
    . = . < 0x4658 ? 0x4658 : .;
    */lv0/src/flash/flash_storage.o(.bss.g_spansion_storage)
    . = . < 0x4688 ? 0x4688 : .;
    */lv0/src/flash/flash_storage.o(.bss.g_flash_storage_manager)
    . = . < 0x4690 ? 0x4690 : .;
    */lv0/src/flash/flash_storage.o(.bss.g_flash_storage_device_table)
    . = . < 0x46a0 ? 0x46a0 : .;
    */lv0/src/console/uart.o(.bss._ZGVZ8get_uartvE7console)
    . = . < 0x46a8 ? 0x46a8 : .;
    */lv0/src/console/uart.o(.bss._ZZ8get_uartvE7console)
    . = . < 0x46b0 ? 0x46b0 : .;
    */lv0/src/flash/spansion_storage.o(.bss._ZZN16spansion_storage13progress_tickEvE5count)
    . = . < 0x46c0 ? 0x46c0 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.bss._ZGVZ19get_nand_flash_ctrlvE4ctrl)
    . = . < 0x46c8 ? 0x46c8 : .;
    */lv0/src/flash/get_nand_flash_ctrl.o(.bss._ZZ19get_nand_flash_ctrlvE4ctrl)
    . = . < 0x46d0 ? 0x46d0 : .;
    */lv0/src/flash/nand_flash_transfer.o(.bss.g_nand_flash_ready_callback)
    . = . < 0x46d8 ? 0x46d8 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48000)
    . = . < 0x46f0 ? 0x46f0 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48800)
    . = . < 0x4700 ? 0x4700 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c00)
    . = . < 0x4720 ? 0x4720 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c22)
    . = . < 0x4728 ? 0x4728 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c30)
    . = . < 0x4738 ? 0x4738 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c40)
    . = . < 0x4748 ? 0x4748 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c80)
    . = . < 0x4758 ? 0x4758 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48c90)
    . = . < 0x4788 ? 0x4788 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48d00)
    . = . < 0x4798 ? 0x4798 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48d20)
    . = . < 0x47a0 ? 0x47a0 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48d3e)
    . = . < 0x47f0 ? 0x47f0 : .;
    */lv0/src/storage/nv_storage.o(.bss.nv_buf_48d28)
    . = . < 0x4808 ? 0x4808 : .;
    */lv0/src/storage/nv_storage.o(.bss.g_sc_nv_storage_version)
    . = . < 0x480a ? 0x480a : .;
    */lv0/src/storage/nv_storage.o(.bss.g_sc_nv_storage_version_initialized)
    . = . < 0x4810 ? 0x4810 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.bss._ZGVZ20get_systemio_dmac_dxvE4dmac)
    . = . < 0x4818 ? 0x4818 : .;
    */lv0/src/io/get_systemio_dmac_dx.o(.bss._ZZ20get_systemio_dmac_dxvE4dmac)
    . = . < 0x4840 ? 0x4840 : .;
    */lv0/src/io/systemio_dmac_px.o(.bss._ZGVZ20get_systemio_dmac_pxvE4dmac)
    . = . < 0x4848 ? 0x4848 : .;
    */lv0/src/io/systemio_dmac_px.o(.bss._ZZ20get_systemio_dmac_pxvE4dmac)
    . = . < 0x4868 ? 0x4868 : .;
    */lv0/src/log/log.o(.bss.g_log_handlers)
    . = . < 0x4888 ? 0x4888 : .;
    */lv0/src/log/log.o(.bss.g_log_final_handler)
    . = . < 0x4890 ? 0x4890 : .;
    */lv0/src/console/cp_bridge.o(.bss._ZZ13get_cp_bridgevE5ready)
    . = . < 0x4898 ? 0x4898 : .;
    */lv0/src/console/cp_bridge.o(.bss.g_cp_bridge)
    . = . < 0x48a8 ? 0x48a8 : .;
    */lv0/src/console/internal_console.o(.bss.g_internal_console)
    . = . < 0x48b8 ? 0x48b8 : .;
    */lv0/src/console/internal_console.o(.bss.g_internal_console_channel)
    . = . < 0x4940 ? 0x4940 : .;
    */lv0/src/io/gbe_work.o(.bss.g_gbe_work)
    . = . < 0x4968 ? 0x4968 : .;
    */lv0/src/io/pci_mini_driver.o(.bss.g_pci_mini_driver)
    . = . < 0x4a70 ? 0x4a70 : .;
    */lv0/src/io/pci_mini_driver.o(.bss.g_pci_devparams)
    . = . < 0x4c70 ? 0x4c70 : .;
    */lv0/src/console/cp_ch4_channel.o(.bss.g_cp_ch4_channel)
    . = . < 0x4cd8 ? 0x4cd8 : .;
    */lv0/src/console/cp_ch4_channel.o(.bss.g_cp_ch4_channel_devparam)
    . = . < 0x4ce8 ? 0x4ce8 : .;
    */lv0/src/console/cp_channel.o(.bss.g_cp_channel)
    . = . < 0x4d50 ? 0x4d50 : .;
    */lv0/src/console/cp_channel.o(.bss.g_cp_channel_devparam)
    . = . < 0x4d60 ? 0x4d60 : .;
    */lv0/src/console/physical_console.o(.bss.g_physical_console)
    . = . < 0x4d80 ? 0x4d80 : .;
    */lv0/src/console/physical_console.o(.bss.g_physical_console_tx_buffer)
    . = . < 0x4d88 ? 0x4d88 : .;
    */lv0/src/console/physical_console.o(.bss.g_physical_console_rx_buffer)
    . = . < 0x4d90 ? 0x4d90 : .;
    */lv0/src/console/file_transfer.o(.bss.g_ft_channels)
    . = . < 0x4da0 ? 0x4da0 : .;
    */lv0/src/console/file_transfer.o(.bss.g_log_cons_ctl)
    . = . < 0x4e28 ? 0x4e28 : .;
    */lv0/src/console/file_transfer.o(.bss.g_log_cons_data)
    . = . < 0x4eb0 ? 0x4eb0 : .;
    */lv0/src/util/mmio_accessor.o(.bss.g_mmio_accessor)
    . = . < 0x4ec8 ? 0x4ec8 : .;
    */lv0/src/util/mmio_accessor.o(.bss.g_mmio_accessor_ptr)
  }
  __bss_end = ALIGN(128);
  /DISCARD/ : { *(*) }
}
