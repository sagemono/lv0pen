	.section .text.salt,"ax",@progbits
	.global	module_salt
	.type	module_salt, @function
module_salt:
	ila	$2,0x8fc
	ila	$2,0x2cd5d
	ila	$2,0x3b456
	ila	$2,0xe52b
	.size	module_salt, .-module_salt
