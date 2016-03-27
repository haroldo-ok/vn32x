// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"

#define FBF_WIDTH 320
#define FBF_HEIGHT 202

extern vu16 bedday[], pose[], text_frame[];
extern unsigned char default_font[];
int numColors;

unsigned char tempImgBuffer[FBF_WIDTH * FBF_HEIGHT];

void slave()
{
	while (1) {}
}

void drawApgImage(int x, int y, vu16 *apg, char semiTransparent) {
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
	unsigned int rgb;
	
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
	
	outside = x + visibleW - FBF_WIDTH;
	if (outside > 0) {
		// Image partly outside the left
		visibleW -= outside;
	}

	for (i = 0; i != visibleH; i++) {
		srcCol = srcLin;
		dstCol = dstLin;
		
		for (j = 0; j != visibleW; j++) {
			color = *srcCol;
			if (color != transparency) {
				if (semiTransparent) {
					// 'original color' * 0.25 + 'new color' * 0.75
					rgb = (pal[color] >> 1) & 0x3DEF; // 'new color' * 0.5
					*dstCol = ((*dstCol >> 2) & 0x1CE7) + rgb + ((rgb >> 1) & 0x1CE7);
				} else {
					*dstCol = pal[color];
				}
			}
			srcCol++; dstCol++;
		}

		srcLin += width;
		dstLin += FBF_WIDTH;
	}
}

int drawChar(char ch, int x, int y, vu16 color) {
	vu16 *o16 = (void *) default_font;
	vu16 imgW  = *o16++;
	vu16 imgH  = *o16++;
	vu16 imgSize = *o16++;
	vu16 *img = (void *) o16;
	vu16 charX, charW, incrO, incrD;
	int i, j;
	unsigned char *o, opacity, transp, bg;
	vu16 r0, g0, b0, r, g, b;
	vu16 *d;
	
	r0 = color & 0x1F;
	g0 = (color >> 5) & 0x1F;
	b0 = (color >> 10) & 0x1F;
	
	o = (void *) img;
	o += imgSize + ((ch - 32) << 2);
	o16 = (void *) o;
	
	charX = *o16++;
	charW = *o16++;
	incrO = imgW - charW;
	incrD = FBF_WIDTH - charW;
	
	o = (void *) img;
	o += charX;
	d = &MARS_FRAMEBUFFER + (y * FBF_WIDTH) + x + 0x100;

//	*d = o < default_font + 1790 * 16 + 6 ? color : 0;
	for (i = 0; i != imgH; i++) {
		for (j = 0; j != charW; j++) {
			opacity = *o;
			if (opacity) {
				if (opacity == 255) {
					*d = color;				
				} else {
					opacity++;
					transp = 256 - opacity;
					bg = *d;
					
					r = bg & 0x1F;
					g = (bg >> 5) & 0x1F;
					b = (bg >> 10) & 0x1F;
					
					r = (r0 * opacity + r * transp);
					g = (g0 * opacity + g * transp);
					b = (b0 * opacity + b * transp);
					r >>= 8;
					g >>= 8;
					b >>= 8;
					
					*d = COLOR(r, g, b);
				}
			}
			o++; d++;
		}		
		o += incrO;
		d += incrD;
	}
	*d++ = charX == 547 ? color : 0;
	*d++ = charW == 9 ? color : 0;
	
	return 0;
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

		drawApgImage(0, 0, bedday, 0);
		drawApgImage(80, 0, pose, 0);
		drawApgImage(0, FBF_HEIGHT - 80, text_frame, 1);
		
		drawChar('A', 0, 0, 0x1F);
		drawChar('B', 16, 0, 0x1F);
		drawChar('C', 32, 0, 0x1F);
		
		t++;

		setupLineTable();
     }

	return 0;
}
