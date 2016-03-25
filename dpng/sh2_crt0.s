! SEGA 32X support code for SH2
! by Chilly Willy
! Rom header and SH2 init/exception code - must be first in object list

        .text

! Standard MD Header at 0x000

        .incbin "m68k_crt0.bin"

! Standard Mars Header at 0x3C0

        .ascii	"32X Demo        "		        /* module name */
	    .long	0x00000000			            /* version */
        .long    _etext-0x02000000              /* Source (in ROM) */
        .long    0x00000000                     /* Destination (in SDRAM) */
        .long    _sdata                         /* Size */
        .long    0x06000240                     /* Master SH2 Jump */
        .long    0x06000244                     /* Slave SH2 Jump */
        .long    0x06000000                     /* Master SH2 VBR */
        .long    0x06000120                     /* Slave SH2 VBR */

! Standard MD startup code at 0x3F0

        .incbin "m68k_crt1.bin"


        .data

! Master Vector Base Table at 0x06000000

        .long   mstart      /* Cold Start PC */
        .long   0x06040000  /* Cold Start SP */
        .long   mstart      /* Manual Reset PC */
        .long   0x06040000  /* Manual Reset SP */
        .long   main_err    /* Illegal instruction */
        .long   0x00000000  /* reserved */
        .long   main_err    /* Invalid slot instruction */
        .long   0x20100400  /* reserved */
        .long   0x20100420  /* reserved */
        .long   main_err    /* CPU address error */
        .long   main_err    /* DMA address error */
        .long   main_err    /* NMI vector */
        .long   main_err    /* User break vector */
        .space  76          /* reserved */
        .long   main_err    /* TRAPA #32 */
        .long   main_err    /* TRAPA #33 */
        .long   main_err    /* TRAPA #34 */
        .long   main_err    /* TRAPA #35 */
        .long   main_err    /* TRAPA #36 */
        .long   main_err    /* TRAPA #37 */
        .long   main_err    /* TRAPA #38 */
        .long   main_err    /* TRAPA #39 */
        .long   main_err    /* TRAPA #40 */
        .long   main_err    /* TRAPA #41 */
        .long   main_err    /* TRAPA #42 */
        .long   main_err    /* TRAPA #43 */
        .long   main_err    /* TRAPA #44 */
        .long   main_err    /* TRAPA #45 */
        .long   main_err    /* TRAPA #46 */
        .long   main_err    /* TRAPA #47 */
        .long   main_err    /* TRAPA #48 */
        .long   main_err    /* TRAPA #49 */
        .long   main_err    /* TRAPA #50 */
        .long   main_err    /* TRAPA #51 */
        .long   main_err    /* TRAPA #52 */
        .long   main_err    /* TRAPA #53 */
        .long   main_err    /* TRAPA #54 */
        .long   main_err    /* TRAPA #55 */
        .long   main_err    /* TRAPA #56 */
        .long   main_err    /* TRAPA #57 */
        .long   main_err    /* TRAPA #58 */
        .long   main_err    /* TRAPA #59 */
        .long   main_err    /* TRAPA #60 */
        .long   main_err    /* TRAPA #61 */
        .long   main_err    /* TRAPA #62 */
        .long   main_err    /* TRAPA #63 */
        .long   main_irq    /* Level 1 IRQ */
        .long   main_irq    /* Level 2 & 3 IRQ's */
        .long   main_irq    /* Level 4 & 5 IRQ's */
        .long   main_irq    /* PWM interupt */
        .long   main_irq    /* Command interupt */
        .long   main_irq    /* H Blank interupt */
        .long   main_irq    /* V Blank interupt */
        .long   main_irq    /* Reset Button */

