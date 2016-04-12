#ifndef _GFX_H
#define _GFX_H

#include "32x.h"

#define FBF_WIDTH 320
#define FBF_HEIGHT 202

void initGfx();
int setupLineTable();
void drawApgImage(int x, int y, vu16 *apg, char semiTransparent);
void translucentRectangle(int x, int y, int width, int height, vu16 color);

#endif /* _GFX_H */