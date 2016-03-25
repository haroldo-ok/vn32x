# SEGA 32X support code for the 68000
# by Chilly Willy
# Third part of rom header

        .text

# Standard Mars startup code at 0x3F0 - this is included as binary as SEGA
# uses this as a security key. US law allows us to include this as-is because
# it's used for security. The interoperability clauses in the law state this
# trumps copyright... and the Supreme Court agrees. :P

        .word   0x287C,0xFFFF,0xFFC0,0x23FC,0x0000,0x0000,0x00A1,0x5128
        .word   0x46FC,0x2700,0x4BF9,0x00A1,0x0000,0x7001,0x0CAD,0x4D41
        .word   0x5253,0x30EC,0x6600,0x03E6,0x082D,0x0007,0x5101,0x67F8
        .word   0x4AAD,0x0008,0x6710,0x4A6D,0x000C,0x670A,0x082D,0x0000
        .word   0x5101,0x6600,0x03B8,0x102D,0x0001,0x0200,0x000F,0x6706
        .word   0x2B78,0x055A,0x4000,0x7200,0x2C41,0x4E66,0x41F9,0x0000
        .word   0x04D4,0x6100,0x0152,0x6100,0x0176,0x47F9,0x0000,0x04E8
        .word   0x43F9,0x00A0,0x0000,0x45F9,0x00C0,0x0011,0x3E3C,0x0100
        .word   0x7000,0x3B47,0x1100,0x3B47,0x1200,0x012D,0x1100,0x66FA
        .word   0x7425,0x12DB,0x51CA,0xFFFC,0x3B40,0x1200,0x3B40,0x1100
        .word   0x3B47,0x1200,0x149B,0x149B,0x149B,0x149B,0x41F9,0x0000
        .word   0x04C0,0x43F9,0x00FF,0x0000,0x22D8,0x22D8,0x22D8,0x22D8
        .word   0x22D8,0x22D8,0x22D8,0x22D8,0x41F9,0x00FF,0x0000,0x4ED0
        .word   0x1B7C,0x0001,0x5101,0x41F9,0x0000,0x06BC,0xD1FC,0x0088
        .word   0x0000,0x4ED0,0x0404,0x303C,0x076C,0x0000,0x0000,0xFF00
        .word   0x8137,0x0002,0x0100,0x0000,0xAF01,0xD91F,0x1127,0x0021
        .word   0x2600,0xF977,0xEDB0,0xDDE1,0xFDE1,0xED47,0xED4F,0xD1E1
        .word   0xF108,0xD9C1,0xD1E1,0xF1F9,0xF3ED,0x5636,0xE9E9,0x9FBF
        .word   0xDFFF,0x4D41,0x5253,0x2049,0x6E69,0x7469,0x616C,0x2026
        .word   0x2053,0x6563,0x7572,0x6974,0x7920,0x5072,0x6F67,0x7261
        .word   0x6D20,0x2020,0x2020,0x2020,0x2020,0x2043,0x6172,0x7472
        .word   0x6964,0x6765,0x2056,0x6572,0x7369,0x6F6E,0x2020,0x2020
        .word   0x436F,0x7079,0x7269,0x6768,0x7420,0x5345,0x4741,0x2045
        .word   0x4E54,0x4552,0x5052,0x4953,0x4553,0x2C4C,0x5444,0x2E20
        .word   0x3139,0x3934,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020
        .word   0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020
        .word   0x2020,0x2020,0x2020,0x524F,0x4D20,0x5665,0x7273,0x696F
        .word   0x6E20,0x312E,0x3000,0x48E7,0xC040,0x43F9,0x00C0,0x0004
        .word   0x3011,0x303C,0x8000,0x323C,0x0100,0x3E3C,0x0012,0x1018
        .word   0x3280,0xD041,0x51CF,0xFFF8,0x4CDF,0x0203,0x4E75,0x48E7
        .word   0x81C0,0x41F9,0x0000,0x063E,0x43F9,0x00C0,0x0004,0x3298
        .word   0x3298,0x3298,0x3298,0x3298,0x3298,0x3298,0x2298,0x3341
        .word   0xFFFC,0x3011,0x0800,0x0001,0x66F8,0x3298,0x3298,0x7000
        .word   0x22BC,0xC000,0x0000,0x7E0F,0x3340,0xFFFC,0x3340,0xFFFC
        .word   0x3340,0xFFFC,0x3340,0xFFFC,0x51CF,0xFFEE,0x22BC,0x4000
        .word   0x0010,0x7E09,0x3340,0xFFFC,0x3340,0xFFFC,0x3340,0xFFFC
        .word   0x3340,0xFFFC,0x51CF,0xFFEE,0x4CDF,0x0381,0x4E75,0x8114
        .word   0x8F01,0x93FF,0x94FF,0x9500,0x9600,0x9780,0x4000,0x0080
        .word   0x8104,0x8F02,0x48E7,0xC140,0x43F9,0x00A1,0x5180,0x08A9
        .word   0x0007,0xFF80,0x66F8,0x3E3C,0x00FF,0x7000,0x7200,0x337C
        .word   0x00FF,0x0004,0x3341,0x0006,0x3340,0x0008,0x4E71,0x0829
        .word   0x0001,0x000B,0x66F8,0x0641,0x0100,0x51CF,0xFFE8,0x4CDF
        .word   0x0283,0x4E75,0x48E7,0x8180,0x41F9,0x00A1,0x5200,0x08A8
        .word   0x0007,0xFF00,0x66F8,0x3E3C,0x001F,0x20C0,0x20C0,0x20C0
        .word   0x20C0,0x51CF,0xFFF6,0x4CDF,0x0181,0x4E75,0x41F9,0x00FF
        .word   0x0000,0x3E3C,0x07FF,0x7000,0x20C0,0x20C0,0x20C0,0x20C0
        .word   0x20C0,0x20C0,0x20C0,0x20C0,0x51CF,0xFFEE,0x3B7C,0x0000
        .word   0x1200,0x7E0A,0x51CF,0xFFFE,0x43F9,0x00A1,0x5100,0x7000
        .word   0x2340,0x0020,0x2340,0x0024,0x1B7C,0x0003,0x5101,0x2E79
        .word   0x0088,0x0000,0x0891,0x0007,0x66FA,0x7000,0x3340,0x0002
        .word   0x3340,0x0004,0x3340,0x0006,0x2340,0x0008,0x2340,0x000C
        .word   0x3340,0x0010,0x3340,0x0030,0x3340,0x0032,0x3340,0x0038
        .word   0x3340,0x0080,0x3340,0x0082,0x08A9,0x0000,0x008B,0x66F8
        .word   0x6100,0xFF12,0x08E9,0x0000,0x008B,0x67F8,0x6100,0xFF06
        .word   0x08A9,0x0000,0x008B,0x6100,0xFF3C,0x303C,0x0040,0x2229
        .word   0x0020,0x0C81,0x5351,0x4552,0x6700,0x0092,0x303C,0x0080
        .word   0x2229,0x0020,0x0C81,0x5344,0x4552,0x6700,0x0080,0x21FC
        .word   0x0088,0x02A2,0x0070,0x303C,0x0002,0x7200,0x122D,0x0001
        .word   0x1429,0x0080,0xE14A,0x8242,0x0801,0x000F,0x660A,0x0801
        .word   0x0006,0x6700,0x0058,0x6008,0x0801,0x0006,0x6600,0x004E
        .word   0x7020,0x41F9,0x0088,0x0000,0x3C28,0x018E,0x4A46,0x6700
        .word   0x0010,0x3429,0x0028,0x0C42,0x0000,0x67F6,0xB446,0x662C
        .word   0x7000,0x2340,0x0028,0x2340,0x002C,0x3E14,0x2C7C,0xFFFF
        .word   0xFFC0,0x4CD6,0x7FF9,0x44FC,0x0000,0x6014,0x43F9,0x00A1
        .word   0x5100,0x3340,0x0006,0x303C,0x8000,0x6004,0x44FC,0x0001

