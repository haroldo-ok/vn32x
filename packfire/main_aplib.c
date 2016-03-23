// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"

extern char maruko,maruko_end;
extern char palette,palette_end;
int numColors;


void slave()
{
	while (1) {}
}


int main()
{
	uint16 currentFB=0;
	uint16 lineOffs;
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	vu16 *cram16 = &MARS_CRAM;
	vu16 *pal16 = (vu16*)&palette;

	int i;

	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}

	// Set 8-bit paletted color mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_256;

	numColors = ((&palette_end)-(&palette)) / 2;
	for (i = 0; i < numColors; i++)
	{
		cram16[i] = pal16[i] & 0x7FFF;
	}

	MARS_VDP_FBCTL = currentFB;

    for(;;)
    {
		MARS_VDP_FBCTL = currentFB ^ 1;
		while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentFB) {}
		currentFB ^= 1;

		aplib_decrunch(&maruko, frameBuffer16 + 0x100);

		// Set up the line table
		lineOffs = 0x100;
		for (i = 0; i < 256; i++)
		{
			frameBuffer16[i] = lineOffs;
			lineOffs += 160;
		}
     }

	return 0;
}
