# Ampl(us)|(OS)2 Port System
For AmplOS2 to become a seriously viable operating system, it, of course, has to
have some kind of way to get Third Party software. `amport` will solve this
problem by being a (reasonably) cross-platform, (reasonably)
\*BSD-Ports/MacPorts-like environment for achieving this. It also implements
ideas similar to those put forward by the distri Linux distribution/package
manager.

## Architecture
`amport` has Ports. Ports are [ADN](https://github.com/Amplus2/adn) files that
contain information about a them. Ports usually are pieces of software, but some
Ports just exist for convenience, to install other Ports bundled into one. These
Ports are also CI-built for a few OSes and architectures. If no prebuilt image
of the Port is available, or the user requested to, the Port will be built from
source on the user's machine. Otherwise a prebuilt image from the CI will be
downloaded.
