CXX     = $(TARGET)-g++
LD      = $(TARGET)-ld
OBJCOPY = $(TARGET)-objcopy

CXXFLAGS  = -Wall -Wextra -pedantic -std=c++2a -O3 -s
CXXFLAGS += -Wno-unused-function -fno-exceptions -ffreestanding -nostartfiles -Ikernel -Iutf8 -I. -Ipsf
LDFLAGS   = -flto

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
	$(LD) -T boot/raspi.ld -o amplos.elf $(KOBJS)

kernel8.img: amplos.elf
	$(OBJCOPY) amplos.elf -O binary kernel8.img
