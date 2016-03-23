m68k-elf-as -m68000 --register-prefix-optional -o m68k_crt0.o m68k_crt0.s
m68k-elf-ld -Tmd.ld --oformat binary -o m68k_crt0.bin m68k_crt0.o
m68k-elf-as -m68000 --register-prefix-optional -o m68k_crt1.o m68k_crt1.s
m68k-elf-ld -Tmd.ld --oformat binary -o m68k_crt1.bin m68k_crt1.o
sh-elf-as -o sh2_crt0.o sh2_crt0.s

sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -width 320 -height 224 -sizefilter 1 -o maruko.apx maruko.png

sh-elf-as -o aplib_decrunch.o aplib_decrunch.s
sh-elf-as -o image.o image_apx.s
sh-elf-gcc -c -O2 -o main.o main_aplib.c
sh-elf-ld -T 32x.ld -e _start --oformat binary -o test_aplib.32x sh2_crt0.o main.o image.o aplib_decrunch.o



del image.o
del aplib_decrunch.o
del main.o
del maruko.apx
del maruko.pal



del m68k_crt0.bin
del m68k_crt1.bin
del m68k_crt0.o
del m68k_crt1.o
del sh2_crt0.o

 