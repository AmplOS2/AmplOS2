# Ampl(us)|(OS)2 Port System
For AmplOS2 to become a seriously viable operating system, it, of course, has to
have some kind of way to get Third Party software. `amport` will solve this
problem by being a (reasonably) cross-platform, (reasonably)
\*BSD-Ports/MacPorts-like environment for achieving this. It also implements
ideas similar to those put forward by the distri Linux distribution/package
manager.

## Architecture
`amport` has Ports. Ports are [ADN](https://github.com/Amplus2/adn) files that
contain information about them. Ports usually are pieces of software, but some
Ports just exist for convenience, to install other Ports bundled into one. These
Ports are also CI-built for a few OSes and architectures. If no prebuilt image
of the Port is available, or the user requested to, the Port will be built from
source on the user's machine. Otherwise a prebuilt image from the CI will be
downloaded. Furthermore, Ports are automatically CI-tested so that (hopefully)
no update ever breakes your system.

## Port definitions (a draft)
This doesn't work currently, see
[CleanMachine1/DollarSkip#37](https://github.com/CleanMachine1/DollarSkip/pull/37)
for more information.
```edn
(port 'dollarskip :license mit
  (info "A simple c program to skip $ when copying shell commands" "https://github.com/CleanMachine1/DollarSkip")
  (v3.1 (ball "https://github.com/CleanMachine1/DollarSkip/archive/refs/tags/3.1.tar.gz"
    (hashes (sha256 "b873e967df7e163df7eec92a94b4655ab4bdb85802333be2a0f4412d838c1a9d"))
    (dependencies libc)
    (build-dependencies make cc)
    (install (make 'install))
    (test (system "$ echo hi | grep hi")))
  (HEAD (git "https://github.com/CleanMachine1/DollarSkip.git") :copy))
```

## Package definitions (a draft, too)
<!--I'm not yet sure if v3.1 should be quoted here but my intuition says "yes"-->
```edn
(package 'c3b45adc-33b7-4901-9b69-2d48814187d6
  (dollarskip 'v3.1)
  (target 'aarch64 'darwin 'mach-o)
  (ball "https://ci.chrissx.de/amport/c3b45adc-33b7-4901-9b69-2d48814187d6.tar.gz")
  (hashes (sha256 "71f54f414b8a5317c76cabd113b212da16424374c06072972a758c17bf392bc8"))
  (dependencies (libc '3bf47da3-6cfb-4c8e-9f8f-94a8d8eff80f))
  (build-dependencies (make '2321e0e1-c5d1-407b-a969-b0a4f62a09ff) (cc 'bda83aa1-bc46-44ac-b6f8-54b9e597a9fa)))
```
