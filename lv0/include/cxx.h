#ifndef LV0_CXX_H
#define LV0_CXX_H

#define CXX_VTABLE(tag, slots_type) \
    struct tag { long offset_to_top; const void *type_info; slots_type slots; }

#define CXX_DROPPED __attribute__((used, section(".dropped")))

#define CXX_ALIAS(name, cxxname) \
    __asm__("\t.weak " #cxxname "\n\t.set " #cxxname "," #name "\n" \
            "\t.weak ." #cxxname "\n\t.set ." #cxxname ",." #name "\n")

#ifdef __cplusplus
inline void *operator new(__SIZE_TYPE__, void *p) throw() { return p; }
#endif

#endif
