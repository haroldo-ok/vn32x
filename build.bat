SETLOCAL

@REM Set this according to your own environment setup
SET DEVKITROOT=C:\Util\dev32x



SET ROMNAME=eightqueens
SET PATH=%DEVKITROOT%\bin;%DEVKITROOT%\sh-elf\bin;%DEVKITROOT%\m68k-elf\bin;%DEVKITROOT%\UTILS
SET MAKE_MODE=unix

sh-elf-as --small -o crt0.o crt0.s
sh-elf-gcc -c -O1 -m1 -o %ROMNAME%.o %ROMNAME%.c
sh-elf-gcc -c -O1 -m1 -o lodepng.o lodepng.c
sh-elf-gcc -c -O1 -m1 -o gfx.o data\gfx.c
sh-elf-ld -T 32x.ld -relax -small -e _start --oformat binary -o 32xcode.bin crt0.o %ROMNAME%.o lodepng.o gfx.o libc.a libgcc.a
m68k-elf-as -m68000 --register-prefix-optional -o 32x_68k_crt0.o 32x_68k_crt0.s
m68k-elf-ld -Tmd.ld --oformat binary -o %ROMNAME%.32x 32x_68k_crt0.o

pause

del 32xcode.bin
del *.o
