#define STACK_TOP	0xC000000
#define MIN_FRAME	128

	.section ".vec", "awx"

	.org	0x100
reset:
	b	start
	.org	0x108
	.globl	lv1ldr_aes_rk
lv1ldr_aes_rk:
	.long	0x31A57D41, 0xFF28CD29, 0x721B41D3, 0x47F48809
	.long	0xD01E8C70, 0xA8F297D2, 0xF652C298, 0x315D9D4F
	.long	0xD3ADB79D, 0x9BC55C38, 0xBF49F866, 0x4FB84281
	.long	0x145A9351, 0x30D5A22A, 0xAE065D2B, 0x3542175D
	.long	0x36337FA7, 0x2E0C24F1, 0x0B8605BB, 0x1957D81D
	.long	0x6DA1D45D, 0xCEBF6B50, 0x3283905B, 0xC1F656CC
	.long	0xD23F4082, 0x34778FBC, 0xA0DAD68C, 0x625951E2
	.long	0x0C2CF06A, 0x001CDA65, 0xE715148C, 0xF1EEB294
	.long	0xD2C4EABF, 0x968CFA6F, 0x23608F6F, 0x6257F317
	.long	0xF53FD595, 0xA656EDC8, 0x739596EA, 0x6F254292
	.long	0x17EC4391, 0x566D330D, 0xB1FE95CD, 0xF45B9F3B
	.globl	lv1ldr_aes_iv
lv1ldr_aes_iv:
	.byte	0xB5, 0xF9, 0xF0, 0xE4, 0x63, 0x07, 0xDB, 0xFA
	.byte	0x5F, 0x54, 0xBE, 0x39, 0x74, 0x75, 0xE2, 0xBB
	.org	0x200
	b	.
	.org	0x300
	b	.
	.org	0x380
	b	.
	.org	0x400
	b	.
	.org	0x480
	b	.
	.org	0x500
	b	.
	.org	0x600
	b	.
	.org	0x700
	b	.
	.org	0x800
	b	.
	.org	0x900
	b	.
	.org	0x980
	b	.

	.align	3, 0
hid_dd2:
	.quad	0x2000004b00000000, 0x9c30100000000000, 0x9e30100000000000
	.quad	0x0600300000000000, 0x0680300000000000, 0
	.quad	0x0000001400000000, 0
hid_dd3:
	.quad	0x0000004b00000000, 0x9c30104000000000, 0x9e30100000000000
	.quad	0x00003f0000000000, 0x00803f0000000000, 0
	.quad	0x0000001400000000, 0
hid_dd5:
	.quad	0x0000004b00000000, 0x9c30104000000000, 0x9e30100000000000
	.quad	0x00003f0000000000, 0x00803f0000000000, 0
	.quad	0x0000001400000000, 0

pvr_table:
	.quad	0x700000, hid_dd2
	.quad	0x700100, hid_dd2
	.quad	0x700101, hid_dd2
	.quad	0x700101, hid_dd2
	.quad	0x700400, hid_dd3
	.quad	0x700500, hid_dd5
	.quad	0x700501, hid_dd5
	.quad	0, 0

msr_value:
	.quad	0x9000000000000000
lpcr_value:
	.quad	6
tscr_value:
	.long	0xf8040000, 0
	.quad	0

start:
	li	0, 0
	mfpvr	4
	lis	5, pvr_table@h
	ori	5, 5, pvr_table@l
1:	ld	6, 0(5)
	cmpd	6, 4
	beq-	3f
	cmpdi	6, 0
	beq-	2f
	addi	5, 5, 16
	b	1b
2:	addi	5, 5, -16
3:	ld	3, lpcr_value@l(0)
	mtspr	318, 3
	isync
	slbia
	ld	3, 8(5)
	ld	4, 8(3)
	lwsync
	mtspr	1009, 4
	lwsync
	isync
	ld	4, 24(3)
	lwsync
	mtspr	1012, 4
	lwsync
	isync
	ld	4, 16(3)
	lwsync
	mtspr	1009, 4
	lwsync
	isync
	ld	4, 32(3)
	lwsync
	mtspr	1012, 4
	lwsync
	isync
	ld	4, 8(3)
	lwsync
	mtspr	1009, 4
	lwsync
	isync
	ld	4, 24(3)
	lwsync
	mtspr	1012, 4
	lwsync
	isync
	ld	4, 0(3)
	lwsync
	mtspr	1008, 4
	lwsync
	isync
	ld	4, 40(3)
	lwsync
	mtspr	1014, 4
	lwsync
	isync
	ld	4, 48(3)
	lwsync
	mtspr	1017, 4
	lwsync
	isync
	ld	4, 56(3)
	lwsync
	mtspr	1018, 4
	lwsync
	isync
	lwz	4, tscr_value@l(0)
	mtspr	921, 4
	isync
	li	4, 0
	isync
	mtspr	319, 4
	isync
	ld	3, msr_value@l(0)
	mtmsrd	3
	lis	3, __bss_start@h
	ori	3, 3, __bss_start@l
	lis	4, __bss_end@h
	ori	4, 4, __bss_end@l
	subf	4, 3, 4
	srdi	4, 4, 7
	mtctr	4
4:	dcbz	0, 3
	addi	3, 3, 128
	bdnz	4b
	lis	1, (STACK_TOP - MIN_FRAME)@h
	ori	1, 1, (STACK_TOP - MIN_FRAME)@l
	lis	2, __toc_base@h
	ori	2, 2, __toc_base@l
	lis	3, .c_entry@h
	ori	3, 3, .c_entry@l
	mtlr	3
	blrl

	.globl	_start
_start:
	.quad	reset, 0
