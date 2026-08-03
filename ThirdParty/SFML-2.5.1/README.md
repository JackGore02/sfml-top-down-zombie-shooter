# SFML 2.5.1

This directory contains the repository-local SFML files required to build the
Visual Studio project without installing SFML separately.

- `include/` contains the SFML C++ headers.
- `lib/Win32/` and `lib/x64/` contain the import libraries for each platform.
- `bin/Win32/` and `bin/x64/` contain the Debug and Release runtime DLLs.

The files come from the official SFML 2.5.1 Visual C++ 15 (2017) packages.
SFML documents those binaries as compatible with Visual Studio 2015 and newer.
See `LICENSE.md` for the SFML zlib/png license.

The project copies the appropriate DLLs to its output directory after every
successful build.
