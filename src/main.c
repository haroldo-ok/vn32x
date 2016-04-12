// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"
#include "gfx.h"
#include "text.h"

extern vu16 bedday[], pose[], text_frame[], next_page_icon[];
int numColors;

#define MENU_ENTRY_COUNT 8

typedef struct _menuEntry {
	unsigned char *s;
	unsigned char idx;
} menuEntry;

menuEntry menuEntries[MENU_ENTRY_COUNT];
unsigned char usedMenuEntries;
unsigned char menuCursor;

void slave()
{
	while (1) {}
}

unsigned char addMenuItem(char *s) {
	menuEntry *m = menuEntries + usedMenuEntries;
	usedMenuEntries++;

	m->s = s;
	m->idx = usedMenuEntries;
	
	return m->idx;
}

unsigned char drawMenu() {
	menuEntry *m = menuEntries;
	int i, y;
	char selected;
	
	y = (FBF_HEIGHT - (int) usedMenuEntries * 36) >> 1;
	for (i = 0; i < usedMenuEntries; i++, y += 36, m++) {
		selected = m->idx == menuCursor;
		translucentRectangle(8, y, 304, 32, !selected ? 0 : COLOR(3, 3, 3));
		drawWrappedText(m->s, 12, y, 300, 32, !selected ? 0x7FFF : COLOR(0x1F, 0x1F, 0x0F));
	}
}

vu16 readJoypad1() {
	return MARS_SYS_COMM8;
}

int main()
{
	uint16 currentFB=0;
	usedMenuEntries = 0;
	menuCursor = 1;

	int i, j, t;
	
	vu16 joy;
	char *textToDisplay = 0, *nextText;
	int blinkControl = 0;
	
	initGfx();

	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}

	// Set 8-bit paletted color mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_32K;

	MARS_VDP_FBCTL = currentFB;
	
	addMenuItem("Option one");
	addMenuItem("Option two");
	addMenuItem("Option three");

    for(;;) {
		
		joy = readJoypad1();
		
		MARS_VDP_FBCTL = currentFB ^ 1;
		while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentFB) {}
		currentFB ^= 1;

		drawApgImage(0, 0, bedday, 0);
		drawApgImage(80, 0, pose, 0);
		drawApgImage(0, FBF_HEIGHT - 80, text_frame, 1);
		
		if ((blinkControl & 0x03) < 2) {
			drawApgImage(FBF_WIDTH - 24, FBF_HEIGHT - 20, next_page_icon, 1);			
		}
		blinkControl++;
		
		drawChar('A', t, 0, 0x1F);
		drawChar('B', 16, 0, 0x1F);
		drawChar('C', 32 + fontWidth('C'), 0, 0x1F);
		drawText("Test", 8, 126, 0);
		
		if (!textToDisplay) {
			textToDisplay = "Here's some text. It's so long, it spans multiple lines. Testing word wrapping.\nLine breaks are supported, too.\nAlso, it can span multiple pages, if so required.";
		}
		nextText = drawWrappedText(textToDisplay, 8, 142, 304, 48, 0x7FFF);
		
		drawMenu();
		
		if (joy & SEGA_CTRL_A) {
			textToDisplay = nextText;
			while (readJoypad1() & SEGA_CTRL_A);
		}
		
		if (joy & SEGA_CTRL_UP) {
			if (menuCursor > 1) {
				menuCursor--;
			}
		}
		if (joy & SEGA_CTRL_DOWN) {
			if (menuCursor < usedMenuEntries) {
				menuCursor++;
			}
		}
		
		t++;

		setupLineTable();
     }

	return 0;
}
