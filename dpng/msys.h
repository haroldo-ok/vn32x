#ifndef _MSYS_H_
#define _MSYS_H_

#define MSYS_HEAP_START 0x0601F000
#define MSYS_HEAP_END 0x0602F000

extern void MSYS_Free( void *ptr );
extern void *MSYS_Alloc( unsigned size );
extern void MSYS_Init( void *heap, unsigned len );


#endif
