# TARGET  ?= aarch64-none-elf
CXX      = clang++ --target=aarch64-none-elf
LD       = ld.lld
OBJCOPY  = llvm-objcopy

CXXFLAGS  = -Wall -Wextra -pedantic -std=c++2a -O3 -mcpu=cortex-a53
CXXFLAGS += -Wno-unused-function -fno-exceptions -ffreestanding -Ikernel -Iutf8 -I. -Ipsf
LDFLAGS   = --static -x -s

KHDRS = $(wildcard kernel/*.hh) $(wildcard kernel/*.h) $(wildcard kernel/*/*.hh)
KSRCS = $(wildcard kernel/*.cc) $(wildcard kernel/*/*.cc)
KASMS = $(wildcard kernel/*.S) $(wildcard kernel/*/*.S) $(wildcard boot/*.S)
KOBJS = $(KSRCS:.cc=.o) $(KASMS:.S=.o)
SCRIPTS = $(wildcard scripts/*)

#UNIFONT_PSF ?= http://unifoundry.com/pub/unifont/unifont-14.0.03/font-builds/Unifont-APL8x16-14.0.03.psf.gz
UNIFONT_OTF ?= https://fonts.chrissx.de/fonts/unifont-14.0.03.otf

all: raspi logo.ico

%.png: %.svg
	inkscape $< -o $@

%.ico: %.png
	convert $< -resize 128x128 $@

raspi: kernel8.img

%.o: %.cc fonts/unifont.psf.h $(KHDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.S sourcelist/dist/index.html
	$(CXX) $(CXXFLAGS) -c $< -o $@

fonts/%.psf.h: fonts/%.psf
	scripts/bin2h $<

amplos.elf: boot/raspi.ld fonts/unifont.psf.h $(KOBJS)
	$(LD) $(LDFLAGS) -T boot/raspi.ld -o amplos.elf $(KOBJS)

kernel8.img: amplos.elf
	$(OBJCOPY) amplos.elf -S -O binary kernel8.img

#fonts/unifont.psf:
#	@mkdir -p fonts
#	curl -Lo fonts/unifont.psf.gz $(UNIFONT_PSF)
#	gunzip fonts/unifont.psf.gz

fonts/unifont.otf:
	@mkdir -p fonts
	curl -Lo fonts/unifont.otf $(UNIFONT_OTF)

sourcelist/src/logo.ico.zst: logo.ico
	zstd -18 $< -fo $@

sourcelist/src/%.zst: sourcelist/dist/%
	zstd -18 $< -fo $@

sourcelist/dist/index.html: sourcelist/package-lock.json sourcelist/src/index.html sourcelist/src/index.js sourcelist/src/style.css.zst sourcelist/src/logo.ico.zst sourcelist/src/sources.html.zst

sourcelist/dist/%: sourcelist/package-lock.json sourcelist/src/% fonts/unifont.otf logo.ico
	cd sourcelist && npx parcel build src/$*

sourcelist/package-lock.json: sourcelist/package.json
	cd sourcelist && npm install

sourcelist/src/sources.html: LICENSE $(KSRCS) $(KHDRS) $(KASMS) $(SCRIPTS)
	for x in $^ ; do scripts/code2md $$x ; done | pandoc -f markdown -t html > $@

# Append -s and -S to be able to use gdb
test: raspi
	qemu-system-aarch64 -M raspi3b -serial stdio -kernel kernel8.img

update: format
	git submodule foreach git pull

format:
	clang-format -Werror -i --style=file --verbose $(KSRCS) $(KHDRS)
	deno fmt

clean:
	rm -rf amplos.elf kernel8.img $(KOBJS) fonts/ sourcelist/package-lock.json sourcelist/node_modules/ sourcelist/dist/ sourcelist/src/sources.html

.PHONY: all raspi loc test update clean format