! Slave Vector Base Table at 0x06000120

        .long   sstart      /* Cold Start PC */
        .long   0x0603F800  /* Cold Start SP */
        .long   sstart      /* Manual Reset PC */
        .long   0x0603F800  /* Manual Reset SP */
        .long   slav_err    /* Illegal instruction */
        .long   0x00000000  /* reserved */
        .long   slav_err    /* Invalid slot instruction */
        .long   0x20100400  /* reserved */
        .long   0x20100420  /* reserved */
        .long   slav_err    /* CPU address error */
        .long   slav_err    /* DMA address error */
        .long   slav_err    /* NMI vector */
        .long   slav_err    /* User break vector */
        .space  76          /* reserved */
        .long   slav_err    /* TRAPA #32 */
        .long   slav_err    /* TRAPA #33 */
        .long   slav_err    /* TRAPA #34 */
        .long   slav_err    /* TRAPA #35 */
        .long   slav_err    /* TRAPA #36 */
        .long   slav_err    /* TRAPA #37 */
        .long   slav_err    /* TRAPA #38 */
        .long   slav_err    /* TRAPA #39 */
        .long   slav_err    /* TRAPA #40 */
        .long   slav_err    /* TRAPA #41 */
        .long   slav_err    /* TRAPA #42 */
        .long   slav_err    /* TRAPA #43 */
        .long   slav_err    /* TRAPA #44 */
        .long   slav_err    /* TRAPA #45 */
        .long   slav_err    /* TRAPA #46 */
        .long   slav_err    /* TRAPA #47 */
        .long   slav_err    /* TRAPA #48 */
        .long   slav_err    /* TRAPA #49 */
        .long   slav_err    /* TRAPA #50 */
        .long   slav_err    /* TRAPA #51 */
        .long   slav_err    /* TRAPA #52 */
        .long   slav_err    /* TRAPA #53 */
        .long   slav_err    /* TRAPA #54 */
        .long   slav_err    /* TRAPA #55 */
        .long   slav_err    /* TRAPA #56 */
        .long   slav_err    /* TRAPA #57 */
        .long   slav_err    /* TRAPA #58 */
        .long   slav_err    /* TRAPA #59 */
        .long   slav_err    /* TRAPA #60 */
        .long   slav_err    /* TRAPA #61 */
        .long   slav_err    /* TRAPA #62 */
        .long   slav_err    /* TRAPA #63 */
        .long   slav_irq    /* Level 1 IRQ */
        .long   slav_irq    /* Level 2 & 3 IRQ's */
        .long   slav_irq    /* Level 4 & 5 IRQ's */
        .long   slav_irq    /* PWM interupt */
        .long   slav_irq    /* Command interupt */
        .long   slav_irq    /* H Blank interupt */
        .long   slav_irq    /* V Blank interupt */
        .long   slav_irq    /* Reset Button */

! The main SH2 starts here at 0x06000240
.global mstart
.global sstart

mstart:
        bra     mcont
        nop

! The slave SH2 starts here at 0x06000244

sstart:
        bra     scont
        nop

mcont:
! purge cache and turn it off
        mov.l   _cache_ctl,r0
        mov     #0x10,r1
        mov.b   r1,@r0
! wait for 68000 to finish init
        mov.l   _master_st,r0
        mov.l   _master_ok,r1
3:
        mov.l   @r0,r2
        cmp/eq  r1,r2
!        bt      3b
! let Slave SH2 run
        mov.l   _slave_st,r0
        mov     #0,r1
        mov.l   r1,@r0
! purge cache and run main()
        mov.l   _cache_ctl,r0
        mov     #0x19,r1
        mov.b   r1,@r0

!	mov.l	_iwram_lma,r0
!	mov.l	_iwram_start,r1
!	mov.l	_iwram_end,r2
!mcopy_to_iwram:
!	cmp/hs	r2,r1
!	bt	mcopy_to_iwram_done
!	mov.b	@r0+,r3
!	mov.b	r3,@r1
!	bra	mcopy_to_iwram
!	add	#1,r1
!mcopy_to_iwram_done:

! clear bss
        mov     #0,r0
        mov.l   _bss_dst,r1
        mov.l   _bss_end,r2
	cmp/eq	r1,r2
	bt	mbss_empty
2:
        mov.b   r0,@r1
        add     #1,r1
        cmp/eq  r1,r2
        bf      2b
mbss_empty:        


        mov.l   _master_go,r0
        jmp     @r0
        nop

scont:
! wait for Master SH2 and 68000 to finish init
        mov.l   _slave_st,r0
        mov.l   _slave_ok,r1
1:
        mov.l   @r0,r2
        cmp/eq  r1,r2
 !       bt      1b
! purge cache and run slave()
        mov.l   _cache_ctl,r0
        mov     #0x19,r1
        mov.b   r1,@r0
        mov.l   _slave_go,r0
        jmp     @r0
        nop

        .align 4
_cache_ctl:
        .long 0xFFFFFE92

_master_st:
        .long   0x20004020
_master_ok:
        .ascii  "M_OK"
_master_go:
        .long   _main


_slave_st:
        .long   0x20004024
_slave_ok:
        .ascii  "S_OK"
_slave_go:
        .long   _slave

_bss_dst:
        .long   _bstart
_bss_end:
        .long   _bend


! Master exception handler

main_err:
        rte
        nop

! Master IRQ handler

main_irq:
        rte
        nop

! Slave exception handler

slav_err:
        rte
        nop

! Slave IRQ handler

slav_irq:
        rte
        nop


        .text

        .global _start
_start:

! The slave SH2 starts here - remove if define slave() in program
_slave:
        bra     _slave
        nop

.global _end
_end:
	bra	_end
	nop
	