SRCDIR := src
OBJDIR := build
IMGDIR := img
	
RSCS := $(addprefix $(OBJDIR)/,\
	m68k_crt0.bin m68k_crt1.bin\
	bedday.apg pose.apg text_frame.apg down.apg\
	font_dejavu_sans.bmf)

OBJS := $(addprefix $(OBJDIR)/,\
	sh2_crt0.o\
	aplib_decrunch.o image.o main.o)

$(OBJDIR)/m68k_%.o : $(SRCDIR)/m68k_%.s
	m68k-elf-as -m68000 --register-prefix-optional -o $@ $<
	
$(OBJDIR)/m68k_%.bin : $(OBJDIR)/m68k_%.o
	m68k-elf-ld -T$(SRCDIR)/md.ld --oformat binary -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.s
	sh-elf-as -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	sh-elf-gcc -c -O2 -o $@ $<

$(OBJDIR)/%.apx : $(IMGDIR)/%.png
	sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -o $@ $<

$(OBJDIR)/%.apg : $(IMGDIR)/%.png
	sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -o $@tmp $<
	apg $@ $< $@tmp

$(OBJDIR)/%.bmf : $(IMGDIR)/src/%.fnt
	font_conv $@ $<
	
all: $(OBJS)
	sh-elf-ld -T $(SRCDIR)/32x.ld -e _start --oformat binary -o test_aplib.32x $(OBJS)
	
$(OBJS): | $(RSCS) $(OBJDIR)

$(RSCS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
	
clean:
	rm -rf $(OBJDIR)
	rm test_aplib.32x