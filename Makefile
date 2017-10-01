SRCDIR := src
OBJDIR := build
RPYDIR := script
GENDIR := generated
IMGDIR := img
	
RSCS := $(addprefix $(OBJDIR)/,\
	m68k_crt0.bin m68k_crt1.bin\
	bedday.apg pose.apg text_frame.apg down.apg\
	font_dejavu_sans.bmf)

OBJS := $(addprefix $(OBJDIR)/,\
	sh2_crt0.o\
	aplib_decrunch.o image.o generated_images.o\
	gfx.o text.o menu.o vn_engine.o generated_script.o main.o)
	
include $(GENDIR)/include.mk

$(OBJDIR)/m68k_%.o : $(SRCDIR)/m68k_%.s
	m68k-elf-as -m68000 --register-prefix-optional -o $@ $<
	
$(OBJDIR)/m68k_%.bin : $(OBJDIR)/m68k_%.o
	m68k-elf-ld -T$(SRCDIR)/md.ld --oformat binary -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.s
	sh-elf-as -o $@ $<

$(OBJDIR)/%.o : $(GENDIR)/%.s
	sh-elf-as -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	sh-elf-gcc -c -O2 -o $@ $<

$(OBJDIR)/%.o : $(GENDIR)/%.c
	sh-elf-gcc -c -O2 -I$(SRCDIR) -o $@ $<

$(GENDIR)/%.c : $(RPYDIR)/script.rpy
	rpy2c $< $(GENDIR) 

$(GENDIR)/%.mk : $(RPYDIR)/script.rpy
	rpy2c $< $(GENDIR) 

$(OBJDIR)/%.apx : $(IMGDIR)/%.png
	sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -o $@ $<

$(OBJDIR)/%.apg : $(IMGDIR)/%.png
	sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -o $@tmp $<
	apg $@ $< $@tmp

$(OBJDIR)/%.apg : $(RPYDIR)/%.png $(OBJDIR)
	sixpack.exe -image -pack -v -target 32x -codec aplib -format l8 -q 256 -o $@tmp $<
	apg $@ $< $@tmp

$(OBJDIR)/%.bmf : $(IMGDIR)/src/%.fnt
	font_conv $@ $<
	
all: $(GENDIR)/generated_script.c $(GENDIR)/include.mk $(IMGS) $(OBJS) 
	sh-elf-ld -T $(SRCDIR)/32x.ld -e _start --oformat binary -o generated.32x $(OBJS)
	
$(OBJS): | $(RSCS) $(OBJDIR)

$(RSCS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
	
clean:
	rm -rf $(OBJDIR)
	rm $(GENDIR)/*
	rm *.32x