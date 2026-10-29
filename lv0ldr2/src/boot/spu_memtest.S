	.section .data.spu_memtest,"aw",@progbits
	.p2align 7
	.globl	spu_memtest
spu_memtest:
	.incbin	"spu_memtest.elf"
	.size	spu_memtest, .-spu_memtest
