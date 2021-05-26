CXX      = clang++ --target=aarch64-none-elf
LD       = ld.lld
OBJCOPY  = llvm-objcopy
ADSCRIPT = adscript -t aarch64-none-elf

CXXFLAGS  = -Wall -Wextra -pedantic -std=c++2a -O3
CXXFLAGS += -Wno-unused-function -fno-exceptions -ffreestanding -Ikernel -Iutf8 -I. -Ipsf
LDFLAGS   = --static -x -s

KHDRS = $(wildcard kernel/*.hh) $(wildcard kernel/*.h) $(wildcard kernel/*/*.hh)
KSRCS = $(wildcard kernel/*.cc) $(wildcard kernel/*/*.cc)
ADSRC = $(wildcard kernel/*.adscript)
KOBJS = $(KSRCS:.cc=.o) $(ADSRC:.adscript=.o) boot/raspi.o

all: raspi

raspi: kernel8.img

%.o: %.cc fonts/unifont.psf.h $(KHDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.S
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.adscript
	$(ADSCRIPT) -o $@ $<

fonts/%.psf.h: fonts/%.psf
	cd fonts && ../scripts/bin2h $*.psf

amplos.elf: fonts/unifont.psf.h $(KOBJS)
	$(LD) $(LDFLAGS) -T boot/raspi.ld -o amplos.elf $(KOBJS)

kernel8.img: amplos.elf
	$(OBJCOPY) amplos.elf -S -O binary kernel8.img

fonts/unifont.psf:
	mkdir -p fonts
	curl -Lo fonts/unifont.psf.gz http://unifoundry.com/pub/unifont/unifont-13.0.06/font-builds/Unifont-APL8x16-13.0.06.psf.gz
	gunzip fonts/unifont.psf.gz

loc:
	cloc .

# Append -s and -S to be able to use gdb
test: raspi
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel8.img

update:
	git submodule foreach git pull
	clang-format -Werror -i --style=file --verbose $(KSRCS) $(KHDRS)

clean:
	rm -f amplos.elf kernel8.img $(KOBJS) unifont.psf.h

.PHONY: all raspi loc test update clean
