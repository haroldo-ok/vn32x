/* Sega Saturn Linker Script by Anders Montonen		*/
/* based on Jeff Frohwein's GBA script				*/
/*  v0.1 - First release							*/
/*													*/
/* This file is released into the public domain		*/
/* for any use without restrictions placed			*/
/* upon it.											*/

/* This linker script sets up code to run from		*/
/* 0x06004000, as is the norm.						*/

/* TODO:											*/
/* - loram support									*/
/* - overlay support								*/
/* - use MEMORY directive for a cleaner script?		*/
/* - dual CPU support?								*/
/* - verify that C++ is working						*/

/* OUTPUT_FORMAT("elf32-sh") */
OUTPUT_ARCH(sh)
ENTRY(_start)

__text_start = 0x6000000;
__stack_bottom = 0x0603E800;
__stack_top	= 0x0603F000;
_stack = __stack_top;

SECTIONS
{
	.text __text_start :
	{
		*(.text.*)
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		. = ALIGN(2);
	} = 0
	__text_end = .;
	
	.rodata :
	{
		*(.rodata)
		*all.rodata*(*)
		*(.roda)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
		SORT(CONSTRUCTORS)
		. = ALIGN(4);
	} = 0
	
	/* C++ support */
	.tors :
	{
		__CTOR_LIST__ = .;
		LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
		___ctors = . ;
		*(.ctors)
		___ctors_end = . ;
		LONG(0)
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
		___dtors = . ;
		*(.dtors)
		___dtors_end = . ;
		LONG(0)
		__DTOR_END__ = .;
	} = 0
/*	
	.eh_frame :
	{
		KEEP(*(.eh_frame))
		. = ALIGN(4)
	} = 0

	.gcc_except_table :
	{
		*(.gcc_except_table)
		. = ALIGN(4)
	} = 0
*/
	.data ALIGN(4) : /* AT (__data_lma) */
	{
		__data_start = ABSOLUTE(.);
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		CONSTRUCTORS
		. = ALIGN(4);
	} = 0
	
	__data_end = .;

	.bss ALIGN(4) :
	{
		__bss_start = ABSOLUTE(.);
		__bss_start__ = ABSOLUTE(.);
		*(.dynbss)
		*(.gnu.linkonce.b*)
		*(COMMON)
		. = ALIGN(4);
	}
	
	__bss_end = .;
	__bss_end__ = .;

	.stack __stack_bottom : { _stack = .; *(.stack) }

	.stack __stack_bottom :
	{
		*(.stack)
		. = ALIGN(4);
	} = 0xdeaddead
	
	_end = .;
	__end__ = _end;
	PROVIDE (end = _end);
	
	/* Stabs debugging sections.  */
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	/* DWARF debug sections. */
	/* Symbols in the DWARF debugging sections are relative to the beginning */
	/* of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }

	/* These must appear regardless of  .  */
}

