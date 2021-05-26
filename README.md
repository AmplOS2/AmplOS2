# AmplOS
Amplus 2.0 Operating System, originally ANother Operating System, is currently
only designed to run on RasPi 3 and 4. PC/… support is "planned"
(writing the boot code for raspis was already tricky enough).

## Building
1. Make sure you have all of the dependencies installed
2. Git clone (remember to check that submodules are initialized)
3. `cd /path/to/AmplOS2`
4. `make -j$(nproc) raspi`
6. (optionally) `make test`

If you want automatically managed dependencies, you can also use `./build raspi`
instead of step 4. That uses Docker to build in an independent environment.

## Dependencies
If you use the Docker-based `build` and `buildenv` scripts, dependencies are
managed automatically. If you want to build natively on your Host OS, you will
have to install the following dependencies:
* [`curl`](https://curl.se)
* POSIX [`make`](https://www.gnu.org/software/make/) and
[`coreutils`](https://www.gnu.org/software/coreutils/)
* [`python3`](https://www.python.org)
* [`llvm`](https://llvm.org) with [`clang`](https://clang.llvm.org) and
[`lld`](https://lld.llvm.org)
* [`qemu`](https://www.qemu.org) (for testing)

## Why not Rust?
Rust is a good programming language, but redoxOS already exists.

On a more serious note: A bit like Adscript, AmplOS came out of the idea of
using all kind of optimizations. One of anOS's first distinguishing differences
compared to other Operating Systems was the excessive usage of `constexpr` and
`inline` and those are simply not in Rust.

## Why not Adscript?
Adscript is cool, but it isn't stable yet. Once it is, it will be used in a few
places.

It probably won't make it into the kernel for quite a while, because we need
stable APIs for cross-compilation and all of the other hacks that we are using.

## TODO / Roadmap
AmplOS already does some great things, but there's still a lot to do:

* Some File System (probably [FAT](https://github.com/AmplOS2/ampfat))
* `amport` on macOS and Linux
* Memory Management, obviously
* USB support
* Processes, System Calls, IPC
* Port a C library (probably `dietlibc`) and
compiler (`clang`, `gcc`, `lcc`, ...)
* FUSE/9P support for porting filesystems
* Users/Groups
* `amport` on AmplOS
* Port a shell (`ash`, `dash`, `ksh`, ...) and
editor (`nano`, `ed`, `that_editor`, ...)
* Port [`doas`](https://github.com/AmplOS2/doas), instead of reinventing the
wheel with `peb`
* Port or reinvent generic utilities (probably POSIX compliant,
maybe `busybox`, maybe GNU `coreutils`)
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
