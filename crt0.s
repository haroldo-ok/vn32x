	.text
	.global	_start

_start:
! The slave SH2 starts here
	bra 	_start
	nop

! And the main SH2 starts here	
	mov.l	_main_addr,r0
	jsr	@r0
	nop

.align 2
_main_addr:
	.long	_main
	
	