# At this point (0x800), the Work RAM is clear, the VDP initialized, the
# VRAM/VSRAM/CRAM cleared, the Z80 initialized, the 32X initialized,
# both 32X framebuffers cleared, the 32X palette cleared, the SH2s
# checked for a startup error, the adapter TV mode matches the MD TV
# mode, and the ROM checksum checked. If any error is detected, the
# carry is set, otherwise it is cleared. The 68000 main code is now
# entered.

        jmp     __start+0x00880000+0x3F0

# 68000 General exception handler at 0x806

        jmp     __except+0x00880000+0x3F0

# 68000 Level 4 interrupt handler at 0x80C - HBlank IRQ

        jmp     __hblank+0x00880000+0x3F0

# 68000 Level 6 interrupt handler at 0x812 - VBlank IRQ

        jmp     __vblank+0x00880000+0x3F0

__except:
        move.l  d0,-(sp)
        move.l  4(sp),d0    /* jump table return address */
        sub.w   #0x206,d0   /* 0 = BusError, 6 = AddrError, etc */

# handle exception

        move.l  (sp)+,d0
        addq.l  #4,sp       /* pop jump table return address */
        rte

__hblank:
        rte

__vblank:
        rte


__start:
# init MD VDP
        lea     0xC00004,a0
        move.w  #0x8014,(a0) /* reg. 0 - Enable HBL */
        move.w  #0x8174,(a0) /* reg. 1 - Enable display, VBL, DMA + VCell size */
        move.w  #0x8230,(a0) /* reg. 2 - Plane A =$30*$400=$C000 */
        move.w  #0x832C,(a0) /* reg. 3 - Window  =$2C*$400=$B000 */
        move.w  #0x8407,(a0) /* reg. 4 - Plane B =$7*$2000=$E000 */
        move.w  #0x855E,(a0) /* reg. 5 - sprite table begins at $BC00=$5E*$200 */
        move.w  #0x8600,(a0) /* reg. 6 - not used */
        move.w  #0x8700,(a0) /* reg. 7 - Background Color number*/
        move.w  #0x8800,(a0) /* reg. 8 - not used */
        move.w  #0x8900,(a0) /* reg. 9 - not used */
        move.w  #0x8A01,(a0) /* reg 10 - HInterrupt timing */
        move.w  #0x8B00,(a0) /* reg 11 - $0000abcd a=extr.int b=vscr cd=hscr */
        move.w  #0x8C81,(a0) /* reg 12 - hcell mode + shadow/highight + interlaced mode (40 cell, no shadow, no interlace)*/
        move.w  #0x8D2E,(a0) /* reg 13 - HScroll Table = $B800 */
        move.w  #0x8E00,(a0) /* reg 14 - not used */
        move.w  #0x8F02,(a0) /* reg 15 - auto increment data */
        move.w  #0x9011,(a0) /* reg 16 - scrl screen v&h size (64x64) */
        move.w  #0x9100,(a0) /* reg 17 - window hpos */
        move.w  #0x92FF,(a0) /* reg 18 - window vpos */
