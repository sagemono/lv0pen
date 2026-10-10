# lv0pen

Source for the PlayStation 3 boot chain and its isolated SPU modules, rebuilt with the original compilers to the original code.

| | Program | Matching |
|---|---|---|
| `lv0` | lv0 4.93 | 100% |
| `lv0ldr` | bootldr 1.0.0 | 458 / 458 |
| `metldr` | metldr | 182 / 182 |
| `isoldr` | isoldr 4.93 | 273 / 273 |
| `lv1ldr` | lv1ldr 4.93 | 293 / 300 |
| `lv2ldr` | lv2ldr 4.93 | 258 / 259 |
| `appldr` | appldr 4.93 | 446 / 446 |
| `spp_verifier` | spp_verifier 4.93 | 139 / 141 |
| `spu_token_processor` | spu_token_processor 4.93 | 104 / 106 |
| `spu_utoken_processor` | spu_utoken_processor 4.93 | 72 / 75 |
| `spu_pkg_rvk_verifier` | spu_pkg_rvk_verifier 4.93 | 165 / 171 |

No firmware is included. See [BUILDING.md](BUILDING.md) to build.

## License

GPL v3. `lv1ldr/zlib` is altered zlib 1.2.3 under its own license.