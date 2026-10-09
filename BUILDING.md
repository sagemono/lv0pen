# Building

Needs GNU make and the compilers that built each part.

| Variable | Compiler | Used for |
|---|---|---|
| `SPU_GCC402_BIN` | SPU GCC 4.0.2 (CELL 4.1.7) | `lv0ldr`, `metldr` |
| `SPU_GCC341_BIN` | SPU GCC 3.4.1 (CELL 2.2.1) | the crypto library |
| `SPU_GCC411_SDK420_BIN` | SPU GCC 4.1.1 (SDK420) | `isoldr`, `lv1ldr`, `lv2ldr`, `appldr` |
| `PPU_GCC411_SDK420_BIN` | PPU GCC 4.1.1 (SDK420) | `lv0` |
| `SPU_GCC411_BETA_BIN` | SPU GCC 4.1.1 (CELL 4.1.2.6 Beta) | `lv1ldr/encdec` |

Set each to the compiler's `bin` directory, on the command line:

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
| `make metldr`, `make lv0ldr` | build one |
| `make check` | compare the builds with your images |
| `make clean` | remove `build/` |

`lv0ldr` and `metldr` build so far; the rest are coming.

## Comparing

`make check` needs your own images:

- `metldr/image/metldr.bin`
- `lv0ldr/image/lv0ldr_1.0.0.bin`

Both are local store dumps. lv0ldr's check allows the 16 bytes of `.data` the loader changes as it runs, listed in `lv0ldr/lv0ldr.written`.