# init joyports
        move.b  #0x40,0xA10009
        move.b  #0x40,0xA1000B
        move.b  #0x40,0xA1000D
# init comm values
        move.w  #0,0xA1512A     /* clear buttons */
        move.l  #0,0xA1512C     /* clear the vblank count */
# allow SH2 access to MARS hw
        move.w  0xA15100,d0
        or.w    #0x8000,d0
        move.w  d0,0xA15100

# Copy 68000 main loop to Work RAM to keep contention for the ROM with
# SH2s to a minimum.
        lea     __m68k_start(pc),a0
        lea     0x00F01000,a1
        move.w  #__m68k_end-__m68k_start-1,d0
cpyloop:
        move.b  (a0)+,(a1)+
        dbra    d0,cpyloop

        move.l  #0,0xA15120    /* let Master SH2 run */

# jump to main loop in Work RAM
        jmp     0xF01000.l

# this block of code must be pc relative as it's copied into Work RAM

__m68k_start:

# wait on vsync
vshloop:
        move.w  0xC00004,d0
        andi.w  #0x0008,d0
        bne.b   vshloop
vslloop:
        move.w  0xC00004,d0
        andi.w  #0x0008,d0
        beq.b   vslloop

# read 3-button controller
        move.b  #0x40,0xA10003
        nop
        nop
        move.b  0xA10003,d0
        move.b  #0x00,0xA10003
        nop
        nop
        move.b  0xA10003,d1
        andi.w  #0x3F,d0
        andi.w  #0x30,d1
        lsl.w   #2,d1
        or.w    d1,d0
        eori.w  #0x00FF,d0      /* 0 0 0 0 0 0 0 0 S C B A R L D U */
        move.w  d0,0xA1512A     /* set comm port 10 to controller value */

# increment vblank count
        move.l  0xA1512C,d0
        addq.l  #1,d0
        move.l  d0,0xA1512C     /* increment the vblank count */
        bra.b   vshloop

__m68k_end:

        .align 4