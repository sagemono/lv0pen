	.section .text.salt,"ax",@progbits
	.global	module_salt
	.type	module_salt, @function
module_salt:
	ila	$2,0x3580
	ila	$2,0xdc95
	ila	$2,0x346ee
	ila	$2,0x30327
	.size	module_salt, .-module_salt
