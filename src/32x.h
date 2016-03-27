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

#endif
