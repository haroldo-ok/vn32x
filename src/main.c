// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"

extern vu16 maruko[], test[];
int numColors;

unsigned char temp_buffer[320 * 224];


void slave()
{
	while (1) {}
}

void drawApgImage(vu16 *apg) {
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	int i, j;
	int width = apg[0];
	int height = apg[1];
	int transparency = apg[2];
	int palSize = apg[3];
	vu16 *pal = apg + 4;
	vu16 *image = pal + palSize;

	aplib_decrunch(image, temp_buffer);
	
	for (i = 0; i != height; i++) {
		for (j = 0; j != width; j++) {
			if (temp_buffer[i * width + j] != transparency) {
				frameBuffer16[i * 320 + j + 0x100] = pal[temp_buffer[i * width + j]];
			}
		}			
	}
}


int main()
{
	uint16 currentFB=0;
	uint16 lineOffs;
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	unsigned char *frameBuffer8 = &MARS_FRAMEBUFFER;

	int i, j, t;

	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}

	// Set 8-bit paletted color mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_32K;

	/*
	numColors = 128;
	for (i = 0; i < numColors; i++)
	{
		cram16[i] = pal16[i] & 0x7FFF;
		cram16[i + numColors] = pal16b[i] & 0x7FFF;
	}
	*/

	MARS_VDP_FBCTL = currentFB;

    for(;;)
    {
		MARS_VDP_FBCTL = currentFB ^ 1;
		while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentFB) {}
		currentFB ^= 1;

		/*
		aplib_decrunch(&maruko, temp_buffer);
		
		for (i = 0; i != 202; i++) {
			for (j = 0; j != 320; j++) {
				frameBuffer16[i * 320 + j + 0x100] = pal16[temp_buffer[i * 320 + j]];
			}			
		}
		*/

		drawApgImage(maruko);
		drawApgImage(test);
		
		//frameBuffer16[t + 0x100] = t;
		t++;

		// Set up the line table
		lineOffs = 0x100;
		for (i = 11; i < 213; i++) {
			frameBuffer16[i] = lineOffs;
			lineOffs += 320;
		}

		lineOffs = 203 * 320;
		for (i = 0; i < 11; i++) {
			frameBuffer16[i] = lineOffs;
		}
		for (i = 213; i < 256; i++) {
			frameBuffer16[i] = lineOffs;
		}
     }

	return 0;
}
