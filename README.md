# AmplOS
Amplus 2.0 Operating System, originally ANother Operating System, is currently
only designed to run on RasPi 3 and 4. PC/… support is "planned"
(writing the boot code for raspis was already tricky enough).

## Building
1. Make sure you have `make` and [Docker](https://www.docker.com) installed
2. Git clone / Download and extract a ZIP / ...
3. `cd /path/to/AmplOS2`
4. `./build.sh raspi` (build the image)
6. (optionally) test using qemu (if you can build natively, you can use sm test)

## Why not Rust?
Rust is a good programming language, but redoxOS already exists.

## Why not Adscript?
Adscript is cool, but it isn't stable yet. Once it is, it will be used in a few
places.

## TODO / Roadmap
AmplOS already does some great things, but there's still a lot to do:

* Some File System (probably [FAT](https://github.com/AmplOS2/ampfat))
* `aport` on macOS and Linux
* Memory Management, obviously
* USB support
* Processes, System Calls, IPC
* Port a C library (probably `dietlibc`) and
compiler (`clang`, `gcc`, `lcc`, ...)
* Users/Groups
* `aport` on AmplOS
* Port a shell (`ash`, `dash`, `ksh`, ...) and
editor (`nano`, `ed`, `that_editor`, ...)
* Port [`doas`](https://github.com/AmplOS2/doas), instead of reinventing the
wheel with `peb`
* Port `zsh` and `vim`
* A temporary unstable API for full-screen graphics from Userspace
* Port `genericdoom`
* Display Server, Window Manager, SDL port
* Port modern programming languages (Dart (+ Flutter), Go, Rust, ...) and
apps (`that_terminal`, Emacs, some browser (maybe surf), 90s games, ...)
* Networking
* Sound
* SMP
* x86/RISC-V/...
