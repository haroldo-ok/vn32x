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
	initVN();
	
    for(;;) {
		vnText("Here's some text. It's so long, it spans multiple lines. Testing word wrapping.\nLine breaks are supported, too.\nAlso, it can span multiple pages, if so required.");

		initMenu();
		addMenuItem("Option one");
		addMenuItem("Option two");
		addMenuItem("Option three");
		vnMenu();
     }

	return 0;
}
