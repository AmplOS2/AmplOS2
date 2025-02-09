# Ampl<img width=24pt src=logo.png />

Amplus 2.0 Operating System, originally ANother Operating System, is currently
only designed to run on RasPi 3 and 4. PC/… support is “planned” (writing the
boot code for raspis was already tricky enough).

## Building

1. Make sure you have all of the dependencies installed
2. Git clone (remember to check that submodules are initialized)
3. `cd /path/to/AmplOS2`
4. `make -j$(nproc) raspi`
5. (optionally) `make test`

If you want automatically managed dependencies, you can also use `./build raspi`
instead of step 4. That uses Docker to build in an independent environment.

## Dependencies

If you use the Docker-based `build` and `buildenv` scripts, dependencies are
managed automatically. If you want to build natively on your Host OS, you will
have to install the following dependencies:

- [`curl`](https://curl.se)
- POSIX Utilities ([`make`](https://www.gnu.org/software/make/) and
  [`coreutils`](https://www.gnu.org/software/coreutils/))
- [`python3`](https://www.python.org)
- [`llvm`](https://llvm.org) with [`clang`](https://clang.llvm.org) and
  [`lld`](https://lld.llvm.org)
- [`npm`](https://nodejs.org)
- [`pandoc`](https://pandoc.org)
- [`inkscape`](https://inkscape.org)
- [`imagemagick`](https://imagemagick.org)
- [`zstd`](https://facebook.github.io/zstd/)
- [`qemu`](https://www.qemu.org) (for testing)

## Why not Rust?

Rust is a good programming language, but redoxOS already exists.

On a more serious note: A bit like Adscript, AmplOS came out of the idea of
using all kind of optimizations. One of anOS's first distinguishing differences
compared to other Operating Systems was the excessive usage of `constexpr` and
`inline` and those are simply not in Rust.

## Why not Adscript?

Adscript is cool, but it isn't stable yet. Actually it may never be stable,
because its development has nearly halted.

## Why not Carp?

Carp may be used in a few places. **Maybe** even the kernel. Its advantages in
that code just aren't obvious enough to justify the hassle at the moment.

But we really have to convince the Carp team to reimplement the compiler in a
more portable language than Haskell, preferably C or C++, maybe Rust.

## Why not Carbon?

Not stable, not in Ubuntu, …

Seriously tho: Carbon might be a consideration at some point, but at that point
we might as well have rewritten parts in Rust and dealt with problems there.

## Licensing

AmplOS itself is [LGPLv2.1](LICENSE) licensed, but it currently uses code\* with
a variety of licenses:

- [libpsf](https://github.com/AmplOS2/libpsf)
  ([LGPLv2.1](https://github.com/AmplOS2/libpsf/blob/master/LICENSE))
- [branchless-utf8](https://github.com/AmplOS2/branchless-utf8)
  ([Unlicense](https://github.com/AmplOS2/branchless-utf8/blob/master/UNLICENSE))
- [Unifont](http://unifoundry.com/unifont/)
  ([GPLv2, optionally with the GNU font embedding exception, or OFLv1.1](http://unifoundry.com/LICENSE.txt),
  we'd prefer the latter)

\*code = data ⇒ fonts ∈ code

## TODO / Roadmap

AmplOS already does some great things, but there's still a lot to do:

- Experiment with using a proper `malloc` (probably `jemalloc`)
- Memory Management, obviously
- Processes, System Calls, IPC (Mach Ports!!)
- FUSE/9p
- Some File System (probably [FAT](https://github.com/AmplOS2/ampfat))
- `amport` on macOS and Linux
- USB support
- Port a C library (probably `dietlibc`, maybe `musl`) and compiler (`clang`,
  `gcc`, `lcc`, ...)
- Users/Groups
- `amport` on AmplOS
- Port a shell (`ash`, `dash`, `ksh`, ...) and editor (`nano`, `ed`,
  `that_editor`, ...)
- Port [`doas`](https://github.com/AmplOS2/doas), or maybe `sudo`
- Port or reinvent generic utilities (probably POSIX compliant, maybe `busybox`,
  maybe GNU `coreutils`)
- Port `zsh` and `vim` (or `neovim` if that is just as easy)
- A temporary unstable API for full-screen graphics from Userspace
- Port `genericdoom`
- Display Server, Window Manager, ...
- Port SDL
- Port modern programming languages (Dart (+ Flutter), Go, Rust, ...) and apps
  (`that_terminal`, Emacs, some browser (maybe surf), 90s games, ...)
- Networking
- Sound
- SMP
- x86/RISC-V/...
