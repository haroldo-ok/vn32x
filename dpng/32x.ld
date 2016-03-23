OUTPUT_ARCH(sh)
SEARCH_DIR(.)

/*
 * Setup the memory map of the SEGA 32X.
 * stack grows down from high memory.
 *
 * The memory map look like this:
 * +--------------------+ <- low memory
 * | .text              |
 * |        _etext      |
 * |        ctor list   | the ctor and dtor lists are for
 * |        dtor list   | C++ support
 * +--------------------+
 * | .data              | initialized data goes here
 * |        _data       |
 * |        _edata      |
 * +--------------------+
 * | .bss               |
 * |        _bstart     | start of bss, cleared by crt0
 * |        _bend       | start of heap, used by sbrk()
 * +--------------------+
 * .                    .
 * .                    .
 * .                    .
 * |        __stack     | top of stack
 * +--------------------+
 */

MEMORY
{
    rom : ORIGIN = 0x02000000, LENGTH = 0x00400000
    ram : ORIGIN = 0x06000000, LENGTH = 0x00040000
    cache : ORIGIN = 0xC0000000, LENGTH = 0x00000800
}

/*
 * allocate the stack to be at the top of memory, since the stack
 * grows down
 */

__iwram_start	=	ORIGIN(cache);

PROVIDE (__stack = 0x06040000);

SECTIONS
{
  .text 0x02000000:
  {
    *(.text)
    . = ALIGN(0x4);
     __CTOR_LIST__ = .;
    LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
    *(.ctors)
    LONG(0)
    __CTOR_END__ = .;
    __DTOR_LIST__ = .;
    LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
    *(.dtors)
     LONG(0)
    __DTOR_END__ = .;
    *(.rodata*)

    __INIT_SECTION__ = . ;
    *(.init)
    LONG (0x000B0009)    /* rts ; nop */

    __FINI_SECTION__ = . ;
    *(.fini)
    LONG (0x000B0009)    /* rts ; nop */

    *(.lit)
    _etext = .;
  } > rom

  __data_lma = . ;
  .data 0x06000000 :  AT(ADDR(.text) + SIZEOF(.text))
  {
    _data = . ;
    *(.shdata)
    *(.data)
    *(.data.align1)
    *(.data.align2)
    *(.data.align4)
    _edata = . ;
  } > ram

  _sdata = SIZEOF (.data);

  __iwram_lma = 0x06000000 + SIZEOF (.data);

 /*.iwram __iwram_start : AT ( ADDR (.text) + SIZEOF (.text) + SIZEOF (.data))
  {
    _iwram = . ;
    *(.iwram)
    __iwram_end = . ;
  } > cache */
  

  .bss 0x06000000 + SIZEOF (.data):
  {
    _bstart = . ;
    *(.shbss)
    *(.bss)
    *(COMMON)
    *(.eh_fram)
    *(.eh_frame)
    _bend =  . ;
    _bss_end = .;
  } > ram

}
