# TARGET  ?= aarch64-none-elf
CXX      = clang++ --target=aarch64-none-elf
LD       = ld.lld
OBJCOPY  = llvm-objcopy

CXXFLAGS  = -Wall -Wextra -pedantic -std=c++2a -O3
CXXFLAGS += -Wno-unused-function -fno-exceptions -ffreestanding -Ikernel -Iutf8 -I. -Ipsf
LDFLAGS   = --static -x -s

KHDRS = $(wildcard kernel/*.hh) $(wildcard kernel/*.h) $(wildcard kernel/*/*.hh)
KSRCS = $(wildcard kernel/*.cc) $(wildcard kernel/*/*.cc)
KASMS = $(wildcard kernel/*.S) $(wildcard kernel/*/*.S) $(wildcard boot/*.S)
KOBJS = $(KSRCS:.cc=.o) $(KASMS:.S=.o)

UNIFONT ?= http://unifoundry.com/pub/unifont/unifont-14.0.02/font-builds/Unifont-APL8x16-14.0.02.psf.gz

all: raspi

raspi: kernel8.img

%.o: %.cc fonts/unifont.psf.h $(KHDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.S
	$(CXX) $(CXXFLAGS) -c $< -o $@

fonts/%.psf.h: fonts/%.psf
	scripts/bin2h $<

amplos.elf: fonts/unifont.psf.h $(KOBJS)
	$(LD) $(LDFLAGS) -T boot/raspi.ld -o amplos.elf $(KOBJS)

kernel8.img: amplos.elf
	$(OBJCOPY) amplos.elf -S -O binary kernel8.img

fonts/unifont.psf:
	@mkdir -p fonts
	curl -Lo fonts/unifont.psf.gz $(UNIFONT)
	gunzip fonts/unifont.psf.gz

# Append -s and -S to be able to use gdb
test: raspi
	qemu-system-aarch64 -M raspi3b -serial stdio -kernel kernel8.img

update: format
	git submodule foreach git pull

format:
	clang-format -Werror -i --style=file --verbose $(KSRCS) $(KHDRS)

clean:
	rm -f amplos.elf kernel8.img $(KOBJS) fonts/unifont.psf.h

.PHONY: all raspi loc test update clean format
