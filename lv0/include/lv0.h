#ifndef LV0_H
#define LV0_H

#ifdef __cplusplus
#define _Bool bool
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long       u64;
typedef signed char         s8;
typedef short               s16;
typedef int                 s32;
typedef long                s64;

#define LV0_TOC_BASE        0x4DA80ULL

#define LV0_TEXT_BEGIN      0x20220ULL
#define LV0_TEXT_END        0x34748ULL
#define LV0_RODATA_BEGIN    0x34750ULL
#define LV0_RODATA_END      0x41428ULL
#define LV0_OPD_BEGIN       0x41EE8ULL
#define LV0_OPD_END         0x459E0ULL
#define LV0_TOC_BEGIN       0x45A80ULL
#define LV0_TOC_END         0x46B10ULL
#define LV0_BSS_BEGIN       0x46B80ULL
#define LV0_BSS_END         0x4B428ULL

#define LV0_ERROR_BASE          0xB0000000
#define LV0_ERR_CONFIG          0xB0000001
#define LV0_ERR_BOOT_LV1        0xB0000004
#define LV0_ERR_DMA             0xB0000005
#define LV0_ERR_FLASH           0xB0000006
#define LV0_ERR_DEBUG_INTERFACE 0xB0000008
#define LV0_ERR_NO_SPU          0xB0000009
#define LV0_ERR_LV1LDR_STOP     0xB000000A
#define LV0_ERR_INTERNAL        0xB0000063

#define FUNCTION_NAME(name) static const char function_name[] = name

#define DEAD_STRING(var, s) static const char var[] __attribute__((used)) = s

#endif
