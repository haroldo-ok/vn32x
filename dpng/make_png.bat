m68k-elf-as -m68000 --register-prefix-optional -o m68k_crt0.o m68k_crt0.s
m68k-elf-ld -Tmd.ld --oformat binary -o m68k_crt0.bin m68k_crt0.o
m68k-elf-as -m68000 --register-prefix-optional -o m68k_crt1.o m68k_crt1.s
m68k-elf-ld -Tmd.ld --oformat binary -o m68k_crt1.bin m68k_crt1.o

sh-elf-as -o sh2_crt0.o sh2_crt0.s
sh-elf-as -o pngdata.o pngdata.s
sh-elf-gcc -c -O2 -o main.o main.c
sh-elf-gcc -c -O2 -o dpng32x.o dpng32x.c
sh-elf-gcc -c -O2 -m1 -o hw_32x.o hw_32x.c
sh-elf-gcc -c -O2 -o font.o font.c
sh-elf-ld -T 32x.ld -Map showpng.map -e _start --oformat binary -o showpng.32x sh2_crt0.o main.o dpng32x.o hw_32x.o font.o pngdata.o libpng.a libz.a libc.a libgcc.a 
 