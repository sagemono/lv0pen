	.text
	.align	3
	.global	_start
	.type	_start, @function
_start:
	ila	$2,0x3dfa0
	hbra	.Lctor_br,.Lctor_next
	ori	$83,$3,0
	shlqbyi	$81,$4,0
	ori	$1,$2,0
	shlqbyi	$82,$5,0
	ori	$84,$6,0
	shlqbyi	$85,$7,0
	ori	$86,$8,0
	shlqbyi	$87,$9,0
	ori	$88,$10,0
	shlqbyi	$89,$11,0
	ila	$80,__CTOR_END__-4
.Lctor_br:
	br	.Lctor_next
.Lctor_call:
	bisl	$0,$3
.Lctor_next:
	lqd	$2,0($80)
	rotqby	$3,$2,$80
	ai	$80,$80,-4
	ceqi	$2,$3,-1
	brz	$2,.Lctor_call
	nop	$127
	hbra	.Lmain_br,main
	ori	$6,$84,0
	shlqbyi	$7,$85,0
	ori	$8,$86,0
	shlqbyi	$9,$87,0
	ori	$10,$88,0
	shlqbyi	$11,$89,0
	ori	$4,$81,0
	shlqbyi	$5,$82,0
	ori	$3,$83,0
.Lmain_br:
	brsl	$0,main
	brsl	$0,exit
	stop	0
	.size	_start, .-_start

	.section .ctors,"aw",@progbits
	.align	2
__CTOR_LIST__:
	.long	-1
