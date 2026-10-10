	.text
	.align	3
	.global	_start
	.type	_start, @function
_start:
	ila	$2,0x3dfa0
	shlqbyi	$83,$3,0
	ori	$82,$4,0
	shlqbyi	$81,$5,0
	ori	$1,$2,0
	ori	$84,$6,0
	shlqbyi	$85,$7,0
	ori	$86,$8,0
	shlqbyi	$87,$9,0
	ori	$88,$10,0
	shlqbyi	$89,$11,0
	ila	$80,__CTOR_END__-4
.Lctor_next:
	lqd	$2,0($80)
	rotqby	$2,$2,$80
	ceqi	$2,$2,-1
	brnz	$2,.Lmain
	lqd	$2,0($80)
	rotqby	$2,$2,$80
	ai	$80,$80,-4
	bisl	$0,$2
	nop	$127
	br	.Lctor_next
.Lmain:
	ori	$6,$84,0
	shlqbyi	$7,$85,0
	ori	$8,$86,0
	shlqbyi	$9,$87,0
	ori	$10,$88,0
	shlqbyi	$11,$89,0
	ori	$4,$82,0
	shlqbyi	$5,$81,0
	ori	$3,$83,0
	brsl	$0,main
	brsl	$0,exit
	stop	0
	.size	_start, .-_start

	.global	_exit
	.type	_exit, @function
_exit:
	ori	$80,$3,0
	lqa	$2,.Ldtor
	nop	$127
	brnz	$2,.Lstop_now
	ila	$3,__DTOR_LIST__+4
	lqd	$2,0($3)
	stqa	$3,.Ldtor
	rotqby	$2,$2,$3
.Ldtor_next:
	nop	$127
	brz	$2,.Lstop_now
	lqa	$3,.Ldtor
	lqd	$2,0($3)
	rotqby	$2,$2,$3
	bisl	$0,$2
	nop	$127
	hbra	.Ldtor_br,.Ldtor_next
	lqa	$2,.Ldtor
	nop	$127
	nop	$127
	ai	$2,$2,4
	lqd	$3,0($2)
	stqa	$2,.Ldtor
	rotqby	$3,$3,$2
	ori	$2,$3,0
	nop	$127
.Ldtor_br:
	br	.Ldtor_next
.Lstop_now:
	ori	$3,$80,0
	lqr	$4,.Lstop
	cbd	$5,.Lstop+3($1)
	shufb	$0,$80,$4,$5
	stqr	$0,.Lstop
	sync
.Lstop:
	stop	0x2000
	.size	_exit, .-_exit

	.section .ctors,"aw",@progbits
	.align	2
__CTOR_LIST__:
	.long	-1

	.section .dtors,"aw",@progbits
	.align	2
__DTOR_LIST__:
	.long	-1

	.section .ctors.end,"aw",@progbits
	.align	2
__CTOR_END__:
	.long	0

	.section .dtors.end,"aw",@progbits
	.align	2
	.long	0

	.data
	.align	4
.Ldtor:
	.space	16
