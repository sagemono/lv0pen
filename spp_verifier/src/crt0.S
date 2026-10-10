	.text
	.align	3
	.global	_exit
	.type	_exit, @function
_exit:
	ori	$80,$3,0
	brsl	$0,_fini
	ori	$3,$80,0
	br	exit_stop
	.size	_exit, .-_exit

	.global	_start
	.type	_start, @function
_start:
	ila	$8,0x3dfd0
	hbrr	.Linit_call,_init
	ila	$12,0x10203
	shlqbyi	$80,$6,0
	ai	$18,$8,32
	fsmbi	$9,0x0f00
	ila	$15,_end
	shlqbyi	$83,$3,0
	ori	$82,$4,0
	stqd	$18,0($8)
	sf	$10,$15,$8
	lqr	$14,g_stack_reserve
	ori	$81,$5,0
	fsmbi	$16,0
	selb	$1,$8,$10,$9
	ai	$13,$1,48
	stqr	$16,STACK_CHAIN
	rotqbyi	$11,$14,4
	stqr	$13,g_stack_frame
	shufb	$6,$11,$11,$12
	ai	$3,$6,-48
	clgt	$5,$10,$3
	clgti	$4,$3,0
	and	$2,$5,$4
	selb	$7,$10,$3,$2
	selb	$1,$8,$7,$9
.Linit_call:
	brsl	$0,_init
	ori	$4,$82,0
	ori	$5,$81,0
	shlqbyi	$6,$80,0
	ori	$3,$83,0
	brsl	$0,main
	brsl	$0,exit
	stop	0
	.size	_start, .-_start

	.data
	.align	4
	.global	g_stack_frame
g_stack_frame:
	.space	16

	.section .bss,"aw",@nobits
	.align	4
	.global	g_stack_reserve
g_stack_reserve:
	.space	16

	.section .init,"ax",@progbits
	.align	3
	.global	_init
	.type	_init, @function
_init:
	stqd	$0,16($1)
	stqd	$1,-80($1)
	stqd	$80,-48($1)
	stqd	$81,-32($1)
	stqd	$126,-16($1)
	ai	$1,$1,-80
	ila	$80,__CTOR_END__-4
	br	.Lctor_next
.Lctor_call:
	bisl	$0,$3
.Lctor_next:
	lqd	$4,0($80)
	rotqby	$3,$4,$80
	ai	$80,$80,-4
	ceqi	$2,$3,-1
	brz	$2,.Lctor_call
	lqd	$0,96($1)
	lqd	$80,32($1)
	lqd	$81,48($1)
	lqd	$126,64($1)
	ai	$1,$1,80
	bi	$0
	.size	_init, .-_init

	.section .fini,"ax",@progbits
	.align	3
	.global	_fini
	.type	_fini, @function
_fini:
	stqd	$0,16($1)
	stqd	$1,-80($1)
	stqd	$80,-48($1)
	stqd	$81,-32($1)
	stqd	$126,-16($1)
	ai	$1,$1,-80
	lqd	$0,96($1)
	lqd	$80,32($1)
	lqd	$81,48($1)
	lqd	$126,64($1)
	ai	$1,$1,80
	bi	$0
	.size	_fini, .-_fini

	.section .ctors,"aw",@progbits
	.align	2
__CTOR_LIST__:
	.long	-1

	.section .dtors,"aw",@progbits
	.align	2
__DTOR_LIST__:
	.long	-1
