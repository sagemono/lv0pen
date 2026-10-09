# lv0pen

Source for the PlayStation 3 boot chain, rebuilt with the original compilers to the original code.

| | Program | Matching |
|---|---|---|
| `lv0` | lv0 4.93 | 100% |
| `lv0ldr` | bootldr 1.0.0 | 458 / 458 |
| `metldr` | metldr | 182 / 182 |
| `isoldr` | isoldr 4.93 | 273 / 273 |
| `lv1ldr` | lv1ldr 4.93 | 293 / 300 |
| `lv2ldr` | lv2ldr 4.93 | 258 / 259 |
| `appldr` | appldr 4.93 | 446 / 446 |

## Compilers

- PPU GCC 4.1.1 (SDK420): `lv0`
- SPU GCC 4.1.1 (SDK420): `isoldr`, `lv1ldr`, `lv2ldr`, `appldr`
- SPU GCC 4.0.2 (CELL 4.1.7): `lv0ldr`, `metldr`
- SPU GCC 3.4.1 (CELL 2.2.1): crypto
- SPU GCC 4.1.1 (CELL 4.1.2.6 Beta): `lv1ldr/encdec`

No firmware is included.

## License

GPL v3. `lv1ldr/zlib` is altered zlib 1.2.3 under its own license.