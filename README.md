# tinyalsa-cxx

A pure C++ ALSA library.

This project was started to provide a C++ way of interfacing to ALSA in the Linux kernel, an alternative to the original TinyALSA C library.

This project does not rely on the original TinyALSA C library in any way, and they may both be installed.

This project does not intend to replace the orignal TinyALSA implementation.

Use this project if:

- You want a more lean alternative to alsa-lib
- You want to use features of the C++ language like namespaces, exceptions and automatic memory management.

Do not use this project if:

- The project you're using it for is written in C (use the original TinyALSA, instead).
- The project you're using it for depends on having ALSA configuration files.

