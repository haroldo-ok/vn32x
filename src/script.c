#include "menu.h"
#include "vn_engine.h"
#include "script.h"

void *vn_start() {
	vnText("Here's some text. It's so long, it spans multiple lines. Testing word wrapping.\nLine breaks are supported, too.\nAlso, it can span multiple pages, if so required.");

	initMenu();
	addMenuItem("Option one");
	addMenuItem("Option two");
	addMenuItem("Option three");
	vnMenu();

	return vn_start;
}

