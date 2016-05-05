#include "menu.h"
#include "vn_engine.h"
#include "script.h"

extern uint16 bedday[], pose[], text_frame[], next_page_icon[]; // TEMP

extern void *vn_first_option();
extern void *vn_second_option();

void *vn_start() {
	vnScene(bedday);
	vnShow(pose);
	
	vnText("Here's some text. It's so long, it spans multiple lines. Testing word wrapping.\nLine breaks are supported, too.\nAlso, it can span multiple pages, if so required.");

	initMenu();
	int option1 = addMenuItem("Option one");
	int option2 = addMenuItem("Option two");
	int chosenOption = vnMenu();
	
	if (chosenOption == option1) {
		return vn_first_option;		
	} else if (chosenOption == option2) {
		return vn_second_option;		
	}

	return vn_start;
}

void *vn_first_option() {
	vnText("You chose the first option");
	return vn_start;
}

void *vn_second_option() {
	vnText("You chose the second option");
	return vn_start;
}
