#ifndef _TEXT_H
#define _TEXT_H

#include "32x.h"

int drawChar(char ch, int x, int y, vu16 color);
vu16 fontWidth(char ch);
vu16 fontHeight();
int drawText(char *s, int x, int y, vu16 color);
char *drawWrappedText(char *s, int x, int y, int w, int h, vu16 color);

#endif /* _TEXT_H */