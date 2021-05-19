CXX     = clang++ --target=$(TARGET)
LD      = ld.lld
OBJCOPY = llvm-objcopy

CXXFLAGS  = -Wall -Wextra -pedantic -std=c++2a -O3
CXXFLAGS += -Wno-unused-function -fno-exceptions -ffreestanding -Ikernel -Iutf8 -I. -Ipsf
LDFLAGS   = --static -x -s

KSRCS = kernel/main.cc kernel/printf.cc kernel/pages.cc \
		kernel/raspi/mbox.cc kernel/raspi/gpu.cc kernel/raspi/uart0.cc
KOBJS = $(KSRCS:.cc=.o) boot/raspi.o

FONTS = fonts/unifont.psf.h

all: kernel8.img

%.o: %.cc $(FONTS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.S
	$(CXX) $(CXXFLAGS) -c $< -o $@

fonts/%.psf.h: fonts/%.psf
	cd fonts && ./bin2h $*.psf

amplos.elf: $(FONTS) $(KOBJS)
	$(LD) $(LDFLAGS) -T boot/raspi.ld -o amplos.elf $(KOBJS)

kernel8.img: amplos.elf
	$(OBJCOPY) amplos.elf -S -O binary kernel8.img
