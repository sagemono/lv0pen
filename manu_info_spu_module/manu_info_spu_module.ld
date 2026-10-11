ENTRY(_start)

SECTIONS
{
  . = 0x880;

  .text :
  {
    . = . < 0x0 ? 0x0 : .;
    KEEP(*/manu_info_spu_module/src/salt.o(.text.salt))
    . = . < 0x10 ? 0x10 : .;
    */spu_token_processor/src/crt0.o(.text)
    . = . < 0x120 ? 0x120 : .;
    */manu_info_spu_module/src/main.o(.text._Z11is_valid_eay)
    . = . < 0x160 ? 0x160 : .;
    */manu_info_spu_module/src/main.o(.text._Z9put_valueyyj)
    . = . < 0x240 ? 0x240 : .;
    */manu_info_spu_module/src/main.o(.text.main)
    . = . < 0x2e0 ? 0x2e0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC1Ev)
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelC2Ev)
    . = . < 0x2f8 ? 0x2f8 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel5issueEjyjjjj)
    . = . < 0x3a0 ? 0x3a0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channel4waitEj)
    . = . < 0x3d0 ? 0x3d0 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD0Ev)
    . = . < 0x3e8 ? 0x3e8 : .;
    */spp_verifier/src/runtime/dma_channel.o(.text._ZN11dma_channelD1Ev)
    . = . < 0x400 ? 0x400 : .;
    */spp_verifier/src/runtime/delete.o(.text._ZdlPv)
    . = . < 0x408 ? 0x408 : .;
    */spp_verifier/src/runtime/exit.o(.text.exit)
    . = . < 0x430 ? 0x430 : .;
    */spp_verifier/src/runtime/free.o(.text.free)
    . = . < 0x5c0 ? 0x5c0 : .;
    */spp_verifier/src/runtime/memset.o(.text.memset)
    . = . < 0x6b0 ? 0x6b0 : .;
    */spp_verifier/src/runtime/atexit.o(.text.run_atexit)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .rodata :
  {
    *(.rodata._ZTV11dma_channel)
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x80);
  .ctors : { KEEP(*(.ctors)) KEEP(*(.ctors.end)) }
  .dtors : { KEEP(*(.dtors)) KEEP(*(.dtors.end)) }
  .data :
  {
    */spu_token_processor/src/crt0.o(.data)
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .bss :
  {
    */spp_verifier/src/runtime/heap.o(.bss.g_heap)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_count)
    */spp_verifier/src/runtime/atexit.o(.bss.g_atexit_handlers)
    *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  }

  .note.spu_name 0 : { KEEP(*(.note.spu_name)) }

  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
