// Test program for apLib image decoding
// SEGA 32X version
// Mic, 2010

#include "32x.h"
#include "aplib_decrunch.h"
#include "gfx.h"
#include "text.h"
#include "menu.h"
#include "vn_engine.h"
#include "script.h"

void slave()
{
	while (1) {}
}

int main()
{
	initVN();
	
	scriptFunction nextScript = vn_start;
	
    for(;;) {
		nextScript = nextScript();
    }

	return 0;
}
