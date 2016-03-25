#ifndef __32X_H__
#define __32X_H__

#include "types.h"

/* Create a 5:5:5 RGB color */
#define COLOR(r,g,b)    (((r)&0x1F)|((g)&0x1F)<<5|((b)&0x1F)<<10)

#define MARS_CRAM			(*(vu16*)0x20004200)
#define MARS_FRAMEBUFFER 	(*(vu16*)0x24000000)
#define MARS_OVERWRITE_IMG 	(*(vu16*)0x24020000)
#define MARS_SDRAM 			(*(vu16*)0x26000000)

#define MARS_SYS_INTMSK		(*(vu16*)0x20004000)

#define MARS_VDP_DISPMODE	(*(vu16*)0x20004100)
#define MARS_VDP_FBCTL		(*(vu16*)0x2000410A)


#define MARS_SH2_ACCESS_VDP	0x8000
#define MARS_68K_ACCESS_VDP	0x0000

#define MARS_PAL_FORMAT		0x0000
#define MARS_NTSC_FORMAT	0x8000

#define MARS_VDP_PRIO_68K	0x0000
#define MARS_VDP_PRIO_32X	0x0080

#define MARS_224_LINES		0x0000
#define MARS_240_LINES		0x0040

#define MARS_VDP_MODE_OFF	0x0000
#define MARS_VDP_MODE_256	0x0001
#define MARS_VDP_MODE_32K	0x0002
#define MARS_VDP_MODE_RLE	0x0003

#define MARS_VDP_VBLK		0x8000
#define MARS_VDP_HBLK		0x4000
#define MARS_VDP_PEN		0x2000
#define MARS_VDP_FS			0x0001


#define MARS_SYS_DMACTR     (*(volatile unsigned short *)0x20004006)
#define MARS_SYS_DMASAR     (*(volatile unsigned long *)0x20004008)
#define MARS_SYS_DMADAR     (*(volatile unsigned long *)0x2000400C)
#define MARS_SYS_DMALEN     (*(volatile unsigned short *)0x20004010)
#define MARS_SYS_DMAFIFO    (*(volatile unsigned short *)0x20004012)
#define MARS_SYS_VRESI_CLR  (*(volatile unsigned short *)0x20004014)
#define MARS_SYS_VINT_CLR   (*(volatile unsigned short *)0x20004016)
#define MARS_SYS_HINT_CLR   (*(volatile unsigned short *)0x20004018)
#define MARS_SYS_CMDI_CLR   (*(volatile unsigned short *)0x2000401A)
#define MARS_SYS_PWMI_CLR   (*(volatile unsigned short *)0x2000401C)
#define MARS_SYS_COMM0      (*(volatile unsigned short *)0x20004020) /* Master SH2 communication */
#define MARS_SYS_COMM2      (*(volatile unsigned short *)0x20004022)
#define MARS_SYS_COMM4      (*(volatile unsigned short *)0x20004024) /* Slave SH2 communication */
#define MARS_SYS_COMM6      (*(volatile unsigned short *)0x20004026)
#define MARS_SYS_COMM8      (*(volatile unsigned short *)0x20004028) /* controller 1 current value */
#define MARS_SYS_COMM10     (*(volatile unsigned short *)0x2000402A) /* controller 2 current value */
#define MARS_SYS_COMM12     (*(volatile unsigned long *)0x2000402C)  /* vcount current value */

#define SEGA_CTRL_UP        0x0001
#define SEGA_CTRL_DOWN      0x0002
#define SEGA_CTRL_LEFT      0x0004
#define SEGA_CTRL_RIGHT     0x0008
#define SEGA_CTRL_B         0x0010
#define SEGA_CTRL_C         0x0020
#define SEGA_CTRL_A         0x0040
#define SEGA_CTRL_START     0x0080
#define SEGA_CTRL_Z         0x0100
#define SEGA_CTRL_Y         0x0200
#define SEGA_CTRL_X         0x0400
#define SEGA_CTRL_MODE      0x0800

#define SEGA_CTRL_TYPE      0xF000
#define SEGA_CTRL_THREE     0x0000
#define SEGA_CTRL_SIX       0x1000
#define SEGA_CTRL_NONE      0xF000

#endif
