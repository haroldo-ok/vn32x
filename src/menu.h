#ifndef _MENU_H
#define _MENU_H

#include "32x.h"

void initMenu();
unsigned char addMenuItem(char *s);
unsigned char drawMenu();
unsigned int menuItemCount();

extern unsigned char menuCursor;

#endif /* _MENU_H */