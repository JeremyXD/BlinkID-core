# BlinkID Core Components SDK

_BlinkID_ SDK enables you to perform scans of various ID cards in your app.

For quick start, check the [doxygen documentation](https://blinkid.github.io/blinkid-core/) or one of the demo apps.

## Important links

- [Doxygen documentation](https://blinkid.github.io/blinkid-core/)
- [Release notes](https://github.com/BlinkID/blinkid-core/blob/master/Release%20notes.md)
- [Demo apps](https://github.com/BlinkID/blinkid-core/tree/master/sample-apps)
- [MicroBlink help and support](http://help.microblink.com)
	- or open a [new issue on GitHub](https://github.com/BlinkID/blinkid-core/issues)

# Minimum system requirements

## Minimum hardware requirements

- _BlinkID Core Components SDK_ supports any `x86` and `x86_64` compatible processor that supports **Streaming SIMD Extensions version 2 (SSE2)** instruction set. This includes:
	- Intel Pentium 4 or newer Intel processors
	- AMD Athlon 64 or newer AMD processors, but not Athlon XP
	- for more information about SSE2 support, check [this article](https://en.wikipedia.org/wiki/SSE2#CPU_support)
	- ARM, MIPS, SPARC, Power, Itanium and other non-x86-compatible CPUs are **not supported**
- 20 MB of available hard disk space
- 1 GB of RAM
	- the software may work even on systems with less RAM, but may fail to perform recognition of large images

## Minimum software requirements

### Windows

- _BlinkID Core Components SDK_ requires [Visual Studio 2017 C++ Redistributable library](https://www.visualstudio.com/en-us/productinfo/vs2017-system-requirements-vs#microsoft-visual-c-redistributable-for-visual-studio-2017)
- the minimum software requirements for _BlinkID Core Components SDK_ matches the minimum software requirements for **[Visual Studio 2017 C++ Redistributable library](https://www.visualstudio.com/en-us/productinfo/vs2017-system-requirements-vs#microsoft-visual-c-redistributable-for-visual-studio-2017)**

### MacOS

- _BlinkID Core Components SDK_ supports Intel-based Macs with **Mac OS X 10.11 (El Capitan)** or newer

### Linux

- _BlinkID Core Components SDK_ supports Linux distributions that have **GLIBC 2.13** or newer
- additionally, for running bundled demo apps, a `libjpeg` v8 is required
	- this is a dependency of demo apps, not the SDK itself