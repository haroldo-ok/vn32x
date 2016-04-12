#include "gfx.h"

#define CACHE_SIZE (128*1024)
#define CACHE_END (tempImgBuffer + CACHE_SIZE)
#define CACHE_ENTRY_COUNT 8

typedef struct _cacheEntry {
	unsigned char *compressed, *uncompressed;
	vu16 size;
} cacheEntry;

unsigned char tempImgBuffer[CACHE_SIZE];
cacheEntry imgCacheEntries[CACHE_ENTRY_COUNT];
unsigned char usedCacheEntries;

void initGfx() {
	usedCacheEntries = 0;	
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

/** Very simple and naive cache scheme */
unsigned char *cachedImage(unsigned char *compressed, vu16 size) {
	cacheEntry *entry;
	unsigned char *nextFree;
	int i;
	
	for (i = 0; i < usedCacheEntries; i++) {
		entry = imgCacheEntries + i;
		if (entry->compressed == compressed) {
			return entry->uncompressed;
		}
	}
	
	if (usedCacheEntries) {
		entry = imgCacheEntries + usedCacheEntries - 1;
		nextFree = entry->uncompressed + entry->size;
		// If either maximum slots or maximum RAM has been reached, clear compression cache
		if (usedCacheEntries && (usedCacheEntries >= CACHE_ENTRY_COUNT || nextFree + size >= CACHE_END)) {
			usedCacheEntries = 0;
			nextFree = tempImgBuffer;
		}
	} else {
		// Cache is currently empty
		nextFree = tempImgBuffer;
	}
	
	entry = imgCacheEntries + usedCacheEntries;
	entry->compressed = compressed;
	entry->uncompressed = nextFree;
	entry->size = size;
	aplib_decrunch(compressed, nextFree);
	usedCacheEntries++;
	
	return nextFree;
}

void drawApgImage(int x, int y, vu16 *apg, char semiTransparent) {
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	int i, j;
	vu16 width = apg[0];
	vu16 height = apg[1];
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

	srcLin = cachedImage(image, width * height);
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

void translucentRectangle(int x, int y, int width, int height, vu16 color) {
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	int i, j;
	
	vu16 *dstLin, *dstCol;
	int visibleW, visibleH, outside;
	unsigned int rgb;
	
	if (x <= -width || x >= FBF_WIDTH + width || y <= -height || y >= FBF_HEIGHT + height) {
		// Image is fully outside the screen.
		return;
	}

	dstLin = frameBuffer16 + 0x100;	
	visibleW = width;
	visibleH = height;
	
	if (y < 0) {
		// Image partly outside the top
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
		dstCol = dstLin;
		
		for (j = 0; j != visibleW; j++) {
			// 'original color' * 0.25 + 'new color' * 0.75
			rgb = color & 0x3DEF; // 'new color' * 0.5
			*dstCol = ((*dstCol >> 2) & 0x1CE7) + rgb + ((rgb >> 1) & 0x1CE7);
			dstCol++;
		}

		dstLin += FBF_WIDTH;
	}
}
