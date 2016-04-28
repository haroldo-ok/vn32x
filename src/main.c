// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"
#include "gfx.h"
#include "text.h"
#include "menu.h"

extern vu16 bedday[], pose[], text_frame[], next_page_icon[];
int numColors;

void slave()
{
	while (1) {}
}

vu16 readJoypad1() {
	return MARS_SYS_COMM8;
}

int main()
{
	uint16 currentFB=0;

	int i, j, t;
	
	vu16 joy;
	char *textToDisplay = 0, *nextText;
	int blinkControl = 0;
	
	initVN();
	
	addMenuItem("Option one");
	addMenuItem("Option two");
	addMenuItem("Option three");

    for(;;) {
		
		joy = readJoypad1();

		swapBuffers();

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
			if (menuCursor < menuItemCount()) {
				menuCursor++;
			}
		}
		
		t++;

		setupLineTable();
     }

	return 0;
}
