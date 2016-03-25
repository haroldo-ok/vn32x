! PackFire 1.2e - (tiny depacker) by Neural
! SH2 version by Mic (236 bytes)

	.section .text
	.align 1
	
	.global		_unpackfire_tiny
	.global		unpackfire_tiny_end

! In: r4 = compressed data, r5 = destination
!
_unpackfire_tiny:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15
	
	mov	r4,r12
	add	#26,r12
	mov.b	@r12+,r7
	extu.b	r7,r7
lit_copy:
	mov.b	@r12+,r1
	mov.b	r1,@r5
	add	#1,r5
main_loop:
	bsr	get_bit
	nop
	bt	lit_copy
	mov	#-1,r3
get_index:
	bsr	get_bit
	add	#1,r3
	bf	get_index
	extu.w	r3,r0
	cmp/eq	#0x10,r0
	bt	depack_stop
	bsr	get_pair
	nop
	extu.w	r3,r6
	extu.w	r3,r0
	mov	#2,r1
	cmp/gt	r1,r0
	bf/s	in_range	/* ble */
	mov	#0,r10
	mov	#0,r3		/* if (r3 > 2) r3 = 0; */	           
in_range:
	extu.w 	r3,r0
	tst 	r0,r0
	bt 	5f
	cmp/eq 	#1,r0
	bf 	4f
	add 	#0x10,r10
4:
	add 	#0x10,r10
5:
	mov 	#4,r11
	tst 	#1,r0
	bt 	3f
	shlr 	r11		/* r11 = (r3 & 1) ? 2 : 4 */
3:
	bsr	get_bits
	add 	#0x10,r10	/* r10 = (r3 == 1) ? 0x30 : ((r3 == 2) ? 0x20 : 0x10) */
	bsr	get_pair
	mov	r5,r13
	sub	r3,r13
copy_bytes:
	mov.b	@r13+,r1
	mov.b	r1,@r5
	dt	r6
	bf/s	copy_bytes
	add	#1,r5
	bra	main_loop
	/* The delay slot isn't filled here because it doesn't matter at this point if the mov #0,r14 below is executed */

get_pair:
	mov	#0,r14
	mov	#0xf,r2
calc_len_dist:
	extu.w	r14,r10
	and	r2,r10
	tst	r10,r10
	bf	node
	mov	#1,r9
node:
	extu.w	r14,r0
	shlr	r0
	mov.b	@(r0,r4),r11
	extu.b	r11,r11
	mov	#1,r8
	and	r8,r10
	tst	r10,r10
	bt	nibble
	shlr2	r11
	shlr2	r11
nibble:
	extu.w	r9,r10
	and	r2,r11
	mov	r11,r1
1:
	tst	r1,r1
	bt	2f
	shll	r8	
	bra	1b
	dt	r1
2:
	add	r8,r9
	add	#1,r14
	cmp/pl	r3
	bt/s	calc_len_dist
	dt	r3
get_bits:
	sts.l	pr,@-r15
	mov	#0,r3
getting_bits:
	dt	r11
	cmp/pz	r11
	bt	cont_get_bit
	lds.l	@r15+,pr
	rts
	add	r10,r3
depack_stop:
	lds.l	@r15+,pr
	mov.l	@r15+,r13	
	mov.l	@r15+,r12	
	mov.l	@r15+,r11	
	mov.l	@r15+,r10	
	mov.l	@r15+,r9	
	mov.l	@r15+,r8
	rts
	nop
cont_get_bit:
	bsr	get_bit
	nop
	bra	getting_bits
	addc	r3,r3
	
get_bit:
	extu.b	r7,r0
	tst	#0x7F,r0	/* Are there any bits left? */
	bf	byte_done
	not	r7,r0
	tst	#0x80,r0	/* T = old bit7 */
	mov.b	@r12+,r7
	extu.b	r7,r7
byte_done:
	not	r7,r0
	addc	r7,r7		/* r7 = r7*2 + T */
	tst	#0x80,r0	/* T = new bit7 */
	rts
	extu.b	r7,r7
	

unpackfire_tiny_end:

