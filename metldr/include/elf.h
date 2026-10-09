#ifndef MLDR_ELF_H
#define MLDR_ELF_H

#include "types.h"

typedef struct {
    unsigned char e_ident[16];
    unsigned short e_type;
    unsigned short e_machine;
    unsigned int e_version;
    u64 e_entry;
    u64 e_phoff;
    u64 e_shoff;
    unsigned int e_flags;
    unsigned short e_ehsize;
    unsigned short e_phentsize;
    unsigned short e_phnum;
    unsigned short e_shentsize;
    unsigned short e_shnum;
    unsigned short e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    unsigned int p_type;
    unsigned int p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u64 p_align;
} Elf64_Phdr;

typedef struct {
    unsigned char e_ident[16];
    unsigned short e_type;
    unsigned short e_machine;
    unsigned int e_version;
    unsigned int e_entry;
    unsigned int e_phoff;
    unsigned int e_shoff;
    unsigned int e_flags;
    unsigned short e_ehsize;
    unsigned short e_phentsize;
    unsigned short e_phnum;
    unsigned short e_shentsize;
    unsigned short e_shnum;
    unsigned short e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    unsigned int p_type;
    unsigned int p_offset;
    unsigned int p_vaddr;
    unsigned int p_paddr;
    unsigned int p_filesz;
    unsigned int p_memsz;
    unsigned int p_flags;
    unsigned int p_align;
} Elf32_Phdr;

struct header_data {
    unsigned char buf[256];
    unsigned int len __attribute__((aligned(16)));
};

extern header_data g_header;

class elf_image {
public:
    elf_image() : ehdr(0), phdr(0) { }

    long load(const void *image);

    long get_header(Elf32_Ehdr *out);
    long get_program_header(unsigned int i, Elf32_Phdr *out);

    const Elf32_Ehdr *ehdr;
    const Elf32_Phdr *phdr;
};

#endif
