# Building

Needs GNU make, the compilers that built each part, and scetool for lv0.

| Variable | Tool | Used for |
|---|---|---|
| `SPU_GCC402_BIN` | SPU GCC 4.0.2 (CELL 4.1.7) | `lv0ldr`, `metldr` |
| `SPU_GCC341_BIN` | SPU GCC 3.4.1 (CELL 2.2.1) | the crypto library |
| `SPU_GCC411_SDK420_BIN` | SPU GCC 4.1.1 (SDK420) | `isoldr`, `lv1ldr`, `lv2ldr`, `appldr` |
| `PPU_GCC411_SDK420_BIN` | PPU GCC 4.1.1 (SDK420) | `lv0` |
| `SPU_GCC411_BETA_BIN` | SPU GCC 4.1.1 (CELL 4.1.2.6 Beta) | `lv1ldr/encdec` |
| `SCETOOL` | scetool, with its keys in its own `data/` | the loaders lv0 embeds |

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
| `make lv0`, `make lv0ldr`, `make metldr`, `make isoldr`, `make lv2ldr`, `make appldr` | build one |
| `make check` | compare the builds with your images |
| `make clean` | remove `build/` |

`lv1ldr` does not build yet.

## lv0's modules

lv0 embeds isoldr, lv2ldr and appldr as SELFs. The build encrypts its own builds of them with scetool, so their bytes differ from the original's (the keys and signature are new each time). lv1ldr's slot and the four second headers stay zero for now.

## Comparing

`make check` needs your own images removed from CORE_OS_PACKAGE.pkg:

- `lv0/lv0_ida/lv0.elf`
- `lv0ldr/image/lv0ldr_1.0.0.bin`
- `metldr/image/metldr.bin`
- `isoldr/image/isoldr.elf`
- `lv2ldr/image/lv2ldr.elf`
- `appldr/image/appldr.elf`

lv0 is compared byte for byte outside its module slots; each slot is checked
by decrypting both the built and the original SELF.

lv0ldr and metldr's images are local store dumps. lv0ldr's check allows the 16 bytes of `.data` the loader changes as it runs (`lv0ldr/lv0ldr.written`). lv2ldr's allows the 20 bytes of the one function that does not match yet (`lv2ldr/lv2ldr.unmatched`).