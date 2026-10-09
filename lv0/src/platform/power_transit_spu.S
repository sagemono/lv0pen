	.section ".rodata"
	.align	7
	.globl	power_transit_spu_image_start
power_transit_spu_image_start:
	.incbin	"power_transit_spu.bin"
	.globl	power_transit_spu_image_end
power_transit_spu_image_end:
