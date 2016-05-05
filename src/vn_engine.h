#ifndef _VN_ENGINE_H
#define _VN_ENGINE_H

#include "32x.h"

extern void initVN();
extern void swapBuffers();
extern void vnText(char *text);
extern uint8 vnMenu();

typedef void * (*scriptFunction)();

#endif /* _VN_ENGINE_H */
