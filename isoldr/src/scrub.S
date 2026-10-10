	.section .text.scrub,"ax",@progbits
	.global	scrub_and_jump
	.type	scrub_and_jump, @function
scrub_and_jump:
	il	$34,0
	ila	$30,_start
	ila	$31,0x3fff0
.Lclear:
	clgt	$32,$30,$31
	brnz	$32,.Lwait
	stqd	$34,0($30)
	ai	$30,$30,16
	bra	.Lclear
.Lwait:
	rchcnt	$25,$ch28
	nop	$26
	brz	$25,.Lwait
	il	$0,0
	il	$1,0
	il	$2,0
	.irp	r,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127
	il	$\r,0
	.endr
	bi	$23
	.size	scrub_and_jump, .-scrub_and_jump
