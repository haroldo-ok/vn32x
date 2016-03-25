! apLib decruncher for SEGA 32X
!
! Based on the M68000 version by MML 2010
! Size optimized (164 bytes) by Franck "hitchhikr" Charlet. 
!
! SH2 version (236 bytes) by Mic 2010

	.section .text
	.align 1
	
	.global		_aplib_decrunch
	.global		aplib_decrunch_end

! In: r4 = compressed data, r5 = destination
!
_aplib_decrunch:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15

	mov.w	imm_32000,r12
	mov.w	imm_1280,r13
	
    	mov	#1,r6		! Initialize bits counter
copy_byte:
	mov.b	@r4+,r1
	mov.b	r1,@r5
	add	#1,r5
next_sequence_init:
	mov	#0,r7		! Initialize LWM
next_sequence:
	bsr   	get_bit		! if bit sequence is %0..., then copy next byte
	nop
	bf	copy_byte
	bsr	get_bit
	nop
	bf	code_pair	! if bit sequence is %10..., then is a code pair
	bsr	get_bit
	mov	#0,r0
	bf	short_match	! if bit sequence is %110..., then is a short match
	
       ! The sequence is %111..., the next 4 bits are the offset (0-15)
	bsr	get_bit
	nop
	bsr	get_bit
	addc	r0,r0
	bsr	get_bit
	addc	r0,r0
	bsr	get_bit
	addc	r0,r0
	addc	r0,r0
	
	tst	r0,r0
	bt	write_byte	! if offset == 0, then write 0x00
	
        ! If offset != 0, then write the byte on destination - offset
        mov	r5,r1
        sub	r0,r1
        mov.b	@r1,r0
write_byte:
        mov.b	r0,@r5
        bra	next_sequence_init
        add	#1,r5

! Code pair %10...
code_pair:     
	bsr	decode_gamma
	nop
	mov	r2,r0
	dt	r0
	dt	r0
	bf	normal_code_pair
	tst	r7,r7
	bf	normal_code_pair
	bsr	decode_gamma
	mov	r8,r0		! offset = old_offset
	bra	copy_code_pair
	nop
normal_code_pair:
	add	r7,r0
	dt	r0
	mov.b	@r4+,r1
	shll8	r0
	extu.b	r1,r1
	bsr	decode_gamma
	or	r1,r0
	cmp/hs	r12,r0
	bf	compare_1280
	bra	continue_short_match
	add	#2,r2
compare_1280:
	cmp/hs	r13,r0
	bf	compare_128
	bra	continue_short_match
	add	#1,r2
compare_128:
	mov	#0x80,r1
	extu.b	r1,r1
	cmp/hs	r1,r0
	bt	continue_short_match
	bra	continue_short_match
	add	#2,r2

! get_bit: Get bits from the crunched data (r3) and insert the most significant bit in the carry flag.
get_bit: 
	dt	r6
	bf	still_bits_left
	mov	#8,r6
	mov.b	@r4+,r3
	shll16	r3
	shll8	r3
still_bits_left:
	rts
	shll	r3		
	
! decode_gamma: Decode values from the crunched data using gamma code
decode_gamma:
	sts.l	pr,@-r15
	mov   	#1,r2
get_more_gamma:
	bsr   	get_bit
	nop
	bsr	get_bit
	addc	r2,r2
	bt	get_more_gamma
	lds.l	@r15+,pr
	nop
	rts
	nop

! Short match %110...
short_match:  
	mov	#1,r2
	mov.b	@r4+,r1		! Get offset (offset is 7 bits + 1 bit to mark if copy 2 or 3 bytes) 
	extu.b	r1,r1
	shlr	r1
	addc	r2,r2		! r2 becomes either 2 or 3
	tst	r1,r1
	bt	end_decrunch
	shlr8	r0
	shll8	r0		! Clear least significant byte
	or	r1,r0
continue_short_match:
	mov	r0,r8		! old_offset = offset
copy_code_pair:   
	mov	r5,r1
	sub	r0,r1
loop_do_copy:
	mov.b	@r1+,r0
	mov.b	r0,@r5
	dt	r2
	bf/s	loop_do_copy
	add	#1,r5
	mov	#1,r7		! LWM = 1
	
        bra   	next_sequence   ! Process next sequence
	nop
	
end_decrunch:
	lds.l	@r15+,pr
	mov.l	@r15+,r13	
	mov.l	@r15+,r12	
	mov.l	@r15+,r8	
	rts
	nop

imm_32000:
	.short 32000
imm_1280:
	.short 1280

aplib_decrunch_end:
