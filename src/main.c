// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"

#define FBF_WIDTH 320
#define FBF_HEIGHT 202

extern vu16 maruko[], test[];
int numColors;

unsigned char tempImgBuffer[FBF_WIDTH * FBF_HEIGHT];

void slave()
{
	while (1) {}
}

void drawApgImage(int x, int y, vu16 *apg) {
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	int i, j;
	int width = apg[0];
	int height = apg[1];
	int transparency = apg[2];
	int palSize = apg[3];
	vu16 *pal = apg + 4;
	vu16 *image = pal + palSize;
	
	unsigned char *srcLin, *srcCol;
	vu16 *dstLin, *dstCol;
	int visibleW, visibleH, outside;
	unsigned char color;
	
	if (x <= -width || x >= FBF_WIDTH + width || y <= -height || y >= FBF_HEIGHT + height) {
		// Image is fully outside the screen.
		return;
	}

	aplib_decrunch(image, tempImgBuffer);
	
	srcLin = tempImgBuffer;		
	dstLin = frameBuffer16 + 0x100;	
	visibleW = width;
	visibleH = height;
	
	if (y < 0) {
		// Image partly outside the top
		srcLin -= y * width;
		visibleH += y;
	} else {
		dstLin += y * FBF_WIDTH;		
	}
	
	outside = y + visibleH - FBF_HEIGHT;
	if (outside > 0) {
		// Image partly outside the bottom
		visibleH -= outside;
	}
	
	if (x < 0) {
		// Image partly outside the left
		srcLin -= x;
		visibleW += x;
	} else {
		dstLin += x;		
	}
		
	for (i = 0; i != visibleH; i++) {
		srcCol = srcLin;
		dstCol = dstLin;
		
		for (j = 0; j != visibleW; j++) {
			color = *srcCol;
			if (color != transparency) {
				*dstCol = pal[color];
			}
			srcCol++; dstCol++;
		}

		srcLin += width;
		dstLin += FBF_WIDTH;
	}
}

int setupLineTable() {
	int i, lineOffs;
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	
	// Set up the line table
	lineOffs = 0x100;
	for (i = 11; i < 213; i++) {
		frameBuffer16[i] = lineOffs;
		lineOffs += FBF_WIDTH;
	}

	lineOffs = (FBF_HEIGHT + 1) * FBF_WIDTH;
	for (i = 0; i < 11; i++) {
		frameBuffer16[i] = lineOffs;
	}
	for (i = 213; i < 256; i++) {
		frameBuffer16[i] = lineOffs;
	}	
}

int main()
{
	uint16 currentFB=0;

	int i, j, t;

	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}

	// Set 8-bit paletted color mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_32K;

	MARS_VDP_FBCTL = currentFB;

    for(;;)
    {
		MARS_VDP_FBCTL = currentFB ^ 1;
		while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentFB) {}
		currentFB ^= 1;

		drawApgImage(0, 0, maruko);
//		drawApgImage(t, FBF_HEIGHT - 64, test);
//		drawApgImage(t, FBF_HEIGHT - 120 + t, test);
		drawApgImage(8 - t, 32, test);
		
		//frameBuffer16[t + 0x100] = t;
		t++;

		setupLineTable();
     }

	return 0;
}
