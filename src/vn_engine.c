#include "32x.h"
#include "gfx.h"
#include "text.h"
#include "menu.h"
#include "vn_engine.h"

uint16 currentFB;
vu16 joy;
char *textToDisplay, *nextText;
int blinkControl;

void initVN() {
	currentFB = 0;
	textToDisplay = 0;
	blinkControl = 0;

	initGfx();
	initMenu();

	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}

	// Set 8-bit paletted color mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_32K;

	MARS_VDP_FBCTL = currentFB;
}

void swapBuffers() {
	MARS_VDP_FBCTL = currentFB ^ 1;
	while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentFB) {}
	currentFB ^= 1;	
}