#include "32x.h"
#include "gfx.h"
#include "text.h"
#include "menu.h"
#include "vn_engine.h"

uint16 currentFB;
uint16 joy;
char *textToDisplay, *nextText;
int blinkControl;

extern vu16 bedday[], pose[], text_frame[], next_page_icon[]; // TEMP

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

void readJoy() {
	joy = readJoypad1();
}

void vnText(char *text) {
	for (textToDisplay = text; textToDisplay;) {
		readJoy();
		swapBuffers();
		drawApgImage(0, 0, bedday, 0);			
		drawApgImage(0, FBF_HEIGHT - 80, text_frame, 1);
		
		if ((blinkControl & 0x03) < 2) {
			drawApgImage(FBF_WIDTH - 24, FBF_HEIGHT - 20, next_page_icon, 1);			
		}
		blinkControl++;

		nextText = drawWrappedText(textToDisplay, 8, 142, 304, 48, 0x7FFF);

		if (joy & SEGA_CTRL_A) {
			textToDisplay = nextText;
			while (readJoypad1() & SEGA_CTRL_A);
		}
		
		setupLineTable();
	}	
}