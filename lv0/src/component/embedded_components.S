	.macro	component name, file
	.globl	embedded_\name
embedded_\name:
	.incbin	"embedded/\file"
	.globl	embedded_\name\()_end
embedded_\name\()_end:
	.globl	embedded_\name\()_size
	.set	embedded_\name\()_size, embedded_\name\()_end - embedded_\name
	.endm

	.section ".data.embedded_components","aw"
	.p2align 2
	component lv1ldr, lv1ldr.bin
	component lv2ldr, lv2ldr.bin
	component isoldr, isoldr.bin
	component appldr, appldr.bin
	component lv1ldr_2, lv1ldr_2.bin
	component lv2ldr_2, lv2ldr_2.bin
	component isoldr_2, isoldr_2.bin
	component appldr_2, appldr_2.bin
