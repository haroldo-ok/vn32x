#include "gfx.h"
#include "text.h"

extern unsigned char default_font[];

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
	o += imgSize + ((vu16) (ch - 32) << 2);
	o16 = (void *) o;
	
	charX = *o16++;
	charW = *o16++;
	incrO = imgW - charW;
	incrD = FBF_WIDTH - charW;
	
	o = (void *) img;
	o += charX;
	d = &MARS_FRAMEBUFFER + (y * FBF_WIDTH) + x + 0x100;

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
					
					r = r0 * opacity + r * transp;
					g = g0 * opacity + g * transp;
					b = b0 * opacity + b * transp;
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
	
	return charW;
}

vu16 fontWidth(char ch) {
	vu16 *o16 = (void *) default_font;
	o16 += 2; // Skip imgW and imgH
	vu16 imgSize = *o16++;
	vu16 *img = (void *) o16;
	unsigned char *o;

	o = (void *) img;
	o += imgSize + ((vu16) (ch - 32) << 2);
	o16 = (void *) o;
	
	o16++; // Skip charX
	
	return *o16++; // Return charW
}

vu16 fontHeight() {
	vu16 *o16 = (void *) default_font;
	o16++; // Skips the width
	return *o16++; // Returns the height
}

int drawText(char *s, int x, int y, vu16 color) {
	char *o, ch;
	int tx = x, ty = y;
	
	for (o = s; *o; o++) {
		ch = *o;
		if (ch == '\n') {
			tx += drawChar('X', 0, 0, 0x1F);			
			tx = x;
			ty += fontHeight(); 
		} else {
			tx += drawChar(*o, tx, ty, color);			
		}
	}
}

char *drawWrappedTextLine(char *s, int x, int y, int w, vu16 color) {
	char *o, ch;
	int tx = x;
	
	char *startOfLine, *endOfLine;
	int currW, bestW, charW, spaceW;
	
	startOfLine = s;
	
	currW = 0;
	bestW = 0;

	// Skips initial spaces for current line
	spaceW = fontWidth(' ');
	for (o = startOfLine; *o == ' '; o++) {
		tx += spaceW;
		currW += spaceW;
		bestW = currW;
	}
	startOfLine = o;
	
	if (!*o || currW >= w) {
		return 0;
	}

	// Scans words that fit the maximum width
	endOfLine = startOfLine;
	for (o = startOfLine; *o && *o != '\n' && currW <= w; o++) {
		ch = *o;
		if (ch == ' ') {
			currW += spaceW;
			if (currW <= w) {
				endOfLine = o;
				bestW = currW;
			}
		} else {
			currW += fontWidth(ch);
		}
	}
	
	// Corner cases: last word in string, and exceedingly long words
	if (currW <= w || !bestW) {
		endOfLine = o;
		bestW = currW;		
	}

	// Renders the line of text
	for (o = startOfLine; o <= endOfLine; o++) {
		ch = *o;
		if (ch && ch != '\n') {
			tx += drawChar(ch, tx, y, color);			
		}
	}
	
	// Skips spaces at end of line.
	while (*endOfLine == ' ') {
		endOfLine++;
	}

	// Skips one line break, if necessary.
	if (*endOfLine == '\n') {
		endOfLine++;
	}
	return *endOfLine ? endOfLine : 0;
}

char *drawWrappedText(char *s, int x, int y, int w, int h, vu16 color) {
	char *o = s;
	int fh = fontHeight();
	int ty = y;
	int maxY = y + h;
	
	while (o && *o && ty + fh <= maxY) {
		o = drawWrappedTextLine(o, x, ty, w, color);
		ty += fh;
//		drawChar(o ? 'X' : 'Y', x, ty, color);
	}
	
	return o;
}
