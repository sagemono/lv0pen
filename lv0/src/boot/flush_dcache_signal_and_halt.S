	.section ".text"
	.align	2
	.globl	.flush_dcache_signal_and_halt
	.type	.flush_dcache_signal_and_halt, @function
.flush_dcache_signal_and_halt:
	li	5, 0
1:	cmpd	5, 3
	bge-	2f
	dcbf	0, 5
	addi	5, 5, 128
	b	1b
2:	sync
	li	5, 1
	stw	5, 0(4)
3:	b	3b
	.size	.flush_dcache_signal_and_halt, . - .flush_dcache_signal_and_halt
