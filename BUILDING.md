# Building

Needs GNU make, the compilers that built each part, and scetool for lv0.

| Variable | Tool | Used for |
|---|---|---|
| `SPU_GCC402_BIN` | SPU GCC 4.0.2 (CELL 4.1.7) | `lv0ldr`, `metldr` |
| `SPU_GCC341_BIN` | SPU GCC 3.4.1 (CELL 2.2.1) | the crypto library, `mc_iso_spu_module`'s DES |
| `SPU_GCC411_SDK420_BIN` | SPU GCC 4.1.1 (SDK420) | `lv0ldr2`, `metldr2`, `isoldr`, `lv1ldr`, `lv2ldr`, `appldr`, the other SPU modules |
| `SPU_GCC411_SDK350_BIN` | SPU GCC 4.1.1 (SDK350) | `lv0ldr2`'s libgcc division functions |
| `SPU_GCC411_SDK250_BIN` | SPU GCC 4.1.1 (SDK 2.50) | `mc_iso_spu_module` |
| `PPU_GCC411_SDK420_BIN` | PPU GCC 4.1.1 (SDK420) | `lv0` |
| `SPU_GCC411_BETA_BIN` | SPU GCC 4.1.1 (CELL 4.1.2.6 Beta) | `lv1ldr/encdec`, `sc_iso`'s secure-communication library |
| `SCETOOL` | scetool, with its keys in its own `data/` | the loaders lv0 embeds |
| `HOST_CC` | a C compiler for the build machine (default `cc`) | the tool that encrypts lv1ldr for lv0 |

Set each to the compiler's `bin` directory (and `SCETOOL` to the scetool
executable), on the command line:

```
make SPU_GCC402_BIN=/path/to/spu/bin SPU_GCC341_BIN=/path/to/spu/bin
```

or once in `mk/local.mk`:

```
SPU_GCC402_BIN := /path/to/spu/bin
SPU_GCC341_BIN := /path/to/spu/bin
```

## Targets

| Target | |
|---|---|
| `make` | build everything into `build/` |
| `make lv0`, `make lv0ldr`, `make lv0ldr2`, `make metldr`, `make metldr2`, `make isoldr`, `make lv1ldr`, `make lv2ldr`, `make appldr`, `make spp_verifier`, `make spu_token_processor`, `make spu_utoken_processor`, `make spu_pkg_rvk_verifier`, `make aim_spu_module`, `make manu_info_spu_module`, `make sb_iso_spu_module`, `make mc_iso_spu_module`, `make sc_iso` | build one |
| `make check` | compare the builds with your images |
| `make clean` | remove `build/` |

`lv0ldr2` and `metldr2` are lv0ldr.2 and metldr.2, the loaders of firmware 3.60 and later. They share units with `isoldr`, `lv0ldr` (the crypto library), `lv1ldr` and `lv2ldr`. `lv0ldr2`'s build takes the memory test's SPU program (the ELF the loader carries at 0x2A280, which is not in this tree) out of your image. It also compiles libgcc's four 64-bit division functions again until each comes out as the image's code, because that compiler does not always produce the same code for them.

`mc_iso_spu_module`'s code is older than the other modules': it builds with SDK 2.50's compiler, which has no `spu_intrinsics.h` or `spu_mfcio.h` of its own, so its units use `lv1ldr/encdec/sys`'s.

## lv0's modules

lv0 embeds lv1ldr, lv2ldr, isoldr and appldr as SELFs. The build encrypts its own builds of them with scetool, so their bytes differ from the original's (the keys and signature are new each time). lv1ldr's SELF is then encrypted once more with lv0's own AES-CTR, by a small host tool built from lv0's `aes.c` (`lv0/host/lv1ldr_ctr.c`). The four second headers stay zero for now.

## Comparing

`make check` needs your own images removed from CORE_OS_PACKAGE.pkg:

- `lv0/lv0_ida/lv0.elf`
- `lv0ldr/image/lv0ldr_1.0.0.bin`
- `lv0ldr2/image/lv0ldr2.bin`
- `metldr/image/metldr.bin`
- `metldr2/image/metldr2.bin`
- `isoldr/image/isoldr.elf`
- `lv2ldr/image/lv2ldr.elf`
- `appldr/image/appldr.elf`
- `lv1ldr/image/lv1ldr.elf`
- `spp_verifier/image/spp_verifier.elf`
- `spu_token_processor/image/spu_token_processor.elf`
- `spu_utoken_processor/image/spu_utoken_processor.elf`
- `spu_pkg_rvk_verifier/image/spu_pkg_rvk_verifier.elf`
- `aim_spu_module/image/aim_spu_module.elf`
- `manu_info_spu_module/image/manu_info_spu_module.elf`
- `sb_iso_spu_module/image/sb_iso_spu_module.elf`
- `mc_iso_spu_module/image/mc_iso_spu_module.elf`
- `sc_iso/image/sc_iso.elf`

lv0 is compared byte for byte outside its module slots; each slot is checked
by decrypting both the built and the original SELF.

lv0ldr and metldr's images are local store dumps. lv0ldr2's and metldr2's are the loaders' decrypted bodies (the part after the 0x40-byte header), which load at local-store address 0x400. lv0ldr's check allows the 16 bytes of `.data` the loader changes as it runs (`lv0ldr/lv0ldr.written`). lv0ldr2's, lv2ldr's, lv1ldr's and the SPU modules' checks allow the bytes of the functions that do not match yet (each program's `.unmatched`). For lv0ldr2, that also covers one message in `xdr_dump.cpp` that its build does not yet place where the image has it.